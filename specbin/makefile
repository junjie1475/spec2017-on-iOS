OBJ = Integer/perlbench.o Integer/exchange2.o Integer/xz.o Integer/leela_r.o Integer/deepsjeng.o Integer/x264.o Integer/omnetpp.o Integer/ldecode.o Integer/mcf.o Integer/xalancbmk.o Integer/gcc.o Floating-Point/nab.o Floating-Point/blender.o Floating-Point/bwaves.o Floating-Point/lbm.o Floating-Point/fotonik3d.o Floating-Point/roms.o Floating-Point/parest.o Floating-Point/povray_r.o Floating-Point/imagick.o Floating-Point/cactusBSSN.o Floating-Point/namd.o Floating-Point/cam4.o warp.o specEntry.o Floating-Point/wrf.o Floating-Point/divdc3.o Floating-Point/divsc3.o

CC = $(shell xcrun --sdk iphoneos --find clang)
LIB = $(shell xcrun --sdk iphoneos --show-sdk-path)
CFLAGS= -c -g -arch arm64
LDFLAGS = -isysroot $(LIB)

build: $(OBJ)
	libtool -static -o rary.a $(OBJ)

warp.o: warp.c
	$(CC) $< -o $@ $(CFLAGS) $(LDFLAGS)

specEntry.o: specEntry.c specEntry.h
	$(CC) $< -o $@ $(CFLAGS) $(LDFLAGS)

Integer/gcc.o: Integer/gcc_original.o
	cd Integer/ && \
	./init.sh # gcc workaround generate 5 copies with different symbol names

clean:
	rm warp.o specEntry.o Integer/gcc.o
