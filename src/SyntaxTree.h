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
#pragma once

#include "DataLispConfig.h"

namespace DL
{
	struct DataNode;
	struct ValueNode;
	struct StatementNode;
	struct ExpressionNode;
	struct ArrayNode;

	struct DataNode
	{
		string_t Key;
		ValueNode* Value;
	};

	struct ArrayNode
	{
		list_t<ValueNode*>::type Nodes;
	};

	enum ValueNodeType
	{
		VNT_Statement,
		VNT_Integer,
		VNT_Float,
		VNT_String,
		VNT_Boolean,
		VNT_Array,
		VNT_Expression,
		VNT_Unknown
	};
	struct ValueNode
	{
		ValueNodeType 		Type;
		union
		{
			StatementNode*  _Statement;
			ArrayNode*		_Array;
			Integer			_Integer;
			Float			_Float;
			bool			_Boolean;
			ExpressionNode* _Expression;
		};
		string_t			_String;//Can not be in the union......
	};

	struct StatementNode
	{
		string_t Name;
		list_t<DataNode*>::type Nodes;
	};

	struct ExpressionNode
	{
		string_t Name;
		list_t<DataNode*>::type Nodes;
	};

	struct SyntaxTree
	{
		list_t<StatementNode*>::type Nodes;
	};
}
