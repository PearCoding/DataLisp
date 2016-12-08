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
#pragma once

#include "DataLispConfig.h"

namespace DL
{
	typedef uint32 line_t;
	typedef uint32 column_t;

	/// Message importance level for SourceLogger
	enum Level
	{
		L_Debug,	///< Debug information. Truncated in release code
		L_Info,		///< General information. Truncated if not in verbose mode
		L_Warning,	///< Warning information. Should be investigated, but does not crash applicaton
		L_Error,	///< Error information. Generated content maybe not useable
		L_Fatal		///< Fatal information. %DataLisp can not recover but program will @b NOT be aborted 
	};

	/** @class SourceLogger SourceLogger.h DL/SourceLogger.h
	 * @brief Class logging information to the standard output
	 *
	 * This class logs information to the standard output.<br> 
	 * Other output styles can be added by overriding the virtual functions.
	 */
	class DL_LIB SourceLogger
	{
	public:
		SourceLogger();
		virtual ~SourceLogger();

		/**
		 * @brief Logs the information additionaly with line and column to standard output
		 * @param line The line the information refers
		 * @param column The column the information refers
		 * @param level Importance level
		 * @param str The actual message
		 */ 
		virtual void log(line_t line, column_t column, Level level, const string_t& str);

		/**
		 * @brief Logs the information to standard output
		 * @param level Importance level
		 * @param str The actual message
		 */ 
		virtual void log(Level level, const string_t& str);

		/**
		 * @brief Amount of warnings occured
		 */
		int warningCount() const;

		/**
		 * @brief Amount of errors and fatal errors occured
		 */
		int errorCount() const;

	private:
		int mWarningCount;
		int mErrorCount;
	};
}
