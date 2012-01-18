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
 * File Name: fsthanet.h
 * 		
 * 
 * File Authors:
 * 		Reggie Beavers (reggiebeavers@fstha.com)
 */
#include "fstha.h"

int tcpClient (char *node,char *port,char *buf) {
char msg[BUFSIZ]="";
char s[BUFSIZ]="";

  int sd, rc,x=0;
  struct sockaddr_in localAddr, servAddr;
  struct hostent *h;
  
  h = gethostbyname(node);
  if(h==NULL) {
	sprintf(msg,"tcpClient: unknown host: %s: Ignoring...",node);
	logit(msg);
	return(1);
  }

  servAddr.sin_family = h->h_addrtype;
  memcpy((char *) &servAddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
  servAddr.sin_port = htons(atoi(port));

  /* create socket */
  sd = socket(AF_INET, SOCK_STREAM, 0);
  if(sd<0) {
	if (shm->verbose == YES)
	logit("tcpClient: cannot open socket ");
	return(1);
  }

  /* bind any port number */
  localAddr.sin_family = AF_INET;
  localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  localAddr.sin_port = htons(0);
  
  rc = bind(sd, (struct sockaddr *) &localAddr, sizeof(localAddr));
  if(rc<0) {
    sprintf(msg,"tcpClient: cannot bind port TCP %s:%s",node,port);
    logit(msg);
	close(sd);
    return(1);
  }
				
  /* connect to server */
  rc = connect(sd, (struct sockaddr *) &servAddr, sizeof(servAddr));
  if(rc<0) {
    if (shm->verbose == YES) logit("tcpClient: cannot connect ");
	close(sd);
    return(1);
  }
	
	
	/* encrypt the string before sending */
	for (x=0; x<strlen(buf) && buf[x] != 0; x++)
			s[x]=buf[x]+27;

  rc = send(sd, s, strlen(buf) + 1, 0);
  close(sd);
  if(rc<0) {
      logit("tcpClient: cannot send data ");
	close(sd);
      return(1);
  }

  close(sd);
  return 0;
}


int fsthathere(char *msg) {
char buf[BUFSIZ]="";
int rc;

if (shm->verbose == YES) {
	sprintf(buf,"fsthathere sending %s",msg);
	logit(buf);
}
if (shm->nodeN !=0 ) {
	if (shm->hb0stat0 == UP) {
		rc =  tcpClient(shm->hb0pg0,shm->port,msg);
	} else if (shm->hb1stat0 == UP) {
		rc =  tcpClient(shm->hb1pg0,shm->port,msg);
	} else if (shm->node0stat == UP) {
		rc =  tcpClient(shm->node0,shm->port,msg);
	} else {
		if (shm->verbose == YES)
			logit("fsthathere Failed for node0.");
		rc =1;
	}
}
if (shm->nodeN !=1 ) {
	if (shm->hb0stat1 == UP) {
		rc =  tcpClient(shm->hb0pg1,shm->port,msg);
	} else if (shm->hb1stat1 == UP) {
		rc =  tcpClient(shm->hb1pg1,shm->port,msg);
	} else if (shm->node1stat == UP) {
		rc =  tcpClient(shm->node1,shm->port,msg);
	} else {
		if (shm->verbose == YES)
			logit("fsthathere Failed for node1.");
		rc =1;
	}
}
if (shm->nodeN !=2 ) {
	if (shm->hb0stat2 == UP) {
		rc =  tcpClient(shm->hb0pg2,shm->port,msg);
	} else if (shm->hb1stat2 == UP) {
		rc =  tcpClient(shm->hb1pg2,shm->port,msg);
	} else if (shm->node2stat == UP) {
		rc =  tcpClient(shm->node2,shm->port,msg);
	} else {
		if (shm->verbose == YES)
			logit("fsthathere Failed for node2.");
		rc =1;
	}
}
if (shm->nodeN !=3 ) {
	if (shm->hb0stat3 == UP) {
		rc =  tcpClient(shm->hb0pg3,shm->port,msg);
	} else if (shm->hb1stat3 == UP) {
		rc =  tcpClient(shm->hb1pg3,shm->port,msg);
	} else if (shm->node3stat == UP) {
		rc =  tcpClient(shm->node3,shm->port,msg);
	} else {
		if (shm->verbose == YES)
			logit("fsthathere Failed for node3.");
		rc =1;
	}
}

if (shm->verbose == YES) {
	sprintf(buf,"tcpClient returned %d",rc);
	logit(buf);
}
return rc;

}

int fsthasend(char *msg) {
int x,y;

y=fsthathere(msg);
x=fsthahere(msg);

if (x != 0 || y != 0) {
        if (shm->verbose == YES)
                logit("fsthathere Failed.");
        return 1;
}
return 0;
}

