import pydatalisp as dl
import numbers

# Only useful with dict!
class Importer:
    def __init__(self, data):
        self.data = data
    

    def _toData(self, obj):
        data = dl.Data()
        if isinstance(obj, (str, unicode)):
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


    def _toGroup(self, obj):
        if isinstance(obj, dict):
            id = '__unknown__'
            if '__id__' in obj:
                id = obj['__id__']
        elif isinstance(obj, list):
            id = ''
            obj = dict(obj)
        else:
            id = type(obj).__name__
            obj = dict(obj)
        
        grp = dl.DataGroup(id)

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