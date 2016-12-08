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

#include "Data.h"

namespace DL
{
	/** @class DataArray DataArray.h DL/DataArray.h
	 * @brief Capsulate an anonymous group containing only anonymous data 
	 *
	 * Resembles an entry of type:
	 * @code{.lisp}
	 * [
	 *   ;...
	 * ]
	 * @endcode
	 */
	class DL_LIB DataArray
	{
	public:
		DataArray();
		~DataArray();

		/**
		 * @brief Returns amount of anonymous data
		 */
		size_t size() const;

		/**
		 * @brief Returns anonymous data at index i
		 * @param i Index of data
		 */
		Data at(size_t i) const;

		/**
		 * @brief Appends given data at the back
		 * @param d Non-invalid data 
		 */
		void add(const Data& d);

		/**
		 * @brief Replaces data at index i
		 *
		 * If index i is equal to size(), data will be added instead.<br>
		 * It ignores an out of bound index.
		 * @param i Index of data to be replaced
		 * @param d Non-invalid data
		 */
		void set(size_t i, const Data& d);

		/**
		 * @brief Checks if all data is from the same type
		 * @param type The type to check against
		 * @return Returns true if all entries are from the same type, false otherwise
		 * @see DL::Data::type
		 */
		bool isAllType(Data::Type type) const;

		/**
		 * @brief Checks if all data is a number
		 * @return Returns true if all entries are numbers, false otherwise
		 * @see DL::Data::isNumber
		 */
		bool isAllNumber() const;
	private:
		vector_t<Data>::type mList;
	};
}
