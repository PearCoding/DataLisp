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
#pragma once

#include "Config.h"

#include <list>

namespace DL
{
	struct DataNode;
	struct ValueNode;
	struct StatementNode;
	struct ArrayNode;

	struct DataNode
	{
		string_t Key;
		ValueNode* Value;
	};

	struct ArrayNode
	{
		std::list<ValueNode*> Nodes;
	};

	struct ValueNode
	{
		int Type;//0 == Statement, 1 == INTEGER, 2 == FLOAT, 3 == STRING, 4 == BOOLEAN, 5 == Array
		union
		{
			StatementNode*  Statement;
			ArrayNode*		Array;
			int32			Integer;
			float			Float;
			bool			Boolean;
		};
		string_t			String;//Can not be in the union......
	};

	struct StatementNode
	{
		string_t Name;
		std::list<DataNode*> Nodes;
	};

	struct SyntaxTree
	{
		std::list<StatementNode*> Nodes;
	};
}
