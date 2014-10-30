#! /bin/sh
echo "making"
make
echo "mounting mmc1"
cp kernel/kernel.bin /media/card
cp tasks/bin/*.bin /media/card
umount /media/card
mount /media/card
echo "done mounting"
