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
#include "DataContainer.h"
#include "DataGroup.h"
#include "SourceLogger.h"
#include "VM.h"
#include "internal/Expressions.h"

#include <sstream>

namespace DL {
namespace Expressions {
Data if_func(const vector_t<Data>& args, VM& vm)
{
	if (args.size() != 3 && args.size() != 2) {
		vm.logger()->log(L_Error, "Invalid arguments given for $(if ...)");
		return Data();
	}

	vector_t<Data> arr = args;

	Data cond = arr[0];
	Data succ = arr[1];

	Data fail;
	if (arr.size() == 3)
		fail = arr[2];

	Data conv = vm.castTo(cond, DT_Bool);

	bool c = false;
	if (conv.type() == DT_Bool) {
		c = conv.getBool();
	} else {
		vm.logger()->log(L_Error, "Non boolean condition given for $(if ...)");
		return Data();
	}

	return c ? succ : fail;
}

static Data not_func_e(const Data& d, VM& vm)
{
	Data r = vm.castTo(d, DT_Bool);
	r.setBool(!r.getBool());
	return r;
}

Data not_func(const vector_t<Data>& args, VM& vm)
{
	return vm.doElementWise(not_func_e, args);
}

Data and_func(const vector_t<Data>& args, VM& vm)
{
	if (args.size() == 0) {
		return Data();
	} else if (args.size() == 1) {
		return vm.castTo(args.front(), DT_Bool);
	} else {
		bool b = true;

		for (const Data& d : args) {
			Data e = vm.castTo(d, DT_Bool);
			if (!e.isValid() || !e.getBool()) {
				b = false;
				break;
			}
		}

		Data r;
		r.setBool(b);
		return r;
	}
}

Data or_func(const vector_t<Data>& args, VM& vm)
{
	if (args.size() == 0) {
		return Data();
	} else if (args.size() == 1) {
		return vm.castTo(args.front(), DT_Bool);
	} else {
		bool b = false;

		for (const Data& d : args) {
			Data e = vm.castTo(d, DT_Bool);
			if (e.isValid() && e.getBool()) {
				b = true;
				break;
			}
		}

		Data r;
		r.setBool(b);
		return r;
	}
}
} // namespace Expressions
} // namespace DL