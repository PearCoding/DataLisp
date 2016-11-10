#!/usr/bin/python

import pydatalisp as dl
import datalisp2python
import sys
import json

def run(input):
    sourcelogger = dl.SourceLogger()
    datalisp = dl.DataLisp(sourcelogger)
    datalisp.parse(input)
    if sourcelogger.errorCount > 0:
        print("Error while parsing.")
        return None

    container = dl.DataContainer()
    datalisp.build(container)

    try:
        return json.dumps(datalisp2python.run(container, asDict=True))
    except:
        print("Error while exporting to json." )
        return None


if __name__ == "__main__":
    if len(sys.argv) != 3 and len(sys.argv) != 2:
        print('Use datalisp2json FILE [OUTPUT]')
        exit(0)

    input = sys.argv[1]
    output = "output.json"

    if len(sys.argv) == 3:
        output = sys.argv[2]
    
    try:
        with open(input, 'r') as f:
            str = f.read()
    except:
        print("Error while reading '%s'." % input)
        exit(-1)

    str = run(str)
    if not str:
	exit(-2)

    try:
        with open(output, 'w') as f:
            f.write(str)
    except:
        print("Error while writing to '%s'." % output)
        exit(-3)
