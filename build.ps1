# Build Image
docker build --rm -f win-build.dockerfile -t bg-memory:latest .

# Run Compilation Script
docker run -v ${PWD}:/usr/proj bg-memory sh ./scripts/win-build.sh