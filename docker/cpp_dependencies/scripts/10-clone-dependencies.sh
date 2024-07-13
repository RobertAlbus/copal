#!/bin/sh
set -euxo pipefail

source /scripts/_dependency_settings.env

# --------------------------------
# PREFETCH SOURCES

TAG=$PORTAUDIO_TAG
NAME=$PORTAUDIO_NAME
REPO=$PORTAUDIO_REPO

git clone --branch $TAG --depth 10 $REPO $SRC_DIR/$NAME
cd $SRC_DIR/$NAME
git checkout $PORTAUDIO_COMMIT
cd /

# ----

TAG=$GOOGLE_TEST_TAG
NAME=$GOOGLE_TEST_NAME
REPO=$GOOGLE_TEST_REPO

git clone --branch $TAG --depth 1 $REPO $SRC_DIR/$NAME

# ----

TAG=$GOOGLE_BENCHMARK_TAG
NAME=$GOOGLE_BENCHMARK_NAME
REPO=$GOOGLE_BENCHMARK_REPO

git clone --branch $TAG --depth 1 $REPO $SRC_DIR/$NAME

# ----

TAG=$GOOGLE_HIGHWAY_TAG
NAME=$GOOGLE_HIGHWAY_NAME
REPO=$GOOGLE_HIGHWAY_REPO

git clone --branch $TAG --depth 1 $REPO $SRC_DIR/$NAME