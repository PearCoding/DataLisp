/*
 Copyright (c) 2014-2020, OEmercan Yazici <omercan AT pearcoding.eu>
 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.

 3. Neither the name of the copyright owner may be used
 to endorse or promote products derived from this software without
 specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
 */
#include "Lexer.h"
#include <sstream>

namespace DL {
Lexer::Lexer(stream_t* provider, SourceLogger* logger)
	: mLineNumber(1)
	, mColumnNumber(1)
	, mProvider(provider)
	, mLogger(logger)
	, mNextToken{ T_EOF, "" }
	, mNextLineNumber(0)
	, mNextColumnNumber(0)
{
	mCurrentChar = mProvider->get();
}

Lexer::~Lexer()
{
}

Token Lexer::getNextToken()
{
	if (!mProvider->good()) {
		Token token;
		token.Type = T_EOF;
		return token;
	} else if (mCurrentChar == '$') {
		mCurrentChar = mProvider->get();
		++mColumnNumber;

		if (mProvider->good() && mCurrentChar == '(') {
			mCurrentChar = mProvider->get();
			++mColumnNumber;

			Token token;
			token.Type = T_ExpressionParanthese;
			return token;
		} else if (!mProvider->good()) {
			std::stringstream stream;
			stream << "No '(' after '$'";
			mLogger->log(mLineNumber, mColumnNumber, L_Error, stream.str());

			Token token;
			token.Type = T_EOF;
			return token;
		} else {
			std::stringstream stream;
			stream << "Invalid character '" << mCurrentChar << "' after '$'";
			mLogger->log(mLineNumber, mColumnNumber, L_Error, stream.str());
			mCurrentChar = mProvider->get();
			++mColumnNumber;
			return getNextToken();
		}
	} else if (mCurrentChar == '(') {
		mCurrentChar = mProvider->get();
		++mColumnNumber;

		Token token;
		token.Type = T_OpenParanthese;
		return token;
	} else if (mCurrentChar == ')') {
		mCurrentChar = mProvider->get();
		++mColumnNumber;

		Token token;
		token.Type = T_CloseParanthese;
		return token;
	} else if (mCurrentChar == '[') {
		mCurrentChar = mProvider->get();
		++mColumnNumber;

		Token token;
		token.Type = T_OpenSquareBracket;
		return token;
	} else if (mCurrentChar == ']') {
		mCurrentChar = mProvider->get();
		++mColumnNumber;

		Token token;
		token.Type = T_CloseSquareBracket;
		return token;
	} else if (mCurrentChar == ',') {
		mCurrentChar = mProvider->get();
		++mColumnNumber;

		Token token;
		token.Type = T_Comma;
		return token;
	} else if (mCurrentChar == ':') {
		mCurrentChar = mProvider->get();
		++mColumnNumber;

		Token token;
		token.Type = T_Colon;
		return token;
	} else if (mCurrentChar == ';') //Comment
	{
		while (mProvider->good() && mCurrentChar != '\n') {
			mCurrentChar = mProvider->get();
			++mColumnNumber;
		}

		return getNextToken();
	} else if (mCurrentChar == '"' || mCurrentChar == '\'') //String
	{
		char start = mCurrentChar;

		string_t str;
		while (true) {
			mCurrentChar = mProvider->get();
			++mColumnNumber;

			if (!mProvider->good() || mCurrentChar == '\n') {
				std::stringstream stream;
				stream << "The string \"" << str << "\" is not closed";
				mLogger->log(mLineNumber, mColumnNumber, L_Error, stream.str());
				break;
			} else if (mCurrentChar == '\\') {
				mCurrentChar = mProvider->get();
				++mColumnNumber;

				if (!mProvider->good()) {
					mLogger->log(mLineNumber, mColumnNumber, L_Error, "Invalid use of the '\\' operator");
				} else if (mCurrentChar == '\n') {
					++mLineNumber;
					mCurrentChar = mProvider->get();
					mColumnNumber = 1;
				} else {
					switch (mCurrentChar) {
					case 'n': // New line
						str += '\n';
						break;
					case 't': // Tab
						str += '\t';
						break;
					case 'r': // Carriage return
						str += '\r';
						break;
					case 'a': // Audible bell
						str += '\a';
						break;
					case 'b': // Backspace
						str += '\b';
						break;
					case 'f': // Form feed
						str += '\f';
						break;
					case 'v': // Vertical tab
						str += '\v';
						break;
					case 'x': //Binary [2]
					case 'u': //Unicode [4]
					case 'U': //Unicode [8]
					{
						size_t length = mCurrentChar == 'x' ? 2 : (mCurrentChar == 'u' ? 4 : 8);
						string_t uni_val;
						for (size_t i = 0; i < length; ++i) {
							mCurrentChar = mProvider->get();
							++mColumnNumber;
							if (!mProvider->good() || mCurrentChar == '\n') {
								mLogger->log(mLineNumber, mColumnNumber, L_Error, "Invalid use of Unicode escape sequence.");
								break;
							}

							uni_val += mCurrentChar;
						}

						if (uni_val.length() == length) {
							size_t r = 0;
							unsigned long uni;
							try {
								uni = std::stoul(uni_val, &r, 16);
							} catch (const std::exception&) {
								mLogger->log(mLineNumber, mColumnNumber, L_Error, "Given escape sequence is invalid.");
								break;
							}

							if (r != length) {
								mLogger->log(mLineNumber, mColumnNumber, L_Error, "Given Unicode sequence is invalid.");
							} else if (length != 2) {
								if (uni <= 0x7F) {
									str += (char)uni;
								} else if (uni <= 0x7FF) {
									unsigned long d = uni & 0x7FF;
									str += char(0xC0 | ((d & 0x7C0) >> 6));
									str += char(0x80 | (d & 0x3F));
								} else if (uni <= 0xFFFF) {
									unsigned long d = uni & 0xFFFF;
									str += char(0xE0 | ((d & 0xF000) >> 12));
									str += char(0x80 | ((d & 0xFC0) >> 6));
									str += char(0x80 | (d & 0x3F));
								} else if (uni <= 0x10FFFF) {
									unsigned long d = uni & 0x10FFFF;
									str += char(0xF0 | ((d & 0x1C0000) >> 18));
									str += char(0x80 | ((d & 0x3F000) >> 12));
									str += char(0x80 | ((d & 0xFC0) >> 6));
									str += char(0x80 | (d & 0x3F));
								} else {
									mLogger->log(mLineNumber, mColumnNumber, L_Error, "Invalid Unicode range.");
								}
							} else //Binary
							{
								str += (char)uni;
							}
						}
					} break;
					default:
						str += mCurrentChar;
						break;
					}
				}
			} else if (mCurrentChar == start) {
				mCurrentChar = mProvider->get();
				++mColumnNumber;
				break;
			} else {
				str += mCurrentChar;
			}
		}

		Token token;
		token.Type  = T_String;
		token.Value = str;
		return token;
	} else if (isdigit(mCurrentChar) || mCurrentChar == '-' || mCurrentChar == '+' || mCurrentChar == '.') {
		string_t identifier;

		//bool hasSign = false;
		bool hasData = false;
		bool hasDot  = false;

		bool hasExp		= false;
		bool hasExpSign = false;
		bool hasExpData = false;

		if (mCurrentChar == '-' || mCurrentChar == '+') {
			//hasSign = true;
		} else if (mCurrentChar == '.') {
			hasDot = true;
		} else {
			hasData = true;
		}

		// Skip first character
		identifier += mCurrentChar;
		mCurrentChar = mProvider->get();
		++mColumnNumber;

		// Consume digits
		if (!hasDot) {
			while (mProvider->good()) {
				if (isdigit(mCurrentChar)) {
					hasData = true;
					identifier += mCurrentChar;
					mCurrentChar = mProvider->get();
					++mColumnNumber;
				} else {
					break;
				}
			}

			// Floating point
			if (mProvider->good() && mCurrentChar == '.') {
				hasDot = true;
				identifier += mCurrentChar;
				mCurrentChar = mProvider->get();
				++mColumnNumber;
			}
		}

		// Consume digits
		while (mProvider->good()) {
			if (isdigit(mCurrentChar)) {
				hasData = true;
				identifier += mCurrentChar;
				mCurrentChar = mProvider->get();
				++mColumnNumber;
			} else {
				break;
			}
		}

		// Exponent
		if (mProvider->good() && (mCurrentChar == 'e' || mCurrentChar == 'E')) {
			hasExp = true;
			identifier += mCurrentChar;
			mCurrentChar = mProvider->get();
			++mColumnNumber;
		}

		// Exponent signs
		if (hasExp) {
			if (mProvider->good() && (mCurrentChar == '+' || mCurrentChar == '-')) {
				hasExpSign = true;
				identifier += mCurrentChar;
				mCurrentChar = mProvider->get();
				++mColumnNumber;
			}

			// Consume digits
			while (mProvider->good()) {
				if (isdigit(mCurrentChar)) {
					hasExpData = true;
					identifier += mCurrentChar;
					mCurrentChar = mProvider->get();
					++mColumnNumber;
				} else {
					break;
				}
			}
		}

		Token token;
		if (hasData && (hasDot || (hasExp && hasExpData))) {
			token.Type  = T_Float;
			token.Value = identifier;
		} else if (hasData && !hasDot && !hasExp && !hasExpData && !hasExpSign) {
			token.Type  = T_Integer;
			token.Value = identifier;
		}

		return token;
	} else if (isAlpha(mCurrentChar)) //Identifier
	{
		string_t identifier;
		identifier += mCurrentChar;

		mCurrentChar = mProvider->get();
		++mColumnNumber;
		while (mProvider->good()) {
			if (isAscii(mCurrentChar)) {
				identifier += mCurrentChar;
				mCurrentChar = mProvider->get();
				++mColumnNumber;
			} else {
				break;
			}
		}

		Token token;
		if (identifier == "true") {
			token.Type = T_True;
		} else if (identifier == "false") {
			token.Type = T_False;
		} else {
			token.Type  = T_Identifier;
			token.Value = identifier;
		}
		return token;
	} else if (isWhitespace(mCurrentChar)) {
		if (mCurrentChar == '\n') {
			++mLineNumber;
			mColumnNumber = 0;
		}

		mCurrentChar = mProvider->get();
		++mColumnNumber;
		return getNextToken();
	} else {
		std::stringstream stream;
		stream << "Invalid character '" << mCurrentChar << "'";
		mLogger->log(mLineNumber, mColumnNumber, L_Error, stream.str());
		mCurrentChar = mProvider->get();
		++mColumnNumber;
		return getNextToken();
	}
}

Token Lexer::next()
{
	// Restore from look()
	if (mNextToken.Type != T_EOF) {
		Token token		= mNextToken;
		mColumnNumber   = mNextColumnNumber;
		mLineNumber		= mNextLineNumber;
		mNextToken.Type = T_EOF;
		return token;
	} else {
		return getNextToken();
	}
}

Token Lexer::look()
{
	line_t l   = mLineNumber;
	column_t c = mColumnNumber;

	Token t = next();
	// Save for next() call
	mNextToken		  = t;
	mNextColumnNumber = mColumnNumber;
	mNextLineNumber   = mLineNumber;

	mColumnNumber = c;
	mLineNumber   = l;
	return t;
}

line_t Lexer::currentLine() const
{
	return mLineNumber;
}

column_t Lexer::currentColumn() const
{
	return mColumnNumber;
}

bool Lexer::isWhitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\v' || c == '\f') {
		return true;
	} else {
		return false;
	}
}

bool Lexer::isAscii(char c)
{
	if (c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' || c == '0' || isAlpha(c)) {
		return true;
	} else {
		return false;
	}
}

bool Lexer::isAlpha(char c)
{
	if (c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f' || c == 'g' || c == 'h' || c == 'i' || c == 'j' || c == 'k' || c == 'l' || c == 'm' || c == 'n' || c == 'o' || c == 'p' || c == 'q' || c == 'r' || c == 's' || c == 't' || c == 'u' || c == 'v' || c == 'w' || c == 'x' || c == 'y' || c == 'z' || c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'E' || c == 'F' || c == 'G' || c == 'H' || c == 'I' || c == 'J' || c == 'K' || c == 'L' || c == 'M' || c == 'N' || c == 'O' || c == 'P' || c == 'Q' || c == 'R' || c == 'S' || c == 'T' || c == 'U' || c == 'V' || c == 'W' || c == 'X' || c == 'Y' || c == 'Z' || c == '_') {
		return true;
	} else {
		return false;
	}
}
} // namespace DL