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
#include "DataLisp.h"
#include "Parser.h"
#include "DataContainer.h"
#include "Expressions.h"

#include "Data.h"
#include "DataGroup.h"
#include "DataArray.h"

#include "SourceLogger.h"

#include <sstream>

namespace DL
{
	class DataLisp_Internal
	{
	public:
		typedef map_t<string_t, expr_t>::type ExpressionMap;

		DataLisp_Internal(SourceLogger* logger):
			mTree(nullptr), mLogger(logger), mExpressions()
		{}

		~DataLisp_Internal()
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

		static string_t dumpNode(StatementNode* n, int depth)
		{
			string_t white;
			for (int i = 0; i < depth; ++i)
				white += " ";

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

		static string_t dumpNode(ExpressionNode* n, int depth)
		{
			string_t white;
			for (int i = 0; i < depth; ++i)
				white += " ";

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

		static string_t dumpNode(DataNode* n, int depth)
		{
			string_t white;
			for (int i = 0; i < depth; ++i)
				white += " ";

			string_t str;
			str += white + "Data (" + n->Key + ":\n";

			str += dumpNode(n->Value, depth);

			str += white + ")\n";
			return str;
		}

		static string_t dumpNode(ValueNode* n, int depth)
		{
			string_t white;
			for (int i = 0; i < depth; ++i)
				white += " ";

			string_t str;

			switch (n->Type)
			{
			case VNT_Statement:
				str += dumpNode(n->_Statement, depth + 1);
				break;
			case VNT_Integer:/* TODO: string_t */
			{
				std::stringstream stream;
				stream << n->_Integer;

				str += stream.str();
			}
			break;
			case VNT_Float:
			{
				std::stringstream stream;
				stream << n->_Float;

				str += stream.str();
			}
			break;
			case VNT_String:
				str += "\"" + n->_String + "\"";
				break;
			case VNT_Boolean:
				if (n->_Boolean)
					str += "true";
				else
					str += "false";
				break;
			case VNT_Array:
				for (list_t<ValueNode*>::type::const_iterator it = n->_Array->Nodes.begin();
					it != n->_Array->Nodes.end();
					++it)
				{
					str += dumpNode((*it), depth + 1);
				}
				break;
			case VNT_Expression:
				str += dumpNode(n->_Expression, depth + 1);
				break;
			default:
				str += "UNKNOWN";
			}
			return white + str + "\n";
		}

		static string_t generateDataGroup(DataGroup* d, int depth)
		{
			DL_ASSERT(d);

			string_t white;
			for (int i = 0; i < depth; ++i)
				white += " ";

			string_t str;

			str = white + "(" + d->id() + "\n";

			for (size_t i = 0; i < d->anonymousCount(); ++i)
				str += generateData(d->at(i), depth + 1) + "\n";

			list_t<Data>::type list = d->getNamedEntries();
			for (list_t<Data>::type::const_iterator it = list.begin();
				it != list.end();
				++it)
			{
				str += generateData(*it, depth + 1) + "\n";
			}
			str += white + ")";

			return str;
		}

		static string_t generateData(const Data& d, int depth)
		{
			string_t white;
			for (int i = 0; i < depth; ++i)
				white += " ";

			string_t str;

			if (d.key().empty())
				str = generateValue(d, depth + 1);
			else
				str = ":" + d.key() + "  " + generateValue(d, depth + 1);
			return white + str;
		}

		static string_t generateArray(DataArray* d, int depth)
		{
			DL_ASSERT(d);

			string_t white;
			for (int i = 0; i < depth; ++i)
				white += " ";

			string_t str;

			if (d->size() == 0)
			{
				str = "[]";
			}
			else
			{
				str = "[" + generateValue(d->at(0), depth + 1) + ",\n";

				for (size_t i = 1; i < d->size(); ++i)
					str += white + generateValue(d->at(i), depth + 1) + ",\n";

				str += white + "]";
			}

			return str;
		}

		static string_t generateValue(const Data& d, int depth)
		{
			string_t str;

			switch (d.type())
			{
			case Data::T_Group:
				str = generateDataGroup(d.getGroup(), depth);
				break;
			case Data::T_Array:
				str = generateArray(d.getArray(), depth);
				break;
			case Data::T_Bool:
				if (d.getBool())
					str = "true";
				else
					str = "false";
				break;
			case Data::T_Float:
			{
				std::stringstream stream;
				stream << d.getFloat();
				str = stream.str();
			}
			break;
			case Data::T_Integer:
			{
				std::stringstream stream;
				stream << d.getInt();
				str = stream.str();
			}
			break;
			case Data::T_String:
				str = "\"" + d.getString() + "\"";
				break;
			case Data::T_None:
				break;
			}

			return str;
		}

		static void deleteNode(StatementNode* n)
		{
			if (!n)
				return;

			for (list_t<DataNode*>::type::const_iterator it = n->Nodes.begin();
				it != n->Nodes.end();
				++it)
			{
				deleteNode(*it);
			}

			delete n;
		}

		static void deleteNode(ExpressionNode* n)
		{
			if (!n)
				return;

			for (list_t<DataNode*>::type::const_iterator it = n->Nodes.begin();
				it != n->Nodes.end();
				++it)
			{
				deleteNode(*it);
			}

			delete n;
		}

		static void deleteNode(DataNode* n)
		{
			if (!n)
				return;

			deleteNode(n->Value);

			delete n;
		}

		static void deleteNode(ValueNode* n)
		{
			if (!n)
				return;

			if (n->Type == VNT_Statement)
			{
				deleteNode(n->_Statement);
			}
			else if (n->Type == VNT_Array)
			{
				for (list_t<ValueNode*>::type::const_iterator it = n->_Array->Nodes.begin();
					it != n->_Array->Nodes.end();
					++it)
				{
					deleteNode(*it);
				}

				delete n->_Array;
			}
			else if (n->Type == VNT_Expression)
			{
				deleteNode(n->_Expression);
			}

			delete n;
		}

		DataGroup* buildGroup(StatementNode* n, DataContainer& container)
		{
			DL_ASSERT(n);

			DataGroup* group = container.createGroup();
			group->setID(n->Name);

			for (list_t<DataNode*>::type::iterator it = n->Nodes.begin();
				it != n->Nodes.end();
				++it)
			{
				Data data = buildData(*it, container);
				if (data.isValid())
					group->addData(data);
			}

			return group;
		}

		Data buildData(DataNode* n, DataContainer& container)
		{
			DL_ASSERT(n);

			Data data;
			switch (n->Value->Type)
			{
			case VNT_Statement:
			{
				data = Data(n->Key);
				DataGroup* group = buildGroup(n->Value->_Statement, container);
				data.setGroup(group);
			}
			break;
			case VNT_Integer:
				data = Data(n->Key);
				data.setInt(n->Value->_Integer);
				break;
			case VNT_Float:
				data = Data(n->Key);
				data.setFloat(n->Value->_Float);
				break;
			case VNT_String:
				data = Data(n->Key);
				data.setString(n->Value->_String);
				break;
			case VNT_Boolean:
				data = Data(n->Key);
				data.setBool(n->Value->_Boolean);
				break;
			case VNT_Array:
			{
				data = Data(n->Key);
				DataArray* a = buildArray(n->Value->_Array, container);
				data.setArray(a);
			}
			break;
			case VNT_Expression:
			{
				data = buildExpression(n->Value->_Expression, container);
				data.setKey(n->Key);
			}
			break;
			default:
				break;
			};

			return data;
		}

		DataArray* buildArray(ArrayNode* n, DataContainer& container)
		{
			DL_ASSERT(n);

			DataArray* a = container.createArray();

			for (list_t<ValueNode*>::type::iterator it = n->Nodes.begin();
				it != n->Nodes.end();
				++it)
			{
				Data dat = buildArrayValue(*it, container);

				if (dat.isValid())
					a->add(dat);
			}

			return a;
		}

		Data buildArrayValue(ValueNode* n, DataContainer& container)
		{
			DL_ASSERT(n);

			Data data;
			switch (n->Type)
			{
			case VNT_Statement:
			{
				DataGroup* group = buildGroup(n->_Statement, container);
				data.setGroup(group);
			}
			break;
			case VNT_Integer:
				data.setInt(n->_Integer);
				break;
			case VNT_Float:
				data.setFloat(n->_Float);
				break;
			case VNT_String:
				data.setString(n->_String);
				break;
			case VNT_Boolean:
				data.setBool(n->_Boolean);
				break;
			case VNT_Array:
			{
				DataArray* a = buildArray(n->_Array, container);
				data.setArray(a);
			}
			break;
			case VNT_Expression:
				data = buildExpression(n->_Expression, container);
				break;
			default:
				break;
			};

			return data;
		}

		Data buildExpression(ExpressionNode* n, DataContainer& container)
		{
			list_t<Data>::type args;
			for (list_t<DataNode*>::type::iterator it = n->Nodes.begin();
				it != n->Nodes.end();
				++it)
			{
				Data data = buildData(*it, container);

				if (data.isValid())
					args.push_back(data);
			}

			return exec_expression(n->Name, args);
		}

		Data exec_expression(const string_t& name, const list_t<Data>::type& args)
		{
			if (!mExpressions.count(name))
			{
				std::stringstream stream;
				stream << "Couldn't find expression '" << name << "'";
				mLogger->log(L_Error, stream.str());

				return Data();
			}

			return mExpressions[name](args, mLogger);
		}

	public:
		SyntaxTree* mTree;
		SourceLogger* mLogger;
		ExpressionMap mExpressions;
	};

//---------------------------------------------------

	DataLisp::DataLisp(SourceLogger* log, bool stdlib) :
		mInternal(new DataLisp_Internal(log))
	{
		DL_ASSERT(log);

		if(stdlib)
		{
			addExpression("print", Expressions::print_func);
			addExpression("if", Expressions::if_func);
		}
	}

	DataLisp::~DataLisp()
	{
		delete mInternal;
	}

	void DataLisp::parse(const string_t& source)
	{
		DL_ASSERT(mInternal->mTree == nullptr);
		Parser parser(source, mInternal->mLogger);
		mInternal->mTree = parser.parse();
	}

	void DataLisp::build(DataContainer& container)
	{
		DL_ASSERT(mInternal->mTree);

		for (list_t<StatementNode*>::type::iterator it = mInternal->mTree->Nodes.begin();
			it != mInternal->mTree->Nodes.end();
			++it)
		{
			DataGroup* group = mInternal->buildGroup(*it, container);
			container.addTopGroup(group);
		}
	}

	string_t DataLisp::generate(const DataContainer& container)
	{		string_t output;

		list_t<DataGroup*>::type top = container.getTopGroups();
		for (list_t<DataGroup*>::type::const_iterator it = top.begin();
			it != top.end();
			++it)
		{
			output += DataLisp_Internal::generateDataGroup(*it, 0) + "\n";
		}

		return output;
	}

	string_t DataLisp::dump()
	{
		if (!mInternal->mTree)
		{
			return "";
		}
		else
		{
			string_t str;
			for (list_t<StatementNode*>::type::const_iterator it = mInternal->mTree->Nodes.begin();
				it != mInternal->mTree->Nodes.end();
				++it)
			{
				str += DataLisp_Internal::dumpNode((*it), 1);
			}
			return str;
		}
	}

	void DataLisp::addExpression(const string_t& name, expr_t handler)
	{
		mInternal->mExpressions[name] = handler;
	}

	expr_t DataLisp::expression(const string_t& name)
	{
		if (mInternal->mExpressions.count(name))
			return mInternal->mExpressions.at(name);
		else
			return nullptr;
	}
}