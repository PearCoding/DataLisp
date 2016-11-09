#!/usr/bin/python

import pydatalisp as dl
import python2datalisp
import sys
import json

if __name__ == "__main__":
    if len(sys.argv) != 3 and len(sys.argv) != 2:
        print('Use json2datalisp FILE [OUTPUT]')
        exit(0)

    input = sys.argv[1]
    output = "output.dl"

    if len(sys.argv) == 3:
        output = sys.argv[2]
    
    try:
        with open(input, 'r') as f:
            data = json.loads(f.read())
    except:
        print("Error while reading '%s'." % input)
        exit(-1)

    if not data:
        print("Error while parsing '%s'." % input)
        exit(-2)

    container = python2datalisp.run(data)
    str = dl.DataLisp.generate(container)

    try:
        with open(output, 'w') as f:
            f.write(str)
    except:
        print("Error while writing to '%s'." % output)
        exit(-3)