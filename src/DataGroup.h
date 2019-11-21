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
#include "DataType.h"

#include <memory>

namespace DL {
class Data;

/** @class DataGroup DataGroup.h DL/DataGroup.h
 * @brief Capsulate a generic group identified with an id
 *
 * A generic group is an array if the condition holds:<br>
 *   @p id() is empty<br>
 * Resembles an entry of type:
 * @code{.lisp}
 * (ID
 *   ;...
 * )
 * @endcode
 * or if anonymous:
 * @code{.lisp}
 * [
 *   ;...
 * ]
 * @endcode
 *
 * @attention This class uses reference counting without Copy on Write.<br>
 * Every change will be transferred to other instances aswell.
 * @attention This class is not thread safe!
 */
class DL_LIB DataGroup {
public:
	/**
	 * @brief Constructs a fresh group
	 * @param id The id of the new created group. Let it empty to create an array
	 */
	DataGroup(const string_t& id = "");

	/**
	 * @brief References container and increases reference count
	 */
	DataGroup(const DataGroup& other);

	/**
	 * @brief Moves container content
	 */
	DataGroup(DataGroup&& o) noexcept;

	/**
	 * @brief Deconstructs class
	 *
	 * Decreases reference count.
	 * If reference count reaches zero internal content will be deleted.
	 */
	~DataGroup();

	/**
	 * @brief Assigns container and increases reference count
	 */
	DataGroup& operator=(const DataGroup& other);

	/**
	 * @brief Moves container content
	 */
	DataGroup& operator=(DataGroup&& other) noexcept;

	/**
	 * @brief Current count of references
	 */
	uint32 referenceCount() const;

	/**
	 * @brief Sets the reference count to 1 and copies data
	 */
	void makeUnique();

	/**
	 * @brief Returns id
	 */
	string_t id() const;

	/**
	 * @brief Replaces id with given one
	 * @param str New id name
	 */
	void setID(const string_t& str);

	/**
	 * @brief Adds (anonymous or non anonymous) data into group
	 * @param data Data to add. Can be anonymous or non anonymous
	 */
	void add(const Data& data);

	/**
	 * @brief Returns anonymous data from position i
	 * @param i Index of the anonymous data
	 * @return Data at position i if available, invalid Data otherwise
	 */
	Data at(size_t i) const;

	/**
	 * @brief Amount of anonymous data inside the group
	 */
	size_t anonymousCount() const;

	/**
	 * @brief Returns non anonymous data with the given id
	 *
	 * If more than one data has the same id. The first one will be returned.<br>
	 * Use getAllFromKey to return all instances.
	 * @param str The id of the non anonymous data
	 * @return Data with the given id if available, invalid Data otherwise
	 * @see getAllFromKey
	 */
	Data getFromKey(const string_t& str) const;

	/**
	 * @brief Returns all non anonymous data with the given id
	 * @param str The id of the non anonymous data
	 * @see getFromKey
	 */
	vector_t<Data>::type getAllFromKey(const string_t& key) const;

	/**
	 * @brief Checks if id is available
	 * @param key The id of the non anonymous data
	 * @return Returns true if group has the data with the given id, false otherwise
	 */
	bool hasKey(const string_t& key) const;

	/**
	 * @brief Checks if group is an array (anonymous group)
	 *
	 * A generic group is an array when the condition holds:
	 *   @p id() is empty
	 * @return Returns true if group has the data with the given id, false otherwise
	 */
	bool isArray() const;

	/**
	 * @brief Returns all data entries
	 *
	 * It is more efficient to iterate over named and anonymous entries separately.
	 */
	vector_t<Data>::type getAllEntries() const;

	/**
	 * @brief Returns all non anonymous data entries
	 */
	const vector_t<Data>::type& getNamedEntries() const;

	/**
	 * @brief Returns all anonymous data entries
	 */
	const vector_t<Data>::type& getAnonymousEntries() const;

	/**
	 * @brief Checks if all data is a number
	 * @return Returns true if all entries are numbers, false otherwise
	 * @see DL::Data::isNumber
	 */
	bool isAllNumber() const;

	/**
	 * @brief Checks if all anonymous data is a number
	 * @return Returns true if all entries are numbers, false otherwise
	 * @see DL::Data::isNumber
	 */
	bool isAllAnonymousNumber() const;

	/**
	 * @brief Checks if all anonymous data is a number
	 * @return Returns true if all entries are numbers, false otherwise
	 * @see DL::Data::isNumber
	 */
	bool isAllNamedNumber() const;

	/**
	 * @brief Checks if all data is of the given type
	 * @return Returns true if all entries are of the given, false otherwise
	 */
	bool isAllOfType(DL::DataType type) const;

	/**
	 * @brief Checks if all anonymous data is of the given type
	 * @return Returns true if all entries are of the given type, false otherwise
	 */
	bool isAllAnonymousOfType(DL::DataType type) const;

	/**
	 * @brief Checks if all anonymous data is of the given type
	 * @return Returns true if all entries are of the given type, false otherwise
	 */
	bool isAllNamedOfType(DL::DataType type) const;

private:
	std::shared_ptr<struct DataInternal> mShared;
};
} // namespace DL
