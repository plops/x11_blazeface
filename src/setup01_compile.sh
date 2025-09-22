export BASE=/home/kiel/.cache/bazel/_bazel_kiel/57ef6fb45657e6fea7a32ef2be3cb1ed
export EXT=$BASE/external
export SYSROOT=$EXT/sysroot_linux_x86_64
g++ \
    -nostdinc -nostdinc++ \
    -std=c++20 \
    main.cc \
    -L. -lLiteRtRuntimeCApi \
    -I ~/src/LiteRT/ \
    -I$EXT/opencl_headers/ \
    -I$EXT/com_google_absl/ \
    -I$SYSROOT/usr/include/linux/ \
    -I$SYSROOT/usr/include/c++/8/ \
    -I$SYSROOT/usr/include/x86_64-linux-gnu/c++/8 \
    -I$SYSROOT/usr/include/c++/8/tr1 \
    -I$EXT/llvm_linux_x86_64/include/c++/v1 \
    -I$EXT/llvm_linux_x86_64/include/x86_64-unknown-linux-gnu/c++/v1 \
    -I$SYSROOT/usr/lib/gcc/x86_64-linux-gnu/8/include \
    -I/usr/include \
    -I.
    
    
