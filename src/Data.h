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

namespace DL
{
	class DataArray;
	class DataContainer;
	class DataGroup;
	class DL_LIB Data
	{
	public:
		enum Type
		{
			T_Group,
			T_Integer,
			T_Float,
			T_Bool,
			T_String,
			T_Array
		};

		Data(const string_t& key = "");
		~Data();

		inline string_t key() const
		{
			return mKey;
		}

		void setKey(const string_t& key);

		inline Type isType() const
		{
			return mType;
		}

		DataGroup* getGroup() const;
		void setGroup(DataGroup*);

		DataArray* getArray() const;
		void setArray(DataArray*);

		int32 getInt() const;
		void setInt(int32);

		float getFloat() const;
		float getFloatConverted() const;
		void setFloat(float);

		bool getBool() const;
		void setBool(bool);

		string_t getString() const;
		void setString(const string_t&);

		bool isNumber() const;
	private:
		string_t mKey;

		Type mType;
		union
		{
			DataGroup* mGroup;
			DataArray* mArray;
			int32 mInt;
			float mFloat;
			bool mBool;
		};
		string_t mString;
	};
}
