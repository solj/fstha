#!/bin/ksh 
# HA/FST - High Availability Cluster
# Copyright (C) 2003-2006 Reggie Beavers
# 
# Licensed under the terms of the GNU  General Public License:
# 		http://www.opensource.org/licenses/gpl-license.php
# 
# For further information visit:
# 		http://www.fstha.com/
# 
# 
# File Name: fstcstat
# 		scan `uname -n`.conf for change and report to fsthatool
# 
# File Authors:
# 		Reggie Beavers (reggiebeavers@fstha.com)
#
node=`uname -n`

prev=0
while [ `ps -e|grep -c "$PPID\ "` -ne 0 ]; do
	if [ `ps -e | grep -c fsthbm` -ne 0 ]; then
		curr="`/usr/bin/cksum /opt/FSTha/bin/${node}.conf`"
		if [  "$prev" !=  "$curr" ]; then 
			echo $curr
			prev="$curr"
		fi
	else
		continue
	fi
	sleep 1
done
