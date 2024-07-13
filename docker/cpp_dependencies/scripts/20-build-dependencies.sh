#!/bin/sh
set -euxo pipefail

source /scripts/_dependency_settings.env

# --------------------------------
# BUILD: PORTAUDIO

TAG=$PORTAUDIO_TAG
NAME=$PORTAUDIO_NAME

mkdir $SRC_DIR/$NAME/build
cd $SRC_DIR/$NAME/build

cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
      -DCMAKE_INSTALL_INCLUDEDIR=include/portaudio \
      -DCMAKE_FRAMEWORK=FALSE \
      -DCMAKE_BUILD_TYPE=Release \
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
      -DCMAKE_BUILD_TYPE=Release \
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
      -DBENCHMARK_ENABLE_TESTING=OFF \
      -DBENCHMARK_ENABLE_GTEST_TESTS=OFF \
      -DCMAKE_BUILD_TYPE=Release \
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
      -DHWY_ENABLE_TESTS=OFF \
      -DHWY_ENABLE_CONTRIB=OFF \
      -DHWY_ENABLE_EXAMPLES=OFF \
      -DCMAKE_BUILD_TYPE=Release \
      ..

make -j
make install

