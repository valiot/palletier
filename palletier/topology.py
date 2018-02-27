class Corner:
    def __init__(self, x, z):
        self.x = x
        self.z = z

    def __repr__(self):
        return 'Corner(x={0}, z={1})'.format(self.x, self.z)


class Topology:
    def __init__(self, pallet_x, pallet_z):
        self.corners = [Corner(pallet_x, 0)]
        self.max_x = pallet_x
        self.max_z = pallet_z
        self.is_even = False

    @property
    def smallest(self):
        return min(self.corners, key=lambda x: x.z)

    @smallest.deleter
    def smallest(self):
        self.corners.remove(self.smallest)

    @property
    def smallest_index(self):
        return self.corners.index(self.smallest)

    @property
    def smallest_next(self):
        """The next corner after the smallest gap"""
        return self.corners[self.smallest_index + 1]

    @smallest_next.deleter
    def smallest_next(self):
        self.corners.remove(self.smallest_next)

    @property
    def smallest_prev(self):
        """The corner just before the smallest_gap"""
        return self.corners[self.smallest_index - 1]

    @smallest_prev.deleter
    def smallest_prev(self):
        self.corners.remove(self.smallest_prev)

    def update(self, dims, situation):
        """
        Update the topology according to the current situation
        and the dimensions of the box that was packed.

        Return the x coordinate at which the box was packed.
        """
        x_coord = None
        if situation == 1:
            x_coord = 0
            if dims.dim1 == self.smallest.x:
                self.smallest.z += dims.dim3
            else:
                new_corner = Corner(dims.dim1, self.smallest.z + dims.dim3)
                self.corners.insert(0, new_corner)

        elif situation == 2:
            if dims.dim1 == self.smallest.x:
                x_coord = 0
                if self.smallest.z + dims.dim3 == self.smallest_next.z:
                    del self.smallest
                else:
                    self.smallest.z += dims.dim3
            else:
                x_coord = self.smallest.x - dims.dim1
                if self.smallest.z + dims.dim3 == self.smallest_next.z:
                    self.smallest.x -= dims.dim1
                else:
                    self.smallest.x -= dims.dim1
                    new_corner = Corner(self.smallest.x,
                                        self.smallest.z + dims.dim3)
                    self.corners.insert(self.smallest_index+1, new_corner)

        elif situation == 3:
            x_coord = self.smallest_prev.x
            if dims.dim1 == self.smallest.x - self.smallest_prev.x:
                if self.smallest.z + dims.dim3 == self.smallest_prev.z:
                    self.smallest_prev.x = self.smallest.x
                    # Delete from the smallest gap to the end
                    del self.corners[self.smallest_index:]
                else:
                    self.smallest.z += dims.dim3
            else:
                if self.smallest.z + dims.dim3 == self.smallest_prev.z:
                    self.smallest_prev.x += dims.dim1
                else:
                    new_corner = Corner(self.smallest_prev.x + dims.dim1,
                                        self.smallest.z + dims.dim3)
                    self.corners.insert(self.smallest_index, new_corner)
        elif situation == 4.1:
            if dims.dim1 == self.smallest.x - self.smallest_prev.x:
                x_coord = self.smallest_prev.x
                if self.smallest.z + dims.dim3 == self.smallest_next.z:
                    self.smallest_prev.x = self.smallest_next.x
                    del self.smallest
                else:
                    self.smallest.z += dims.dim3
            elif self.smallest_prev.x < (self.max_x - self.smallest.x):
                if self.smallest.z + dims.dim3 == self.smallest_prev.z:
                    self.smallest.x -= dims.dim1
                    x_coord = self.smallest.x
                else:
                    x_coord = self.smallest_prev.x
                    new_corner = Corner(self.smallest_prev.x + dims.dim1,
                                        self.smallest.z + dims.dim3)
                    self.corners.insert(self.smallest_index, new_corner)
            else:
                if self.smallest.z + dims.dim3 == self.smallest_prev.z:
                    self.smallest_prev.x += dims.dim1
                    x_coord = self.smallest_prev.x
                else:
                    x_coord = self.smallest.x - dims.dim1
                    new_corner = Corner(self.smallest.x,
                                        self.smallest.z + dims.dim3)
                    self.corners.insert(self.smallest_index + 1, new_corner)
                    self.smallest.x -= dims.dim1

        elif situation == 4.2:
            x_coord = self.smallest_prev.x
            if dims.dim1 == self.smallest.x - self.smallest_prev.x:
                if self.smallest.z + dims.dim3 == self.smallest_prev.z:
                    self.smallest_prev.x = self.smallest.x
                    del self.smallest
                else:
                    self.smallest.z += dims.dim3
            else:
                if self.smallest.z + dims.dim3 == self.smallest_prev.z:
                    self.smallest_prev.x += dims.dim1
                elif self.smallest.z + dims.dim3 == self.smallest_next.z:
                    x_coord = self.smallest.x - dims.dim1
                    self.smallest.x -= dims.dim1
                else:
                    new_corner = Corner(self.smallest_prev.x + dims.dim1,
                                        self.smallest.z + dims.dim3)
                    self.corners.insert(self.smallest_index, new_corner)
        return x_coord

    def even(self):
        """
        Even out the topology, leaving only one corner at the lower right
        corner available
        """
        self.is_even = True
        if self.smallest_index == 0:
            del self.smallest
        elif self.smallest is self.corners[-1]:
            self.smallest_prev.x = self.smallest.x
            # Delete from the smallest gap to the end
            del self.corners[self.smallest_index:]
        else:
            if self.smallest_prev.z == self.smallest_next.z:
                self.smallest_prev.x = self.smallest_next.x
                del self.smallest_next
                del self.smallest
            else:
                if self.smallest_prev.z < self.smallest_next.z:
                    self.smallest_prev.x = self.smallest.x
                del self.smallest
