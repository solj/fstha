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
 * File Name: fstrdchk.c
 * 		Checks usability of device as reservation disk
 * 
 * File Authors:
 * 		Reggie Beavers (reggiebeavers@fstha.com)
 */
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mhd.h>

int reserve_disk(char *s) {
        int hdl,rcode;

        hdl=open(s,O_NDELAY);
        if (hdl == -1)
                return(-1);
        rcode=ioctl(hdl,MHIOCQRESERVE);
        if (rcode == 0) {
                ioctl(hdl,MHIOCSTATUS);
                ioctl(hdl,MHIOCTKOWN);
        }

        close(hdl);
        return(rcode);
}
int release_disk(char *s) {
        int hdl,rcode;

        hdl=open(s,O_NDELAY);
        if (hdl == -1)
                return(-1);
        rcode=ioctl(hdl,MHIOCRELEASE);
        close(hdl);
        return(rcode);
}

main (int argc, char **argv) {
int fd;

if (argc != 2) {
	fprintf(stderr,"Usage: %s  <path-to-raw-device>\n",basename(argv[0]));
	exit(1);
}
fd=open(argv[1],O_NDELAY);
if (fd == -1) {
   printf("open of %s failed.\n",argv[1]);
   exit (1);
}
close(fd);

if ((fd=reserve_disk(argv[1]))!= 0)
	printf("Reservation FAILED.\n");
else
	printf("Reservation Succeeded.\n");
if ((release_disk(argv[1]))!= 0)
	printf("Release     FAILED.\n");
else
	printf("Release     Succeeded.\n");
exit(fd);
}
