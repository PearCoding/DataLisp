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
#pragma once

#include "DataLispConfig.h"
#include "SourceLogger.h"
#include "Token.h"

namespace DL {
class DL_INTERNAL_LIB Lexer {
public:
	Lexer(stream_t* provider, SourceLogger* logger);
	virtual ~Lexer();

	Token next();
	Token look();

	line_t currentLine() const;
	column_t currentColumn() const;

private:
	Token getNextToken();
	static bool isWhitespace(char c); /* UNICODE? */
	static bool isAscii(char c);
	static bool isAlpha(char c);

	line_t mLineNumber;
	column_t mColumnNumber;
	
	stream_t* mProvider;
	char mCurrentChar;
	SourceLogger* mLogger;

	Token mNextToken;
	line_t mNextLineNumber;
	column_t mNextColumnNumber;
};
} // namespace DL
