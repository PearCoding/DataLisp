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
	/**
	 * @brief Contains the StdLib expressions
	 */
	namespace Expressions
	{
		map_t<string_t, expr_t>::type getStdLib();

		/**
		* @brief Prints content to SourceLogger
		*/
		Data print_func(const list_t<Data>::type& args, VM& vm);

		/**
		 * @brief Standard if function. 
		 * 
		 * Has to be called with one or two arguments
		 */
		Data if_func(const list_t<Data>::type& args, VM& vm);

		/**
		 * @brief Standard boolean 'not' function. 
		 * 
		 * Has to be called at least one argument
		 * @see and_func
		 * @see or_func
		 */
		Data not_func(const list_t<Data>::type& args, VM& vm);

		/**
		 * @brief Standard boolean 'and' function. 
		 * 
		 * Has to be called at least one argument
		 * @see not_func
		 * @see or_func
		 */
		Data and_func(const list_t<Data>::type& args, VM& vm);

		/**
		 * @brief Standard boolean 'or' function. 
		 * 
		 * Has to be called at least one argument
		 * @see not_func
		 * @see and_func
		 */
		Data or_func(const list_t<Data>::type& args, VM& vm);

		/**
		 * @brief Returns the union of anonymous data from the args
		 * @see named_func
		 * @see union_func
		 */
		Data anonymous_func(const list_t<Data>::type& args, VM& vm);

		/**
		 * @brief Returns the union of named data from the args
		 * @see anonymous_func
		 * @see union_func
		 */
		Data named_func(const list_t<Data>::type& args, VM& vm);

		/**
		 * @brief Returns the union of the args
		 *
		 * Return type will be it itself if args count is 1.<br>
		 * It will be of the type Data::T_Array if every parameter is an array.<br>
		 * Data::T_Group otherwise.
		 * @see anonymous_func
		 * @see named_func
		 */
		Data union_func(const list_t<Data>::type& args, VM& vm);

		/**
		 * @brief Cast elementwise to bool
		 */
		Data bool_func(const list_t<Data>::type& args, VM& vm);

		/**
		 * @brief Cast elementwise to integer
		 */
		Data int_func(const list_t<Data>::type& args, VM& vm);

		/**
		 * @brief Cast elementwise to float
		 */
		Data float_func(const list_t<Data>::type& args, VM& vm);
	}
}
