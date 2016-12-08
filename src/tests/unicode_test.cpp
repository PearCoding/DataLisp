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
#include <iostream>
#include <fstream>

#include "DataLisp.h"
#include "DataContainer.h"
#include "DataGroup.h"
#include "Data.h"
#include "SourceLogger.h"

const char* TEST_FILE =
"(test "
":test \"Binary: \\xE2\\x80\\xA2 \xe2\x80\xa2\\nU(4): \\u2022 \u2022\\nU(8): \\U00002022 \U00002022\""
")"
;

int main(int argc, char** argv)
{
	DL::SourceLogger logger;
	DL::DataLisp lisp(&logger);
	DL::DataContainer container;

	lisp.parse(TEST_FILE);
	lisp.build(container);
	std::string str = container.getTopGroups().front()->getFromKey("test").getString();

	std::cout << str << std::endl;

	std::ofstream stream("test.out");
	stream << str;

	return 0;
}