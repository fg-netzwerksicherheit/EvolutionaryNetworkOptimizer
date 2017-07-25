#!/bin/bash
if [ "$EUID" -ne 0 ]
    then echo "Please run this script as root (using sudo)"
    exit
fi
execpath=$(realpath $(dirname "$0"))

echo "Installing GUI dependencies..."
echo

apt install python-software-properties
curl -sL https://deb.nodesource.com/setup_6.x | sudo -E bash -
apt install nodejs
npm install -g bower
runuser -l "$SUDO_USER" -c "npm --prefix \"$execpath\" install \"$execpath\""
runuser -l "$SUDO_USER" -c "bower \"$execpath/public\" install \"$execpath/public\""


