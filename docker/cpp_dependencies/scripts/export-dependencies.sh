#!/bin/sh
set -euxo pipefail

source /scripts/_dependency_settings.env

if  [ -z "$DEPENDENCY_MOUNT_PATH" ];
then \
    echo "Specify the DEPENDENCY_MOUNT_PATH variable and mount a volume at this path"
    exit 1;
else
    if grep -q "$DEPENDENCY_MOUNT_PATH" /proc/self/mountinfo; then
        cp -r "$INSTALL_DIR"/* "$DEPENDENCY_MOUNT_PATH"
        cp -r "$SRC_DIR"     "$DEPENDENCY_MOUNT_PATH/$SRC_DIR"
    else
        echo "$DEPENDENCY_MOUNT_PATH is not a mount from the host."
        exit 1
    fi
fi;