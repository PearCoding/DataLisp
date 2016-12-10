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
#include "DataGroup.h"
#include "Data.h"

namespace DL
{
	struct DL_INTERNAL_LIB DataInternal {
		string_t ID;
		vector_t<Data>::type AnonymousData;
		vector_t<Data>::type NamedData;
		uint32 References;
	};

	DataGroup::DataGroup(const string_t& id) :
		mShared(nullptr)
	{
		mShared = new DataInternal;

		mShared->ID = id;
		mShared->References = 1;
	}

	DataGroup::DataGroup(const DataGroup& other) :
		mShared(other.mShared)
	{
		DL_ASSERT(mShared->References > 0);
		mShared->References++;
	}

	DataGroup::~DataGroup()
	{
		if(mShared)
		{
			DL_ASSERT(mShared->References > 0);

			mShared->References -= 1;
			if(mShared->References == 0)
				delete mShared;
		}
	}

	DataGroup::DataGroup(DataGroup&& other) noexcept
	{
		mShared = other.mShared;
		other.mShared = nullptr;
	}

	DataGroup& DataGroup::operator =(DataGroup&& other) noexcept
	{
		DL_ASSERT(other.mShared->References > 0);

		if(mShared != other.mShared)
		{
			mShared = other.mShared;
			other.mShared = nullptr;
		}

		return *this;
	}
		
	DataGroup& DataGroup::operator =(const DataGroup& other)
	{
		DL_ASSERT(other.mShared->References > 0);
		
		if(mShared != other.mShared)
		{
			mShared = other.mShared;
			mShared->References++;
		}
		
		return *this;
	}

	uint32 DataGroup::referenceCount() const
	{
		DL_ASSERT(mShared);

		return mShared->References;
	}
		
	void DataGroup::makeUnique()
	{
		DL_ASSERT(mShared && mShared->References > 0);

		if(mShared->References == 1)
			return;
		
		DataInternal* p = new DataInternal;
		p->ID = mShared->ID;
		p->AnonymousData = mShared->AnonymousData;
		p->NamedData = mShared->NamedData;

		p->References = 1;
		mShared->References -= 1;

		mShared = p;
	}

	void DataGroup::add(const Data& data)
	{
		DL_ASSERT(mShared && mShared->References > 0);

		if(!data.isValid())
			return;
			
		if (data.key().empty())
			mShared->AnonymousData.push_back(data);
		else
			mShared->NamedData.push_back(data);
	}

	Data DataGroup::at(size_t i) const
	{
		DL_ASSERT(mShared && mShared->References > 0);

		if(i < mShared->AnonymousData.size())
			return mShared->AnonymousData.at(i);
		else
			return Data();
	}

	size_t DataGroup::anonymousCount() const
	{
		DL_ASSERT(mShared && mShared->References > 0);

		return mShared->AnonymousData.size();
	}

	Data DataGroup::getFromKey(const string_t& str) const
	{
		DL_ASSERT(mShared && mShared->References > 0);

		for (const Data& d : mShared->NamedData)
		{
			if (d.key() == str)
				return d;
		}

		return Data();
	}

	vector_t<Data>::type DataGroup::getAllFromKey(const string_t& key) const
	{
		DL_ASSERT(mShared && mShared->References > 0);

		vector_t<Data>::type list;
		for (const Data& d : mShared->NamedData)
		{
			if (d.key() == key)
				list.push_back(d);
		}

		return list;
	}

	bool DataGroup::hasKey(const string_t& key) const
	{
		return getFromKey(key).isValid();
	}

	vector_t<Data>::type DataGroup::getAllEntries() const
	{
		DL_ASSERT(mShared && mShared->References > 0);

		vector_t<Data>::type ret;

		ret.insert(ret.end(), mShared->NamedData.begin(), mShared->NamedData.end());
		ret.insert(ret.end(), mShared->AnonymousData.begin(), mShared->AnonymousData.end());

		return ret;
	}

	const vector_t<Data>::type& DataGroup::getNamedEntries() const
	{
		DL_ASSERT(mShared && mShared->References > 0);

		return mShared->NamedData;
	}

	const vector_t<Data>::type& DataGroup::getAnonymousEntries() const
	{
		DL_ASSERT(mShared && mShared->References > 0);

		return mShared->AnonymousData;
	}

	bool DataGroup::isAllNumber() const
	{
		return isAllAnonymousNumber() && isAllNamedNumber();
	}

	bool DataGroup::isAllAnonymousNumber() const
	{
		DL_ASSERT(mShared && mShared->References > 0);

		if (mShared->AnonymousData.empty())
			return false;

		for (const Data& d : mShared->AnonymousData)
		{
			if (!d.isNumber())
				return false;
		}

		return true;
	}

	bool DataGroup::isAllNamedNumber() const
	{
		DL_ASSERT(mShared && mShared->References > 0);

		if (mShared->NamedData.empty())
			return false;

		for (const Data& d : mShared->NamedData)
		{
			if (!d.isNumber())
				return false;
		}

		return true;
	}

	bool DataGroup::isArray() const { return mShared->ID.empty(); }
	string_t DataGroup::id() const { return mShared->ID; }
	void DataGroup::setID(const string_t& str) { mShared->ID = str; }
}