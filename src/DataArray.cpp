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
#include "DataArray.h"

namespace DL
{
	DataArray::DataArray()
	{
	}

	DataArray::~DataArray()
	{
	}

	size_t DataArray::size() const
	{
		return mList.size();
	}

	Data DataArray::at(size_t i) const
	{
		if(i < mList.size())
			return mList.at(i);
		else
			return Data();
	}

	void DataArray::add(const Data& d)
	{
		if(d.isValid())
			mList.push_back(d);
	}

	void DataArray::set(size_t i, const Data& d)
	{
		if(!d.isValid())
			return;
			
		if(i == mList.size())
			mList.push_back(d);
		else if(i < mList.size())
			mList[i] = d;
	}

	bool DataArray::isAllType(Data::Type type) const
	{
		if (mList.empty())
			return false;

		for (std::vector<Data>::const_iterator it = mList.begin();
			it != mList.end();
			it++)
		{
			if (it->type() != type)
				return false;
		}

		return true;
	}

	bool DataArray::isAllNumber() const
	{
		if (mList.empty())
			return false;

		for (std::vector<Data>::const_iterator it = mList.begin();
			it != mList.end();
			it++)
		{
			if (!it->isNumber())
				return false;
		}

		return true;
	}
}