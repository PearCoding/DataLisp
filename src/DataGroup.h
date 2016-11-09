/*
 Copyright (c) 2014-2016, �mercan Yazici <omercan AT pearcoding.eu>
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

#include "dl_Config.h"

namespace DL
{
	class Data;

	class DL_LIB DataGroup
	{
	public:
		DataGroup();
		~DataGroup();

		inline string_t id() const
		{
			return mID;
		}

		inline void setID(const string_t& str)
		{
			mID = str;
		}

		void addData(const Data& data);

		Data at(size_t i) const;
		size_t anonymousCount() const;

		Data getFromKey(const string_t& str) const;
		list_t<Data>::type getAllFromKey(const string_t& key) const;

		bool hasKey(const string_t& key) const;

		const list_t<Data>::type& getNamedEntries() const;

	private:
		string_t mID;
		vector_t<Data>::type mData;
		list_t<Data>::type mNamedData;
	};
}
