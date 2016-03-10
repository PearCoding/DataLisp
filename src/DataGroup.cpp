/*
 Copyright (c) 2014-2016, Ömercan Yazici <omercan AT pearcoding.eu>
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
#include "DataGroup.h"
#include "Data.h"

namespace DL
{
	DataGroup::DataGroup(const string_t& id) :
		mID(id)
	{
	}

	DataGroup::~DataGroup()
	{
	}

	void DataGroup::addData(Data* data)
	{
		DL_ASSERT(data);

		if (data->key().empty())
		{
			mData.push_back(data);
		}
		else
		{
			mNamedData.push_back(data);
		}
	}

	Data* DataGroup::at(size_t i) const
	{
		DL_ASSERT(i < mData.size());

		return mData.at(i);
	}

	size_t DataGroup::unnamedCount() const
	{
		return mData.size();
	}

	Data* DataGroup::getFromKey(const string_t& str) const
	{
		DL_ASSERT(!str.empty());

		for (list_t<Data*>::type::const_iterator it = mNamedData.begin();
			it != mNamedData.end();
			++it)
		{
			if ((*it)->key() == str)
			{
				return *it;
			}
		}

		return nullptr;
	}

	list_t<Data*>::type DataGroup::getAllFromKey(const string_t& key) const
	{
		DL_ASSERT(!key.empty());

		list_t<Data*>::type list;
		for (list_t<Data*>::type::const_iterator it = mNamedData.begin();
			it != mNamedData.end();
			++it)
		{
			if ((*it)->key() == key)
			{
				list.push_back(*it);
			}
		}

		return list;
	}

	bool DataGroup::hasKey(const string_t& key) const
	{
		DL_ASSERT(!key.empty());

		for (list_t<Data*>::type::const_iterator it = mNamedData.begin();
			it != mNamedData.end();
			++it)
		{
			if ((*it)->key() == key)
			{
				return true;
			}
		}

		return false;
	}

	const list_t<Data*>::type& DataGroup::getNamedEntries() const
	{
		return mNamedData;
	}
}