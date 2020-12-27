#!/usr/bin/zsh

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
