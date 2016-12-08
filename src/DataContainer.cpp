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
#include "DataContainer.h"
#include "SyntaxTree.h"

#include "DataGroup.h"
#include "DataArray.h"
#include "Data.h"

#include "Expressions.h"

namespace DL
{
	DataContainer::DataContainer()
	{
	}

	DataContainer::~DataContainer()
	{
		for (list_t<DataGroup*>::type::iterator it = mGroups.begin();
			it != mGroups.end();
			++it)
		{
			delete *it;
		}

		for (list_t<DataArray*>::type::iterator it = mArrays.begin();
			it != mArrays.end();
			++it)
		{
			delete *it;
		}
	}

	DataGroup* DataContainer::createGroup()
	{
		DataGroup* grp = new DataGroup();
		mGroups.push_back(grp);

		return grp;
	}

	DataArray* DataContainer::createArray()
	{
		DataArray* arr = new DataArray();
		mArrays.push_back(arr);

		return arr;
	}

	list_t<DataGroup*>::type DataContainer::getTopGroups() const
	{
		return mTopGroups;
	}

	list_t<DataGroup*>::type DataContainer::getAllGroupsID(const string_t& id) const
	{
		list_t<DataGroup*>::type list;

		for (list_t<DataGroup*>::type::const_iterator it = mGroups.begin();
			it != mGroups.end();
			++it)
		{
			if ((*it)->id() == id)
			{
				list.push_back(*it);
			}
		}

		return list;
	}

	void DataContainer::addTopGroup(DataGroup* group)
	{
		DL_ASSERT(group);
		mTopGroups.push_back(group);
	}
}