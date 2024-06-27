#!/bin/sh

eval "$(aws configure export-credentials --profile default --format env)"
docker run \
	--mount type=bind,source="$(pwd)/scripts",target=/scripts \
	-e AWS_ACCESS_KEY_ID -e AWS_SECRET_ACCESS_KEY -e AWS_SESSION_TOKEN \
	-it highway-1.1.0 /bin/sh