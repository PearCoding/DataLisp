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
#include "Expressions.h"
#include "DataGroup.h"
#include "DataContainer.h"
#include "SourceLogger.h"
#include "VM.h"

#include <sstream>

#include "helper.h"

namespace DL
{
	namespace Expressions
	{
		Data if_func(const list_t<Data>::type& args, VM& vm)
		{
			if (args.size() != 3 && args.size() != 2)
			{
				vm.logger()->log(L_Error, "Invalid arguments given for $(if ...)");
				return Data();
			}

			list_t<Data>::type arr = args;

			Data cond = arr.front();
			arr.pop_front();

			Data succ = arr.front();
			arr.pop_front();

			Data fail;
			if (!arr.empty())
				fail = arr.front();

			Data conv = castTo(cond, Data::T_Bool, vm);

			bool c = false;
			if (conv.type() == Data::T_Bool)
			{
				c = conv.getBool();
			}
			else
			{
				vm.logger()->log(L_Error, "Non boolean condition given for $(if ...)");
				return Data();
			}

			return c ? succ : fail;
		}

		Data not_func_e(const Data& d, VM& vm)
		{
			Data r = castTo(d, Data::T_Bool, vm);
			r.setBool(!r.getBool());
			return r;
		}

		Data not_func(const list_t<Data>::type& args, VM& vm)
		{
			return doElementWise(not_func_e, args, vm);
		}

		Data and_func(const list_t<Data>::type& args, VM& vm)
		{
			if(args.size() == 0)
			{
				return Data();
			}
			else if(args.size() == 1)
			{
				return castTo(args.front(), Data::T_Bool, vm);
			}
			else
			{
				bool b = true;
				
				for (const Data& d : args)
				{
					Data e = castTo(d, Data::T_Bool, vm);
					if(!e.isValid() || !e.getBool())
					{
						b = false;
						break;
					}
				}

				Data r;
				r.setBool(b);
				return r;
			}
		}

		Data or_func(const list_t<Data>::type& args, VM& vm)
		{
			if(args.size() == 0)
			{
				return Data();
			}
			else if(args.size() == 1)
			{
				return castTo(args.front(), Data::T_Bool, vm);
			}
			else
			{
				bool b = false;
				
				for (const Data& d : args)
				{
					Data e = castTo(d, Data::T_Bool, vm);
					if(e.isValid() && e.getBool())
					{
						b = true;
						break;
					}
				}

				Data r;
				r.setBool(b);
				return r;
			}
		}
	}
}