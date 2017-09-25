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
echo -n "push Enter"
read input

./nokia r
./nokia +1 "Line 6"
./nokia +2 "Line 5"
./nokia +3 "Line 4"
./nokia +4 "Line 3"
./nokia +5 "Line 2"
./nokia +6 "Line 1"
./nokia c 60
sudo ./nokia s

for i in {7..20}
do
  ./nokia -L
  cmd="Line "$i
  ./nokia +1 "$cmd"
  sleep 1
  sudo ./nokia s
done
echo -n "push Enter"
read input

./nokia r
./nokia +a "Line 1"
./nokia +b "Line 2"
./nokia +c "Line 3"
./nokia +d "Line 4"
./nokia +e "Line 5"
./nokia +f "Line 6"
./nokia c 60
sudo ./nokia s

for i in {7..20}
do
  ./nokia +L
  cmd="Line "$i
  ./nokia +f "$cmd"
  sleep 1
  sudo ./nokia s
done

