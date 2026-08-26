#!/bin/bash
# Upload updated files to the running board without rebooting.
# Files that can be updated live: host.exe, a.xclbin.
# Files that REQUIRE a reboot: BOOT.BIN, Image, boot.scr (see README.md --
# not needed at all if the board already booted from a compatible image).

USER="petalinux"
HOST="10.100.70.23"
PASS="petalinux"
REMOTE_PATH="/home/petalinux"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Parse flags
UPLOAD_XCLBIN=false

for arg in "$@"; do
    case $arg in
        --xclbin) UPLOAD_XCLBIN=true ;;
        --help)
            echo "Usage: $0 [--xclbin]"
            echo "  (no flags)  Upload host.exe only"
            echo "  --xclbin    Also upload a.xclbin"
            exit 0 ;;
    esac
done

ssh-keygen -R "$HOST" >/dev/null 2>&1 || true

echo "Uploading host.exe ..."
sshpass -p "$PASS" scp -o StrictHostKeyChecking=accept-new \
    "$PROJECT_ROOT/host.exe" \
    "${USER}@${HOST}:${REMOTE_PATH}/host.exe"

if $UPLOAD_XCLBIN; then
    echo "Uploading a.xclbin ..."
    sshpass -p "$PASS" scp -o StrictHostKeyChecking=accept-new \
        "$PROJECT_ROOT/a.xclbin" \
        "${USER}@${HOST}:${REMOTE_PATH}/a.xclbin"
fi

echo "Done. Run on the board:"
echo "  cd ${REMOTE_PATH} && ./host.exe a.xclbin"
