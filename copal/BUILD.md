# Building the Copal Project

This project uses CMake and a dockerized build environment.

## Modifying Copal
Dependencies are built in a Docker container and then copied to the repository root directory.

From the repository root directory:
```sh
mkdir -p dependencies && \
docker run               \
--env DEPENDENCY_MOUNT_PATH="/deps_out"                    \
--mount type=bind,src=$(pwd)/dependencies,target=/deps_out \
cpp_dependencies
```

## Building Copal Without Docker
For those who prefer managing their own dependencies and toolchain, please feel free. Instructions are not provided.

## Building Copal with Docker
1. Follow staps 1-3 in [/README.md#quick-start](../README.md#quick-start)
2. From the repository root directory:
```sh
# ----------------
# without S3 backed sccache
docker run --user $(id -u):$(id -g) \
-w /copal -e BUILD_DIR=build-docker \
--mount type=bind,src=$(pwd)/copal,target=/copal \
-it copal_build_env ./build.sh


# ----------------
# with S3 backed sccache
source .env
aws sso login

(eval "$(aws configure export-credentials --profile default --format env)" &&
docker run \
--mount type=bind,src=$(pwd)/copal,target=/copal \
-w /copal \
-e AWS_ACCESS_KEY_ID      \
-e AWS_SECRET_ACCESS_KEY  \
-e AWS_SESSION_TOKEN      \
-e USE_SCCACHE="true"     \
-e SCCACHE_BUCKET         \
-e SCCACHE_REGION         \
-e SCCACHE_S3_KEY_PREFIX  \
-e SCCACHE_S3_USE_SSL     \
-e BUILD_DIR=build-docker \
-it copal_build_env ./build.sh)
```
