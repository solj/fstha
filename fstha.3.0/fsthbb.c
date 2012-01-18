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
 * File Name: fsthbb.c
 * 		Send heartbeats
 * 
 * File Authors:
 * 		Reggie Beavers (reggiebeavers@fstha.com)
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <netinet/in.h> 
#include <netdb.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include "fsthanet.h"

main(int argc, char *argv[]) {
int	param,rcode;
time_t	now;
char msg[128]="";
char myload[128]="";
uint16_t hbport;
        int sockfd;
        struct sockaddr_in their_addr; /* connector's address information */
        struct hostent *he;
        int numbytes;


if (get_shm(0) != 0) 
	die("Failed to Attach");
mypid=getpid();
/*
 * Be sure only one process is running
sprintf(msg,"/usr/bin/ps -e|/usr/bin/grep -v grep|/usr/bin/grep -v %d|/usr/bin/grep %s > /dev/null 2>&1",
        mypid,basename(fsthaprogname));
if (system(msg)==0) {
        msg[0]=0;
        die(msg);
}
 */
sprintf(msg,"fsthbb_pid %d",mypid);
fsthahere(msg);

strcpy(fsthaprogname,argv[0]);
fclose(stdin);
fclose(stdout);
fclose(stderr);
setsid();
umask(077);


if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
         perror("socket");
         exit(1);
}
their_addr.sin_family = AF_INET;      /* host byte order */
hbport=(uint16_t)atoi(shm->hbport);
their_addr.sin_port = htons(hbport);  /* short, network byte order */
bzero(&(their_addr.sin_zero), 8);     /* zero the rest of the struct */
while (1) {
	if (shm->cstat == UP)
		sprintf(myload,"ping %.2f ",shm->myloadf);
	if (shm->qnode == shm->nodeN)
		sprintf(myload,"qnode %.2f ",shm->myloadf);
	

		he=gethostbyname(shm->hb0pg0);
	        their_addr.sin_addr = *((struct in_addr *)he->h_addr);
        	sendto(sockfd,myload,strlen(myload),0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
		he=gethostbyname(shm->hb0pg1);
	        their_addr.sin_addr = *((struct in_addr *)he->h_addr);
        	sendto(sockfd,myload,strlen(myload),0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
		he=gethostbyname(shm->hb0pg2);
	        their_addr.sin_addr = *((struct in_addr *)he->h_addr);
        	sendto(sockfd,myload,strlen(myload),0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
		he=gethostbyname(shm->hb0pg3);
	        their_addr.sin_addr = *((struct in_addr *)he->h_addr);
        	sendto(sockfd,myload,strlen(myload),0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));

		he=gethostbyname(shm->hb1pg0);
	        their_addr.sin_addr = *((struct in_addr *)he->h_addr);
        	sendto(sockfd,myload,strlen(myload),0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
		he=gethostbyname(shm->hb1pg1);
	        their_addr.sin_addr = *((struct in_addr *)he->h_addr);
        	sendto(sockfd,myload,strlen(myload),0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
		he=gethostbyname(shm->hb1pg2);
	        their_addr.sin_addr = *((struct in_addr *)he->h_addr);
        	sendto(sockfd,myload,strlen(myload),0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
		he=gethostbyname(shm->hb1pg3);
	        their_addr.sin_addr = *((struct in_addr *)he->h_addr);
        	sendto(sockfd,myload,strlen(myload),0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));

		he=gethostbyname(shm->node0);
	        their_addr.sin_addr = *((struct in_addr *)he->h_addr);
        	sendto(sockfd,myload,strlen(myload),0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
		he=gethostbyname(shm->node1);
	        their_addr.sin_addr = *((struct in_addr *)he->h_addr);
        	sendto(sockfd,myload,strlen(myload),0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
		he=gethostbyname(shm->node2);
	        their_addr.sin_addr = *((struct in_addr *)he->h_addr);
        	sendto(sockfd,myload,strlen(myload),0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
		he=gethostbyname(shm->node3);
	        their_addr.sin_addr = *((struct in_addr *)he->h_addr);
        	sendto(sockfd,myload,strlen(myload),0,(struct sockaddr *)&their_addr, sizeof(struct sockaddr));
	sleep(shm->hbdelay);
}

}
