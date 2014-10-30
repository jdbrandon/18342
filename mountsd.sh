sudo losetup /dev/loop0 ~/cmu/18342/sdcard.img;
sleep 1;
sudo kpartx -a /dev/loop0;
sleep 1;
sudo mount /dev/mapper/loop0p1 /media/bootfs;
sleep 1;
sync;
