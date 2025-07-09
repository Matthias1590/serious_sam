#!/bin/bash

set -e

(docker rm -f serious_sam_server || true) > /dev/null 2>&1
docker build -t serious_sam_server .
docker run -d --rm --name serious_sam_server -p 25600:25600/udp serious_sam_server > /dev/null

echo "Server running"
