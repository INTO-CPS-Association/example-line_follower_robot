###############################################################################
# Makefile for the project AURobot
###############################################################################

## General Flags
PROJECT = AURobot
MCU = atmega1284p
TARGET = AURobot.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -DFATAL_ERROR"(message)"        -DF_CPU=8000000UL -O0 -fsigned-char
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=AURobot.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"C:\Users\AU542988\Desktop\case-study_line_follower_robot\deployment\generated\fmi" -I"C:\Users\AU542988\Desktop\case-study_line_follower_robot\deployment\generated\vdmlib" -I"C:\Users\AU542988\Desktop\case-study_line_follower_robot\deployment\generated" 

## Libraries
LIBS = -lc -lm 

## Objects that must be built in order to link
OBJECTS = adcutil.o main.o BoolPort.o Controller.o Fmu.o FmuModel.o HardwareInterface.o IntPort.o Port.o RealPort.o RobotSensor.o RobotServo.o StringPort.o System.o World.o CSVLib.o IOLib.o MATHLib.o PatternBindMatch.o PrettyPrint.o TypedValue.o VdmBasicTypes.o VdmClass.o VdmGC.o VdmMap.o VdmProduct.o VdmSeq.o VdmSet.o VdmUnpackString.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = "C:\Users\AU542988\Desktop\case-study_line_follower_robot\deployment\uart.o" 

## Build
all: $(TARGET) AURobot.hex AURobot.eep AURobot.lss size

## Compile
adcutil.o: ../adcutil.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

BoolPort.o: ../generated/BoolPort.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Controller.o: ../generated/Controller.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Fmu.o: ../generated/Fmu.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

FmuModel.o: ../generated/FmuModel.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

HardwareInterface.o: ../generated/HardwareInterface.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

IntPort.o: ../generated/IntPort.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

Port.o: ../generated/Port.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

RealPort.o: ../generated/RealPort.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

RobotSensor.o: ../generated/RobotSensor.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

RobotServo.o: ../generated/RobotServo.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

StringPort.o: ../generated/StringPort.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

System.o: ../generated/System.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

World.o: ../generated/World.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

CSVLib.o: ../generated/vdmlib/CSVLib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

IOLib.o: ../generated/vdmlib/IOLib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

MATHLib.o: ../generated/vdmlib/MATHLib.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

PatternBindMatch.o: ../generated/vdmlib/PatternBindMatch.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

PrettyPrint.o: ../generated/vdmlib/PrettyPrint.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

TypedValue.o: ../generated/vdmlib/TypedValue.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

VdmBasicTypes.o: ../generated/vdmlib/VdmBasicTypes.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

VdmClass.o: ../generated/vdmlib/VdmClass.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

VdmGC.o: ../generated/vdmlib/VdmGC.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

VdmMap.o: ../generated/vdmlib/VdmMap.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

VdmProduct.o: ../generated/vdmlib/VdmProduct.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

VdmSeq.o: ../generated/vdmlib/VdmSeq.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

VdmSet.o: ../generated/vdmlib/VdmSet.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

VdmUnpackString.o: ../generated/vdmlib/VdmUnpackString.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) AURobot.elf dep/* AURobot.hex AURobot.eep AURobot.lss AURobot.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

