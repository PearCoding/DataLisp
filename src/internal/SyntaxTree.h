/*
 Copyright (c) 2014-2020, OEmercan Yazici <omercan AT pearcoding.eu>
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

namespace DL {
struct DataNode;
struct ValueNode;
struct StatementNode;
struct ExpressionNode;
struct ArrayNode;

struct DL_INTERNAL_LIB DataNode {
	string_t Key;
	ValueNode* Value;
};

enum ValueNodeType {
	VNT_Statement,
	VNT_Integer,
	VNT_Float,
	VNT_String,
	VNT_Boolean,
	VNT_Expression,
	VNT_Unknown
};
struct DL_INTERNAL_LIB ValueNode {
	ValueNodeType Type;
	union {
		StatementNode* _Statement;
		Integer _Integer;
		Float _Float;
		bool _Boolean;
		ExpressionNode* _Expression;
	};
	string_t _String; //Can not be in the union......
};

struct DL_INTERNAL_LIB StatementNode {
	string_t Name;
	vector_t<DataNode*> Nodes;
};

struct DL_INTERNAL_LIB ExpressionNode {
	string_t Name;
	vector_t<DataNode*> Nodes;
};

struct DL_INTERNAL_LIB SyntaxTree {
	vector_t<StatementNode*> Nodes;
};
} // namespace DL
