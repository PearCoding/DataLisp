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
#include "Data.h"
#include "DataGroup.h"
#include "DataContainer.h"
#include "SourceLogger.h"
#include "VM.h"

#include <sstream>

namespace DL
{
	namespace Expressions
	{
		Data anonymous_func(const list_t<Data>::type& args, VM& vm)
		{
			Data d = union_func(args, vm);
			
			if(d.type() == Data::T_Group)
			{
				DataGroup grp;
				for(size_t i = 0; i < d.getGroup().anonymousCount(); ++i)
					grp.add(d.getGroup().at(i));

				Data r;
				r.setGroup(grp);
				return r;
			}
			
			return d;
		}

		Data named_func(const list_t<Data>::type& args, VM& vm)
		{
			Data d = union_func(args, vm);
			
			if(d.type() != Data::T_Group)
				return Data();
			
			if(d.getGroup().getNamedEntries().empty())
				return Data();

			DataGroup grp;
			for (vector_t<Data>::type::const_iterator it = d.getGroup().getNamedEntries().begin();
				 it != d.getGroup().getNamedEntries().end();
				 ++it)
			{
				grp.add(*it);
			}

			Data r;
			r.setGroup(grp);
			return r;
		}

		Data union_func(const list_t<Data>::type& args, VM& vm)
		{
			if(args.size() == 0)
			{
				return Data();
			}
			else if(args.size() == 1)
			{
				return args.front();
			}
			else 
			{
				DataGroup grp;
				for (list_t<Data>::type::const_iterator it = args.begin();
					 it != args.end();
					 ++it)
				{					
					if(it->type() == Data::T_Group)
					{
						for (vector_t<Data>::type::const_iterator it2 = it->getGroup().getNamedEntries().begin();
							it2 != it->getGroup().getNamedEntries().end();
							++it2)
						{
							grp.add(*it2);
						}

						for(size_t i = 0; i < it->getGroup().anonymousCount(); ++i)
						{
							grp.add(it->getGroup().at(i));
						}
					}
					else
					{
						grp.add(*it);
					}
				}

				Data r;
				r.setGroup(grp);
				return r;
			}
		}
	}
}