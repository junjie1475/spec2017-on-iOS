build: ../gcc.o

../gcc.o: gcc_renamed.o warp.o
	llvm-objcopy --redefine-syms redefine gcc_renamed.o
	ld -r -o ../gcc.o gcc_renamed.o warp.o

gcc_renamed.o: ../gcc_original.o
	./init.sh

warp.o: warp.c
	`xcrun --sdk iphoneos --find clang` warp.c -c -g -arch arm64 -o warp.o -isysroot `xcrun --sdk iphoneos --show-sdk-path`
	
clean:
	rm gcc*
