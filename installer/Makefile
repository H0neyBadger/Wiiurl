CC=powerpc-eabi-gcc
CFLAGS=-std=gnu99 -nostdinc -fno-builtin -c
LD=powerpc-eabi-ld
LDFLAGS=-Ttext 1800000 --oformat binary
project	:=	.
root:=$(CURDIR)
build	:=	 $(root)/bin
libs := $(root)/../../libwiiu/bin
www :=$(root)/../../www
framework:=$(root)/../../framework

all: clean setup client main532

setup:
	mkdir -p $(root)/bin/

client:
	cd ../client/ && make clean; make

main532:
	$(CC) $(CFLAGS) -DVER=532 $(project)/wiiurl.c
	#-Wa,-a,-ad
	cp -r $(root)/*.o $(build)
	rm $(root)/*.o
	#$(LD) $(LDFLAGS) -o $(build)/code532.bin $(build)/wiiurl.o $(libs)/532/*.o `find $(build) -name "*.o" ! -name "wiiurl.o"`
	$(LD) $(LDFLAGS) -o $(build)/code532.bin $(build)/wiiurl.o `find $(build) -name "*.o" ! -name "wiiurl.o"`

clean:
	rm -rf $(build)/*
