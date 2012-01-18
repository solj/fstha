#!/sbin/sh
#

exitres=0
case "$1" in
'start')
	echo 'Samba smbd starting.'
	/usr/local/samba/bin/smbd -D
	res=$?
	exitres=`expr $res + $exitres`
	echo 'Samba nmbd starting.'
	/usr/local/samba/bin/nmbd -D
	res=$?
	exitres=`expr $res + $exitres`
	;;

'stop')
	/usr/bin/pkill -x -u 0 smbd
	/usr/bin/pkill -x -u 0 nmbd
	;;

*)
	echo "Usage: $0 { start | stop }"
	exit 1
	;;

esac
if [ "$exitres" -ne 0 ]; then
	exit 1
fi
