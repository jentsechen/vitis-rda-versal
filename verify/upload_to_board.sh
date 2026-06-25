#!/bin/bash
# Upload updated files to the running board without rebooting.
# Files that can be updated live: host.exe, a.xclbin, and data files.
# Files that REQUIRE a reboot: BOOT.BIN, Image, boot.scr, uEnv.txt

USER="petalinux"
HOST="10.100.70.8"
PASS="petalinux"
REMOTE_PATH="/home/petalinux"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
SD_CARD_PATH="$PROJECT_ROOT/sd_card"

# Parse flags
UPLOAD_XCLBIN=false
UPLOAD_DATA=false

for arg in "$@"; do
    case $arg in
        --xclbin) UPLOAD_XCLBIN=true ;;
        --data)   UPLOAD_DATA=true ;;
        --all)    UPLOAD_XCLBIN=true; UPLOAD_DATA=true ;;
        --help)
            echo "Usage: $0 [--xclbin] [--data] [--all]"
            echo "  (no flags)  Upload host.exe only"
            echo "  --xclbin    Also upload a.xclbin"
            echo "  --data      Also upload data files (*.npy)"
            echo "  --all       Upload everything above"
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

if $UPLOAD_DATA; then
    echo "Uploading data files ..."
    sshpass -p "$PASS" sftp -o StrictHostKeyChecking=accept-new "${USER}@${HOST}" <<EOF
cd ${REMOTE_PATH}
lcd ${SCRIPT_DIR}
mput *.npy
bye
EOF
fi

echo "Done. Run on the board:"
echo "  cd ${REMOTE_PATH} && ./host.exe"
