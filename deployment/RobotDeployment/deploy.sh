#!/bin/bash
set -e

if [ $# -eq 0 ]
  then
    echo "No arguments supplied"
		echo "Please specify path to a C source code FMU"
fi

rm -rf build
mkdir build

echo "Preparing...Main with valueReferences"

cp $1 build/fmu.zip
cd build

unzip -o fmu.zip > /dev/null
rm -f sources/main.c

leftval=`grep 'name="lfLeftVal"' modelDescription.xml | awk -F \" '{print $4}'`
forwardrotate=`grep 'name="forwardRotate"' modelDescription.xml | awk -F \" '{print $4}'`
servoleft=`grep 'name="servoLeftVal"' modelDescription.xml | awk -F \" '{print $4}'`
servoright=`grep 'name="servoRightVal"' modelDescription.xml | awk -F \" '{print $4}'`
forwardspeed=`grep 'name="forwardSpeed"' modelDescription.xml | awk -F \" '{print $4}'`
rightval=`grep 'name="lfRightVal"' modelDescription.xml | awk -F \" '{print $4}'`
backwardrotate=`grep 'name="backwardRotate"' modelDescription.xml | awk -F \" '{print $4}'`

cp ../main-template.c main.c

sed -i '.original' "s/XX_leftval_XX/$leftval/g" main.c
sed -i '.original' "s/XX_forwardrotate_XX/$forwardrotate/g" main.c
sed -i '.original' "s/XX_servoleft_XX/$servoleft/g" main.c
sed -i '.original' "s/XX_servoright_XX/$servoright/g" main.c
sed -i '.original' "s/XX_forwardspeed_XX/$forwardspeed/g" main.c
sed -i '.original' "s/XX_rightval_XX/$rightval/g" main.c
sed -i '.original' "s/XX_backwardrotate_XX/$backwardrotate/g" main.c

echo "Copying libraries in place"

cp ../uart* .
cp ../adcutil* .

echo "Compiling..."

cd sources

for i in `find . -type f -name '*.c'`
do
	avr-gcc -I"." -I"./fmi" -I"./vdmlib" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega1284p -DF_CPU=8000000UL -MMD -MP -MF"$i.d" -MT"$i.o" -c -o "$i.o" "$i"
done

avr-gcc -I"." -I".." -I"./fmi" -I"./vdmlib" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega1284p -DF_CPU=8000000UL -MMD -MP -MF"$i.d" -MT"uart.o" -c -o "uart.o" ../uart.c > /dev/null

avr-gcc -I"." -I".." -I"./fmi" -I"./vdmlib" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega1284p -DF_CPU=8000000UL -MMD -MP -MF"$i.d" -MT"adcutil.o" -c -o "adcutil.o" ../adcutil.c > /dev/null

avr-gcc -I"." -I".." -I"./fmi" -I"./vdmlib" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega1284p -DF_CPU=8000000UL -MMD -MP -MF"$i.d" -MT"main.o" -c -o "main.o" ../main.c > /dev/null

ofiles=`find . -type f -name '*.o'`

avr-gcc -Wl,-Map,AURobot.map -mmcu=atmega1284p -o "AURobot.elf"  $ofiles
avr-objdump -h -S AURobot.elf  > "AURobot.lss"
avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex AURobot.elf  "AURobot.hex"
avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex AURobot.elf  "AURobot.eep"
avr-size --format=avr --mcu=atmega1284p AURobot.elf




echo "Deploying..."
avrdude -v -pm1284p -cjtagmkII -Pusb:00B00000356C -Uflash:w:AURobot.hex:a




echo "Cleaning..."
cd ..
rm -rf resources sources binaries LFRController.zip modelDescription.xml main.c AURobot.* *.c.d *.c.o




echo "Done."
