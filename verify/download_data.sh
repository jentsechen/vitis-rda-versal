#!/bin/bash
USER="petalinux"
HOST="10.100.70.15"
PASS="petalinux"
REMOTE_COL_PROC_OUT="/home/petalinux/col_proc_out.npy"
REMOTE_ROW_PROC_OUT="/home/petalinux/row_proc_out.npy"
LOCAL_PATH="$(pwd)"  # Save to the folder where the script is called

ssh-keygen -R "$HOST" >/dev/null || true
sshpass -p "$PASS" sftp -o StrictHostKeyChecking=accept-new ${USER}@${HOST} <<EOF
lcd ${LOCAL_PATH}
get ${REMOTE_COL_PROC_OUT}
get ${REMOTE_ROW_PROC_OUT}
bye
EOF
