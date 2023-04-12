# spec2017-on-iOS
![IMG_0033](https://user-images.githubusercontent.com/86281724/231185271-e2694e60-e83b-48d5-af06-3f0928b41c40.PNG)
Minimum iOS version iOS16

This is only an experimental playground to run spec2017 on iOS. It supports all the specRate2017 Interger Benchmarks. Use at your own risk!

# How to install
1. Download ipa file on release page
2. Use sideload app to sideload it.(if you don't have a computer, you can use scarlet or flekstore.)

# How to compile it yourself
1. You need to create a folder name ``Input`` under Spec2017 folder
2. Create sub folders with naming like 502.gcc under the Input folder
3. Copy all the spec2017 reference Input data to that sub folder according to the benchmark name
4. Add specbin/rary.a as static lib

# How to compile static library(llvm flang) for Fortran benchmark
1. Follow the instruction on offical repo (https://github.com/llvm/llvm-project/tree/main/flang) to build the flang compiler for your own platform.
2. Clone the repo again for building ios lib
3. Next run the following commands(PATH_TO_LLVM_BUILD_FOLDER is the folder of the native platform build)
```
rm -rf build
mkdir build
rm -rf install
mkdir install
ROOTDIR=`pwd`
INSTALLDIR=$ROOTDIR/install

cd build
cmake \
  -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=$INSTALLDIR \
  -DCMAKE_CXX_STANDARD=17 \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
  -DCMAKE_CXX_LINK_FLAGS="-Wl,-rpath,$LD_LIBRARY_PATH" \
  -DLLVM_TABLEGEN=/PATH_TO_LLVM_BUILD_FOLDER/bin/llvm-tblgen \
  -DCLANG_TABLEGEN=/PATH_TO_LLVM_BUILD_FOLDERbin/clang-tblgen \
  -DMLIR_TABLEGEN=/PATH_TO_LLVM_BUILD_FOLDER/bin/mlir-tblgen \
  -DMLIR_TABLEGEN_EXE=/PATH_TO_LLVM_BUILD_FOLDER/bin/mlir-tblgen \
  -DMLIR_LINALG_ODS_YAML_GEN=/PATH_TO_LLVM_BUILD_FOLDER/bin/mlir-linalg-ods-yaml-gen \
  -DMLIR_LINALG_ODS_YAML_GEN_EXE=/PATH_TO_LLVM_BUILD_FOLDER/bin/mlir-linalg-ods-yaml-gen \
  -DFLANG_INCLUDE_TESTS=FALSE \
  -DFLANG_BUILD_EXAMPLES=FALSE \
  -DFLANG_BUILD_TOOLS=OFF \
  -DLLVM_ENABLE_ASSERTIONS=ON \
  -DLLVM_TARGETS_TO_BUILD=AArch64 \
  -DLLVM_LIT_ARGS=-v \
  -DLLVM_ENABLE_PROJECTS="clang;mlir;flang;openmp" \
  -DLLVM_TARGET_ARCH=AArch64 \
  -DLLVM_DEFAULT_TARGET_TRIPLE=arm64-apple-darwin-macho \
  -DLLVM_BUILD_RUNTIME=Off -DLLVM_INCLUDE_TESTS=Off \
  -DLLVM_INCLUDE_EXAMPLES=Off -DLLVM_ENABLE_BACKTRACES=Off \
  -DCMAKE_OSX_ARCHITECTURES="arm64" \
  -DLLVM_ENABLE_PIC=False \
  -DCMAKE_OSX_SYSROOT=/Library/Developer/CommandLineTools/SDKs/iPhoneOS16.2.sdk \
  -DCMAKE_C_FLAGS="-arch arm64 -target arm64-apple-darwin-macho -miphoneos-version-min=14" \
  -DCMAKE_CXX_FLAGS="-arch arm64 -target arm64-apple-darwin-macho -miphoneos-version-min=14" \
  ../llvm-project/llvm
  
  && ninja
  ```
  5. You will find the static lib under lib/ folder. (Mainly libFortranCommon.a
libFortranDecimal.a
libFortranEvaluate.a
libFortranEvaluateTesting.a
libFortranLower.a
libFortran_main.a
libFortranParser.a
libFortranRuntime.a
libFortranSemantics.a)
