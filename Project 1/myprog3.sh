#Haydar Taha Tunç 150119745
#Burak Dursun 150119743
#Emir Ege Eren 150119739

clear

thisFile=$0
size=`expr ${#thisFile} - 1`
for ((j=$size;j>0;j--)); do
  if [[ "${thisFile:j:1}" == "/" ]]; then
    continue
  else
    thisFileArr[$j]="${thisFile:j:1}"
  fi
done


arrNum=2
count=0
thisFileCheck=0
for file in *
do
  for (( x = 0; x < ${#file}; x++ )); do
  fileArr[$x]="${file:x:1}"
  if [[ ${fileArr[$x]} == ${thisFileArr[$arrNum]} ]]; then
    thisFileCheck=1
  else
    thisFileCheck=0
  fi
  arrNum=$((arrNum + 1))
  done
  arrNum=2
  if [[ -d $file ]]; then
    continue
  fi
  if [[ -w $file ]]; then
    if [[ $thisFileCheck -eq 0 ]]; then
      fileNameArr[$count]=$file
      count=`expr $count + 1`
    fi
  fi
  thisFileCheck=0
done

if [[ $count -eq 0 ]]; then
  echo "There is no writable file in directory."
else
  mkdir writable
  for (( i = 0; i < $count; i++ )); do
    mv ${fileNameArr[$i]} writable
  done
  echo $count "files moved to writable directory."
fi

exit 0
