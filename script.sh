#!bin/sh
echo ""
echo "######################################################"
echo "#        CHECKING---Internet Connection---           #"
echo "######################################################"
echo ""

	packet_loss=$(ping -c 5 -q 74.125.236.52 | \
	grep -oP '\d+(?=% packet loss)')

	if [ $packet_loss -le 50 ]
	then
		echo "::::::::::::Internet is working properly::::::::::::"
		echo "::::::::::::Installing Dependencies::::::::::::"

if [ $(uname -m) = 'x86_64' ]; then
arch=amd64
else
arch=i386
fi

compile()
{
	aclocal
	autoheader
	autoreconf -i
	automake -a
	libtoolize -f
	intltoolize --force
	autoconf
	autoheader
	./configure --prefix=/usr
	make
	sudo make install
}

check_apt()
{
result=$(dpkg-query -W -f='${package}\n' "$1")
if  [ "$result" = "$1" ]; then
		echo "$1 already installed"
	else
		echo "$1 is not installed in your system"
		echo "You want to install it now: (y for yes, " \
			 "otherwise aborted):"
		read Y
		if [ $Y = y ] || [ $Y = Y ]; then
			sudo apt-get install $1
			sudo apt-get -f install
		else 
			echo "Aborted" 
			exit
		fi
	fi
}
check_wget()
{
result=$(dpkg-query -W -f='${package}\n' "$1")
if  [ "$result" = "$1" ]; then
		echo "$1 already installed"
	else
		echo "$1 is not installed in your system"
		echo "You want to install it now: (y for yes, " \
			 "otherwise aborted):"
		read Y
		if [ $Y = y ] || [ $Y = Y ]; then
			if [ ! -d "tmp" ]; then
				mkdir tmp
				cd tmp
			else
				cd tmp
			fi
				wget -c http://mirror.fsf.org/trisquel/pool/main/g/guile-1.6/guile-1.6-dev_1.6.8-10ubuntu4_$arch.deb http://mirror.fsf.org/trisquel/pool/main/g/guile-1.6/guile-1.6-libs_1.6.8-10ubuntu4_$arch.deb http://mirror.fsf.org/trisquel/pool/main/g/guile-1.6/guile-1.6_1.6.8-10ubuntu4_$arch.deb http://mirror.fsf.org/trisquel/pool/main/g/guile-1.6/libguile-ltdl-1_1.6.8-10ubuntu4_$arch.deb
				sudo dpkg -i libguile-ltdl-1_1.6.8-10ubuntu4_$arch.deb guile-1.6_1.6.8-10ubuntu4_$arch.deb guile-1.6-libs_1.6.8-10ubuntu4_$arch.deb guile-1.6-dev_1.6.8-10ubuntu4_$arch.deb

				cd ..
				
		else 
			echo "Aborted" 
			exit
		fi
	fi
}

Dependencies()
{
dep=(automake autoconf libc-dev libtinfo-dev libncurses5-dev libreadline6-dev libgtk2.0-0 libgtk2.0-0-dbg libgtk2.0-dev libglade2-0 libglade2-dev libxml2-dev intltool libtool gnutls-bin binutils binutils-dev)
dep2=(guile1.6)
for i in "${dep[@]}"
	do
		check_apt $i
	done
for i in "${dep2[@]}"
	do
		check_wget $i
	done

}

Dependencies
compile

rm -rf tmp

echo ""
echo "######################################################"
echo "#    Dr.Geo has been Installed successfully!         #"
echo "#    Run drgeo directly from command line.           #"	
echo "######################################################"
echo ""

else 
echo ":::::::::::No Internet Connection Found!:::::::::::"
echo ":::::::::::Check Your Internet Connection::::::::::"
echo ":::::::::::Or Try Again Later:::::::::::"
exit
fi
