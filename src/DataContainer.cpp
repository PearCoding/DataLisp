/*
 Copyright (c) 2014, Ömercan Yazici <pearcoding AT gmail.com>
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

namespace DL
{
	DataContainer::DataContainer()
	{
	}

	DataContainer::~DataContainer()
	{
		for(std::list<DataGroup*>::iterator it = mGroups.begin();
			it != mGroups.end();
			++it)
		{
			delete *it;
		}

		for(std::list<Data*>::iterator it = mData.begin();
			it != mData.end();
			++it)
		{
			delete *it;
		}
	}
	
	std::list<DataGroup*> DataContainer::getTopGroups() const
	{
		return mTopGroups;
	}

	std::list<DataGroup*> DataContainer::getAllGroupsID(const string_t& id) const
	{
		std::list<DataGroup*> list;

		for(std::list<DataGroup*>::const_iterator it = mGroups.begin();
			it != mGroups.end();
			++it)
		{
			if((*it)->id() == id)
			{
				list.push_back(*it);
			}
		}

		return list;
	}

	std::list<Data*> DataContainer::getAllDataFromKey(const string_t& key) const
	{
		std::list<Data*> list;

		for(std::list<Data*>::const_iterator it = mData.begin();
			it != mData.end();
			++it)
		{
			if((*it)->key() == key)
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

		//TODO: Add recursive??
	}
	
	void DataContainer::build(SyntaxTree* tree)
	{
		DL_ASSERT(tree);

		for(std::list<StatementNode*>::iterator it = tree->Nodes.begin();
			it != tree->Nodes.end();
			++it)
		{
			DataGroup* group = buildGroup(*it);
			mTopGroups.push_back(group);
			mGroups.push_back(group);
		}
	}

	DataGroup* DataContainer::buildGroup(StatementNode* n)
	{
		DL_ASSERT(n);

		DataGroup* group = new DataGroup(n->Name);

		for(std::list<DataNode*>::iterator it = n->Nodes.begin();
			it != n->Nodes.end();
			++it)
		{
			Data* data = buildData(*it);
			mData.push_back(data);

			group->addData(data);
		}

		return group;
	}

	Data* DataContainer::buildData(DataNode* n)
	{
		DL_ASSERT(n);

		Data* data = new Data(n->Key);

		switch(n->Value->Type)
		{
		case 0:
			{
				DataGroup* group = buildGroup(n->Value->Statement);
				mGroups.push_back(group);
				data->setGroup(group);
			}
			break;
		case 1:
			data->setInt(n->Value->Integer);
			break;
		case 2:
			data->setFloat(n->Value->Float);
			break;
		case 3:
			data->setString(n->Value->String);
			break;
		case 4:
			data->setBool(n->Value->Boolean);
			break;
		case 5:
			{
				DataArray* a = buildArray(n->Value->Array);
				data->setArray(a);
			}
			break;
		default:
			data->setGroup(0);
			break;
		};

		return data;
	}

	DataArray* DataContainer::buildArray(ArrayNode* n)
	{
		DL_ASSERT(n);

		DataArray* a = new DataArray;

		for (std::list<ValueNode*>::iterator it = n->Nodes.begin();
			it != n->Nodes.end();
			++it)
		{
			Data* dat = buildArrayValue(*it);
			a->add(dat);
			mData.push_back(dat);
		}

		return a;
	}

	Data* DataContainer::buildArrayValue(ValueNode* n)
	{
		DL_ASSERT(n);

		Data* data = new Data("");

		switch (n->Type)
		{
		case 0:
			{
				  DataGroup* group = buildGroup(n->Statement);
				  mGroups.push_back(group);
				  data->setGroup(group);
			}
			break;
		case 1:
			data->setInt(n->Integer);
			break;
		case 2:
			data->setFloat(n->Float);
			break;
		case 3:
			data->setString(n->String);
			break;
		case 4:
			data->setBool(n->Boolean);
			break;
		case 5:
			{
				  DataArray* a = buildArray(n->Array);
				  data->setArray(a);
			}
			break;
		default:
			data->setGroup(0);
			break;
		};

		return data;
	}
}
