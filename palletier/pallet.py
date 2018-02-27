from collections import namedtuple
import itertools

Coords = namedtuple('Coords', ['x', 'y', 'z'])
Dims = namedtuple('Dims', ['dim1', 'dim2', 'dim3'])


class Pallet:
    idx_gen = itertools.count(start=0, step=1)

    def __init__(self, dims, idx=None, max_weight=0, name=None,
                 orientation=Dims(0, 0, 0)):
        if idx is not None:
            self.idx = idx
        else:
            self.idx = next(Pallet.idx_gen)
        if name is not None:
            self.name = name
        else:
            self.name = 'NoName'
        self.dims = Dims(*dims)
        self.orientation = orientation
        self.weight = 0
        self.max_weight = max_weight
        self.vol = 1
        for dim in self.dims:
            self.vol *= dim

    def __repr__(self):
        repr_str = f'Pallet(idx={self.idx}, dims={self.dims}'
        if self.max_weight != 0:
            repr_str += f', max_weight={self.max_weight}'
        if self.name != 'NoName':
            repr_str += f', name={self.name}'
        if self.orientation != Dims(0, 0, 0):
            repr_str += ', orientation={self.orientation}'
        repr_str += ')'
        return repr_str

    def __str__(self):
        output = f'Pallet([{self.dims.x}, {self.dims.y}, {self.dims.z}]'
        if self.max_weight != 0:
            output += f', max_weight={self.max_weight}'
        output += ')'
        return output
