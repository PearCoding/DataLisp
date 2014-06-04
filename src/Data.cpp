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
#include "Data.h"

namespace DL
{
	Data::Data(const string_t& source) :
		mKey(source), mType(T_Group)
	{
	}

	Data::~Data()
	{
		//Groups are deleted by the container...
	}

	DataGroup* Data::getGroup() const
	{
		DL_ASSERT(mType == T_Group);
		return mGroup;
	}

	void Data::setGroup(DataGroup* g)
	{
		mType = T_Group;
		mGroup = g;
	}

	DataArray* Data::getArray() const
	{
		DL_ASSERT(mType == T_Array);
		return mArray;
	}

	void Data::setArray(DataArray* a)
	{
		mType = T_Array;
		mArray = a;
	}

	int32 Data::getInt() const
	{
		DL_ASSERT(mType == T_Integer);
		return mInt;
	}

	void Data::setInt(int32 i)
	{
		mType = T_Integer;
		mInt = i;
	}

	float Data::getFloat() const
	{
		DL_ASSERT(mType == T_Float);
		return mFloat;
	}

	float Data::getFloatConverted() const
	{
		DL_ASSERT(mType == T_Float || mType == T_Integer);

		if (mType == T_Float)
		{
			return mFloat;
		}
		else if (mType == T_Integer)
		{
			return static_cast<float>(mInt);
		}
		else
		{
			return -1;
		}
	}

	void Data::setFloat(float f)
	{
		mType = T_Float;
		mFloat = f;
	}

	bool Data::getBool() const
	{
		DL_ASSERT(mType == T_Bool);
		return mBool;
	}

	void Data::setBool(bool b)
	{
		mType = T_Bool;
		mBool = b;
	}

	string_t Data::getString() const
	{
		DL_ASSERT(mType == T_String);
		return mString;
	}

	void Data::setString(const string_t& str)
	{
		mType = T_String;
		mString = str;
	}

	bool Data::isNumber() const
	{
		return mType == T_Float || mType == T_Integer;
	}
}
