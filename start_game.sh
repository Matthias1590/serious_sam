#!/bin/bash

set -e

# If serious_sam does not exist, download it
if [ ! -d "serious_sam" ]; then
  echo "Downloading Serious Sam Classic..."
  echo "Not implemented yet"
  exit 1
fi

cd serious_sam

# Directory
CURRENT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Initialization
cd "${CURRENT_DIR}"

echo "Game running"
cd "./SamTFE/Bin"
export LD_LIBRARY_PATH=".:${LD_LIBRARY_PATH}"
vblank_mode=0 LD_PRELOAD="/sgoinfre/mwijnsma/serious_sam/hack/preload.so" ./SeriousSam
