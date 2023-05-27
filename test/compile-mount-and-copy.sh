#!/bin/bash

BASEDIR=$(dirname "$0")
cd $BASEDIR

# Usage: ./compile-mount-and-copy.sh file1 file2 ...
# If no arguments are given, fallback is `test`
if [ $# -eq 0 ]; then
    files=("test" "syscall_test" "dummy")
else
    files=("$@")
fi

make clean
make "${files[@]}"
mkdir ./mntdir
sudo mount ../tizen-image/rootfs.img ./mntdir
sudo cp "${files[@]}" ./mntdir/root

sudo umount ./mntdir
rmdir ./mntdir