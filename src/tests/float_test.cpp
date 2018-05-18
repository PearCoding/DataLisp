/*
Copyright (c) 2014-2018, OEmercan Yazici <omercan AT pearcoding.eu>
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
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "DataLisp.h"

const char* TEST_FILE =
"(test \n"
":test01 1\n"
":test02 +1\n"
":test03 -1\n"
":test04 1.0\n"
":test05 +1.0\n"
":test06 -1.0\n"
":test07 1e42\n"
":test08 +1e42\n"
":test09 -1e42\n"
":test10 1e+42\n"
":test11 +1e+42\n"
":test12 -1e+42\n"
":test13 1e-42\n"
":test14 +1e-42\n"
":test15 -1e-42\n"
":test16 1.0e42\n"
":test17 +1.0e42\n"
":test18 -1.0e42\n"
":test19 1.0e+42\n"
":test20 +1.0e+42\n"
":test21 -1.0e+42\n"
":test22 1.0e-42\n"
":test23 +1.0e-42\n"
":test24 -1.0e-42\n"
")"
;

int main(int argc, char** argv)
{
	DL::SourceLogger logger;
	DL::DataLisp lisp(&logger);
	DL::DataContainer container;

	lisp.parse(TEST_FILE);
	lisp.build(container);
	DL::DataGroup grp = container.getTopGroups().front();

	// Integers
	bool err = grp.getFromKey("test01").type() != DL::Data::T_Integer;
	err = err || grp.getFromKey("test02").type() != DL::Data::T_Integer;
	err = err || grp.getFromKey("test03").type() != DL::Data::T_Integer;

	for(int i = 4; i <= 24; ++i) {
		std::stringstream stream;
		stream << "test" << std::setw(2) << std::setfill('0') << i;

		err = err || grp.getFromKey(stream.str()).type() != DL::Data::T_Float;
		if(err) {
			std::cout << "Error in test " << i << std::endl;
			break; 
		}
	}

	return err ? -1 : 0;
}