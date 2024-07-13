#!/bin/sh
set -euxo pipefail

source /scripts/_dependency_settings.env

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

required_vars_compiler_cache="\
AWS_ACCESS_KEY_ID \
AWS_SECRET_ACCESS_KEY \
AWS_SESSION_TOKEN \
SCCACHE_BUCKET \
SCCACHE_REGION \
SCCACHE_S3_KEY_PREFIX \
SCCACHE_S3_USE_SSL \
"

required_vars_dependency_settings="\
SRC_DIR \
INSTALL_DIR \
PORTAUDIO_TAG \
PORTAUDIO_COMMIT \
PORTAUDIO_NAME \
PORTAUDIO_REPO \
GOOGLE_TEST_TAG \
GOOGLE_TEST_NAME \
GOOGLE_TEST_REPO \
GOOGLE_HIGHWAY_TAG \
GOOGLE_HIGHWAY_NAME \
GOOGLE_HIGHWAY_REPO \
GOOGLE_BENCHMARK_TAG \
GOOGLE_BENCHMARK_NAME \
GOOGLE_BENCHMARK_REPO \
"

missing_vars_dependency_settings=$(check_missing_vars $required_vars_dependency_settings)
for var in $missing_vars_dependency_settings; do
  eval value=\$$var
  if [ -z "$value" ]; then
    echo "Error: Missing environment variables for dependencies." >&2
    echo "- $var" >&2
    exit 1
  fi
done

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
# BUILD DEPENDENCIES
sh /scripts/20-build-dependencies.sh

# ----------------------------------------------------------------
# STOP SCCACHE
if [ "$USE_SCCACHE" = "true" ]; then
  sccache --stop-server
fi

