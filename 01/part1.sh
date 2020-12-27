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

expenses=()
while read line; do
  expenses+=($line)
done < $1

# for now ignore if an entry is 1010
for a in $expenses; do
  for b in $expenses; do
    (($a+$b==2020)) && echo $(($a*$b))
  done
done

for a in $expenses; do
  for b in $expenses; do
    for c in $expenses; do
      (($c+$a+$b==2020)) && echo $(($a*$b*$c))
    done
  done
done
