

# --------------------------------
# cpp_build_env

FROM cpp_base AS cpp_build_env

RUN pacman -Syyu --noconfirm
RUN pacman -S --noconfirm base-devel
RUN pacman -S --noconfirm clang cmake make git
RUN pacman -S --noconfirm sccache gperftools

ENV CXX="clang++"
