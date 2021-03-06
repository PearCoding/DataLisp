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

#include "DataGroup.h"

namespace DL {
/** @class Data Data.h DL/Data.h
 * @brief Main class encapsulating all types of data useable by %DataLisp
 *
 * The data can be anonymous (key is empty) or non anonymous (key is set).<br>
 * Resembles a source code entry of type:
 * @code{.lisp}
 * ;...
 * :KEY ;Content
 * ;...
 * @endcode
 */
class DL_LIB Data {
public:
	/**
	 * @brief Constructs a data with the given key
	 *
	 * Initial type is @link DT_None @endlink, therefor initially the data is invalid.
	 * @param key If empty an anonymous data will be created, non anonymous otherwise
	 */
	Data(const string_t& key = "");

	/**
	 * @brief Constructs a data with the given key and group
	 *
	 * Initial type is @link DT_Group @endlink.
	 * @param key If empty an anonymous data will be created, non anonymous otherwise
	 * @param grp DataGroup to initialize with
	 */
	Data(const string_t& key, const DataGroup& grp);

	/**
	 * @brief Constructs a data with the given key and integer
	 *
	 * Initial type is @link DT_Integer @endlink.
	 * @param key If empty an anonymous data will be created, non anonymous otherwise
	 * @param i Integer to initialize with
	 */
	Data(const string_t& key, Integer i);

	/**
	 * @brief Constructs a data with the given key and floating point number
	 *
	 * Initial type is @link DT_Float @endlink.
	 * @param key If empty an anonymous data will be created, non anonymous otherwise
	 * @param f Float to initialize with
	 */
	Data(const string_t& key, Float f);

	/**
	 * @brief Constructs a data with the given key and bool
	 *
	 * Initial type is @link DT_Bool @endlink.
	 * @param key If empty an anonymous data will be created, non anonymous otherwise
	 * @param b Boolean to initialize with
	 */
	Data(const string_t& key, bool b);

	/**
	 * @brief Constructs a data with the given key and string
	 *
	 * Initial type is @link DT_String @endlink.
	 * @param key If empty an anonymous data will be created, non anonymous otherwise
	 * @param str String to initialize with
	 */
	Data(const string_t& key, const string_t& str);

	~Data();

	/**
	 * @brief Returns the @p key (also called @p id)
	 *
	 * The data is anonymous if key is empty, non anonyous otherwise.
	 * @see setKey
	 */
	inline string_t key() const;

	/**
	 * @brief Sets the key of the data
	 *
	 * The data is anonymous if key is empty, non anonyous otherwise.
	 * @see key
	 */
	inline void setKey(const string_t& key);

	/**
	 * @brief Returns the type of the encapsulated value
	 *
	 * @link DT_None @endlink stands for an invalid data.
	 * @see Type
	 */
	inline DataType type() const;

	/**
	 * @brief Checks if data is valid to use
	 *
	 * Equals to @code{.cpp} type() != DT_None @endcode
	 */
	inline bool isValid() const;

	/**
	 * @brief Returns encapsulated DataGroup value
	 *
	 * Only valid if type is @link DT_Group @endlink.
	 * @see setGroup
	 */
	inline const DataGroup& getGroup() const;

	/**
	 * @brief Returns encapsulated DataGroup value
	 *
	 * Only valid if type is @link DT_Group @endlink.
	 * @see setGroup
	 */
	inline DataGroup& getGroup();

	/**
	 * @brief Set data to a DataGroup value. Type will be set to @link DT_Group @endlink
	 * @see getGroup
	 */
	inline void setGroup(const DataGroup& grp);

	/**
	 * @brief Returns encapsulated integer value
	 *
	 * Only valid if type is @link DT_Integer @endlink.
	 * @see setInt
	 */
	inline Integer getInt() const;

	/**
	 * @brief Set data to an integer value. Type will be set to @link DT_Integer @endlink
	 * @see getInt
	 */
	inline void setInt(Integer);

	/**
	 * @brief Returns encapsulated float value
	 *
	 * Only valid if type is @link DT_Float @endlink.
	 * @see setFloat
	 */
	inline Float getFloat() const;

	/**
	 * @brief Set data to a float value. Type will be set to @link DT_Float @endlink
	 * @see getFloat
	 */
	inline void setFloat(Float);

	/**
	 * @brief Returns encapsulated integer or float value as a float value
	 *
	 * Only valid if type is @link DT_Integer @endlink or @link DT_Float @endlink.
	 * @see isNumber
	 */
	inline Float getNumber() const;

	/**
	 * @brief Checks if value is a number
	 *
	 * A number is defined as a type of @link DT_Integer @endlink or @link DT_Float @endlink.
	 * @see getNumber
	 */
	inline bool isNumber() const;

	/**
	 * @brief Returns encapsulated boolean value
	 *
	 * Only valid if type is @link DT_Bool @endlink.
	 * @see setBool
	 */
	inline bool getBool() const;

	/**
	 * @brief Set data to a boolean value. Type will be set to @link DT_Bool @endlink
	 * @see getBool
	 */
	inline void setBool(bool);

	/**
	 * @brief Returns encapsulated string value
	 *
	 * Only valid if type is @link DT_String @endlink.
	 * @see setString
	 */
	inline string_t getString() const;

	/**
	 * @brief Set data to a string value. Type will be set to @link DT_String @endlink
	 * @see getString
	 */
	inline void setString(const string_t&);

private:
	string_t mKey;

	DataType mType;
	union {
		Integer mInt;
		Float mFloat;
		bool mBool;
	};
	DataGroup mGroup;
	string_t mString;
};
} // namespace DL

#define _DL_DATA_INL_
#include "Data.inl"
#undef _DL_DATA_INL_
