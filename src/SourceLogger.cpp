/*
 Copyright (c) 2014-2016, OEmercan Yazici <omercan AT pearcoding.eu>
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
#include "SourceLogger.h"

#include <stdarg.h>
#include <sstream>
#include <iostream>

namespace DL
{
	SourceLogger::SourceLogger() :
		mWarningCount(0), mErrorCount(0)
	{
	}

	SourceLogger::~SourceLogger()
	{
	}

	void SourceLogger::log(line_t line, column_t column, Level level, const string_t& str)
	{
		std::stringstream stream;
		stream << "[" << line << "](" << column << ") " << str;

		string_t pre;
		switch (level)
		{
		case L_Debug:
			pre = "Debug: ";
			break;
		case L_Info:
			pre = "Info: ";
			break;
		case L_Warning:
			pre = "Warning: ";
			break;
		case L_Error:
			pre = "Error: ";
			break;
		case L_Fatal:
			pre = "Fatal: ";
			break;
		}
		std::cout << pre << stream.str() << std::endl;
	}

	void SourceLogger::log(Level level, const string_t& str)
	{
		string_t pre;
		switch (level)
		{
		case L_Debug:
			pre = "Debug: ";
			break;
		case L_Info:
			pre = "Info: ";
			break;
		case L_Warning:
			pre = "Warning: ";
			break;
		case L_Error:
			pre = "Error: ";
			break;
		case L_Fatal:
			pre = "Fatal: ";
			break;
		}
		std::cout << pre << str << std::endl;
	}

	int SourceLogger::warningCount() const
	{
		return mWarningCount;
	}

	int SourceLogger::errorCount() const
	{
		return mErrorCount;
	}
}