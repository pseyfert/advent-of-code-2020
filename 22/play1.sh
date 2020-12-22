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

deck1=(9 2 6 3 1)
deck2=(5 8 4 7 10)
while true; do 
# for i in seq 3 ; do
  # echo
  # echo
  # echo
  # echo "starting a new round"
  # echo
  # echo "deck1 is $deck1"
  # echo "deck2 is $deck2"
  #
  step || break
done

if (($?==1)); then
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
