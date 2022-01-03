#
# author        Oliver Blaser
# date          03.01.2022
# copyright     GNU GPLv3 - Copyright (c) 2022 Oliver Blaser
#

cc = /opt/toolchain-mipsel_24kc_gcc-7.5.0_musl/bin/mipsel-openwrt-linux-musl-gcc
link = /opt/toolchain-mipsel_24kc_gcc-7.5.0_musl/bin/mipsel-openwrt-linux-musl-gcc

cflags = -c -Wall -W -pedantic
lflags = -Wall -W -pedantic

objs = main.o
exe = led-clock

# source files
src_main = ../src/main.c

$(exe): $(objs)
	$(link) $(lflags) -o $(exe) $(objs)

main.o: $(src_main)
	$(cc) $(cflags) $(src_main)

clean:
	rm $(objs)
	rm $(exe)
