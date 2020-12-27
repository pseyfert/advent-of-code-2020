#!/usr/bin/zsh

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

good_passwords=0
while read line; do
  count=0
  min=${line//-*}
  max=${${line#*-}// *}
  for i in {1..${#line##*: }} ; do
    current_letter=${${line##*: }[$i]}
    seek=${${line//: *}#* }
    [[ $current_letter = $seek ]] && ((count++))
  done
  # echo "counted $count times, which should be between $min and $max"
  (($count<=$max && $count>=$min)) && ((good_passwords++))
done < input.txt

echo "$good_passwords were valid"
