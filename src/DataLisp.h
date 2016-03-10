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
#pragma once

#include "dl_Config.h"
#include "Expressions.h"

namespace DL
{
	struct SyntaxTree;
	struct DataNode;
	struct StatementNode;
	struct ValueNode;
	struct ExpressionNode;

	class DataContainer;
	class Data;
	class DataGroup;
	class DataArray;

	class SourceLogger;

	class DL_LIB DataLisp
	{
	public:
		DataLisp(SourceLogger* log);
		~DataLisp();

		void parse(const string_t& source);

		void build(DataContainer* container);
		static string_t generate(DataContainer* container);

		void addExpression(const string_t& name, expr_t handler);
		expr_t expression(const string_t& name);

		string_t dump();
	private:
		static string_t dumpNode(StatementNode* node, int depth);
		static string_t dumpNode(DataNode* node, int depth);
		static string_t dumpNode(ValueNode* node, int depth);
		static string_t dumpNode(ExpressionNode* node, int depth);

		static string_t generateDataGroup(DataGroup* d, int depth);
		static string_t generateData(Data* d, int depth);
		static string_t generateArray(DataArray* d, int depth);
		static string_t generateValue(Data* d, int depth);

		static void deleteNode(StatementNode* n);
		static void deleteNode(DataNode* n);
		static void deleteNode(ValueNode* n);
		static void deleteNode(ExpressionNode* n);

		SyntaxTree* mTree;
		SourceLogger* mLogger;
		Expressions mExpressions;
	};
}
