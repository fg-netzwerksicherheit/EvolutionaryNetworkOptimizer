#!/bin/bash
if [ "$EUID" -e 0 ]
    then echo "Please DON'T run this script as root (using sudo)"
    exit
fi
sudo ./install_sudo.sh
./install_nosudo.sh 
echo "Success! Run 'openea gui' to start the GUI."

