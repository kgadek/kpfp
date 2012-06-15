#!/bin/sh -e

DIRNAME=`dirname $0`
cd $DIRNAME
USAGE="$0 [ --update ]"
if [ `id -u` != 0 ]; then
echo 'You must be root to run this script'
exit 1
fi

if [ $# -eq 1 ]; then
	if [ "$1" = "--update" ] ; then
		time=`ls -l --time-style="+%x %X" boa.te | awk '{ printf "%s %s", $6, $7 }'`
		rules=`ausearch --start $time -m avc --raw -se boa`
		if [ x"$rules" != "x" ] ; then
			echo "Found avc's to update policy with"
			echo -e "$rules" | audit2allow -R
			echo "Do you want these changes added to policy [y/n]?"
			read ANS
			if [ "$ANS" = "y" -o "$ANS" = "Y" ] ; then
				echo "Updating policy"
				echo -e "$rules" | audit2allow -R >> boa.te
				# Fall though and rebuild policy
			else
				exit 0
			fi
		else
			echo "No new avcs found"
			exit 0
		fi
	else
		echo -e $USAGE
		exit 1
	fi
elif [ $# -ge 2 ] ; then
	echo -e $USAGE
	exit 1
fi

echo "Building and Loading Policy"
set -x
make -f /usr/share/selinux/devel/Makefile || exit
/usr/sbin/semodule -i boa.pp

# Fixing the file context on /usr/sbin/boa
/sbin/restorecon -F -R -v /usr/sbin/boa
# Fixing the file context on /etc/rc\.d/init\.d/boa
/sbin/restorecon -F -R -v /etc/rc\.d/init\.d/boa
# Fixing the file context on /var/log/boa
/sbin/restorecon -F -R -v /var/log/boa
# Fixing the file context on /etc/boa
/sbin/restorecon -F -R -v /etc/boa
# Fixing the file context on /var/www/boa
/sbin/restorecon -F -R -v /var/www/boa
# Fixing the file context on /var/www/boa/bin-cgi
/sbin/restorecon -F -R -v /var/www/boa/bin-cgi
