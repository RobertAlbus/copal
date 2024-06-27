source /scripts/202-dependency-variables.sh

# --------------------------------
# PREFETCH SOURCES

TAG=$PORTAUDIO_TAG
NAME=$PORTAUDIO_NAME
REPO=$PORTAUDIO_REPO

git clone --branch $TAG --depth 10 $REPO $SRC_DIR/$NAME
cd $SRC_DIR/$NAME
git checkout 18a606e1f928852bfc29639d9539ae74d37b5dee
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