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

		for (list_t<Data*>::type::iterator it = mData.begin();
			it != mData.end();
			++it)
		{
			delete *it;
		}
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

	list_t<Data*>::type DataContainer::getAllDataFromKey(const string_t& key) const
	{
		list_t<Data*>::type list;

		for (list_t<Data*>::type::const_iterator it = mData.begin();
			it != mData.end();
			++it)
		{
			if ((*it)->key() == key)
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

	void DataContainer::build(SyntaxTree* tree, Expressions* expr)
	{
		DL_ASSERT(tree);

		for (list_t<StatementNode*>::type::iterator it = tree->Nodes.begin();
			it != tree->Nodes.end();
			++it)
		{
			DataGroup* group = buildGroup(*it, expr);
			mTopGroups.push_back(group);
			mGroups.push_back(group);
		}
	}

	DataGroup* DataContainer::buildGroup(StatementNode* n, Expressions* expr)
	{
		DL_ASSERT(n);

		DataGroup* group = new DataGroup(n->Name);

		for (list_t<DataNode*>::type::iterator it = n->Nodes.begin();
			it != n->Nodes.end();
			++it)
		{
			Data* data = buildData(*it, expr);

			if (data)
			{
				mData.push_back(data);
				group->addData(data);
			}
		}

		return group;
	}

	Data* DataContainer::buildData(DataNode* n, Expressions* expr)
	{
		DL_ASSERT(n);

		Data* data = nullptr;

		switch (n->Value->Type)
		{
		case 0:
		{
			data = new Data(n->Key);
			DataGroup* group = buildGroup(n->Value->Statement, expr);
			mGroups.push_back(group);
			data->setGroup(group);
		}
		break;
		case 1:
			data = new Data(n->Key);
			data->setInt(n->Value->Integer);
			break;
		case 2:
			data = new Data(n->Key);
			data->setFloat(n->Value->Float);
			break;
		case 3:
			data = new Data(n->Key);
			data->setString(n->Value->String);
			break;
		case 4:
			data = new Data(n->Key);
			data->setBool(n->Value->Boolean);
			break;
		case 5:
		{
			data = new Data(n->Key);
			DataArray* a = buildArray(n->Value->Array, expr);
			data->setArray(a);
		}
		break;
		case 6:
		{
			data = buildExpression(n->Value->Expression, expr);

			if (data)
			{
				data->setKey(n->Key);
			}
		}
		break;
		default:
			break;
		};

		return data;
	}

	DataArray* DataContainer::buildArray(ArrayNode* n, Expressions* expr)
	{
		DL_ASSERT(n);

		DataArray* a = new DataArray;

		for (list_t<ValueNode*>::type::iterator it = n->Nodes.begin();
			it != n->Nodes.end();
			++it)
		{
			Data* dat = buildArrayValue(*it, expr);

			if (dat)
			{
				a->add(dat);
				mData.push_back(dat);
			}
		}

		return a;
	}

	Data* DataContainer::buildArrayValue(ValueNode* n, Expressions* expr)
	{
		DL_ASSERT(n);

		Data* data = nullptr;

		switch (n->Type)
		{
		case 0:
		{
			data = new Data;
			DataGroup* group = buildGroup(n->Statement, expr);
			mGroups.push_back(group);
			data->setGroup(group);
		}
		break;
		case 1:
			data = new Data;
			data->setInt(n->Integer);
			break;
		case 2:
			data = new Data;
			data->setFloat(n->Float);
			break;
		case 3:
			data = new Data;
			data->setString(n->String);
			break;
		case 4:
			data = new Data;
			data->setBool(n->Boolean);
			break;
		case 5:
		{
			data = new Data;
			DataArray* a = buildArray(n->Array, expr);
			data->setArray(a);
		}
		break;
		case 6:
		{
			data = buildExpression(n->Expression, expr);
		}
		break;
		default:
			break;
		};

		return data;
	}

	Data* DataContainer::buildExpression(ExpressionNode* n, Expressions* expr)
	{
		list_t<Data*>::type args;
		for (list_t<DataNode*>::type::iterator it = n->Nodes.begin();
			it != n->Nodes.end();
			++it)
		{
			Data* data = buildData(*it, expr);

			if (data)
			{
				//mData.push_back(data);
				args.push_back(data);
			}
		}

		Data* ret = expr->exec(n->Name, args);

		for (list_t<Data*>::type::iterator it = args.begin();
			it != args.end();
			++it)
		{
			if (ret != *it)
			{
				delete (*it);
			}
		}

		return ret;
	}
}