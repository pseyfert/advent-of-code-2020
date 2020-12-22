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

step() {
  (($#deck2==0)) && return 1
  (($#deck1==0)) && return 2
  if (($deck1[1]>$deck2[1])) ; then
    deck1=($deck1[2,-1] $deck1[1] $deck2[1])
    deck2=($deck2[2,-1])
  else
    deck2=($deck2[2,-1] $deck2[1] $deck1[1])
    deck1=($deck1[2,-1])
  fi
  return 0
}

score1() {
  acc=0 ; for index in $(seq $#deck1) ; do ((acc += $index * $deck1[-$index])) ; done
  return $acc
}
score2() {
  acc=0
  echo "scoring deck $deck2"
  for index in $(seq $#deck2) ; do
    ((acc += $index * $deck2[-$index]))
  done
  return $acc
}

deck1=()
deck2=()
read2=False
while read line; do
  [[ $line = Player*2* ]] && read2=True
  [[ $line = Player* ]] && continue
  (( $#line == 0 )) && continue
  if [[ $read2 == True ]]; then
    deck2+=($line)
  else
    deck1+=($line)
  fi
done < input.txt

echo "deck1 at start of game is $deck1"
echo "deck2 at start of game is $deck2"

while true; do 
# for i in $(seq $1) ; do
  echo
  echo
  echo "starting a round $i out of $1"
  echo
  echo "deck1 is $deck1"
  echo "deck2 is $deck2"

  # step || break
  step
  if (($?==0)); then
    echo "asking for next round"
    continue
  else
    winner=$?
    echo "aborting"
    break
  fi
done

echo "winning player has number $winner"

if (($winner==1)); then
  echo "player 1 won"
  score1
  sc=$?
  echo "score is $sc"
else
  echo "player 2 won"
  score2
  sc=$?
  echo "score is $sc"
fi
