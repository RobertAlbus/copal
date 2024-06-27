#!/bin/sh
if  [ -z "$DEPENDENCY_MOUNT_PATH" ];
then \
    echo "Specify the DEPENDENCY_MOUNT_PATH variable and mount a volume at this path"
    exit 1;
else
    if grep -q "$DEPENDENCY_MOUNT_PATH" /proc/self/mountinfo; then
        rm -fr "$DEPENDENCY_MOUNT_PATH/dependencies"
        cp -r /dependencies "$DEPENDENCY_MOUNT_PATH"
    else
        echo "$DEPENDENCY_MOUNT_PATH is not a mount from the host."
        exit 1
    fi
fi;