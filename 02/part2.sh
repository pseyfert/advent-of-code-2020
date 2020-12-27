#!/usr/bin/zsh

good_passwords=0
while read line; do
  count=0
  pone=${line//-*}
  ptwo=${${line#*-}// *}
  seek_letter=${${line//: *}#* }
  letter_one=${${line##*: }[$pone]}
  letter_two=${${line##*: }[$ptwo]}
  [[ $letter_two = $seek_letter && $letter_one != $seek_letter ]] && ((good_passwords++))
  [[ $letter_two != $seek_letter && $letter_one = $seek_letter ]] && ((good_passwords++))
done < input.txt

echo "$good_passwords were valid"
