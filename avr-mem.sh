#!/bin/bash
MAXPERCENT=40
IMAGESIZE=$(ls -l $1.bin | awk '{ print $5 }')
FLASHSIZE=$(perl -e "print 0x$(echo -e "#include <avr/io.h>\nFLASHEND" | avr-cpp -mmcu=$2 | tail -n 1 |cut -c3-);")
FLASHSIZE=$[$FLASHSIZE + 1]
PERCENT=$(perl -e "printf('%.2f', $IMAGESIZE.0 / $FLASHSIZE.0 *100.0);" )
PER=$(perl -e "printf('%i', $IMAGESIZE / $FLASHSIZE *$MAXPERCENT);" )

echo "Imagesize: $IMAGESIZE/$FLASHSIZE bytes (${PERCENT}%)"
if [ $IMAGESIZE -gt $FLASHSIZE ];then
  echo "  WARNING! Your Image is too big for the selected chip. WARNING!"
  echo ""
  exit 1
else
	echo -n "  ["
	COUNTER=0
	while [  $COUNTER -lt $MAXPERCENT ]; do
		if [ $COUNTER -lt $PER ]; then
			echo -n "="
		else
			echo -n "-"
		fi
		let COUNTER=COUNTER+1
	done
	echo "]"
fi
