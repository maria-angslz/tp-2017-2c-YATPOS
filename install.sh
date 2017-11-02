#!/bin/bash
echo "utnso" | sudo -S su &> /dev/null

[ -f "/usr/lib/libcommons.so" ]; libcommons_installed=$?
dpkg-query -W -f='${Status}' libreadline6-dev 2>/dev/null | grep -q "ok installed"; libreadline_installed=$?
dpkg-query -W -f='${Status}' libssl-dev 2>/dev/null | grep -q "ok installed"; libssl_installed=$?

if [ $libreadline_installed -ne 0 -o $libssl_installed -ne 0 ] ; then
	echo "Updating packages…"
	sudo apt-get update -qq
fi

if [ $libreadline_installed -ne 0 -o $libssl_installed -ne 0 -o $libcommons_installed -ne 0 ] ; then
	echo "Installing dependencies…"
fi

if [ $libcommons_installed -ne 0 ] ; then
echo -n " • libcommons: downloading…"
git clone -q https://github.com/sisoputnfrba/so-commons-library.git
echo -ne "\r\e[0K • libcommons: installing…"
sudo make install -s -B -C so-commons-library &> /dev/null
sudo rm -rf so-commons-library
echo -e "\r\e[0K • libcommons: done."
fi

if [ $libreadline_installed -ne 0 ] ; then
echo -n " • libreadline6-dev: downloading…"
sudo apt-get download -qq libreadline6-dev libtinfo-dev
echo -ne "\r\e[0K • libreadline6-dev: installing…"
sudo dpkg -i *.deb &> /dev/null
rm -f *.deb
echo -e "\r\e[0K • libreadline6-dev: done."
fi

if [ $libssl_installed -ne 0 ] ; then
echo -n " • libssl-dev: downloading…"
sudo apt-get download -qq libssl-dev zlib1g-dev
echo -ne "\r\e[0K • libssl-dev: installing…"
sudo dpkg -i *.deb &> /dev/null
rm -f *.deb
echo -e "\r\e[0K • libssl-dev: done."
fi

if [ ! -d ~/git/tp-2017-2c-YATPOS ]; then
echo "Reticulating splines…"
sleep 0.5

echo "Installing components…"
mkdir -p ~/git/tp-2017-2c-YATPOS
cp -r * ~/git/tp-2017-2c-YATPOS

echo "Building modules…"

echo -n " • DataNode: building…"
make all -s -B -C /home/utnso/git/tp-2017-2c-YATPOS/DataNode/Debug &> /dev/null
echo -e "\r\e[0K • DataNode: done."
echo -n " • FileSystem: building…"
make all -s -B -C /home/utnso/git/tp-2017-2c-YATPOS/FileSystem/Debug &> /dev/null
echo -e "\r\e[0K • FileSystem: done."
echo -n " • Master: building…"
make all -s -B -C /home/utnso/git/tp-2017-2c-YATPOS/Master/Debug &> /dev/null
echo -e "\r\e[0K • Master: done."
echo -n " • Worker: building…"
make all -s -B -C /home/utnso/git/tp-2017-2c-YATPOS/Worker/Debug &> /dev/null
echo -e "\r\e[0K • Worker: done."
echo -n " • YAMA: building…"
make all -s -B -C /home/utnso/git/tp-2017-2c-YATPOS/YAMA/Debug &> /dev/null
echo -e "\r\e[0K • YAMA: done."
fi

if [ ! -L /usr/local/bin/dnode ]; then
echo "Creating symlinks…"
sudo ln -sf /home/utnso/git/tp-2017-2c-YATPOS/DataNode/Debug/DataNode /usr/local/bin/dnode
sudo ln -sf /home/utnso/git/tp-2017-2c-YATPOS/FileSystem/Debug/FileSystem /usr/local/bin/fs
sudo ln -sf /home/utnso/git/tp-2017-2c-YATPOS/Master/Debug/Master /usr/local/bin/master
sudo ln -sf /home/utnso/git/tp-2017-2c-YATPOS/Worker/Debug/Worker /usr/local/bin/worker
sudo ln -sf /home/utnso/git/tp-2017-2c-YATPOS/YAMA/Debug/YAMA /usr/local/bin/yama
fi

if [ ! -d ~/yatpos ]; then
	echo "Creating configuration files…"
	mkdir -p ~/yatpos/config
	cp -r ~/git/tp-2017-2c-YATPOS/Shared/rsc/config/* ~/yatpos/config
fi

echo "Ready to run."
