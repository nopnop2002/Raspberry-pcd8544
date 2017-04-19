#!/bin/bash
sudo ./nokia r
sudo ./nokia +1 ABCDEFGHIJ
sudo ./nokia +2 abcdefghij
sudo ./nokia +3 1234567890
sudo ./nokia +d ABCDEFGHIJ
sudo ./nokia +e abcdefghij
sudo ./nokia +f 1234567890
sudo ./nokia s

i=1
contrast=50
while [ $i -le 10 ]
do 
  echo -n "contrast="
  echo $contrast
  sudo ./nokia c $contrast
  sudo ./nokia s
  contrast=$(expr $contrast + 1)
  i=$(expr $i + 1)
  sleep 1
done
