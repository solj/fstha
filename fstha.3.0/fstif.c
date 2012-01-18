/*
 * HA/FST - High Availability Cluster
 * Copyright (C) 2003-2006 Reggie Beavers
 * 
 * Licensed under the terms of the GNU  General Public License:
 * 		http://www.opensource.org/licenses/gpl-license.php
 * 
 * For further information visit:
 * 		http://www.fstha.com/
 * 
 * 
 * File Name: fstif.c
 * 		Bring heartbeat network interfaces up or down
 * 
 * File Authors:
 * 		Reggie Beavers (reggiebeavers@fstha.com)
 */
#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/param.h>
#include <netdb.h>
#include <sys/fcntl.h>
#include <sys/time.h>
#include "fstha.h"

main(int argc, char *argv[]) {
char	msg[128]="";
char	msg2[128]="";


fclose(stdin);
fclose(stdout);
fclose(stderr);
setsid();
umask(077);

// "Usage: %s  <UP|DOWN> dev inet netmask bcast"
	if (strcmp(argv[1],"UP") == 0 ) {
		sprintf(msg,"/usr/sbin/ifconfig %s plumb",argv[2]);
		if (system(msg) != 0)  {
			sprintf(msg2,"ERROR: %s",msg);
			logit(msg2);
			return(1);
		}
		sprintf(msg,"/usr/sbin/ifconfig %s %s netmask %s broadcast %s private up",
			argv[2],argv[3],argv[4],argv[5]);
		if (system(msg) != 0) {
			sprintf(msg2,"ERROR: %s",msg);
			logit(msg2);
			return(1);
		}
		return 0;
	}
// "Usage: %s  <UP|DOWN> <Verbose?> dev 
	else if (strcmp(argv[1],"DOWN") == 0) {
		sprintf(msg,"/usr/sbin/ifconfig %s unplumb",argv[2]);
		if (system(msg) != 0) {
			sprintf(msg2,"ERROR: %s",msg);
			logit(msg2);
			return(1);
		}
		return 0;
	}
return 1;
}
