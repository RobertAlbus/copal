# --------------------------------
# cpp_base

FROM archlinux AS cpp_base

SHELL ["/bin/bash", "-c"]
RUN sed -i '/^\[options\]/a ParallelDownloads = 20' /etc/pacman.conf
