#!/bin/bash
./nokia r
./nokia +1 ABCDEFGHIJ
./nokia +2 abcdefghij
./nokia +3 1234567890
./nokia +d ABCDEFGHIJ
./nokia +e abcdefghij
./nokia +f 1234567890
sudo ./nokia s

i=1
contrast=50
while [ $i -le 10 ]
do 
  echo -n "contrast="
  echo $contrast
  ./nokia c $contrast
  sudo ./nokia s
  contrast=$(expr $contrast + 1)
  i=$(expr $i + 1)
  sleep 1
done
