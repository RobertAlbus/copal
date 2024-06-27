#!/bin/sh

docker run \
		--env DEPENDENCY_MOUNT_PATH="/deps_out" \
		--mount type=bind,src=$(pwd),target=/deps_out \
		-it cpp_built_dependencies:latest \
		"/scripts/300-export-dependencies.sh"
