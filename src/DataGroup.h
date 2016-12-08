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
	class Data;

	/** @class DataGroup DataGroup.h DL/DataGroup.h
	 * @brief Capsulate a generic group identified with an id
	 *
	 * Resembles an entry of type:
	 * @code{.lisp}
	 * (ID
	 *   ;...
	 * )
	 * @endcode
	 */
	class DL_LIB DataGroup
	{
	public:
		DataGroup();
		~DataGroup();

		/**
		 * @brief Returns id
		 */
		inline string_t id() const
		{
			return mID;
		}

		/**
		 * @brief Replaces id with given one
		 * @param str New id name
		 */
		inline void setID(const string_t& str)
		{
			mID = str;
		}

		/**
		 * @brief Adds (anonymous or non anonymous) data into group
		 * @param data Data to add. Can be anonymous or non anonymous
		 */
		void addData(const Data& data);

		/**
		 * @brief Returns anonymous data from position i
		 * @param i Index of the anonymous data
		 * @return Data at position i if available, invalid Data otherwise
		 */
		Data at(size_t i) const;

		/**
		 * @brief Amount of anonymous data inside the group
		 */
		size_t anonymousCount() const;

		/**
		 * @brief Returns non anonymous data with the given id
		 *
		 * If more than one data has the same id. The first one will be returned.<br>
		 * Use getAllFromKey to return all instances.
		 * @param str The id of the non anonymous data
		 * @return Data with the given id if available, invalid Data otherwise
		 * @see getAllFromKey
		 */ 
		Data getFromKey(const string_t& str) const;

		/**
		 * @brief Returns all non anonymous data with the given id
		 * @param str The id of the non anonymous data
		 * @see getFromKey
		 */
		list_t<Data>::type getAllFromKey(const string_t& key) const;

		/**
		 * @brief Checks if id is available
		 * @param key The id of the non anonymous data
		 * @return Returns true if group has the data with the given id, false otherwise
		 */
		bool hasKey(const string_t& key) const;

		/**
		 * @brief Returns all non anonymous data entries
		 */
		const list_t<Data>::type& getNamedEntries() const;
	private:
		string_t mID;
		vector_t<Data>::type mData;
		list_t<Data>::type mNamedData;
	};
}
