import pydatalisp as dl

class Exporter:
    def __init__(self, container, asDict=False):
        self.container = container
        self.asDict = asDict
    

    def _fromData(self, data):
        if data.type == dl.Type.Group:
            return self._fromGroup(data.group)
        elif data.type == dl.Type.Array:
            return self._fromArray(data.array)
        elif data.type == dl.Type.Integer:
            return data.int
        elif data.type == dl.Type.Float:
            return data.float
        elif data.type == dl.Type.Bool:
            return data.bool
        else:
            return None


    def _fromArray(self, arr):
        a = []
        for data in arr:
            a.append(self._fromData(data))

        return a


    def _fromGroup(self, grp):
        d = dict()

        for data in grp.namedEntries:
            d[data.key] = self._fromData(data)

        for i in range(grp.anonymousCount):
            d[i] = self._fromData(grp.at(i))

        if self.asDict:
            d['__id__'] = grp.id
            return d
        else:
            return type(grp.id, (), d)()


    def run(self):
        d = dict()
        for grp in self.container.topGroups:
            d[grp.id] = self._fromGroup(grp)
        
        return d


def run(container, asDict=False):
    exp = Exporter(container, asDict)
    return exp.run()


def runFile(path):
    sourcelogger = dl.SourceLogger()
    datalisp = dl.DataLisp(sourcelogger)

    with open(input, 'r') as f:
        datalisp.parse(f.read())

    if sourcelogger.errorCount > 0:
        raise Exception("Error while parsing", input)

    container = dl.DataContainer()
    datalisp.build(container)

    return run(container)