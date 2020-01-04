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

#ifndef _DL_DATA_INL_
#error Data.inl should only be included by Data.h
#endif

namespace DL {
string_t Data::key() const
{
	return mKey;
}

void Data::setKey(const string_t& key)
{
	mKey = key;
}

DataType Data::type() const
{
	return mType;
}

bool Data::isValid() const
{
	return mType != DT_None;
}

const DataGroup& Data::getGroup() const
{
	DL_ASSERT(mType == DT_Group);
	return mGroup;
}

DataGroup& Data::getGroup()
{
	DL_ASSERT(mType == DT_Group);
	return mGroup;
}


void Data::setGroup(const DataGroup& g)
{
	mType  = DT_Group;
	mGroup = g;
}

Integer Data::getInt() const
{
	DL_ASSERT(mType == DT_Integer);
	return mInt;
}

void Data::setInt(Integer i)
{
	mType = DT_Integer;
	mInt  = i;
}

Float Data::getFloat() const
{
	DL_ASSERT(mType == DT_Float);
	return mFloat;
}

Float Data::getNumber() const
{
	DL_ASSERT(mType == DT_Float || mType == DT_Integer);

	if (mType == DT_Float)
		return mFloat;
	else
		return static_cast<Float>(mInt);
}

void Data::setFloat(Float f)
{
	mType  = DT_Float;
	mFloat = f;
}

bool Data::getBool() const
{
	DL_ASSERT(mType == DT_Bool);
	return mBool;
}

void Data::setBool(bool b)
{
	mType = DT_Bool;
	mBool = b;
}

string_t Data::getString() const
{
	DL_ASSERT(mType == DT_String);
	return mString;
}

void Data::setString(const string_t& str)
{
	mType   = DT_String;
	mString = str;
}

bool Data::isNumber() const
{
	return mType == DT_Float || mType == DT_Integer;
}
} // namespace DL
