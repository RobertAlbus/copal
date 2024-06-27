mkdir -p build && cd build

cmake ..                                            \
    -DCMAKE_CXX_COMPILER_LAUNCHER=sccache           \
    -DCMAKE_C_COMPILER_LAUNCHER=sccache             \
    -DEXTRA_LIB_DIR=../../dependencies/lib          \
    -DEXTRA_INCLUDE_DIR=../../dependencies/include

make -j