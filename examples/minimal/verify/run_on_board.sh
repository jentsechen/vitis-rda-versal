#!/bin/bash
# Upload host.exe + a.xclbin to the board and run the minimal example.
# Same board/credentials as the top-level project's verify/upload_to_board.sh.

USER="petalinux"
HOST="10.100.70.23"
PASS="petalinux"
REMOTE_PATH="/home/petalinux"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

ssh-keygen -R "$HOST" >/dev/null 2>&1 || true

echo "Uploading host.exe and a.xclbin ..."
sshpass -p "$PASS" scp -o StrictHostKeyChecking=accept-new \
    "$PROJECT_ROOT/host.exe" "$PROJECT_ROOT/a.xclbin" \
    "${USER}@${HOST}:${REMOTE_PATH}/"

echo "Running on board ..."
sshpass -p "$PASS" ssh -o StrictHostKeyChecking=accept-new "${USER}@${HOST}" \
    "cd ${REMOTE_PATH} && ./host.exe a.xclbin"
