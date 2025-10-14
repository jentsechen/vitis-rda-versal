#!/bin/bash
USER="petalinux"
HOST="10.100.70.13"
PASS="petalinux"
REMOTE_PATH="."
LOCAL_PATH="/media/jtc/Data/2024.2/Vitis/rda_dev_gmio/sd_card"
DATA_FILE_PATH="/home/jtc/range_comp/data_rx_1024_complex_64.npy"

ssh-keygen -R "$HOST" >/dev/null || true
sshpass -p "$PASS" sftp -o StrictHostKeyChecking=accept-new ${USER}@${HOST} <<EOF
cd ${REMOTE_PATH}
lcd ${LOCAL_PATH}
mput *
put ${DATA_FILE_PATH}
bye
EOF
