#
# author        Oliver Blaser
# date          02.01.2022
# copyright     GNU GPLv3 - Copyright (c) 2022 Oliver Blaser
#

CC = /opt/toolchain-mipsel_24kc_gcc-7.5.0_musl/bin/mipsel-openwrt-linux-musl-gcc
LINK = /opt/toolchain-mipsel_24kc_gcc-7.5.0_musl/bin/mipsel-openwrt-linux-musl-gcc

CFLAGS = -c -Wall -W -pedantic
LFLAGS = -Wall -W -pedantic

OBJS = main.o
EXE = led-clock

# source files
F_MAIN = ../src/main.c

$(EXE): $(OBJS)
	$(LINK) $(LFLAGS) -o $(EXE) $(OBJS)

main.o: $(F_MAIN)
	$(CC) $(CFLAGS) $(F_MAIN)

clean:
	rm $(OBJS)
	rm $(EXE)
