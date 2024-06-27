/scripts/221-check-env-vars.sh
source /scripts/222-dependency-variables.sh

# ----------------------------------------------------------------
# START SCCACHE

sccache --start-server

# --------------------------------
# BUILD: PORTAUDIO

TAG=$PORTAUDIO_TAG
NAME=$PORTAUDIO_NAME

mkdir $SRC_DIR/$NAME/build
cd $SRC_DIR/$NAME/build

cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
        -DCMAKE_INSTALL_INCLUDEDIR=include/portaudio \
        -DCMAKE_FRAMEWORK=FALSE \
        -DCMAKE_C_COMPILER_LAUNCHER=sccache \
        -DCMAKE_CXX_COMPILER_LAUNCHER=sccache \
        ..

make -j
make install


# --------------------------------
# BUILD: GOOGLE TEST

TAG=$GOOGLE_TEST_TAG
NAME=$GOOGLE_TEST_NAME

mkdir $SRC_DIR/$NAME/build
cd $SRC_DIR/$NAME/build

cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
        -DCMAKE_C_COMPILER_LAUNCHER=sccache \
        -DCMAKE_CXX_COMPILER_LAUNCHER=sccache \
        ..

make -j
make install


# --------------------------------
# BUILD: GOOGLE BENCHMARK

TAG=$GOOGLE_BENCHMARK_TAG
NAME=$GOOGLE_BENCHMARK_NAME

mkdir $SRC_DIR/$NAME/build
cd $SRC_DIR/$NAME/build

cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
        -DCMAKE_C_COMPILER_LAUNCHER=sccache \
        -DCMAKE_CXX_COMPILER_LAUNCHER=sccache \
        -DBENCHMARK_ENABLE_TESTING=OFF \
        -DBENCHMARK_ENABLE_GTEST_TESTS=OFF \
        ..

make -j
make install


# --------------------------------
# BUILD: GOOGLE HIGHWAY

TAG=$GOOGLE_HIGHWAY_TAG
NAME=$GOOGLE_HIGHWAY_NAME

mkdir $SRC_DIR/$NAME/build
cd $SRC_DIR/$NAME/build

cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
        -DCMAKE_C_COMPILER_LAUNCHER=sccache \
        -DCMAKE_CXX_COMPILER_LAUNCHER=sccache \
        -DHWY_ENABLE_TESTS=OFF \
        -DHWY_ENABLE_CONTRIB=OFF \
        -DHWY_ENABLE_EXAMPLES=OFF \
        ..

make -j
make install


# ----------------------------------------------------------------
# STOP SCCACHE

sccache --stop-server