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
 * File Name: fsthbserv
 * 		Receive heartbeats
 * 
 * File Authors:
 * 		Reggie Beavers (reggiebeavers@fstha.com)
 */
#include <stdio.h> 
#include <stdlib.h> 
#include <errno.h> 
#include <string.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <sys/wait.h> 
#include <unistd.h> 
#include <fcntl.h> 
#include "fsthanet.h"

#define MAXBUFLEN 100

    main(int argc, char **argv)
    {
        int sockfd;
	uint16_t socport;
        struct sockaddr_in my_addr;    /* my address information */
        struct sockaddr_in their_addr; /* connector's address information */
        struct hostent *he;

        int addr_len, numbytes;
	float myload=0.0;
        char pingstr[MAXBUFLEN]="",buf[MAXBUFLEN];
time_t  now;
char msg[128]="";


strcpy(fsthaprogname,argv[0]);
mypid=getpid();
fclose(stdin);
fclose(stdout);
fclose(stderr);


umask(077);
setsid();
/*
 * Be sure only one process is running
 */
sprintf(msg,"/usr/bin/ps -e|/usr/bin/grep -v grep|/usr/bin/grep -v %d|/usr/bin/grep %s > /dev/null 2>&1",
        mypid,basename(fsthaprogname));
if (system(msg)==0) {
        msg[0]=0;
        die(msg);
}
if (get_shm(0) != 0)
        die("Failed to Attach");

sprintf(msg,"fsthbserv_pid %d",mypid);
fsthahere(msg);

     /******* monitor existence of fsthbm **********/
if (isrunning("fsthbm") != YES) {
        sprintf(msg,"fsthbm Is Missing. Aborted.");
        logit(msg);
        shm->cstat = DOWN;
}

        if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            perror("socket");
            exit(1);
        }
  //  fcntl(sockfd, F_SETFL, O_NONBLOCK);

        my_addr.sin_family = AF_INET;         /* host byte order */
	socport=atoi(shm->hbport);
        my_addr.sin_port = htons(socport);     /* short, network byte order */
        my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
        bzero(&(my_addr.sin_zero), 8);        /* zero the rest of the struct */

        if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr))  == -1) {
            perror("bind");
            exit(1);
        }

        addr_len = sizeof(struct sockaddr);
while (1) {
        if ((numbytes=recvfrom(sockfd, buf, MAXBUFLEN, 0, \
                           (struct sockaddr *)&their_addr, &addr_len)) == -1) {
        }
else if (numbytes > 0 ) {
//        printf("got packet from %s\n",inet_ntoa(their_addr.sin_addr));
//        printf("packet is %d bytes long\n",numbytes);
        buf[numbytes] = '\0';
//        printf("packet contains \"%s\"\n",buf);
if (strcmp(inet_ntoa(their_addr.sin_addr),"127.0.0.1") == 0) {
	continue;
} else if (strcmp(inet_ntoa(their_addr.sin_addr),shm->hb0pg0) == 0) {
	time(&now);
	sprintf(msg,"hb0tm0 %d",now);
	fsthahere(msg);
} else if (strcmp(inet_ntoa(their_addr.sin_addr),shm->hb1pg0) == 0) {
	time(&now);
	sprintf(msg,"hb1tm0 %d",now);
	fsthahere(msg);
}
else if (strcmp(inet_ntoa(their_addr.sin_addr),shm->hb0pg1) == 0) {
	time(&now);
	sprintf(msg,"hb0tm1 %d",now);
	fsthahere(msg);
} else if (strcmp(inet_ntoa(their_addr.sin_addr),shm->hb1pg1) == 0) {
	time(&now);
	sprintf(msg,"hb1tm1 %d",now);
	fsthahere(msg);
}
else if (strcmp(inet_ntoa(their_addr.sin_addr),shm->hb0pg2) == 0) {
	time(&now);
	sprintf(msg,"hb0tm2 %d",now);
	fsthahere(msg);
} else if (strcmp(inet_ntoa(their_addr.sin_addr),shm->hb1pg2) == 0) {
	time(&now);
	sprintf(msg,"hb1tm2 %d",now);
	fsthahere(msg);
}
else if (strcmp(inet_ntoa(their_addr.sin_addr),shm->hb0pg3) == 0) {
	time(&now);
	sprintf(msg,"hb0tm3 %d",now);
	fsthahere(msg);
} else if (strcmp(inet_ntoa(their_addr.sin_addr),shm->hb1pg3) == 0) {
	time(&now);
	sprintf(msg,"hb1tm3 %d",now);
	fsthahere(msg);
} else  {
	pingstr[0]=0;
	if (strchr(buf,' ') != NULL) {
		sscanf(buf,"%s %f",pingstr,&myload);

	}
	else {
		sscanf(buf,"%s",pingstr);
		myload=0.0;
	}
	if (strcmp(inet_ntoa(their_addr.sin_addr),shm->node0) == 0) {
		time(&now);
		sprintf(msg,"pbtm0 %d",now);
		fsthahere(msg);
		sprintf(msg,"node0load %.2f",myload);
		fsthahere(msg);
		if (strcmp(pingstr,"qnode")==0) {
			sprintf(msg,"qnode 0");
			fsthahere(msg);
		}
	} 
	if (strcmp(inet_ntoa(their_addr.sin_addr),shm->node1) == 0) {
		time(&now);
		sprintf(msg,"pbtm1 %d",now);
		fsthahere(msg);
		sprintf(msg,"node1load %.2f",myload);
		fsthahere(msg);
		if (strcmp(pingstr,"qnode")==0) {
			sprintf(msg,"qnode 1");
			fsthahere(msg);
		}
	} 
	if (strcmp(inet_ntoa(their_addr.sin_addr),shm->node2) == 0) {
		time(&now);
		sprintf(msg,"pbtm2 %d",now);
		fsthahere(msg);
		sprintf(msg,"node2load %.2f",myload);
		fsthahere(msg);
		if (strcmp(pingstr,"qnode")==0) {
			sprintf(msg,"qnode 2");
			fsthahere(msg);
		}
	} 
	if (strcmp(inet_ntoa(their_addr.sin_addr),shm->node3) == 0) {
		time(&now);
		sprintf(msg,"pbtm3 %d",now);
		fsthahere(msg);
		sprintf(msg,"node3load %.2f",myload);
		fsthahere(msg);
		if (strcmp(pingstr,"qnode")==0) {
			sprintf(msg,"qnode 3");
			fsthahere(msg);
		}
	}
}


}
} //while
        close(sockfd);
 
}
