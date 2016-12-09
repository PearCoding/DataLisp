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

/** @mainpage notitle
 * DataLisp is an modular and modable configuration scripting language for C++ and Python.<br>
 * More information about DataLisp can you find here: http://pearcoding.eu/projects/datalisp.
 *
 * @par Building
 * At the moment you can build the library for Windows and Linux with cmake.<br>
 * Currently there is no tested support for MacOS X, but due to the non-use of external libraries, there should be no problems.<br>
 * There is also an official C# port: https://github.com/PearCoding/DataLisp-CSharp
 *
 * @par Support
 * If you want to help us, please write an e-mail to &Ouml;mercan Yazici < omercan AT pearcoding.eu >
 */

 /**
  * @brief Main namespace containing all classes, structures, functions and types
  */
namespace DL
{
	class DataContainer;
	class SourceLogger;

	/** @class DataLisp DataLisp.h DL/DataLisp.h
	 * @brief General class to parse or generate %DataLisp source code
	 *
	 * This class handles all variations to use %DataLisp. 
	 *
	 * @section Parsing
	 * When parsing a source code first the source has to be parsed,
	 * afterwards it can be @link build @endlink to fill a DataContainer.<br>
	 * Possible expression should be added before filling the container.

	 * @subsection Example
	 * @code{.cpp}
	 * DL::DataContainer parse_datalisp(const std::string& source, bool& error) {
	 *   DL::SourceLogger logger;
	 *   DL::DataLisp datalisp(&logger);
	 *   DL::DataContainer container;
	 * 
	 *   datalisp.parse(source);
	 *   if(logger.errorCount() == 0)
	 *     datalisp.build(container);
	 * 
	 *   error = logger.errorCount() > 0;
	 *   return container;
	 * }
	 * @endcode 
     *
	 * @section Generating
	 * It is really easy to generate a source code from a container.<br>
	 * Just use @link generate @endlink to produce the code.<br>
	 * Be aware that no expressions will be added,
	 * as there is no way to generate expression code from plain data.
     *
	 * @subsection Example
	 * @code{.cpp}
	 * // Better use it directly, but this is just an example code
	 * std::string generate_datalisp(const DL::DataContainer& container) {
	 *   return DL::DataLisp::generate(container);
	 * }
	 * @endcode 
	 */
	class DL_LIB DataLisp
	{
	public:
		/**
		 * @brief Construct the class with a given logging class
		 * @param log Logging class. Should never be NULL
		 * @param stdlib Enable the standard library expressions
		 */
		DataLisp(SourceLogger* log, bool stdlib = true);
		~DataLisp();

		/**
		 * @brief Parse a given string

		 * @attention Parsing errors or warnings will be post to the given SourceLogger instance.
		 * @param source A source string. Can be UTF8 encoded
		 * @see build
		 */
		void parse(const string_t& source);

		/**
		 * @brief Fills a DataContainer with the content parsed beforehand

		 * @link parse @endlink should be called beforehand or nothing will be built.
		 * The given expression will be executed.
		 * @attention Building errors or warnings will be post to the given SourceLogger instance.
		 * @param container The container to fill. Will not be cleared!
		 * @see parse
		 * @see expression
		 */
		void build(DataContainer& container);

		/**
		 * @brief Add expression to run when built
		 *
		 * @param name Name of the expression. Will replace if already set
		 * @param handler Callback function to run
		 * @see build
		 */
		void addExpression(const string_t& name, expr_t handler);

		/**
		 * @brief Returns callback function of a expression
		 * 
		 * @param name Name of the expression
		 * @return Callback of the expression. Can be NULL if not found
		 * @see addExpression
		 */
		expr_t expression(const string_t& name);

		/**
		 * @brief Generates a overview of the parsed content
		 * 
		 * No expression will be evaluated.
		 * @see parse
		 */
		string_t dump();

		/**
		 * @brief Returns a source string based on the content of the container
		 *
		 * No expression will be in the source.
		 * @param container A container to whom to construct the source code
		 */
		static string_t generate(const DataContainer& container);

	private:
		class DataLisp_Internal* mInternal;
	};
}
