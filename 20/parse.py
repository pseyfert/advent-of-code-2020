# Copyright (C) 2020  Paul Seyfert
# Author: Paul Seyfert <pseyfert.mathphys@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

import re
import cppyy
cppyy.include("api.h")
from cppyy.gbl import TileTemplate

AllTiles = []
with open("input.txt", 'r') as reader:
    ImageLength = None
    LineBuffer = []
    CurrentTile = None
    for line in reader.readlines():
        if re.match("Tile.*", line):
            CurrentTile = int(re.sub("Tile (.*):\n", "\\1", line))
            LineBuffer = []
            continue
        if ImageLength is None:
            ImageLength = len(line[:-1])
        if line != "\n":
            LineBuffer.append(line[:-1])
        else:
            if len(LineBuffer) != ImageLength:
                continue
                # raise ValueError("messed up")
            newTile = TileTemplate(ImageLength-2)()
            print("tile is {}".format(CurrentTile))
            newTile.Id = CurrentTile
            i = 0
            for row in LineBuffer:
                for char in row:
                    if char == "#":
                        newTile.Raw[i] = 1
                    else:
                        newTile.Raw[i] = 0
                    i += 1
            AllTiles.append(newTile)
            CurrentTile = None
            LineBuffer = []
from cppyy.gbl import lut, dofps
fps = dofps(AllTiles)
LUT = lut(AllTiles)
corners = []
for i, o in enumerate(fps):
    # it turns out, there are no ambiguities about which tiles match against each other
    # for many it holds that for each border there is exactly one other tile with one edge that has the same fingerprint
    # those with 1 edge for which that doesn't hold are the edges of the image
    # those with 2 edges for which that doesn't hold are corners
    ones = 0
    twos = 0
    for edge in range(4):
        if LUT[o.second.lowered()[edge]].size() == 1:
            ones += 1
        else:
            twos += 1
        if o.first not in [t.Id for t in LUT[o.second.lowered()[edge]]]:
            print(o.first, "is not in ", LUT[o.second.lowered()[edge]])

    if ones == 1:
        print("Tile", o.first, "is on an edge")
    elif ones == 2:
        print("Tile", o.first, "is in a corner")
        corners.append(o.first)
    elif ones > 2:
        print("This tile doesn't work")

cornerproduct = 1
for c in corners:
    cornerproduct *= c

print("solution is:", cornerproduct)
