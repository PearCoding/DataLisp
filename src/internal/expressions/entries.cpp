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
#include "Data.h"
#include "DataContainer.h"
#include "DataGroup.h"
#include "SourceLogger.h"
#include "VM.h"
#include "internal/Expressions.h"

#include <sstream>

namespace DL {
namespace Expressions {
Data anonymous_func(const list_t<Data>::type& args, VM& vm)
{
	Data d = union_func(args, vm);

	if (d.type() == DT_Group) {
		DataGroup grp;
		for (const Data& data : d.getGroup().getAnonymousEntries())
			grp.add(data);

		Data r;
		r.setGroup(grp);
		return r;
	}

	return d;
}

Data named_func(const list_t<Data>::type& args, VM& vm)
{
	Data d = union_func(args, vm);

	if (d.type() != DT_Group)
		return Data();

	if (d.getGroup().getNamedEntries().empty())
		return Data();

	DataGroup grp;
	for (const Data& data : d.getGroup().getNamedEntries())
		grp.add(data);

	Data r;
	r.setGroup(grp);
	return r;
}

Data union_func(const list_t<Data>::type& args, VM& /*vm*/)
{
	if (args.size() == 0) {
		return Data();
	} else if (args.size() == 1) {
		return args.front();
	} else {
		DataGroup grp;
		for (const Data& d : args) {
			if (d.type() == DT_Group) {
				for (const Data& data : d.getGroup().getNamedEntries())
					grp.add(data);

				for (const Data& data : d.getGroup().getAnonymousEntries())
					grp.add(data);
			} else {
				grp.add(d);
			}
		}

		Data r;
		r.setGroup(grp);
		return r;
	}
}
} // namespace Expressions
} // namespace DL