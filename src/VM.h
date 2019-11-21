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

#include "Data.h"

namespace DL {
class DataContainer;
class SourceLogger;

/** @class VM VM.h DL/VM.h
 * @brief Utility class for expressions
 */
class DL_LIB VM {
public:
	/**
	 * @brief Constructs the class with the assigned container and logger
	 */
	VM(DataContainer& container, SourceLogger* logger);
	~VM();

	/**
	 * @brief Returns assigned DataContainer
	 */
	inline const DataContainer& container() const { return mContainer; }

	/**
	 * @brief Returns assigned DataContainer
	 */
	inline DataContainer& container() { return mContainer; }

	/**
	 * @brief Returns assigned SourceLogger
	 */
	inline SourceLogger* logger() const { return mLogger; }

	/**
	 * @brief Casts data to type based on casting table
	 *
	 * @param d The original data
	 * @param type The new type to cast to
	 * @param isExplicit Cast explicit or implicit
	 * @return Data of the new type or @link DT_None @endlink if failure
	 */
	Data castTo(const Data& d, DL::DataType type, bool isExplicit = false);

	typedef Data (*element_expr_t)(const Data&, VM&); ///< Elementwise expression callback

	/**
	 * @brief Call function per element
	 *
	 * @param expr The expression to call per element
	 * @param args Array to iterate over
	 * @return Data of type @link DT_Group @endlink or @link DT_None @endlink if failure
	 */
	Data doElementWise(element_expr_t expr, const list_t<Data>::type& args);

private:
	DataContainer& mContainer;
	SourceLogger* mLogger;
};
} // namespace DL
