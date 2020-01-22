#! /bin/sh
rm times.txt
touch times.txt
base=1577880000 # 1.1.2020
offset=86400 # Ein Tag in Sekunden

echo "$base" >> times.txt
for i in $(seq 1 "$1")
do
    echo "$((base+offset))" >> times.txt
    base=$((base+offset))
done