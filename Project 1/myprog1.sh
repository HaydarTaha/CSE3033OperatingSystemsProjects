#Haydar Taha Tunç 150119745
#Burak Dursun 150119743
#Emir Ege Eren 150119739

clear
stringInput=$1
numberInput=$2

if [[ $# -ne 2 ]]; then
  echo "You must enter 2 argument !"
  exit 1
fi

stringLength=${#stringInput}
numberLength=${#numberInput}

if [[ $numberLength -eq 1 ]]; then
  for (( i = 0; i < stringLength; i++ )); do
    numbers[$i]=$numberInput
  done
else
  for (( i = 0; i < numberLength; i++ )); do
    numbers[$i]=${numberInput: $i: 1}
  done
fi

numbersArrLength=${#numbers[@]}

if [[ stringLength -ne numberLength ]]; then
  if [[ numberLength -ne 1 ]]; then
    echo "String length and number length must be same or number length must be 1"
    exit 1
  fi
fi

for (( i = 0; i < stringLength; i++ )); do
  charOfString[$i]=${stringInput: $i: 1}
done


if [[ $numbersArrLength -eq stringLength ]]; then
  alphabet=(a b c d e f g h i j k l m n o p q r s t u v w x y z)
  for (( i = 0; i < ${#1}; i++ )); do
    check=0
    for (( j = 0; j < ${#alphabet[@]}; j++ )); do
      if [[ ${1:$i:1} == ${alphabet[$j]} ]]; then
        check=1
        break
      else
        check=0
      fi
    done
    if [[ $check -eq 0 ]]; then
      echo "Please enter little english alphabet character"
      exit 1
    fi
    check=0
  done
  alphabetLength=${#alphabet[@]}
  for (( i = 0; i < stringLength; i++ )); do
    for (( j = 0; j < alphabetLength; j++ )); do
      if [[ ${charOfString[$i]} = ${alphabet[$j]} ]]; then
        charOfNumbers[$i]=$j
      fi
    done
  done

  sum=0
  for (( i = 0; i < stringLength; i++ )); do
    sum[$i]=`expr ${charOfNumbers[$i]} + ${numbers[$i]}`
  done

  for (( i = 0; i < ${#numbers[@]}; i++ )); do
    if [[ ${sum[$i]} -gt 25 ]]; then
      newSum=`expr ${sum[$i]} - 26`
      echo -n ${alphabet[$newSum]}
    else
      echo -n ${alphabet[${sum[$i]}]}
    fi
  done
fi

echo ""

exit 0
