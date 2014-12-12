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
#include "DataLisp.h"
#include "Parser.h"
#include "DataContainer.h"

#include "Data.h"
#include "DataGroup.h"
#include "DataArray.h"

#include "SourceLogger.h"

#include <sstream>

namespace DL
{
	DataLisp::DataLisp(SourceLogger* log) :
		mTree(nullptr), mLogger(log), mExpressions(log)
	{
		DL_ASSERT(log);
	}

	DataLisp::~DataLisp()
	{
		if (mTree)
		{
			for (list_t<StatementNode*>::type::iterator it = mTree->Nodes.begin();
				it != mTree->Nodes.end();
				++it)
			{
				deleteNode(*it);
			}

			delete mTree;
		}
	}

	void DataLisp::parse(const string_t& source)
	{
		DL_ASSERT(mTree == nullptr);
		Parser parser(source, mLogger);
		mTree = parser.parse();
	}

	void DataLisp::build(DataContainer* container)
	{
		DL_ASSERT(container);
		container->build(mTree, &mExpressions);
	}

	string_t DataLisp::generate(DataContainer* container)
	{
		DL_ASSERT(container);

		string_t output;

		list_t<DataGroup*>::type top = container->getTopGroups();
		for (list_t<DataGroup*>::type::const_iterator it = top.begin();
			it != top.end();
			++it)
		{
			output += generateDataGroup(*it, 0) + "\n";
		}

		return output;
	}

	string_t DataLisp::dump()
	{
		if (!mTree)
		{
			return "";
		}
		else
		{
			string_t str;
			for (list_t<StatementNode*>::type::const_iterator it = mTree->Nodes.begin();
				it != mTree->Nodes.end();
				++it)
			{
				str += dumpNode((*it), 1);
			}
			return str;
		}
	}

	string_t DataLisp::dumpNode(StatementNode* n, int depth)
	{
		string_t white;
		for (int i = 0; i < depth; ++i)
		{
			white += " ";
		}

		string_t str;
		str = white + "Statement {" + n->Name + "\n";

		for (list_t<DataNode*>::type::const_iterator it = n->Nodes.begin();
			it != n->Nodes.end();
			++it)
		{
			str += dumpNode((*it), depth + 1);
		}

		str += white + "}\n";

		return str;
	}

	string_t DataLisp::dumpNode(ExpressionNode* n, int depth)
	{
		string_t white;
		for (int i = 0; i < depth; ++i)
		{
			white += " ";
		}

		string_t str;
		str = white + "Expression {" + n->Name + "\n";

		for (list_t<DataNode*>::type::const_iterator it = n->Nodes.begin();
			it != n->Nodes.end();
			++it)
		{
			str += dumpNode((*it), depth + 1);
		}

		str += white + "}\n";

		return str;
	}

	string_t DataLisp::dumpNode(DataNode* n, int depth)
	{
		string_t white;
		for (int i = 0; i < depth; ++i)
		{
			white += " ";
		}

		string_t str;
		str += white + "Data (" + n->Key + ":\n";

		str += dumpNode(n->Value, depth);

		str += white + ")\n";
		return str;
	}

	string_t DataLisp::dumpNode(ValueNode* n, int depth)
	{
		string_t white;
		for (int i = 0; i < depth; ++i)
		{
			white += " ";
		}

		string_t str;

		switch (n->Type)
		{
		case 0:
			str += dumpNode(n->Statement, depth + 1);
			break;
		case 1:/* TODO: string_t */
		{
			std::stringstream stream;
			stream << n->Integer;

			str += stream.str();
		}
		break;
		case 2:
		{
			std::stringstream stream;
			stream << n->Float;

			str += stream.str();
		}
		break;
		case 3:
			str += "\"" + n->String + "\"";
			break;
		case 4:
			if (n->Boolean)
			{
				str += "true";
			}
			else
			{
				str += "false";
			}
			break;
		case 5:
			for (list_t<ValueNode*>::type::const_iterator it = n->Array->Nodes.begin();
				it != n->Array->Nodes.end();
				++it)
			{
				str += dumpNode((*it), depth + 1);
			}
			break;
		case 6:
			str += dumpNode(n->Expression, depth + 1);
			break;
		default:
			str += "UNKNOWN";
		}
		return white + str + "\n";
	}

	string_t DataLisp::generateDataGroup(DataGroup* d, int depth)
	{
		DL_ASSERT(d);

		string_t white;
		for (int i = 0; i < depth; ++i)
		{
			white += " ";
		}

		string_t str;

		str = white + "(" + d->id() + "\n";

		for (size_t i = 0; i < d->unnamedCount(); ++i)
		{
			str += generateData(d->at(i), depth + 1) + "\n";
		}

		list_t<Data*>::type list = d->getNamedEntries();
		for (list_t<Data*>::type::const_iterator it = list.begin();
			it != list.end();
			++it)
		{
			str += generateData(*it, depth + 1) + "\n";
		}
		str += white + ")";

		return str;
	}

	string_t DataLisp::generateData(Data* d, int depth)
	{
		DL_ASSERT(d);

		string_t white;
		for (int i = 0; i < depth; ++i)
		{
			white += " ";
		}

		string_t str;

		if (d->key().empty())
		{
			str = generateValue(d, depth + 1);
		}
		else
		{
			str = ":" + d->key() + "  " + generateValue(d, depth + 1);
		}
		return white + str;
	}

	string_t DataLisp::generateArray(DataArray* d, int depth)
	{
		DL_ASSERT(d);

		string_t white;
		for (int i = 0; i < depth; ++i)
		{
			white += " ";
		}

		string_t str;

		if (d->size() == 0)
		{
			str = "[]";
		}
		else
		{
			str = "[" + generateValue(d->at(0), depth + 1) + ",\n";

			for (size_t i = 1; i < d->size(); ++i)
			{
				str += white + generateValue(d->at(i), depth + 1) + ",\n";
			}

			str += white + "]";
		}

		return str;
	}

	string_t DataLisp::generateValue(Data* d, int depth)
	{
		DL_ASSERT(d);
		string_t str;

		switch (d->isType())
		{
		case Data::T_Group:
			str = generateDataGroup(d->getGroup(), depth);
			break;
		case Data::T_Array:
			str = generateArray(d->getArray(), depth);
			break;
		case Data::T_Bool:
			if (d->getBool())
			{
				str = "true";
			}
			else
			{
				str = "false";
			}
			break;
		case Data::T_Float:
		{
			std::stringstream stream;
			stream << d->getFloat();
			str = stream.str();
		}
		break;
		case Data::T_Integer:
		{
			std::stringstream stream;
			stream << d->getInt();
			str = stream.str();
		}
		break;
		case Data::T_String:
			str = "\"" + d->getString() + "\"";
			break;
		}

		return str;
	}

	void DataLisp::deleteNode(StatementNode* n)
	{
		if (!n)
		{
			return;
		}

		for (list_t<DataNode*>::type::const_iterator it = n->Nodes.begin();
			it != n->Nodes.end();
			++it)
		{
			deleteNode(*it);
		}

		delete n;
	}

	void DataLisp::deleteNode(ExpressionNode* n)
	{
		if (!n)
		{
			return;
		}

		for (list_t<DataNode*>::type::const_iterator it = n->Nodes.begin();
			it != n->Nodes.end();
			++it)
		{
			deleteNode(*it);
		}

		delete n;
	}

	void DataLisp::deleteNode(DataNode* n)
	{
		if (!n)
		{
			return;
		}

		deleteNode(n->Value);

		delete n;
	}

	void DataLisp::deleteNode(ValueNode* n)
	{
		if (!n)
		{
			return;
		}

		if (n->Type == 0)
		{
			deleteNode(n->Statement);
		}
		else if (n->Type == 5)
		{
			for (list_t<ValueNode*>::type::const_iterator it = n->Array->Nodes.begin();
				it != n->Array->Nodes.end();
				++it)
			{
				deleteNode(*it);
			}
		}
		else if (n->Type == 6)
		{
			deleteNode(n->Expression);
		}

		delete n;
	}

	void DataLisp::addExpression(const string_t& name, expr_t handler)
	{
		mExpressions.addExpression(name, handler);
	}

	expr_t DataLisp::expression(const string_t& name)
	{
		return mExpressions.expression(name);
	}
}