#!/usr/bin/zsh
line="1-3 a: abcde"
count=0
for i in {1..${#line##*: }} ; do
  current_letter=${${line##*: }[$i]}
  echo $current_letter
  seek=${${line//: *}#* }
  echo $seek
  [[ $current_letter = $seek ]] && echo "match" || echo "nooope"
done
