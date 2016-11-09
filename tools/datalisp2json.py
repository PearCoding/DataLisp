#!/usr/bin/python

import pydatalisp as dl
import datalisp2python
import sys
import json

if __name__ == "__main__":
    if len(sys.argv) != 3 and len(sys.argv) != 2:
        print('Use datalisp2json FILE [OUTPUT]')
        exit(0)

    input = sys.argv[1]
    output = "output.json"

    if len(sys.argv) == 3:
        output = sys.argv[2]
    
    sourcelogger = dl.SourceLogger()
    datalisp = dl.DataLisp(sourcelogger)

    try:
        with open(input, 'r') as f:
            datalisp.parse(f.read())
    except:
        print("Error while reading '%s'." % input)
        exit(-1)

    if sourcelogger.errorCount > 0:
        print("Error while parsing '%s'." % input)
        exit(-2)

    container = dl.DataContainer()
    datalisp.build(container)

    try:
        str = json.dumps(datalisp2python.run(container, asDict=True))
    except:
        print("Error while exporting file '%s' to json." % input)
        exit(-3)

    try:
        with open(output, 'w') as f:
            f.write(str)
    except:
        print("Error while writing to '%s'." % output)
        exit(-4)