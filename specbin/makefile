OBJ = bwaves.o cactusBSSN.o cam4.o deepsjeng.o exchange2.o fotonik3d.o gcc.o ldecode.o leela_r.o mcf.o omnetpp.o perlbench.o roms.o specEntry.o warp.o x264.o xalancbmk.o xz.o namd.o parest.o lbm.o imagick.o nab.o blender.o

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

gcc.o: gcc_original.o
	./init.sh # gcc workaround generate 5 copies with different symbol names
	llvm-objcopy --redefine-syms redefine gcc.o # for custom memory allcator

clean:
	rm warp.o specEntry.o gcc.o
