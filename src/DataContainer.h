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
	class Data;
	class DataGroup;
	class DataArray;

	struct SyntaxTree;
	struct StatementNode;
	struct DataNode;
	struct ArrayNode;
	struct ValueNode;

	class DL_LIB DataContainer
	{
	public:
		DataContainer();
		~DataContainer();

		void build(SyntaxTree* tree);
	
		std::list<DataGroup*> getTopGroups() const;
		std::list<DataGroup*> getAllGroupsID(const string_t& id) const;
		std::list<Data*> getAllDataFromKey(const string_t& key) const;
	
		void addTopGroup(DataGroup* group);
	private:
		DataGroup* buildGroup(StatementNode* n);
		Data* buildData(DataNode* n);
		DataArray* buildArray(ArrayNode* n);
		Data* buildArrayValue(ValueNode* n);

		std::list<DataGroup*> mTopGroups;

		std::list<DataGroup*> mGroups;
		std::list<Data*> mData;
	};
}
