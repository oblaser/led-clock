#
# author        Oliver Blaser
# date          03.01.2022
# copyright     GNU GPLv3 - Copyright (c) 2022 Oliver Blaser
#

cc = /opt/toolchain-mipsel_24kc_gcc-7.5.0_musl/bin/mipsel-openwrt-linux-musl-gcc
link = /opt/toolchain-mipsel_24kc_gcc-7.5.0_musl/bin/mipsel-openwrt-linux-musl-gcc

commonflags = -Wall -W -pedantic -DOMW_CCDEF_OPENWRT
cflags = -c $(commonflags)
lflags = $(commonflags)

src_main = ../src/main.c
obj_main = main.o
inc_serialport = ../src/serialPort.h
src_serialport = ../src/serialPort.c
obj_serialPort = serialPort.o

objs = $(obj_main) $(obj_serialPort)
exe = led-clock

all: $(exe)
	@echo "\033[92mDone\033[39m"

$(exe): $(objs)
	$(link) $(lflags) -o $(exe) $(objs)

install-scp: $(exe)
	@echo "\033[94mscp\033[39m"
	@sh ./spc-to-vocore.sh
	@echo "\033[92mDone\033[39m"

$(obj_main): $(src_main)  $(inc_serialport)
	$(cc) $(cflags) $(src_main)

$(obj_serialPort): $(src_serialport) $(inc_serialport)
	$(cc) $(cflags) $(src_serialport)

clean:
	rm -f $(objs)
	rm -f $(exe)
