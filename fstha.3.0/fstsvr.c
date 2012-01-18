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
 * File Name: fstsvr.c
 * 		Cluster update server
 * 
 * File Authors:
 * 		Reggie Beavers (reggiebeavers@fstha.com)
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/uio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <limits.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <libgen.h>
#include <time.h>
#include "fsthanet.h"

#define ERROR   1

#define END_LINE 0x0A


int fsthasync(char *ip,char *msg) {
char buf[BUFSIZ]="";
int rc;

if (shm->verbose == YES) {
	sprintf(buf,"fsthathere sending %s to %s",msg,ip);
	logit(buf);
}
rc = tcpClient(ip,shm->port,msg);
if (shm->verbose == YES) {
	sprintf(buf,"tcpClient returned %d",rc);
	logit(buf);
}
return rc;
}

int whatisip(char *s) {
if (strcmp(s,"10.1.1.1") || strcmp(s,"10.1.2.1"))
	return 0;
if (strcmp(s,"10.1.1.2") || strcmp(s,"10.1.2.2"))
	return 1;
if (strcmp(s,"10.1.1.3") || strcmp(s,"10.1.2.3"))
	return 2;
if (strcmp(s,"10.1.1.4") || strcmp(s,"10.1.2.4"))
	return 3;
}

int readline(int fd, char *ptr, int maxlen) {
  int n, rc;
  char c;

  for (n = 1; n < maxlen; n++) {
    if ( (rc = read(fd, &c, 1)) == 1) {
      *ptr++ = c;
      if (c == '\n')
        break;
    } else if (rc == 0) {
      if (n == 1)
        return(0);      /* EOF, no data read */
      else
        break;          /* EOF, some data was read */
    } else
      return(-1);       /* error */
  }

  *ptr = 0;
  return(n);
}

dump_conf(char *sync_to_node) {
char s[BUFSIZ];

sprintf(s,"Received Dump Request from %s",sync_to_node);
logit(s);

while (shm->cstat != UP);

while(reserve_sem() != 0);

sprintf(s,"Dumping to %s",sync_to_node);
logit(s);

shm->insync = YES;
	sprintf(s,"other_strict %d", shm->reserve_strict ); fsthasync(sync_to_node,s);
	sprintf(s,"lh0name %s", shm->lh0name ); fsthasync(sync_to_node,s);
	sprintf(s,"lh0node %d", shm->lh0node ); fsthasync(sync_to_node,s);
	sprintf(s,"lh0fallback %d", shm->lh0fallback ); fsthasync(sync_to_node,s);
	sprintf(s,"lh0failto %d", shm->lh0failto ); fsthasync(sync_to_node,s);
	if (strlen(shm->lh0name) > 0 ) {
		sprintf(s,"lh0path %s", shm->lh0path ); fsthasync(sync_to_node,s);
		sprintf(s,"lh0stat %d", shm->lh0stat ); fsthasync(sync_to_node,s);
		sprintf(s,"lh0app0name %s", shm->lh0app0name ); fsthasync(sync_to_node,s);
		if (strlen(shm->lh0app0name) >0) {
	 		sprintf(s,"lh0app0stat %d", shm->lh0app0stat ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app0start %s", shm->lh0app0start ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app0stop %s", shm->lh0app0stop ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app0test %s", shm->lh0app0test ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app0timeo %d", shm->lh0app0timeo ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app0secs %d", shm->lh0app0secs ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app0pause %d", shm->lh0app0pause ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app0crit %d", shm->lh0app0crit ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app0remnode %d", shm->lh0app0remnode ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app0remtest %s", shm->lh0app0remtest ); fsthasync(sync_to_node,s);
		}
	 	else sprintf(s,"lh0app0stat %d", 9 ); fsthasync(sync_to_node,s);
		sprintf(s,"lh0app1name %s", shm->lh0app1name ); fsthasync(sync_to_node,s);
		if (strlen(shm->lh0app1name) >0) {
	 		sprintf(s,"lh0app1stat %d", shm->lh0app1stat ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app1start %s", shm->lh0app1start ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app1stop %s", shm->lh0app1stop ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app1test %s", shm->lh0app1test ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app1timeo %d", shm->lh0app1timeo ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app1secs %d", shm->lh0app1secs ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app1pause %d", shm->lh0app1pause ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app1crit %d", shm->lh0app1crit ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app1remnode %d", shm->lh0app1remnode ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app1remtest %s", shm->lh0app1remtest ); fsthasync(sync_to_node,s);
		}
	 	else sprintf(s,"lh0app1stat %d", 9 ); fsthasync(sync_to_node,s);
		sprintf(s,"lh0app2name %s", shm->lh0app2name ); fsthasync(sync_to_node,s);
		if (strlen(shm->lh0app2name) >0) {
	 		sprintf(s,"lh0app2stat %d", shm->lh0app2stat ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app2start %s", shm->lh0app2start ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app2stop %s", shm->lh0app2stop ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app2test %s", shm->lh0app2test ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app2timeo %d", shm->lh0app2timeo ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app2secs %d", shm->lh0app2secs ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app2pause %d", shm->lh0app2pause ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app2crit %d", shm->lh0app2crit ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app2remnode %d", shm->lh0app2remnode ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app2remtest %s", shm->lh0app2remtest ); fsthasync(sync_to_node,s);
		}
	 	else sprintf(s,"lh0app2stat %d", 9 ); fsthasync(sync_to_node,s);
		sprintf(s,"lh0app3name %s", shm->lh0app3name ); fsthasync(sync_to_node,s);
		if (strlen(shm->lh0app3name) >0) {
	 		sprintf(s,"lh0app3stat %d", shm->lh0app3stat ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app3start %s", shm->lh0app3start ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app3stop %s", shm->lh0app3stop ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app3test %s", shm->lh0app3test ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app3timeo %d", shm->lh0app3timeo ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app3secs %d", shm->lh0app3secs ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app3pause %d", shm->lh0app3pause ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app3crit %d", shm->lh0app3crit ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app3remnode %d", shm->lh0app3remnode ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh0app3remtest %s", shm->lh0app3remtest ); fsthasync(sync_to_node,s);
		}
	 	else sprintf(s,"lh0app3stat %d", 9 ); fsthasync(sync_to_node,s);
	}


	sprintf(s,"lh1name %s", shm->lh1name ); fsthasync(sync_to_node,s);
	sprintf(s,"lh1node %d", shm->lh1node ); fsthasync(sync_to_node,s);
	sprintf(s,"lh1fallback %d", shm->lh1fallback ); fsthasync(sync_to_node,s);
	sprintf(s,"lh1failto %d", shm->lh1failto ); fsthasync(sync_to_node,s);
	if (strlen(shm->lh1name) > 0 ) {
		sprintf(s,"lh1path %s", shm->lh1path ); fsthasync(sync_to_node,s);
		sprintf(s,"lh1stat %d", shm->lh1stat ); fsthasync(sync_to_node,s);
		sprintf(s,"lh1app0name %s", shm->lh1app0name ); fsthasync(sync_to_node,s);
		if (strlen(shm->lh1app0name) >0) {
	 		sprintf(s,"lh1app0stat %d", shm->lh1app0stat ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app0start %s", shm->lh1app0start ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app0stop %s", shm->lh1app0stop ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app0test %s", shm->lh1app0test ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app0timeo %d", shm->lh1app0timeo ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app0secs %d", shm->lh1app0secs ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app0pause %d", shm->lh1app0pause ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app0crit %d", shm->lh1app0crit ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app0remnode %d", shm->lh1app0remnode ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app0remtest %s", shm->lh1app0remtest ); fsthasync(sync_to_node,s);
		}
	 	else sprintf(s,"lh1app0stat %d", 9 ); fsthasync(sync_to_node,s);
		sprintf(s,"lh1app1name %s", shm->lh1app1name ); fsthasync(sync_to_node,s);
		if (strlen(shm->lh1app1name) >0) {
	 		sprintf(s,"lh1app1stat %d", shm->lh1app1stat ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app1start %s", shm->lh1app1start ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app1stop %s", shm->lh1app1stop ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app1test %s", shm->lh1app1test ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app1timeo %d", shm->lh1app1timeo ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app1secs %d", shm->lh1app1secs ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app1pause %d", shm->lh1app1pause ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app1crit %d", shm->lh1app1crit ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app1remnode %d", shm->lh1app1remnode ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app1remtest %s", shm->lh1app1remtest ); fsthasync(sync_to_node,s);
		}
	 	else sprintf(s,"lh1app1stat %d", 9 ); fsthasync(sync_to_node,s);
		sprintf(s,"lh1app2name %s", shm->lh1app2name ); fsthasync(sync_to_node,s);
		if (strlen(shm->lh1app2name) >0) {
	 		sprintf(s,"lh1app2stat %d", shm->lh1app2stat ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app2start %s", shm->lh1app2start ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app2stop %s", shm->lh1app2stop ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app2test %s", shm->lh1app2test ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app2timeo %d", shm->lh1app2timeo ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app2secs %d", shm->lh1app2secs ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app2pause %d", shm->lh1app2pause ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app2crit %d", shm->lh1app2crit ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app2remnode %d", shm->lh1app2remnode ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app2remtest %s", shm->lh1app2remtest ); fsthasync(sync_to_node,s);
		}
	 	else sprintf(s,"lh1app2stat %d", 9 ); fsthasync(sync_to_node,s);
		sprintf(s,"lh1app3name %s", shm->lh1app3name ); fsthasync(sync_to_node,s);
		if (strlen(shm->lh1app3name) >0) {
	 		sprintf(s,"lh1app3stat %d", shm->lh1app3stat ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app3start %s", shm->lh1app3start ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app3stop %s", shm->lh1app3stop ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app3test %s", shm->lh1app3test ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app3timeo %d", shm->lh1app3timeo ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app3secs %d", shm->lh1app3secs ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app3pause %d", shm->lh1app3pause ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app3crit %d", shm->lh1app3crit ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app3remnode %d", shm->lh1app3remnode ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh1app3remtest %s", shm->lh1app3remtest ); fsthasync(sync_to_node,s);
		}
	 	else sprintf(s,"lh1app3stat %d", 9 ); fsthasync(sync_to_node,s);
	}


	sprintf(s,"lh2name %s", shm->lh2name ); fsthasync(sync_to_node,s);
	sprintf(s,"lh2node %d", shm->lh2node ); fsthasync(sync_to_node,s);
	sprintf(s,"lh2fallback %d", shm->lh2fallback ); fsthasync(sync_to_node,s);
	sprintf(s,"lh2failto %d", shm->lh2failto ); fsthasync(sync_to_node,s);
	if (strlen(shm->lh2name) > 0 ) {
		sprintf(s,"lh2path %s", shm->lh2path ); fsthasync(sync_to_node,s);
		sprintf(s,"lh2stat %d", shm->lh2stat ); fsthasync(sync_to_node,s);
		sprintf(s,"lh2app0name %s", shm->lh2app0name ); fsthasync(sync_to_node,s);
		if (strlen(shm->lh2app0name) >0) {
	 		sprintf(s,"lh2app0stat %d", shm->lh2app0stat ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app0start %s", shm->lh2app0start ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app0stop %s", shm->lh2app0stop ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app0test %s", shm->lh2app0test ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app0timeo %d", shm->lh2app0timeo ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app0secs %d", shm->lh2app0secs ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app0pause %d", shm->lh2app0pause ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app0crit %d", shm->lh2app0crit ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app0remnode %d", shm->lh2app0remnode ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app0remtest %s", shm->lh2app0remtest ); fsthasync(sync_to_node,s);
		}
	 	else sprintf(s,"lh2app0stat %d", 9 ); fsthasync(sync_to_node,s);
		sprintf(s,"lh2app1name %s", shm->lh2app1name ); fsthasync(sync_to_node,s);
		if (strlen(shm->lh2app1name) >0) {
	 		sprintf(s,"lh2app1stat %d", shm->lh2app1stat ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app1start %s", shm->lh2app1start ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app1stop %s", shm->lh2app1stop ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app1test %s", shm->lh2app1test ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app1timeo %d", shm->lh2app1timeo ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app1secs %d", shm->lh2app1secs ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app1pause %d", shm->lh2app1pause ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app1crit %d", shm->lh2app1crit ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app1remnode %d", shm->lh2app1remnode ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app1remtest %s", shm->lh2app1remtest ); fsthasync(sync_to_node,s);
		}
	 	else sprintf(s,"lh2app1stat %d", 9 ); fsthasync(sync_to_node,s);
		sprintf(s,"lh2app2name %s", shm->lh2app2name ); fsthasync(sync_to_node,s);
		if (strlen(shm->lh2app2name) >0) {
	 		sprintf(s,"lh2app2stat %d", shm->lh2app2stat ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app2start %s", shm->lh2app2start ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app2stop %s", shm->lh2app2stop ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app2test %s", shm->lh2app2test ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app2timeo %d", shm->lh2app2timeo ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app2secs %d", shm->lh2app2secs ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app2pause %d", shm->lh2app2pause ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app2crit %d", shm->lh2app2crit ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app2remnode %d", shm->lh2app2remnode ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app2remtest %s", shm->lh2app2remtest ); fsthasync(sync_to_node,s);
		}
	 	else sprintf(s,"lh2app2stat %d", 9 ); fsthasync(sync_to_node,s);
		sprintf(s,"lh2app3name %s", shm->lh2app3name ); fsthasync(sync_to_node,s);
		if (strlen(shm->lh2app3name) >0) {
	 		sprintf(s,"lh2app3stat %d", shm->lh2app3stat ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app3start %s", shm->lh2app3start ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app3stop %s", shm->lh2app3stop ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app3test %s", shm->lh2app3test ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app3timeo %d", shm->lh2app3timeo ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app3secs %d", shm->lh2app3secs ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app3pause %d", shm->lh2app3pause ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app3crit %d", shm->lh2app3crit ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app3remnode %d", shm->lh2app3remnode ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh2app3remtest %s", shm->lh2app3remtest ); fsthasync(sync_to_node,s);
		}
	 	else sprintf(s,"lh2app3stat %d", 9 ); fsthasync(sync_to_node,s);
	}


	sprintf(s,"lh3name %s", shm->lh3name ); fsthasync(sync_to_node,s);
	sprintf(s,"lh3node %d", shm->lh3node ); fsthasync(sync_to_node,s);
	sprintf(s,"lh3fallback %d", shm->lh3fallback ); fsthasync(sync_to_node,s);
	sprintf(s,"lh3failto %d", shm->lh3failto ); fsthasync(sync_to_node,s);
	if (strlen(shm->lh3name) > 0 ) {
		sprintf(s,"lh3path %s", shm->lh3path ); fsthasync(sync_to_node,s);
		sprintf(s,"lh3stat %d", shm->lh3stat ); fsthasync(sync_to_node,s);
		sprintf(s,"lh3app0name %s", shm->lh3app0name ); fsthasync(sync_to_node,s);
		if (strlen(shm->lh3app0name) >0) {
	 		sprintf(s,"lh3app0stat %d", shm->lh3app0stat ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app0start %s", shm->lh3app0start ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app0stop %s", shm->lh3app0stop ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app0test %s", shm->lh3app0test ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app0timeo %d", shm->lh3app0timeo ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app0secs %d", shm->lh3app0secs ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app0pause %d", shm->lh3app0pause ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app0crit %d", shm->lh3app0crit ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app0remnode %d", shm->lh3app0remnode ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app0remtest %s", shm->lh3app0remtest ); fsthasync(sync_to_node,s);
		}
	 	else sprintf(s,"lh3app0stat %d", 9 ); fsthasync(sync_to_node,s);
		sprintf(s,"lh3app1name %s", shm->lh3app1name ); fsthasync(sync_to_node,s);
		if (strlen(shm->lh3app1name) >0) {
	 		sprintf(s,"lh3app1stat %d", shm->lh3app1stat ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app1start %s", shm->lh3app1start ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app1stop %s", shm->lh3app1stop ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app1test %s", shm->lh3app1test ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app1timeo %d", shm->lh3app1timeo ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app1secs %d", shm->lh3app1secs ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app1pause %d", shm->lh3app1pause ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app1crit %d", shm->lh3app1crit ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app1remnode %d", shm->lh3app1remnode ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app1remtest %s", shm->lh3app1remtest ); fsthasync(sync_to_node,s);
		}
	 	else sprintf(s,"lh3app1stat %d", 9 ); fsthasync(sync_to_node,s);
		sprintf(s,"lh3app2name %s", shm->lh3app2name ); fsthasync(sync_to_node,s);
		if (strlen(shm->lh3app2name) >0) {
	 		sprintf(s,"lh3app2stat %d", shm->lh3app2stat ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app2start %s", shm->lh3app2start ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app2stop %s", shm->lh3app2stop ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app2test %s", shm->lh3app2test ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app2timeo %d", shm->lh3app2timeo ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app2secs %d", shm->lh3app2secs ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app2pause %d", shm->lh3app2pause ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app2crit %d", shm->lh3app2crit ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app2remnode %d", shm->lh3app2remnode ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app2remtest %s", shm->lh3app2remtest ); fsthasync(sync_to_node,s);
		}
	 	else sprintf(s,"lh3app2stat %d", 9 ); fsthasync(sync_to_node,s);
		sprintf(s,"lh3app3name %s", shm->lh3app3name ); fsthasync(sync_to_node,s);
		if (strlen(shm->lh3app3name) >0) {
	 		sprintf(s,"lh3app3stat %d", shm->lh3app3stat ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app3start %s", shm->lh3app3start ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app3stop %s", shm->lh3app3stop ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app3test %s", shm->lh3app3test ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app3timeo %d", shm->lh3app3timeo ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app3secs %d", shm->lh3app3secs ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app3pause %d", shm->lh3app3pause ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app3crit %d", shm->lh3app3crit ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app3remnode %d", shm->lh3app3remnode ); fsthasync(sync_to_node,s);
	 		sprintf(s,"lh3app3remtest %s", shm->lh3app3remtest ); fsthasync(sync_to_node,s);
		}
	 	else sprintf(s,"lh3app3stat %d", 9 ); fsthasync(sync_to_node,s);
	}





sprintf(s,"passwd %s",shm->passwd); fsthasync(sync_to_node,s);
sprintf(s,"cstat %d",shm->cstat); fsthasync(sync_to_node,s);
release_sem();
shm->insync = NO;
}

  


int read_buf(int newSd, char *line_to_return) {
  
static char rcv_msg[MAX_MSG];
static int n;
int x;

/* read data from socket */
memset(rcv_msg,0x0,MAX_MSG); /* init buffer */
n = recv(newSd, rcv_msg, MAX_MSG, 0); /* wait for data */
if (n<0) {
	close(newSd);
	return ERROR;
} else if (n==0) {
	close(newSd);
	return ERROR; 
}
strcpy(line_to_return,rcv_msg);
if (strlen(line_to_return) < n-1) {
	sprintf(msg,"lenght=%d but recv=%d line_to_return=%s",
		strlen(line_to_return),n,line_to_return);
	logit(msg);
	printf("line_to_return=%s lenght=%d but recv=%d\n",
		line_to_return,strlen(line_to_return),n);
}

return 0;
}


main(int argc, char *argv[]) {
int	x;
char	s[BUFSIZ]="";
FILE	*f;
int sd, newSd, cliLen,port;
struct sockaddr_in cliAddr, servAddr;
char	buf[MAX_MSG];
char	sbuf[MAX_MSG];
char sync_to_node[128]="";


strcpy(fsthaprogname,argv[0]);
mypid=getpid();
fclose(stdin);
fclose(stdout);
fclose(stderr);
setsid();
umask(077);
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

sprintf(msg,"fstsvr_pid %d",mypid);
fsthahere(msg);

     /******* monitor existence of fsthbm **********/
if (isrunning("fsthbm") != YES) {
        sprintf(msg,"fsthbm Is Missing. Aborted.");
        logit(msg);
        shm->cstat = DOWN;
}


f=fopen("/opt/FSTha/bin/fstha.version","r");
if (f == NULL) {
strcpy(buf,"http://www.fstha.com/donate");
}
else {
fgets(buf,MAX_MSG,f);
buf[strlen(buf)-1]=0;
}
logit(buf);
sprintf(msg,"%s STARTUP %s ", fsthaprogname,BBCVERSION);
logit(msg);


port=atoi(shm->port);

  /* create socket */
if (shm->verbose == YES) 
	logit("Creating Socket.");
sd = socket(AF_INET, SOCK_STREAM, 0);
if(sd<0) {
    logit("cannot open socket ");
    return ERROR;
  }
if (shm->verbose == YES) 
	logit("Socket Created.");
  
if (shm->verbose == YES) 
	logit("Bind Port.");
/* bind server port */
servAddr.sin_family = AF_INET;
servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
servAddr.sin_port = htons(port);
  
if(bind(sd, (struct sockaddr *) &servAddr, sizeof(servAddr))<0) {
    logit("cannot bind port ");
    return ERROR;
}

listen(sd,128);
if (shm->verbose == YES) 
	logit("Listening on Port.");


    /*
     * Make socket non-blocking
if (shm->verbose == YES) 
	logit("Enablng Non-Blocking Port.");
if (fcntl(sd, F_SETFL, O_NDELAY) < 0) {
        perror("Can't set socket to non-blocking");
        exit(0);
    }
if (shm->verbose == YES)
        logit("Enabled Non-Blocking Port.");
     *
     */




while (1) {
	newSd = accept(sd,(struct sockaddr *)&cliAddr,&cliLen);
        if (newSd != -1) {
		memset(buf,0x0,MAX_MSG);
		while(readline(newSd,(char *)buf,MAX_MSG)!=-1) {


strcpy(sync_to_node,inet_ntoa(cliAddr.sin_addr));


		close(newSd);
		if (strlen(buf)==0)
			break;
		/* decrypt */
		for (x=0; x<strlen((char *)buf); x++)
			if ( buf[x] != 0 )
				sbuf[x]=buf[x]-27;
		sbuf[x]=0;

                sprintf(msg,"%s: received from %s: %s\n",argv[0],sync_to_node,sbuf);
		if (shm->verbose == YES) logit(msg);

		if (strncmp(sbuf,"ping",4) == 0 ) {
			continue;
		}
		if (strncmp(sbuf,"dump",4) == 0) {
			if (whatisip(sync_to_node) == 0)
				sprintf(msg,"node0stat %d",JOIN);
			else if (whatisip(sync_to_node) == 1)
				sprintf(msg,"node1stat %d",JOIN);
			else if (whatisip(sync_to_node) == 2)
				sprintf(msg,"node2stat %d",JOIN);
			else if (whatisip(sync_to_node) == 3)
				sprintf(msg,"node3stat %d",JOIN);
			fsthahere(msg);
			sleep(5);
			dump_conf(sync_to_node);
			if (whatisip(sync_to_node) == 0)
				sprintf(msg,"node0stat %d",WAIT);
			else if (whatisip(sync_to_node) == 1)
				sprintf(msg,"node1stat %d",WAIT);
			else if (whatisip(sync_to_node) == 2)
				sprintf(msg,"node2stat %d",WAIT);
			else if (whatisip(sync_to_node) == 3)
				sprintf(msg,"node3stat %d",WAIT);
			fsthahere(msg);
			continue;
			}
		if (update_conf(sbuf) != 0) {
			sprintf(msg,"Unrecognized tag: %s",sbuf);
			if (shm->verbose == YES) logit(msg);
			}
		else {
			sprintf(msg,"dirty %d",YES);
			fsthahere(msg);
		}

		/* init sbuf */
		memset(sbuf,0x0,MAX_MSG);
		memset(buf,0x0,MAX_MSG);
  
		} /* while(read_buf) */
	cliLen = sizeof(struct sockaddr_in);

        } else {
//            sleep(1);
        }
	cliLen = sizeof(struct sockaddr_in);
	/* init buf */
	memset(sbuf,0x0,MAX_MSG);
	memset(buf,0x0,MAX_MSG);

        close(newSd);
	if (shm->cstat == DOWN) 
		break;

}
close(newSd);
close(sd);
if (shm->cstat == DOWN) {
        sprintf(msg,"Waiting for Network Traffic");
	sleep(10);
        logit(msg);
        logit("fstsvr is Sutting Down");
	exit(0);
}


}
