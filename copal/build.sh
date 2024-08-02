#!/bin/bash
set -exo pipefail

BUILD_DIR="${BUILD_DIR:-build}"

check_missing_vars() {
  local required_vars="$@"
  local missing_vars=""

  for var in $required_vars; do
    eval value=\$$var
    if [ -z "$value" ]; then
      missing_vars="$missing_vars $var"
    fi
  done

  echo "$missing_vars"
}

required_vars_compiler_cache="
AWS_ACCESS_KEY_ID 
AWS_SECRET_ACCESS_KEY 
AWS_SESSION_TOKEN 
SCCACHE_BUCKET 
SCCACHE_REGION 
SCCACHE_S3_KEY_PREFIX 
SCCACHE_S3_USE_SSL 
"

# ----------------------------------------------------------------
# START SCCACHE
if [ "$USE_SCCACHE" = "true" ]; then
  missing_vars_compiler_cache=$(check_missing_vars $required_vars_compiler_cache)
  for var in $missing_vars_compiler_cache; do
    eval value=\$$var
    if [ -z "$value" ]; then
      echo "Error: The following variables are required when USE_SCCACHE=true." >&2
      echo "- $var" >&2
      exit 1
    fi
  done

  sccache --start-server
  echo "Compiler cache enabled: sccache ($SCCACHE_BUCKET)"
fi

# ----------------------------------------------------------------
# BUILD
rm -fr bin
mkdir -p bin

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
cmake .. \
  -DEXTRA_LIB_DIR=../../dependencies/lib \
  -DEXTRA_INCLUDE_DIR=../../dependencies/include\
  -DCMAKE_CXX_COMPILER=clang++

make -j

cp copaltest ../bin
cp benchmark ../bin

cd ..

# ----------------------------------------------------------------
# STOP SCCACHE
if [ "$USE_SCCACHE" = "true" ]; then
  sccache --stop-server
fi

