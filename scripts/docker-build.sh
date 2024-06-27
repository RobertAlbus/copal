source scripts/aws_creds.sh
docker buildx build \
  --secret type=env,id=AWS_ACCESS_KEY_ID \
  --secret type=env,id=AWS_SECRET_ACCESS_KEY \
  --secret type=env,id=AWS_SESSION_TOKEN \
  --build-arg SCCACHE_BUCKET="$SCCACHE_BUCKET" \
  --build-arg SCCACHE_REGION="$SCCACHE_REGION" \
  --build-arg SCCACHE_S3_KEY_PREFIX="$SCCACHE_S3_KEY_PREFIX" \
  --build-arg SCCACHE_S3_USE_SSL="$SCCACHE_S3_USE_SSL" \
  -f Dockerfile . $@