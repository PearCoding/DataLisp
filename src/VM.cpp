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
#include "VM.h"
#include "SourceLogger.h"

#include <sstream>

namespace DL
{
	VM::VM(DataContainer& container, SourceLogger* logger) :
		mContainer(container), mLogger(logger)
	{
	}

	VM::~VM() 
	{
	}

	static std::string typeToName(Data::Type type)
	{
		switch(type)
		{
		case Data::T_Bool:
			return "Bool";
		case Data::T_Integer:
			return "Integer";
		case Data::T_Float:
			return "Float";
		case Data::T_String:
			return "String";
		case Data::T_Group:
			return "Group";
		case Data::T_None:
			return "None";
		default:
			return "Unknown";
		}
	}

	Data VM::castTo(const Data& d, DL::Data::Type type, bool isExplicit)
	{
		if(d.type() == type)
			return d;
		
		switch (type)
		{
		case Data::T_Bool:
			switch(d.type())
			{
			case Data::T_Integer:
			{
				Data r(d.key());
				r.setBool(d.getInt() != 0);
				return r;
			}
			case Data::T_Float:
			{
				Data r(d.key());
				r.setBool(d.getFloat() != 0);
				return r;
			}
			default:
				break;
			}
			break;
		case Data::T_Integer:
			switch(d.type())
			{
			case Data::T_Bool:
			{
				Data r(d.key());
				r.setInt(d.getBool() ? 1 : 0);
				return r;
			}
			case Data::T_Float:
			{
				Data r(d.key());
				r.setInt(d.getFloat());
				return r;
			}
			default:
				break;
			}
			break;
		case Data::T_Float:
			switch(d.type())
			{
			case Data::T_Bool:
			{
				Data r(d.key());
				r.setFloat(d.getBool() ? 1 : 0);
				return r;
			}
			case Data::T_Integer:
			{
				if(!isExplicit)
					mLogger->log(L_Warning, "Implicit conversion from 'Integer' to 'Float'");
				
				Data r(d.key());
				r.setFloat(d.getInt());
				return r;
			}
			default:
				break;
			}
			break;
		default:
			break;
		}

		std::stringstream stream;
		stream << "Can not convert '" <<
			typeToName(d.type()) << "' to '" << typeToName(type) << "'"; 
		mLogger->log(L_Error, stream.str());
		return Data();
	}

	Data VM::doElementWise(element_expr_t expr, const list_t<Data>::type& args)
	{
		DL_ASSERT(expr);

		if(args.size() == 0)
		{
			return Data();
		}
		else if(args.size() == 1)
		{
			return expr(args.front(), *this);
		}
		else
		{
			DataGroup grp;
			
			for (const Data& d : args)
				grp.add(expr(d, *this));

			Data r;
			r.setGroup(grp);
			return r;
		}
	}
}