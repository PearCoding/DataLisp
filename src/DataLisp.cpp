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
#include "VM.h"
#include "Expressions.h"

#include "Data.h"
#include "DataGroup.h"

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
				for (StatementNode* ptr : mTree->Nodes)
					deleteNode(ptr);

				delete mTree;
			}
		}

		static string_t dumpNode(StatementNode* n, int depth)
		{
			string_t white;
			for (int i = 0; i < depth; ++i)
				white += " ";

			string_t str;
			if(n->Name.empty())
				str = white + "Array {" + "\n";
			else
				str = white + "Statement {" + n->Name + "\n";

			for (DataNode* ptr : n->Nodes)
				str += dumpNode(ptr, depth + 1);

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

			for (DataNode* ptr : n->Nodes)
				str += dumpNode(ptr, depth + 1);

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
			case VNT_Expression:
				str += dumpNode(n->_Expression, depth + 1);
				break;
			default:
				str += "UNKNOWN";
			}
			return white + str + "\n";
		}

		static string_t generateDataGroup(const DataGroup& d, int depth)
		{
			string_t white;
			for (int i = 0; i < depth; ++i)
				white += " ";

			string_t str;

			if(d.isArray())
				str = white + "[" + "\n";
			else
				str = white + "(" + d.id() + "\n";

			for (const Data& data : d.getAnonymousEntries())
				str += generateData(data, depth + 1) + "\n";

			for (const Data& data : d.getNamedEntries())
				str += generateData(data, depth + 1) + "\n";

			str += white + (!d.isArray() ? ")" : "]");

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

		static string_t generateValue(const Data& d, int depth)
		{
			string_t str;

			switch (d.type())
			{
			case Data::T_Group:
				str = generateDataGroup(d.getGroup(), depth);
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

			for (DataNode* ptr : n->Nodes)
				deleteNode(ptr);

			delete n;
		}

		static void deleteNode(ExpressionNode* n)
		{
			if (!n)
				return;

			for (DataNode* ptr : n->Nodes)
				deleteNode(ptr);

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
				deleteNode(n->_Statement);
			else if (n->Type == VNT_Expression)
				deleteNode(n->_Expression);

			delete n;
		}

		DataGroup buildGroup(StatementNode* n, VM& vm)
		{
			DL_ASSERT(n);

			DataGroup group(n->Name);
			for (DataNode* ptr : n->Nodes)
			{
				Data data = buildData(ptr, vm);
				if (data.isValid())
					group.add(data);
			}

			return group;
		}

		Data buildData(DataNode* n, VM& vm)
		{
			DL_ASSERT(n);

			Data data;
			switch (n->Value->Type)
			{
			case VNT_Statement:
			{
				data = Data(n->Key);
				data.setGroup(buildGroup(n->Value->_Statement, vm));
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
			case VNT_Expression:
			{
				data = buildExpression(n->Value->_Expression, vm);
				data.setKey(n->Key);
			}
			break;
			default:
				break;
			};

			return data;
		}

		Data buildExpression(ExpressionNode* n, VM& vm)
		{
			list_t<Data>::type args;
			for (DataNode* ptr : n->Nodes)
			{
				Data data = buildData(ptr, vm);

				if (data.isValid())
					args.push_back(data);
			}

			return exec_expression(n->Name, args, vm);
		}

		Data exec_expression(const string_t& name, const list_t<Data>::type& args, VM& vm)
		{
			if (!mExpressions.count(name))
			{
				std::stringstream stream;
				stream << "Couldn't find expression '" << name << "'";
				mLogger->log(L_Error, stream.str());

				return Data();
			}

			return mExpressions[name](args, vm);
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
			mInternal->mExpressions = Expressions::getStdLib();
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

		VM vm(container, mInternal->mLogger);
		for (StatementNode* ptr : mInternal->mTree->Nodes)
		{
			container.addTopGroup(mInternal->buildGroup(ptr, vm));
		}
	}

	string_t DataLisp::generate(const DataContainer& container)
	{
		string_t output;
		for (const DataGroup& grp : container.getTopGroups())
			output += DataLisp_Internal::generateDataGroup(grp, 0) + "\n";

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
			for (StatementNode* ptr : mInternal->mTree->Nodes)
				str += DataLisp_Internal::dumpNode(ptr, 1);

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