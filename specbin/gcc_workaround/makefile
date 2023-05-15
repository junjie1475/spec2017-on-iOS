gcc.o: gcc_renamed.o warp.c
	gcc warp.c -c -g -arch arm64 -o warp.o -isysroot `xcrun --sdk iphoneos --show-sdk-path`
	llvm-objcopy --redefine-syms redefine gcc_renamed.o
	ld -r -o gcc.o gcc_renamed.o warp.o
	cp gcc.o ../gcc.o

gcc_renamed.o: ../gcc_original.o
	./init.sh

clean:
	rm gcc*
