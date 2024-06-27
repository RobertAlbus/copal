# --------------------------------
# BASE

FROM alpine:3.20.0 AS cpp_base

# ENTRYPOINT ["/bin/sh"]

# --------------------------------
# CPP BUILD ENV
FROM cpp_base AS cpp_build_env

COPY scripts/image/100-install-system-packages.sh /scripts/100-install-system-packages.sh
RUN chmod +x -R /scripts

RUN /scripts/100-install-system-packages.sh

ENV CXX "clang++-18"


# --------------------------------
# BUILD DEPENDENCIES

FROM cpp_build_env AS cpp_build_dependencies

COPY scripts/image/20* /scripts
RUN chmod +x -R /scripts

    # --------------------------------
    ARG SCCACHE_BUCKET
    ARG SCCACHE_REGION
    ARG SCCACHE_S3_KEY_PREFIX
    ARG SCCACHE_S3_USE_SSL

    ENV SCCACHE_BUCKET=$SCCACHE_BUCKET
    ENV SCCACHE_REGION=$SCCACHE_REGION
    ENV SCCACHE_S3_KEY_PREFIX=$SCCACHE_S3_KEY_PREFIX
    ENV SCCACHE_S3_USE_SSL=$SCCACHE_S3_USE_SSL
    # --------------------------------

RUN sh /scripts/200-clone-all-dependencies.sh

RUN --mount=type=secret,id=AWS_ACCESS_KEY_ID,required \
    --mount=type=secret,id=AWS_SECRET_ACCESS_KEY,required \
    --mount=type=secret,id=AWS_SESSION_TOKEN,required \
        AWS_ACCESS_KEY_ID=$(cat /run/secrets/AWS_ACCESS_KEY_ID) \
        AWS_SECRET_ACCESS_KEY=$(cat /run/secrets/AWS_SECRET_ACCESS_KEY) \
        AWS_SESSION_TOKEN=$(cat /run/secrets/AWS_SESSION_TOKEN) \
            # build
            sh /scripts/210-build-all-dependencies.sh


# --------------------------------
# BUILT DEPENDENCIES
FROM cpp_base AS cpp_built_dependencies

COPY scripts/image/300-export-dependencies.sh /scripts/300-export-dependencies.sh
RUN chmod +x -R /scripts

COPY --from=cpp_build_dependencies /dependencies /dependencies
