# --------------------------------
# BUILD cpp_build_dependencies

FROM cpp_build_env AS cpp_build_dependencies
SHELL ["/bin/bash", "-c"]

            # --------------------------------
            ARG USE_SCCACHE
            ENV USE_SCCACHE=$USE_SCCACHE
            ENV CMAKE_CXX_COMPILER_LAUNCHER=${USE_SCCACHE:+sccache}

            ARG SCCACHE_BUCKET
            ARG SCCACHE_REGION
            ARG SCCACHE_S3_KEY_PREFIX
            ARG SCCACHE_S3_USE_SSL

            ENV SCCACHE_BUCKET=$SCCACHE_BUCKET
            ENV SCCACHE_REGION=$SCCACHE_REGION
            ENV SCCACHE_S3_KEY_PREFIX=$SCCACHE_S3_KEY_PREFIX
            ENV SCCACHE_S3_USE_SSL=$SCCACHE_S3_USE_SSL
            # --------------------------------

COPY scripts/_dependency_settings.env /scripts/
COPY scripts/10-clone-dependencies.sh /scripts/
COPY scripts/20-build-dependencies.sh /scripts/
COPY scripts/build_dependencies.sh /scripts
RUN chmod +x -R /scripts

RUN ./scripts/10-clone-dependencies.sh

RUN --mount=type=secret,id=AWS_ACCESS_KEY_ID \
    --mount=type=secret,id=AWS_SECRET_ACCESS_KEY \
    --mount=type=secret,id=AWS_SESSION_TOKEN \
        AWS_ACCESS_KEY_ID=$(cat /run/secrets/AWS_ACCESS_KEY_ID) \
        AWS_SECRET_ACCESS_KEY=$(cat /run/secrets/AWS_SECRET_ACCESS_KEY) \
        AWS_SESSION_TOKEN=$(cat /run/secrets/AWS_SESSION_TOKEN) \
            # build
            sh /scripts/build_dependencies.sh

# --------------------------------
# BUILT DEPENDENCIES
FROM alpine:latest AS cpp_built_dependencies

COPY scripts/export-dependencies.sh /scripts/
COPY scripts/_dependency_settings.env /scripts/
RUN chmod +x -R /scripts

COPY --from=cpp_build_dependencies /dependencies /dependencies
COPY --from=cpp_build_dependencies /src /src

CMD ["/scripts/export-dependencies.sh"]