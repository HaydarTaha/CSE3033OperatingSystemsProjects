#Haydar Taha Tunç 150119745
#Burak Dursun 150119743
#Emir Ege Eren 150119739

clear
directoryFunc() {
  arr=("$@")
  size=0
  for i in "${arr[@]}";do
    size=$((j + 1))
  done
  for (( i = 0; i < $size; i++ )); do
    if [[ $i -eq 0 ]]; then
      path=${arr[$i]}
    fi
  done
  size=$((size + 1))
  for file in "$path"/*;do
      if [ -d "$file" ];then
        arr[0]=$file
        directoryFunc "${arr[@]}"
      elif [ -f "$file" ]; then
        if [[ ${arr[1]} == "" ]]; then
          read -p "Do you want to delete $file? (y/n): " option
          if [[ $option == "y" ]]; then
            rm $file
            echo "1 file deleted"
          elif [[ $option == "n" ]]; then
            echo -n ""
          else
            echo "Wrong Choice"
          fi
        else
          fileSize=$((${#file} - 1))
          x=$fileSize
          for (( x = $fileSize; x >= 0; x-- )); do
          if [[ ${file:x:1} == "/" ]]; then
            break
          fi
          fileArr[$x]=${file:x:1}
          done
          newArrSize=0
          l=0
          for (( z = 0; z < ${#arr}; z++ )); do
            if [[ z -eq 0 ]]; then
              continue
            fi
            if [[ ${arr[$z]} == "" ]]; then
              break
            fi
            newArr[$l]=${arr[$z]}
            l=$((l + 1))
            newArrSize=$((newArrSize + 1))
          done
          j=0
          x=$((x + 1))
          g=0
          checkTrue=0
          for (( g = 0; g < $newArrSize; g++ )); do
            fileTrueArr[$g]=${fileArr[$x]}
            x=$((x + 1))
          done
          for (( i = 0; i < $newArrSize; i++ )); do
            if [[ ${fileTrueArr[$i]} == ${newArr[$i]} ]]; then
              checkTrue=1
              if [[ $i -eq $((newArrSize - 1)) ]]; then
                checkTrue=1
              fi
            else
              checkTrue=0
              break
            fi
          done
          if [[ $checkTrue -eq 1 ]]; then
            read -p "Do you want to delete $file? (y/n): " option
            if [[ $option == "y" ]]; then
              rm $file
              echo "1 file deleted"
            elif [[ $option == "n" ]]; then
              echo -n ""
            else
              echo "Wrong Choice"
            fi
          fi
          checkTrue=0
          j=0
        fi
      fi
  done
}

if [[ $# -eq 1 ]]; then
  if [[ $1 == "*" ]]; then
    for file in *
    do
      if [[ -d $file ]]; then
        continue
      else
        read -p "Do you want to delete $file? (y/n): " option
        if [[ $option == "y" ]]; then
          rm $file
          echo "1 file deleted"
        elif [[ $option == "n" ]]; then
          echo -n ""
        else
          echo "Wrong Choice"
        fi
      fi
    done
  else
    for file in *
    do
      if [[ -d $file ]]; then
        continue
      fi
      size=${#1}
      checkCount=`expr ${#1} - 1`
      for (( i = 0; i < $size; i++ )); do
        if [[ "${file:i:1}" == "${1:i:1}" ]]; then
          echo -n ""
        elif [[ "${file:i:1}" != "${1:i:1}" ]]; then
          if [[ "${1:i:1}" != "*" ]]; then
            break
          fi
        fi
        if [[ $i -eq $checkCount ]]; then
          read -p "Do you want to delete $file? (y/n): " option
          if [[ $option == "y" ]]; then
            rm $file
            echo "1 file deleted"
          elif [[ $option == "n" ]]; then
            echo -n ""
          else
            echo "Wrong Choice"
          fi
        fi
      done
    done
  fi
elif [[ $# -eq 2 ]]; then
  size=$((${#1}))
  for (( i = 0; i < size; i++ )); do
    if [[ $i -eq 0 ]]; then
      array[$i]="${2:i:${#1}}"
    else
      j=$((i - 1))
      array[$i]="${1:j:1}"
    fi
  done
  directoryFunc "${array[@]}"
else
  echo "Correct Usage: $0 so* or $0 PathName"
fi

exit 0
