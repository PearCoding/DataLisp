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
#include "DataArray.h"
#include "SourceLogger.h"

#include <sstream>

namespace DL
{
	namespace Expressions
	{
		void print_val(const Data& d, std::stringstream& stream)
		{
			switch (d.type())
			{
			case Data::T_Bool:
				stream << (d.getBool() ? "true " : "false ");
				break;
			case Data::T_Integer:
				stream << d.getInt() << " ";
				break;
			case Data::T_Float:
				stream << d.getFloat() << " ";
				break;
			case Data::T_String:
				stream << d.getString() << " ";
				break;
			case Data::T_Group:
				stream << "DATA(" << d.getGroup()->id() << ") ";
				break;
			case Data::T_Array:
				stream << "[";
				for (size_t i = 0; i < d.getArray()->size(); ++i)
				{
					Data x = d.getArray()->at(i);
					print_val(x, stream);

					if (i != d.getArray()->size() - 1)
					{
						stream << ",";
					}
				}
				stream << "] ";
				break;
			default:
				stream << "###UNKNOWN### ";
				break;
			}
		}

		Data print_func(const list_t<Data>::type& args, SourceLogger* log)
		{
			std::stringstream stream;

			for (list_t<Data>::type::const_iterator it = args.begin();
				it != args.end();
				++it)
			{
				print_val(*it, stream);
			}

			log->log(L_Info, stream.str());
			return Data();
		}

		Data if_func(const list_t<Data>::type& args, SourceLogger* log)
		{
			if (args.size() != 3 && args.size() != 2)
			{
				log->log(L_Error, "Invalid arguments given for $(if ...)");
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

			bool c = false;
			if (cond.type() == Data::T_Bool)
			{
				c = cond.getBool();
			}
			else if (cond.isNumber())
			{
				c = (cond.getNumber() == 0);
			}
			else
			{
				log->log(L_Error, "Non boolean condition given for $(if ...)");
				return Data();
			}

			return c ? succ : fail;
		}
	}
}