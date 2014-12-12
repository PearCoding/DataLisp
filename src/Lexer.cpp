/*
 Copyright (c) 2014, Ömercan Yazici <pearcoding AT gmail.com>
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

namespace DL
{
	Lexer::Lexer(const string_t& source, SourceLogger* logger) :
		mLineNumber(1), mColumnNumber(1), mSource(source), mIterator(), mLogger(logger)
	{
		mIterator = mSource.begin();
	}

	Lexer::~Lexer()
	{
	}

	Token Lexer::next()
	{
		if (mIterator == mSource.end())
		{
			Token token;
			token.Type = T_EOF;
			return token;
		}
		else if (*mIterator == '$')
		{
			++mIterator;
			++mColumnNumber;

			if (mIterator != mSource.end() && *mIterator == '(')
			{
				++mIterator;
				++mColumnNumber;

				Token token;
				token.Type = T_ExpressionParanthese;
				return token;
			}
			else if (mIterator == mSource.end())
			{
				std::stringstream stream;
				stream << "No '(' after '$'";
				mLogger->log(mLineNumber, mColumnNumber, L_Error, stream.str());

				Token token;
				token.Type = T_EOF;
				return token;
			}
			else
			{
				std::stringstream stream;
				stream << "Invalid character '" << *mIterator << "' after '$'";
				mLogger->log(mLineNumber, mColumnNumber, L_Error, stream.str());
				++mIterator;
				++mColumnNumber;
				return next();
			}
		}
		else if (*mIterator == '(')
		{
			++mIterator;
			++mColumnNumber;

			Token token;
			token.Type = T_OpenParanthese;
			return token;
		}
		else if (*mIterator == ')')
		{
			++mIterator;
			++mColumnNumber;

			Token token;
			token.Type = T_CloseParanthese;
			return token;
		}
		else if (*mIterator == '[')
		{
			++mIterator;
			++mColumnNumber;

			Token token;
			token.Type = T_OpenSquareBracket;
			return token;
		}
		else if (*mIterator == ']')
		{
			++mIterator;
			++mColumnNumber;

			Token token;
			token.Type = T_CloseSquareBracket;
			return token;
		}
		else if (*mIterator == ',')
		{
			++mIterator;
			++mColumnNumber;

			Token token;
			token.Type = T_Comma;
			return token;
		}
		else if (*mIterator == ':')
		{
			++mIterator;
			++mColumnNumber;

			Token token;
			token.Type = T_Colon;
			return token;
		}
		else if (*mIterator == ';')//Comment
		{
			while (mIterator != mSource.end() && *mIterator != '\n')
			{
				++mIterator;
				++mColumnNumber;
			}

			return next();
		}
		else if (*mIterator == '"')//String
		{
			string_t str;
			while (true)
			{
				++mIterator;
				++mColumnNumber;

				if (mIterator == mSource.end() ||
					*mIterator == '\n')
				{
					std::stringstream stream;
					stream << "The string \"" << str << "\" is not closed";
					mLogger->log(mLineNumber, mColumnNumber, L_Error, stream.str());
					break;
				}
				else if (*mIterator == '\\')
				{
					++mIterator;
					++mColumnNumber;

					if (mIterator == mSource.end() ||
						*mIterator == '\n')
					{
						mLogger->log(mLineNumber, mColumnNumber, L_Error, "Invalid use of the '\\' operator");
					}
					str += *mIterator;
				}
				else if (*mIterator == '"')
				{
					++mIterator;
					++mColumnNumber;
					break;
				}
				else
				{
					str += *mIterator;
				}
			}

			Token token;
			token.Type = T_String;
			token.Value = str;
			return token;
		}
		else if (isdigit(*mIterator) || *mIterator == '-')
		{
			string_t identifier;
			identifier += *mIterator;

			++mIterator;
			++mColumnNumber;
			while (mIterator != mSource.end())
			{
				if (isdigit(*mIterator) || *mIterator == '.')
				{
					identifier += *mIterator;
					++mIterator;
					++mColumnNumber;
				}
				else
				{
					break;
				}
			}

			Token token;
			if (isInteger(identifier))
			{
				token.Type = T_Integer;
				token.Value = identifier;
			}
			else if (isFloat(identifier))
			{
				token.Type = T_Float;
				token.Value = identifier;
			}
			return token;
		}
		else if (isAlpha(*mIterator))//Identifier
		{
			string_t identifier;
			identifier += *mIterator;

			++mIterator;
			++mColumnNumber;
			while (mIterator != mSource.end())
			{
				if (isAscii(*mIterator))
				{
					identifier += *mIterator;
					++mIterator;
					++mColumnNumber;
				}
				else
				{
					break;
				}
			}

			Token token;
			if (identifier == "true")
			{
				token.Type = T_True;
			}
			else if (identifier == "false")
			{
				token.Type = T_False;
			}
			else
			{
				token.Type = T_Identifier;
				token.Value = identifier;
			}
			return token;
		}
		else if (isWhitespace(*mIterator))
		{
			if (*mIterator == '\n')
			{
				++mLineNumber;
				mColumnNumber = 0;
			}

			++mIterator;
			++mColumnNumber;
			return next();
		}
		else
		{
			std::stringstream stream;
			stream << "Invalid character '" << *mIterator << "'";
			mLogger->log(mLineNumber, mColumnNumber, L_Error, stream.str());
			++mIterator;
			++mColumnNumber;
			return next();
		}
	}

	Token Lexer::look()
	{
		string_t::const_iterator tmp = mIterator;
		line_t l = mLineNumber;
		column_t c = mColumnNumber;

		Token t = next();

		mColumnNumber = c;
		mLineNumber = l;
		mIterator = tmp;
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
		if (c == ' ' || c == '\t' || c == '\r' ||
			c == '\n' || c == '\v' || c == '\f')
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Lexer::isAscii(char c)
	{
		if (c == '1' || c == '2' || c == '3' ||
			c == '4' || c == '5' || c == '6' ||
			c == '7' || c == '8' || c == '9' ||
			c == '0' || isAlpha(c))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Lexer::isAlpha(char c)
	{
		if (c == 'a' || c == 'b' ||
			c == 'c' || c == 'd' || c == 'e' ||
			c == 'f' || c == 'g' || c == 'h' ||
			c == 'i' || c == 'j' || c == 'k' ||
			c == 'l' || c == 'm' || c == 'n' ||
			c == 'o' || c == 'p' || c == 'q' ||
			c == 'r' || c == 's' || c == 't' ||
			c == 'u' || c == 'v' || c == 'w' ||
			c == 'x' || c == 'y' || c == 'z' ||
			c == 'A' || c == 'B' || c == 'C' ||
			c == 'D' || c == 'E' || c == 'F' ||
			c == 'G' || c == 'H' || c == 'I' ||
			c == 'J' || c == 'K' || c == 'L' ||
			c == 'M' || c == 'N' || c == 'O' ||
			c == 'P' || c == 'Q' || c == 'R' ||
			c == 'S' || c == 'T' || c == 'U' ||
			c == 'V' || c == 'W' || c == 'X' ||
			c == 'Y' || c == 'Z' || c == '_')
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool Lexer::isInteger(const std::string& str)
	{
		for (string_t::const_iterator it = str.begin();
			it != str.end();
			++it)
		{
			if (!isdigit(*it) &&
				*it != '-')
			{
				return false;
			}
			else if (*it == '-' && it != str.begin())
			{
				return false;
			}
		}
		return true;
	}

	bool Lexer::isFloat(const std::string& str)
	{
		bool point = false;
		for (string_t::const_iterator it = str.begin();
			it != str.end();
			++it)
		{
			if (*it != '.' &&
				!isdigit(*it) &&
				*it != '-')
			{
				return false;
			}
			else if (*it == '-' && it != str.begin())
			{
				return false;
			}
			else if (*it == '.')
			{
				if (point)
				{
					return false;
				}

				point = true;
			}
		}
		return true;
	}
}