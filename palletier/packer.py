from itertools import permutations
import collections
import copy

from .topology import Topology

Layer = collections.namedtuple('Layer', ['width', 'value'])
Coords = collections.namedtuple('Coords', ['x', 'y', 'z'])
Dims = collections.namedtuple('Dims', ['dim1', 'dim2', 'dim3'])


class Packer:
    def __init__(self, boxes, pallet):
        self.boxes = boxes
        self.pallet = pallet
        self.total_boxes = len(boxes)
        self.pallet_dims = pallet.dims
        self.pallet_vol = self.pallet.vol
        self.packed_vol = 0
        self.best_vol = 0
        self.num_packed = 0
        self.packed_boxes = []
        self.best_boxes = []
        self.best_pallet = pallet
        self.best_num_packed = 0
        self.packed_y = 0
        self.packing = True
        self.prev_layer = 0
        self.layer_in_layer = 0
        self.layer_finished = False
        self.layer_thickness = 0
        self.lilz = 0
        self.used_coords = []

    def reset_boxes(self):
        for box in self.boxes:
            box.is_packed = False
        self.packed_boxes = []
        self.used_coords = []
        self.packed_vol = 0

    @staticmethod
    def get_candidate_layers(boxes, pallet_orientation):
        candidate_layers = []
        for box in boxes:
            # We only want (dim1, dim2, dim3), (dim2, dim1, dim3) and (dim3, dim1, dim2)
            for orientation in list(permutations(box.dims))[::2]:
                ex_dim, dim2, dim3 = orientation
                if (ex_dim > pallet_orientation[1] or
                        ((dim2 > pallet_orientation[0] or
                          dim3 > pallet_orientation[2]) and
                         (dim2 > pallet_orientation[2] or
                          dim3 > pallet_orientation[0]))):
                    continue
                if ex_dim in [layer.width for layer in candidate_layers]:
                    continue
                layer_value = sum(min(abs(ex_dim - dim)
                                      for dim in box2.dims)
                                  for box2 in boxes if box2 is not box)
                layer = Layer(width=ex_dim, value=layer_value)
                candidate_layers.append(layer)
        return candidate_layers

    def get_box(self, max_len_x, gap_len_y, max_len_y, gap_len_z, max_len_z):
        all_dims = {dim for box in self.boxes for dim in box.dims
                    if not box.is_packed}
        max_dims = (max_len_x, max_len_y, max_len_z)
        # 3 booleans that represent if any of the max_dims are too small for any box to fit
        too_little_dims = [all(max_dim < dim for dim in all_dims) for max_dim in max_dims]
        # If any dim is too small, no boxes can fit at all.
        if any(too_little_dims):
            return (None, None), (None, None)
        min_y_diff = min_x_diff = min_z_diff = 99999
        other_y_diff = other_x_diff = other_z_diff = 99999
        # Best box in the best orientation
        best_match = (None, None)
        other_best_match = (None, None)
        checked = []
        for idx, box in enumerate(self.boxes):
            if box.is_packed:
                continue
            if box.dims in checked:
                continue
            else:
                checked.append(box.dims)
            for orientation in set(permutations(box.dims)):
                dim1, dim2, dim3 = orientation
                if dim1 <= max_len_x and dim2 <= max_len_y and dim3 <= max_len_z:
                    if dim2 <= gap_len_y:
                        y_diff = gap_len_y - dim2
                        x_diff = max_len_x - dim1
                        z_diff = abs(gap_len_z - dim3)
                        if (y_diff, x_diff, z_diff) < (min_y_diff, min_x_diff, min_z_diff):
                            min_y_diff = y_diff
                            min_x_diff = x_diff
                            min_z_diff = z_diff
                            best_match = (idx, Dims(dim1, dim2, dim3))
                    # The box doesn't quite fit the layer thickness
                    else:
                        y_diff = dim2 - gap_len_y
                        x_diff = max_len_x - dim1
                        z_diff = abs(gap_len_z - dim3)
                        if (y_diff, x_diff, z_diff) < (other_y_diff, other_x_diff, other_z_diff):
                            other_y_diff = y_diff
                            other_x_diff = x_diff
                            other_z_diff = z_diff
                            other_best_match = (idx, Dims(dim1, dim2, dim3))
        return best_match, other_best_match
    # TODO: Make this a test
    def verify_dimensions(self, coords, dims, pallet):
        assert (coords not in self.used_coords)
        self.used_coords.append(coords)
        assert all((coord + dim <= p_dim for coord, dim, p_dim
                    in zip(coords, dims, pallet)))

    def pack_box(self, box, coords, orientation):
        self.boxes[box].is_packed = True
        box_to_pack = copy.copy(self.boxes[box])
        box_to_pack.is_packed = True
        box_to_pack.pos = coords
        box_to_pack.orientation = orientation
        self.packed_vol += box_to_pack.vol
        self.packed_boxes.append(box_to_pack)
        self.num_packed = len(self.packed_boxes)

    def check_boxes(self, match, other_match, edge):
        edge.is_even = False
        if any(match):
            return match[0], match[1], None
        else:
            if any(other_match) and (self.layer_in_layer or len(edge.corners) == 1):
                if self.layer_in_layer == 0:
                    self.prev_layer = self.layer_thickness
                    self.lilz = edge.smallest.z
                self.layer_in_layer += (other_match[1][1] - self.layer_thickness)
                new_thickness = other_match[1][1]
                return other_match[0], other_match[1], new_thickness
            else:
                if len(edge.corners) == 1:
                    self.layer_finished = True
                else:
                    edge.even()
                return None, None, None

    def get_layer(self, pallet_orientation, remaining_y):
        eval_value = 99999999
        layer_thickness = 0
        pallet_x, pallet_y, pallet_z = pallet_orientation
        for box in self.boxes:
            if box.is_packed:
                continue
            for orientation in list(permutations(box.dims))[::2]:
                ex_dim, dim2, dim3 = orientation
                if (ex_dim <= remaining_y and
                        (dim2 <= pallet_x and dim3 <= pallet_z) or
                        (dim3 <= pallet_x and dim2 <= pallet_z)):
                    layer_eval = sum(min(abs(ex_dim - box_dim)
                                         for box_dim in box2.dims)
                                     for box2 in self.boxes
                                     if not box.is_packed and box2 != box)
                    if layer_eval < eval_value:
                        eval_value = layer_eval
                        layer_thickness = ex_dim
        if layer_thickness == 0 or layer_thickness > remaining_y:
            self.packing = False
        return layer_thickness

    def pack_layer(self, pallet_orientation,
                   remaining_y, remaining_z, packed_y):
        if self.layer_thickness == 0:
            self.packing = False
            return
        pallet_x, pallet_y, pallet_z = pallet_orientation
        edge = Topology(pallet_x, pallet_z)
        while True:
            y_coord = packed_y
            z_coord = edge.smallest.z
            # Situation 1: No Box on sides of gap
            if len(edge.corners) == 1:
                len_x = edge.smallest.x
                lpz = remaining_z - edge.smallest.z
                # Find candidate boxes
                match, other_match = self.get_box(len_x, self.layer_thickness,
                                                  remaining_y, lpz, lpz)
                # Get the best box to fit
                box, orientation, new_thickness = self.check_boxes(match,
                                                                   other_match,
                                                                   edge)
                if self.layer_finished:
                    break
                if edge.is_even:
                    continue
                if new_thickness:
                    self.layer_thickness = new_thickness

                x_coord = edge.update(orientation, situation=1)
                coords = Coords(x_coord, y_coord, z_coord)

                self.verify_dimensions(coords, orientation, pallet_orientation)
                self.pack_box(box, coords, orientation)
            # Situation 2: No Box on left side of gap
            elif edge.smallest_index == 0:
                len_x = edge.smallest.x
                len_z = edge.smallest_next.z - edge.smallest.z
                lpz = remaining_z - edge.smallest.z
                # Find candidate boxes
                match, other_match = self.get_box(len_x, self.layer_thickness,
                                                  remaining_y, len_z, lpz)
                # Get the best box to fit
                box, orientation, new_thickness = self.check_boxes(match,
                                                                   other_match,
                                                                   edge)
                if self.layer_finished:
                    break
                if edge.is_even:
                    continue
                if new_thickness:
                    self.layer_thickness = new_thickness

                x_coord = edge.update(orientation, situation=2)
                coords = Coords(x_coord, y_coord, z_coord)

                self.verify_dimensions(coords, orientation, pallet_orientation)
                self.pack_box(box, coords, orientation)
            # Situation 3: No Box on right side of gap
            elif edge.smallest is edge.corners[-1]:
                len_x = edge.smallest.x - edge.smallest_prev.x
                len_z = edge.smallest_prev.z - edge.smallest.z
                lpz = remaining_z - edge.smallest.z
                # Find candidate boxes
                match, other_match = self.get_box(len_x, self.layer_thickness,
                                                  remaining_y, len_z, lpz)
                box, orientation, new_thickness = self.check_boxes(match,
                                                                   other_match,
                                                                   edge)
                if self.layer_finished:
                    break
                if edge.is_even:
                    continue
                if new_thickness:
                    self.layer_thickness = new_thickness

                x_coord = edge.update(orientation, situation=3)
                coords = Coords(x_coord, y_coord, z_coord)
                self.verify_dimensions(coords, orientation, pallet_orientation)
                self.pack_box(box, coords, orientation)
            # Siuation 4A: Z dims of the gap are the same on both sides
            elif edge.smallest_prev.z == edge.smallest_next.z:
                len_x = edge.smallest.x - edge.smallest_prev.x
                len_z = edge.smallest_prev.z - edge.smallest.z
                lpz = remaining_z - edge.smallest.z
                match, other_match = self.get_box(len_x, self.layer_thickness,
                                                  remaining_y, len_z, lpz)
                box, orientation, new_thickness = self.check_boxes(match,
                                                                   other_match,
                                                                   edge)
                if self.layer_finished:
                    break
                if edge.is_even:
                    continue
                if new_thickness:
                    self.layer_thickness = new_thickness

                x_coord = edge.update(orientation, situation=4.1)
                coords = Coords(x_coord, y_coord, z_coord)
                self.verify_dimensions(coords, orientation, pallet_orientation)
                self.pack_box(box, coords, orientation)
            else:  # Situation 4B: z dims of the gap are different on the sides
                len_x = edge.smallest.x - edge.smallest_prev.x
                len_z = edge.smallest_prev.z - edge.smallest.z
                lpz = remaining_z - edge.smallest.z
                match, other_match = self.get_box(len_x, self.layer_thickness,
                                                  remaining_y, len_z, lpz)
                box, orientation, new_thickness = self.check_boxes(match,
                                                                   other_match,
                                                                   edge)
                if self.layer_finished:
                    break
                if edge.is_even:
                    continue
                if new_thickness:
                    self.layer_thickness = new_thickness

                x_coord = edge.update(orientation, 4.2)
                coords = Coords(x_coord, y_coord, z_coord)
                self.verify_dimensions(coords, orientation, pallet_orientation)
                self.pack_box(box, coords, orientation)

    def iterations(self):
        unique_permutations = set(perm
                                  for perm in permutations(self.pallet_dims))
        for variant, pallet_orientation in enumerate(unique_permutations):
            candidate_layers = self.get_candidate_layers(self.boxes,
                                                         pallet_orientation)
            layers = sorted(candidate_layers, key=lambda x: x.value)
            for iteration, layer in enumerate(layers):
                self.reset_boxes()
                self.layer_thickness = layer.width
                self.packed_vol = 0
                packed_y = 0
                remaining_y = pallet_orientation[1]
                remaining_z = pallet_orientation[2]
                self.num_packed = 0
                self.packing = True
                while self.packing:
                    self.layer_in_layer = 0
                    self.layer_finished = False
                    self.pack_layer(pallet_orientation,
                                    remaining_y, remaining_z, packed_y)
                    packed_y += self.layer_thickness
                    remaining_y = pallet_orientation[1] - packed_y

                    if self.layer_in_layer != 0:
                        prev_packed_y = packed_y
                        prev_remaining_y = remaining_y
                        remaining_y = self.layer_thickness - self.prev_layer
                        packed_y -= self.layer_thickness
                        packed_y += self.prev_layer
                        remaining_z = self.lilz  # What?
                        self.layer_finished = False
                        self.pack_layer(pallet_orientation,
                                        remaining_y, remaining_z, packed_y)
                        packed_y = prev_packed_y
                        remaining_y = prev_remaining_y
                        remaining_z = pallet_orientation[2]

                    self.layer_thickness = self.get_layer(pallet_orientation,
                                                          remaining_y)
                if self.packed_vol >= self.best_vol:
                    self.best_vol = self.packed_vol
                    self.best_pallet.orientation = Dims(*pallet_orientation)
                    self.best_boxes = self.packed_boxes
                    self.best_num_packed = self.num_packed
                if (self.best_vol == self.pallet_vol or
                        self.best_num_packed == self.total_boxes):
                    unpacked = [box for box in self.boxes
                                if box not in self.best_boxes]
                    return (self.best_pallet, self.best_boxes, unpacked,
                            self.best_vol / self.pallet_vol)
        unpacked = [box for box in self.boxes if box not in self.best_boxes]
        return (self.best_pallet, self.best_boxes, unpacked,
                self.best_vol / self.pallet_vol)
