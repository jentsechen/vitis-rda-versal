## How to Mount
### NFS Server (PC)
* Install nfs: `sudo apt install nfs-kernel-server`
* Add folder
```bash
sudo nano /etc/exports
<folder_path> *(rw,sync,no_root_squash,no_subtree_check)
# example: /media/jtc/Data/2024.2/Vitis/rda_dev_gmio/verify/golden_gen *(rw,sync,no_root_squash,no_subtree_check)
^x
sudo exportfs -ra
sudo systemctl restart nfs-kernel-server
```
### Client (FPGA)
```bash
sudo mount -t nfs -o nolock,nfsvers=3 <server_ip>:<server_folder_path> <client_folder_path>
# example: sudo mount -t nfs -o nolock,nfsvers=3 10.100.70.138:/media/jtc/Data/2024.2/Vitis/rda_dev_gmio/verify/golden_gen /mnt
```