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

#include "DataGroup.h"

namespace DL
{
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
	class DL_LIB Data
	{
	public:
		/// Type of encapsulated value
		enum Type
		{
			T_Group, 	///< DataGroup instance
			T_Integer,	///< Integer number 
			T_Float,	///< Floating point number
			T_Bool,		///< Boolean value with two states (@p true and @p false)
			T_String,	///< String value
			T_None		///< No data. Invalid state
		};

		/**
		 * @brief Constructs a data with the given key
		 *
		 * Initial type is @link T_None @endlink, therefor initially the data is invalid.
		 * @param key If empty an anonymous data will be created, non anonymous otherwise
		 */
		Data(const string_t& key = "");
		~Data();

		/**
		 * @brief Returns the @p key (also called @p id)
		 *
		 * The data is anonymous if key is empty, non anonyous otherwise.
		 * @see setKey
		 */
		inline string_t key() const
		{
			return mKey;
		}

		/**
		 * @brief Sets the key of the data
		 *
		 * The data is anonymous if key is empty, non anonyous otherwise.
		 * @see key
		 */
		void setKey(const string_t& key);

		/**
		 * @brief Returns the type of the encapsulated value
		 * 
		 * @link T_None @endlink stands for an invalid data.
		 * @see Type
		 */
		inline Type type() const
		{
			return mType;
		}

		/**
		 * @brief Checks if data is valid to use
		 *
		 * Equals to @code{.cpp} type() != T_None @endcode
		 */ 
		inline bool isValid() const
		{
			return mType != T_None;
		}

		/**
		 * @brief Returns encapsulated DataGroup value
		 *
		 * Only valid if type is @link T_Group @endlink.
		 * @see setGroup
		 */
		const DataGroup& getGroup() const;

		/**
		 * @brief Set data to a DataGroup value. Type will be set to @link T_Group @endlink
		 * @see getGroup
		 */
		void setGroup(const DataGroup& grp);

		/**
		 * @brief Returns encapsulated integer value
		 *
		 * Only valid if type is @link T_Integer @endlink.
		 * @see setInt
		 */
		Integer getInt() const;

		/**
		 * @brief Set data to an integer value. Type will be set to @link T_Integer @endlink
		 * @see getInt
		 */
		void setInt(Integer);

		/**
		 * @brief Returns encapsulated float value
		 *
		 * Only valid if type is @link T_Float @endlink.
		 * @see setFloat
		 */
		Float getFloat() const;

		/**
		 * @brief Set data to a float value. Type will be set to @link T_Float @endlink
		 * @see getFloat
		 */
		void setFloat(Float);

		/**
		 * @brief Returns encapsulated integer or float value as a float value
		 *
		 * Only valid if type is @link T_Integer @endlink or @link T_Float @endlink.
		 * @see isNumber
		 */
		Float getNumber() const;

		/**
		 * @brief Checks if value is a number
		 *
		 * A number is defined as a type of @link T_Integer @endlink or @link T_Float @endlink.
		 * @see getNumber
		 */
		bool isNumber() const;

		/**
		 * @brief Returns encapsulated boolean value
		 *
		 * Only valid if type is @link T_Bool @endlink.
		 * @see setBool
		 */
		bool getBool() const;

		/**
		 * @brief Set data to a boolean value. Type will be set to @link T_Bool @endlink
		 * @see getBool
		 */
		void setBool(bool);

		/**
		 * @brief Returns encapsulated string value
		 *
		 * Only valid if type is @link T_String @endlink.
		 * @see setString
		 */
		string_t getString() const;

		/**
		 * @brief Set data to a string value. Type will be set to @link T_String @endlink
		 * @see getString
		 */
		void setString(const string_t&);
	private:
		string_t mKey;

		Type mType;
		union
		{
			Integer mInt;
			Float mFloat;
			bool mBool;
		};
		DataGroup mGroup;
		string_t mString;
	};
}
