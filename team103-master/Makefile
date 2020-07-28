# File:   Makefile
# Author: Annabelle Ritchie and Braden Smitheram
# Date:   16 Oct 2017
# Descr:  Makefile for PONG! game

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I. -I../../utils -I../../fonts -I../../drivers -I../../drivers/avr
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: game.out


# Compile: create object files from C source files.
game.o: game.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

ir_uart.o: ../../drivers/avr/ir_uart.c ../../drivers/avr/ir_uart.h ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/avr/timer0.h ../../drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

timer.o: ../../drivers/avr/timer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

timer0.o: ../../drivers/avr/timer0.c ../../drivers/avr/bits.h ../../drivers/avr/prescale.h ../../drivers/avr/system.h ../../drivers/avr/timer0.h
	$(CC) -c $(CFLAGS) $< -o $@

usart1.o: ../../drivers/avr/usart1.c ../../drivers/avr/system.h ../../drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

prescale.o: ../../drivers/avr/prescale.c ../../drivers/avr/prescale.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../../drivers/avr/system.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

navswitch.o: ../../drivers/navswitch.c ../../drivers/navswitch.h ../../drivers/avr/pio.h ../../drivers/avr/delay.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

ledmatrix.o: ledmatrix.c ledmatrix.h
	$(CC) -c $(CFLAGS) $< -o $@

led.o: led.c led.h
	$(CC) -c $(CFLAGS) $< -o $@

pacer.o: pacer.c pacer.h
	$(CC) -c $(CFLAGS) $< -o $@

ledmat.o: ../../drivers/ledmat.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

display.o: ../../drivers/display.c ../../drivers/avr/system.h ../../drivers/display.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

font.o: ../../utils/font.c ../../drivers/avr/system.h ../../utils/font.h
	$(CC) -c $(CFLAGS) $< -o $@

tinygl.o: ../../utils/tinygl.c ../../drivers/avr/system.h ../../drivers/display.h ../../utils/font.h ../../utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@

ballphysics.o: ballphysics.c ballphysics.h
	$(CC) -c $(CFLAGS) $< -o $@

ir_send_receive.o: ir_send_receive.c ir_send_receive.h
	$(CC) -c $(CFLAGS) $< -o $@

mmelody.o: ../../extra/mmelody.c ../../drivers/avr/system.h ../../extra/mmelody.h
	$(CC) -c $(CFLAGS) $< -o $@

task.o: ../../utils/task.c ../../utils/task.h
	$(CC) -c $(CFLAGS) $< -o $@

ticker.o: ../../extra/ticker.c
	$(CC) -c $(CFLAGS) $< -o $@

tweeter.o: ../../extra/tweeter.c ../../drivers/avr/system.h ../../extra/ticker.h ../../extra/tweeter.h
	$(CC) -c $(CFLAGS) $< -o $@

buzzer.o: buzzer.c buzzer.h
	$(CC) -c $(CFLAGS) $< -o $@


# Link: create ELF output file from object files.
game.out: game.o system.o navswitch.o ledmatrix.o led.o pacer.o ir_uart.o timer.o timer0.o usart1.o prescale.o font.o tinygl.o display.o ledmat.o ballphysics.o ir_send_receive.o buzzer.o mmelody.o ticker.o tweeter.o task.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: game.out
	$(OBJCOPY) -O ihex game.out game.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash game.hex; dfu-programmer atmega32u2 start
