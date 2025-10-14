#!/bin/bash
USER="petalinux"
HOST="10.100.70.13"
PASS="petalinux"
REMOTE_FILE="/home/petalinux/output.npy"
LOCAL_PATH="$(pwd)"  # Save to the folder where the script is called

ssh-keygen -R "$HOST" >/dev/null || true
sshpass -p "$PASS" sftp -o StrictHostKeyChecking=accept-new ${USER}@${HOST} <<EOF
lcd ${LOCAL_PATH}
get ${REMOTE_FILE}
bye
EOF
