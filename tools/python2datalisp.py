import pydatalisp as dl
import numbers

# Only useful with dict!
class Importer:
    def __init__(self, data):
        self.data = data
    

    def _toData(self, obj):
        data = dl.Data()
        if isinstance(obj, list):
            data.array = self._toArray(obj)
        elif isinstance(obj, (str, unicode)):
            data.string = str(obj)
        elif isinstance(obj, bool):
            data.bool = bool(obj)
        elif isinstance(obj, (int, long)):
            data.int = int(obj)
        elif isinstance(obj, numbers.Real):
            data.float = float(obj)
        elif data:
            data.group = self._toGroup(obj)

        return data


    def _toArray(self, arr):
        a = self.container.createArray()
        for data in arr:
            a.add(self._toData(data))
        return a


    def _toGroup(self, obj):
        if isinstance(obj, dict):
            id = '__unknown__'
            if '__id__' in obj:
                id = obj['__id__']
        else:
            id = type(obj).__name__
            obj = dict(obj)
        
        grp = self.container.createGroup()
        grp.id = str(id)

        for key in obj:
            if key == "__id__":
                continue
            
            d = self._toData(obj[key])
            d.key = str(key)
            grp.addData(d)

        return grp


    def run(self):
        self.container = dl.DataContainer()
        for grp in self.data:
            d = self._toGroup(self.data[grp])
            d.id = str(grp)
            self.container.addTopGroup(d)
        
        return self.container


def run(obj):
    imp = Importer(obj)
    return imp.run()