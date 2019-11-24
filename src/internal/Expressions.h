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
/**
 * @brief Contains the StdLib expressions
 */
namespace Expressions {
DL_INTERNAL_LIB map_t<string_t, expr_t> getStdLib();
DL_INTERNAL_LIB Data print_func(const vector_t<Data>& args, VM& vm);
DL_INTERNAL_LIB Data if_func(const vector_t<Data>& args, VM& vm);
DL_INTERNAL_LIB Data not_func(const vector_t<Data>& args, VM& vm);
DL_INTERNAL_LIB Data and_func(const vector_t<Data>& args, VM& vm);
DL_INTERNAL_LIB Data or_func(const vector_t<Data>& args, VM& vm);
DL_INTERNAL_LIB Data anonymous_func(const vector_t<Data>& args, VM& vm);
DL_INTERNAL_LIB Data named_func(const vector_t<Data>& args, VM& vm);
DL_INTERNAL_LIB Data union_func(const vector_t<Data>& args, VM& vm);
DL_INTERNAL_LIB Data bool_func(const vector_t<Data>& args, VM& vm);
DL_INTERNAL_LIB Data int_func(const vector_t<Data>& args, VM& vm);
DL_INTERNAL_LIB Data float_func(const vector_t<Data>& args, VM& vm);
} // namespace Expressions
} // namespace DL
