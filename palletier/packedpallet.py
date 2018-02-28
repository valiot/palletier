import itertools


class PackedPallet:
    """The representation of a final solution."""
    idx_gen = itertools.count(start=0, step=1)

    def __init__(self, pallet, boxes):
        self.idx = next(PackedPallet.idx_gen)
        self.pallet = pallet
        self.boxes = boxes
        self.boxes_vol = 0
        for box in boxes:
            self.boxes_vol += box.vol
        self.num_boxes = len(boxes)
        self.utilization = '{:0.2f}'.format(
            (self.boxes_vol / self.pallet.vol) * 100)

    def __repr__(self):
        return ('PackedPallet #{0} Pallet: {1} Boxes: {2}'.format(
                    self.idx, self.pallet, self.boxes))
