# Images Overview
## cpp_base
Alpine starting point for all images

## cpp_build_env
C++ build tools

## cpp_dependencies
A multi-stage build
- compiles dependencies
- copies source code and build artifacts into a minimal image
- running this image copies the libs/headers/source out to the host machine for intellisense

## 

## Building Images
### Without compiler cache
From the repository root directory:
```sh
# build images
docker buildx build docker/cpp_base         -t cpp_base:latest
docker buildx build docker/cpp_build_env    -t cpp_build_env:latest
docker buildx build docker/cpp_dependencies -t cpp_dependencies:latest
docker buildx build docker/copal --progress=plain -t copal_build_env

# build copal executables
docker run \                                                        
--mount type=bind,src=$(pwd)/copal,target=/copal \
-w /copal \
-it copal_build_env ./build.sh


# for development: export dependency artifacts for intellisense
docker run --env DEPENDENCY_MOUNT_PATH="/deps_out"         \
--mount type=bind,src=$(pwd)/dependencies,target=/deps_out \
cpp_dependencies
```

### With compiler cache
From the project root directory:
```sh
# first time: set up compiler cache environment variables
cp .env.example .env
vim .env

# load compiler cache variables
source .env

# log in to AWS CLI
aws sso login

# build images
docker buildx build docker/cpp_base -t cpp_base:latest
docker buildx build docker/cpp_build_env -t cpp_build_env:latest

# - env vars are required for the S3-backed sccache.
# - https://github.com/mozilla/sccache/issues/2150
# - use a subshell to contain credentials
(eval "$(aws configure export-credentials --profile default --format env)" &&
docker buildx build \
--secret type=env,id=AWS_ACCESS_KEY_ID                     \
--secret type=env,id=AWS_SECRET_ACCESS_KEY                 \
--secret type=env,id=AWS_SESSION_TOKEN                     \
--build-arg USE_SCCACHE="true"                             \
--build-arg SCCACHE_BUCKET="$SCCACHE_BUCKET"               \
--build-arg SCCACHE_REGION="$SCCACHE_REGION"               \
--build-arg SCCACHE_S3_KEY_PREFIX="$SCCACHE_S3_KEY_PREFIX" \
--build-arg SCCACHE_S3_USE_SSL="$SCCACHE_S3_USE_SSL"       \
docker/cpp_dependencies -t cpp_dependencies:latest)

docker buildx build docker/copal --progress=plain -t copal_build_env
```

## Next Steps
You have complete step 3 in [/README.md#quick-start](/README.md#quick-start).

Continue to [/copal/BUILD.md](/copal/BUILD.md).