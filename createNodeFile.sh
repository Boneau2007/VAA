#! /bin/sh
rm nodeFile.txt
touch nodeFile.txt
port=5000

for i in `seq 1 $1`
do
    echo "$i 127.0.0.1:$(($port+$i))" >> nodeFile.txt
done
