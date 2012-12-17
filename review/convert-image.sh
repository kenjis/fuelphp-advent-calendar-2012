#!/bin/sh

if [ $# -eq 0 ]; then
  echo "Convert PNG/JPEG image density"
  echo " usage: $0 <dir>"
  echo "    eg: $0 images/01"
  exit
fi

list=`find $1 -type f -name *.png`

for file in $list; do
  echo "Converting PNG density: $file"
  convert "$file" "$file.bmp"
  # I don't know why, but this converts png density to about 150 dpi
  convert -density 59 "$file.bmp" "$file"
  rm "$file.bmp"
done

list=`find $1 -type f -name *.jpg`

for file in $list; do
  echo "Converting JPEG density: $file"
  convert "$file" "$file.bmp"
  # I don't know why, but this converts png density to about 150 dpi
  convert -density 59 "$file.bmp" "$file"
  rm "$file.bmp"
done

