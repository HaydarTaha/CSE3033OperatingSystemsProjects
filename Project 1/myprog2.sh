#Haydar Taha Tunç 150119745
#Burak Dursun 150119743
#Emir Ege Eren 150119739

clear
if [ $# -ne 1 ];
then
  echo "You must enter like this: $0 name.txt"
  exit 1
fi

fileName=$1
fileCheck=$(ls | grep $fileName)
check=0

if [[ $fileCheck = $fileName ]]; then
  read -p "$fileName exists. Do you want it to be modified? (y/n): " option
  if [[ $option = "n" ]]; then
    exit 0
  elif [[ $option = "y" ]]; then
    echo "A random story is created and stored in $fileName."
  else
    echo "Invalid Input"
    exit 1
  fi
else
  check=1
fi

#RANDOM number genarete for to choose line from txt files.
first=$(($RANDOM%3))
middle=$(($RANDOM%3))
last=$(($RANDOM%3))

first=`expr $first \* 2`
middle=`expr $middle \* 2`
last=`expr $last \* 2`

i=0
file="giris.txt"
while read -r line; do
  if [[ i -eq $first ]]; then
    echo $line > $fileName
  fi
  i=`expr $i + 1`
done < $file

i=0
file="gelisme.txt"
while read -r line; do
  if [[ i -eq $middle ]]; then
    echo $line >> $fileName
  fi
  i=`expr $i + 1`
done < $file

i=0
file="sonuc.txt"
while read -r line; do
  if [[ i -eq $last ]]; then
    echo $line >> $fileName
  fi
  i=`expr $i + 1`
done < $file

if [[ $check -eq 1 ]]; then
  echo "A random story is created and stored in $fileName."
fi
exit 0
