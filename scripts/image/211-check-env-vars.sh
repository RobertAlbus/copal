#!/bin/sh

if  [ -z "$SCCACHE_BUCKET"         ] || \
    [ -z "$SCCACHE_REGION"         ] || \
    [ -z "$SCCACHE_S3_KEY_PREFIX"  ] || \
    [ -z "$SCCACHE_S3_USE_SSL"     ];   \
    then \
        echo "Must provide all of the following args: "
        echo "SCCACHE_BUCKET SCCACHE_REGION SCCACHE_S3_KEY_PREFIX SCCACHE_S3_USE_SSL."
        exit 1
    fi;

if  [ -z "$AWS_ACCESS_KEY_ID"     ] || \
    [ -z "$AWS_SECRET_ACCESS_KEY" ] || \
    [ -z "$AWS_SESSION_TOKEN"     ];   \
    then \
        echo "Must provide all of the following args: "
        echo "AWS_ACCESS_KEY_ID AWS_SECRET_ACCESS_KEY AWS_SESSION_TOKEN."
        exit 1
    fi;