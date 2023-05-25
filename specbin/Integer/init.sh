#!/bin/bash
for i in {0..4}
do
	nm -Ug gcc_original.o | sed -E "s/.* . (.*)/\1 \1__$i/g" > /tmp/red_sym
	cp gcc_original.o gcc_$i.o
	llvm-objcopy --redefine-syms /tmp/red_sym gcc_$i.o
done

ld -r gcc_{0..4}.o -o gcc.o
rm gcc_{0..4}.o
