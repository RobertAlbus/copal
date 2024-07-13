

# --------------------------------
# cpp_build_env

FROM cpp_base AS cpp_build_env

RUN apk update && apk upgrade --available
RUN apk add --no-cache clang18 git cmake make sccache jack

ENV CXX "clang++-18"