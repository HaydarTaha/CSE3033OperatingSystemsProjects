#Haydar Taha Tunç 150119745
#Burak Dursun 150119743
#Emir Ege Eren 150119739

clear
if [[ $# -eq 0 ]]; then
  echo "You need to enter positive intiger"
  exit 1
fi

number=$1
prime=1

for (( i = 2; i < $number; i++ )); do
  for (( j = 2; j < i; j++ )); do
    calculated=`expr $i % $j`
    if [[ $calculated -eq 0 ]]; then
      prime=0
      break;
    else
      prime=1
    fi
  done
  if [[ $prime -eq 1 ]]; then
    hex=$i
    num=16
    remind=0
    a=0
    while [[ $hex -ne 0 ]]; do
      remind=$((hex % num))
      if [[ $remind -lt 10 ]]; then
        array[a]=$remind
      fi
      if [[ $remind -eq 10 ]]; then
        array[a]="A"
      fi
      if [[ $remind -eq 11 ]]; then
        array[a]="B"
      fi
      if [[ $remind -eq 12 ]]; then
        array[a]="C"
      fi
      if [[ $remind -eq 13 ]]; then
        array[a]="D"
      fi
      if [[ $remind -eq 14 ]]; then
        array[a]="E"
      fi
      if [[ $remind -eq 15 ]]; then
        array[a]="F"
      fi
      hex=$((hex / num))
      a=$((a + 1))
    done
    sol=$((a - 1))
    echo -n "Hexadecimal of $i is "
    for ((;sol >= 0; sol--)); do
     echo -n ${array[$sol]}
    done
    echo ""
  fi
done
