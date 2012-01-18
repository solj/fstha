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
 * File Name: fsthbm.c
 * 		Main cluster daemon. Monitors hung scripts & down nodes
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
#include <sys/uadmin.h>
#include "fsthanet.h"

#define NIPQUAD(addr) \
         ((unsigned char *)&addr)[0], \
         ((unsigned char *)&addr)[1], \
         ((unsigned char *)&addr)[2], \
         ((unsigned char *)&addr)[3]


#define ERROR   1

#define END_LINE 0x0A

char msg2[128]="";


int fsthadump(int syncfromnode) {
char buf[BUFSIZ]="";
char msg[5]="dump";
char msg1[5]="ping";
char syncip[17];
int rc=1;

sleep(2);
sprintf(buf,"fsthadump sending %s",msg);
logit(buf);
if (syncfromnode != -1) {
		if (syncfromnode == 0)
			strcpy(syncip,shm->node0);
		else if (syncfromnode == 1)
			strcpy(syncip,shm->node1);
		else if (syncfromnode == 2)
			strcpy(syncip,shm->node2);
		else if (syncfromnode == 3)
			strcpy(syncip,shm->node3);
		sprintf(buf,"Pinging %s",syncip);
		logit(buf);
		rc =  tcpClient(syncip,shm->port,msg1);
		sleep(2);
		sprintf(buf,"fsthadump sending %s to %s",msg,syncip);
		logit(buf);
		rc =  tcpClient(syncip,shm->port,msg);
} else if (shm->qnode == 0) {
		sprintf(buf,"Pinging %s",shm->node0);
		logit(buf);
		rc =  tcpClient(shm->node0,shm->port,msg1);
		sleep(2);
		sprintf(buf,"fsthadump sending %s to %s",msg,shm->node0);
		logit(buf);
		rc =  tcpClient(shm->node0,shm->port,msg);
} else if (shm->qnode == 1) {
		sprintf(buf,"Pinging %s",shm->node1);
		logit(buf);
		rc =  tcpClient(shm->node1,shm->port,msg1);
		sleep(2);
		sprintf(buf,"fsthadump sending %s to %s",msg,shm->node1);
		logit(buf);
		rc =  tcpClient(shm->node1,shm->port,msg);
} else if (shm->qnode == 2) {
		sprintf(buf,"Pinging %s",shm->node2);
		logit(buf);
		rc =  tcpClient(shm->node2,shm->port,msg1);
		sleep(2);
		sprintf(buf,"fsthadump sending %s to %s",msg,shm->node2);
		logit(buf);
		rc =  tcpClient(shm->node2,shm->port,msg);
} else if (shm->qnode == 3) {
		sprintf(buf,"Pinging %s",shm->node3);
		logit(buf);
		rc =  tcpClient(shm->node3,shm->port,msg1);
		sleep(2);
		sprintf(buf,"fsthadump sending %s to %s",msg,shm->node3);
		logit(buf);
		rc =  tcpClient(shm->node3,shm->port,msg);
}

if (shm->verbose == YES) {
	sprintf(buf,"tcpClient returned %d",rc);
	logit(buf);
}
return rc;

}

float getmyload(int x) {
FILE *f;
char s[BUFSIZ]="",fn[BUFSIZ]="";
float y;

sprintf(fn,"/tmp/%s.load.%d",fsthaprogname,x);
sprintf(msg,"uptime|awk -F',' '{print $4}'|awk '{print $3}' > %s",fn);
system(msg);
f=fopen(fn,"r");
while (fgets(s,BUFSIZ,f) != NULL);
sscanf(s,"%f",&y);
fclose(f);
return y;
}

put_conf() {
	char s[BUFSIZ],dummy[128];
	time_t now;

	sprintf(s,"%s.adv",shm->cf);
        confilep=fopen(s,"w");
        if ( confilep  == NULL) {
                sprintf(msg,"create failed: %s",shm->cf);
                die(msg);
        }
	time(&now);
fprintf(confilep,"verbose %d\n",shm->verbose); fflush(confilep);
fprintf(confilep,"passwd %s\n",shm->passwd); fflush(confilep);
fprintf(confilep,"hbtimeout %d\n",shm->hbtimeout); fflush(confilep);
fprintf(confilep,"hbdelay %d\n",shm->hbdelay); fflush(confilep);
fprintf(confilep,"version %d\n",shm->version); fflush(confilep);
fprintf(confilep,"htmlrefreshsecs %d\n",shm->htmlrefreshsecs); fflush(confilep);
fprintf(confilep,"htmlallowexec %d\n",shm->htmlallowexec); fflush(confilep);
fprintf(confilep,"port %s\n",shm->port); fflush(confilep);
fprintf(confilep,"hbport %s\n",shm->hbport); fflush(confilep);
fprintf(confilep,"logfile %s\n",shm->logfile); fflush(confilep);
fprintf(confilep,"syslogfac %s\n",shm->syslogfac); fflush(confilep);
if (shm->reserve_disk[0]==0) strcpy(shm->reserve_disk,"none");
fprintf(confilep,"reserve_disk %s\n",shm->reserve_disk); fflush(confilep);
fprintf(confilep,"reserve_strict %d\n",shm->reserve_strict); fflush(confilep);
fprintf(confilep,"node0 %s\n",shm->node0); fflush(confilep);
fprintf(confilep,"node1 %s\n",shm->node1); fflush(confilep);
fprintf(confilep,"node2 %s\n",shm->node2); fflush(confilep);
fprintf(confilep,"node3 %s\n",shm->node3); fflush(confilep);
fprintf(confilep,"nodeN %d\n",shm->nodeN); fflush(confilep);
fprintf(confilep,"cstat %d\n",shm->cstat); fflush(confilep);
fprintf(confilep,"hb0 %s %s %s %s\n",shm->hb0dv,shm->hb0ip,shm->hb0mask,shm->hb0bcast); fflush(confilep);
fprintf(confilep,"hb1 %s %s %s %s\n",shm->hb1dv,shm->hb1ip,shm->hb1mask,shm->hb1bcast); fflush(confilep);
fprintf(confilep,"hb0pg0 %s\n",shm->hb0pg0); fflush(confilep);
fprintf(confilep,"hb0pg1 %s\n",shm->hb0pg1); fflush(confilep);
fprintf(confilep,"hb0pg2 %s\n",shm->hb0pg2); fflush(confilep);
fprintf(confilep,"hb0pg3 %s\n",shm->hb0pg3); fflush(confilep);
fprintf(confilep,"hb1pg0 %s\n",shm->hb1pg0); fflush(confilep);
fprintf(confilep,"hb1pg1 %s\n",shm->hb1pg1); fflush(confilep);
fprintf(confilep,"hb1pg2 %s\n",shm->hb1pg2); fflush(confilep);
fprintf(confilep,"hb1pg3 %s\n",shm->hb1pg3); fflush(confilep);
fprintf(confilep,"lhfailtimeo %d\n",shm->lhfailtimeo); fflush(confilep);
fprintf(confilep,"appfailtimeo %d\n",shm->appfailtimeo); fflush(confilep);
fprintf(confilep,"appfailtesttimeo %d\n",shm->appfailtesttimeo); fflush(confilep);


fprintf(confilep,"lh0name %s\n",shm->lh0name); fflush(confilep);
fprintf(confilep,"lh0path %s\n",shm->lh0path); fflush(confilep);
fprintf(confilep,"lh0stat %d\n",shm->lh0stat); fflush(confilep);
fprintf(confilep,"lh0node %d\n",shm->lh0node); fflush(confilep);
fprintf(confilep,"lh0fallback %d\n",shm->lh0fallback); fflush(confilep);
fprintf(confilep,"lh0failto %d\n",shm->lh0failto); fflush(confilep);
fprintf(confilep,"lh0app0name %s\n",shm->lh0app0name); fflush(confilep);
fprintf(confilep,"lh0app0stat %d\n",shm->lh0app0stat); fflush(confilep);
fprintf(confilep,"lh0app0start %s\n",shm->lh0app0start); fflush(confilep);
fprintf(confilep,"lh0app0stop %s\n",shm->lh0app0stop); fflush(confilep);
fprintf(confilep,"lh0app0test %s\n",shm->lh0app0test); fflush(confilep);
fprintf(confilep,"lh0app0timeo %d\n",shm->lh0app0timeo); fflush(confilep);
fprintf(confilep,"lh0app0testtimeo %d\n",shm->lh0app0testtimeo); fflush(confilep);
fprintf(confilep,"lh0app0remtest %s\n",shm->lh0app0remtest); fflush(confilep);
fprintf(confilep,"lh0app0remtesttimeo %d\n",shm->lh0app0remtesttimeo); fflush(confilep);
fprintf(confilep,"lh0app0remnode %d\n",shm->lh0app0remnode); fflush(confilep);
fprintf(confilep,"lh0app0secs %d\n",shm->lh0app0secs); fflush(confilep);
fprintf(confilep,"lh0app0pause %d\n",shm->lh0app0pause); fflush(confilep);
fprintf(confilep,"lh0app0crit %d\n",shm->lh0app0crit); fflush(confilep);
fprintf(confilep,"lh0app1name %s\n",shm->lh0app1name); fflush(confilep);
fprintf(confilep,"lh0app1stat %d\n",shm->lh0app1stat); fflush(confilep);
fprintf(confilep,"lh0app1start %s\n",shm->lh0app1start); fflush(confilep);
fprintf(confilep,"lh0app1stop %s\n",shm->lh0app1stop); fflush(confilep);
fprintf(confilep,"lh0app1test %s\n",shm->lh0app1test); fflush(confilep);
fprintf(confilep,"lh0app1timeo %d\n",shm->lh0app1timeo); fflush(confilep);
fprintf(confilep,"lh0app1testtimeo %d\n",shm->lh0app1testtimeo); fflush(confilep);
fprintf(confilep,"lh0app1remtest %s\n",shm->lh0app1remtest); fflush(confilep);
fprintf(confilep,"lh0app1remtesttimeo %d\n",shm->lh0app1remtesttimeo); fflush(confilep);
fprintf(confilep,"lh0app1remnode %d\n",shm->lh0app1remnode); fflush(confilep);
fprintf(confilep,"lh0app1secs %d\n",shm->lh0app1secs); fflush(confilep);
fprintf(confilep,"lh0app1pause %d\n",shm->lh0app1pause); fflush(confilep);
fprintf(confilep,"lh0app1crit %d\n",shm->lh0app1crit); fflush(confilep);
fprintf(confilep,"lh0app2name %s\n",shm->lh0app2name); fflush(confilep);
fprintf(confilep,"lh0app2stat %d\n",shm->lh0app2stat); fflush(confilep);
fprintf(confilep,"lh0app2start %s\n",shm->lh0app2start); fflush(confilep);
fprintf(confilep,"lh0app2stop %s\n",shm->lh0app2stop); fflush(confilep);
fprintf(confilep,"lh0app2test %s\n",shm->lh0app2test); fflush(confilep);
fprintf(confilep,"lh0app2timeo %d\n",shm->lh0app2timeo); fflush(confilep);
fprintf(confilep,"lh0app2testtimeo %d\n",shm->lh0app2testtimeo); fflush(confilep);
fprintf(confilep,"lh0app2remtest %s\n",shm->lh0app2remtest); fflush(confilep);
fprintf(confilep,"lh0app2remtesttimeo %d\n",shm->lh0app2remtesttimeo); fflush(confilep);
fprintf(confilep,"lh0app2remnode %d\n",shm->lh0app2remnode); fflush(confilep);
fprintf(confilep,"lh0app2secs %d\n",shm->lh0app2secs); fflush(confilep);
fprintf(confilep,"lh0app2pause %d\n",shm->lh0app2pause); fflush(confilep);
fprintf(confilep,"lh0app2crit %d\n",shm->lh0app2crit); fflush(confilep);
fprintf(confilep,"lh0app3name %s\n",shm->lh0app3name); fflush(confilep);
fprintf(confilep,"lh0app3stat %d\n",shm->lh0app3stat); fflush(confilep);
fprintf(confilep,"lh0app3start %s\n",shm->lh0app3start); fflush(confilep);
fprintf(confilep,"lh0app3stop %s\n",shm->lh0app3stop); fflush(confilep);
fprintf(confilep,"lh0app3test %s\n",shm->lh0app3test); fflush(confilep);
fprintf(confilep,"lh0app3timeo %d\n",shm->lh0app3timeo); fflush(confilep);
fprintf(confilep,"lh0app3testtimeo %d\n",shm->lh0app3testtimeo); fflush(confilep);
fprintf(confilep,"lh0app3remtest %s\n",shm->lh0app3remtest); fflush(confilep);
fprintf(confilep,"lh0app3remtesttimeo %d\n",shm->lh0app3remtesttimeo); fflush(confilep);
fprintf(confilep,"lh0app3remnode %d\n",shm->lh0app3remnode); fflush(confilep);
fprintf(confilep,"lh0app3secs %d\n",shm->lh0app3secs); fflush(confilep);
fprintf(confilep,"lh0app3pause %d\n",shm->lh0app3pause); fflush(confilep);
fprintf(confilep,"lh0app3crit %d\n",shm->lh0app3crit); fflush(confilep);

	
fprintf(confilep,"lh1name %s\n",shm->lh1name); fflush(confilep);
fprintf(confilep,"lh1path %s\n",shm->lh1path); fflush(confilep);
fprintf(confilep,"lh1stat %d\n",shm->lh1stat); fflush(confilep);
fprintf(confilep,"lh1node %d\n",shm->lh1node); fflush(confilep);
fprintf(confilep,"lh1fallback %d\n",shm->lh1fallback); fflush(confilep);
fprintf(confilep,"lh1failto %d\n",shm->lh1failto); fflush(confilep);
fprintf(confilep,"lh1app0name %s\n",shm->lh1app0name); fflush(confilep);
fprintf(confilep,"lh1app0stat %d\n",shm->lh1app0stat); fflush(confilep);
fprintf(confilep,"lh1app0start %s\n",shm->lh1app0start); fflush(confilep);
fprintf(confilep,"lh1app0stop %s\n",shm->lh1app0stop); fflush(confilep);
fprintf(confilep,"lh1app0test %s\n",shm->lh1app0test); fflush(confilep);
fprintf(confilep,"lh1app0timeo %d\n",shm->lh1app0timeo); fflush(confilep);
fprintf(confilep,"lh1app0testtimeo %d\n",shm->lh1app0testtimeo); fflush(confilep);
fprintf(confilep,"lh1app0remtest %s\n",shm->lh1app0remtest); fflush(confilep);
fprintf(confilep,"lh1app0remtesttimeo %d\n",shm->lh1app0remtesttimeo); fflush(confilep);
fprintf(confilep,"lh1app0remnode %d\n",shm->lh1app0remnode); fflush(confilep);
fprintf(confilep,"lh1app0secs %d\n",shm->lh1app0secs); fflush(confilep);
fprintf(confilep,"lh1app0pause %d\n",shm->lh1app0pause); fflush(confilep);
fprintf(confilep,"lh1app0crit %d\n",shm->lh1app0crit); fflush(confilep);
fprintf(confilep,"lh1app1name %s\n",shm->lh1app1name); fflush(confilep);
fprintf(confilep,"lh1app1stat %d\n",shm->lh1app1stat); fflush(confilep);
fprintf(confilep,"lh1app1start %s\n",shm->lh1app1start); fflush(confilep);
fprintf(confilep,"lh1app1stop %s\n",shm->lh1app1stop); fflush(confilep);
fprintf(confilep,"lh1app1test %s\n",shm->lh1app1test); fflush(confilep);
fprintf(confilep,"lh1app1timeo %d\n",shm->lh1app1timeo); fflush(confilep);
fprintf(confilep,"lh1app1testtimeo %d\n",shm->lh1app1testtimeo); fflush(confilep);
fprintf(confilep,"lh1app1remtest %s\n",shm->lh1app1remtest); fflush(confilep);
fprintf(confilep,"lh1app1remtesttimeo %d\n",shm->lh1app1remtesttimeo); fflush(confilep);
fprintf(confilep,"lh1app1remnode %d\n",shm->lh1app1remnode); fflush(confilep);
fprintf(confilep,"lh1app1secs %d\n",shm->lh1app1secs); fflush(confilep);
fprintf(confilep,"lh1app1pause %d\n",shm->lh1app1pause); fflush(confilep);
fprintf(confilep,"lh1app1crit %d\n",shm->lh1app1crit); fflush(confilep);
fprintf(confilep,"lh1app2name %s\n",shm->lh1app2name); fflush(confilep);
fprintf(confilep,"lh1app2stat %d\n",shm->lh1app2stat); fflush(confilep);
fprintf(confilep,"lh1app2start %s\n",shm->lh1app2start); fflush(confilep);
fprintf(confilep,"lh1app2stop %s\n",shm->lh1app2stop); fflush(confilep);
fprintf(confilep,"lh1app2test %s\n",shm->lh1app2test); fflush(confilep);
fprintf(confilep,"lh1app2timeo %d\n",shm->lh1app2timeo); fflush(confilep);
fprintf(confilep,"lh1app2testtimeo %d\n",shm->lh1app2testtimeo); fflush(confilep);
fprintf(confilep,"lh1app2remtest %s\n",shm->lh1app2remtest); fflush(confilep);
fprintf(confilep,"lh1app2remtesttimeo %d\n",shm->lh1app2remtesttimeo); fflush(confilep);
fprintf(confilep,"lh1app2remnode %d\n",shm->lh1app2remnode); fflush(confilep);
fprintf(confilep,"lh1app2secs %d\n",shm->lh1app2secs); fflush(confilep);
fprintf(confilep,"lh1app2pause %d\n",shm->lh1app2pause); fflush(confilep);
fprintf(confilep,"lh1app2crit %d\n",shm->lh1app2crit); fflush(confilep);
fprintf(confilep,"lh1app3name %s\n",shm->lh1app3name); fflush(confilep);
fprintf(confilep,"lh1app3stat %d\n",shm->lh1app3stat); fflush(confilep);
fprintf(confilep,"lh1app3start %s\n",shm->lh1app3start); fflush(confilep);
fprintf(confilep,"lh1app3stop %s\n",shm->lh1app3stop); fflush(confilep);
fprintf(confilep,"lh1app3test %s\n",shm->lh1app3test); fflush(confilep);
fprintf(confilep,"lh1app3timeo %d\n",shm->lh1app3timeo); fflush(confilep);
fprintf(confilep,"lh1app3testtimeo %d\n",shm->lh1app3testtimeo); fflush(confilep);
fprintf(confilep,"lh1app3remtest %s\n",shm->lh1app3remtest); fflush(confilep);
fprintf(confilep,"lh1app3remtesttimeo %d\n",shm->lh1app3remtesttimeo); fflush(confilep);
fprintf(confilep,"lh1app3remnode %d\n",shm->lh1app3remnode); fflush(confilep);
fprintf(confilep,"lh1app3secs %d\n",shm->lh1app3secs); fflush(confilep);
fprintf(confilep,"lh1app3pause %d\n",shm->lh1app3pause); fflush(confilep);
fprintf(confilep,"lh1app3crit %d\n",shm->lh1app3crit); fflush(confilep);

	
fprintf(confilep,"lh2name %s\n",shm->lh2name); fflush(confilep);
fprintf(confilep,"lh2path %s\n",shm->lh2path); fflush(confilep);
fprintf(confilep,"lh2stat %d\n",shm->lh2stat); fflush(confilep);
fprintf(confilep,"lh2node %d\n",shm->lh2node); fflush(confilep);
fprintf(confilep,"lh2fallback %d\n",shm->lh2fallback); fflush(confilep);
fprintf(confilep,"lh2failto %d\n",shm->lh2failto); fflush(confilep);
fprintf(confilep,"lh2app0name %s\n",shm->lh2app0name); fflush(confilep);
fprintf(confilep,"lh2app0stat %d\n",shm->lh2app0stat); fflush(confilep);
fprintf(confilep,"lh2app0start %s\n",shm->lh2app0start); fflush(confilep);
fprintf(confilep,"lh2app0stop %s\n",shm->lh2app0stop); fflush(confilep);
fprintf(confilep,"lh2app0test %s\n",shm->lh2app0test); fflush(confilep);
fprintf(confilep,"lh2app0timeo %d\n",shm->lh2app0timeo); fflush(confilep);
fprintf(confilep,"lh2app0testtimeo %d\n",shm->lh2app0testtimeo); fflush(confilep);
fprintf(confilep,"lh2app0remtest %s\n",shm->lh2app0remtest); fflush(confilep);
fprintf(confilep,"lh2app0remtesttimeo %d\n",shm->lh2app0remtesttimeo); fflush(confilep);
fprintf(confilep,"lh2app0remnode %d\n",shm->lh2app0remnode); fflush(confilep);
fprintf(confilep,"lh2app0secs %d\n",shm->lh2app0secs); fflush(confilep);
fprintf(confilep,"lh2app0pause %d\n",shm->lh2app0pause); fflush(confilep);
fprintf(confilep,"lh2app0crit %d\n",shm->lh2app0crit); fflush(confilep);
fprintf(confilep,"lh2app1name %s\n",shm->lh2app1name); fflush(confilep);
fprintf(confilep,"lh2app1stat %d\n",shm->lh2app1stat); fflush(confilep);
fprintf(confilep,"lh2app1start %s\n",shm->lh2app1start); fflush(confilep);
fprintf(confilep,"lh2app1stop %s\n",shm->lh2app1stop); fflush(confilep);
fprintf(confilep,"lh2app1test %s\n",shm->lh2app1test); fflush(confilep);
fprintf(confilep,"lh2app1timeo %d\n",shm->lh2app1timeo); fflush(confilep);
fprintf(confilep,"lh2app1testtimeo %d\n",shm->lh2app1testtimeo); fflush(confilep);
fprintf(confilep,"lh2app1remtest %s\n",shm->lh2app1remtest); fflush(confilep);
fprintf(confilep,"lh2app1remtesttimeo %d\n",shm->lh2app1remtesttimeo); fflush(confilep);
fprintf(confilep,"lh2app1remnode %d\n",shm->lh2app1remnode); fflush(confilep);
fprintf(confilep,"lh2app1secs %d\n",shm->lh2app1secs); fflush(confilep);
fprintf(confilep,"lh2app1pause %d\n",shm->lh2app1pause); fflush(confilep);
fprintf(confilep,"lh2app1crit %d\n",shm->lh2app1crit); fflush(confilep);
fprintf(confilep,"lh2app2name %s\n",shm->lh2app2name); fflush(confilep);
fprintf(confilep,"lh2app2stat %d\n",shm->lh2app2stat); fflush(confilep);
fprintf(confilep,"lh2app2start %s\n",shm->lh2app2start); fflush(confilep);
fprintf(confilep,"lh2app2stop %s\n",shm->lh2app2stop); fflush(confilep);
fprintf(confilep,"lh2app2test %s\n",shm->lh2app2test); fflush(confilep);
fprintf(confilep,"lh2app2timeo %d\n",shm->lh2app2timeo); fflush(confilep);
fprintf(confilep,"lh2app2testtimeo %d\n",shm->lh2app2testtimeo); fflush(confilep);
fprintf(confilep,"lh2app2remtest %s\n",shm->lh2app2remtest); fflush(confilep);
fprintf(confilep,"lh2app2remtesttimeo %d\n",shm->lh2app2remtesttimeo); fflush(confilep);
fprintf(confilep,"lh2app2remnode %d\n",shm->lh2app2remnode); fflush(confilep);
fprintf(confilep,"lh2app2secs %d\n",shm->lh2app2secs); fflush(confilep);
fprintf(confilep,"lh2app2pause %d\n",shm->lh2app2pause); fflush(confilep);
fprintf(confilep,"lh2app2crit %d\n",shm->lh2app2crit); fflush(confilep);
fprintf(confilep,"lh2app3name %s\n",shm->lh2app3name); fflush(confilep);
fprintf(confilep,"lh2app3stat %d\n",shm->lh2app3stat); fflush(confilep);
fprintf(confilep,"lh2app3start %s\n",shm->lh2app3start); fflush(confilep);
fprintf(confilep,"lh2app3stop %s\n",shm->lh2app3stop); fflush(confilep);
fprintf(confilep,"lh2app3test %s\n",shm->lh2app3test); fflush(confilep);
fprintf(confilep,"lh2app3timeo %d\n",shm->lh2app3timeo); fflush(confilep);
fprintf(confilep,"lh2app3testtimeo %d\n",shm->lh2app3testtimeo); fflush(confilep);
fprintf(confilep,"lh2app3remtest %s\n",shm->lh2app3remtest); fflush(confilep);
fprintf(confilep,"lh2app3remtesttimeo %d\n",shm->lh2app3remtesttimeo); fflush(confilep);
fprintf(confilep,"lh2app3remnode %d\n",shm->lh2app3remnode); fflush(confilep);
fprintf(confilep,"lh2app3secs %d\n",shm->lh2app3secs); fflush(confilep);
fprintf(confilep,"lh2app3pause %d\n",shm->lh2app3pause); fflush(confilep);
fprintf(confilep,"lh2app3crit %d\n",shm->lh2app3crit); fflush(confilep);

	
fprintf(confilep,"lh3name %s\n",shm->lh3name); fflush(confilep);
fprintf(confilep,"lh3path %s\n",shm->lh3path); fflush(confilep);
fprintf(confilep,"lh3stat %d\n",shm->lh3stat); fflush(confilep);
fprintf(confilep,"lh3node %d\n",shm->lh3node); fflush(confilep);
fprintf(confilep,"lh3fallback %d\n",shm->lh3fallback); fflush(confilep);
fprintf(confilep,"lh3failto %d\n",shm->lh3failto); fflush(confilep);
fprintf(confilep,"lh3app0name %s\n",shm->lh3app0name); fflush(confilep);
fprintf(confilep,"lh3app0stat %d\n",shm->lh3app0stat); fflush(confilep);
fprintf(confilep,"lh3app0start %s\n",shm->lh3app0start); fflush(confilep);
fprintf(confilep,"lh3app0stop %s\n",shm->lh3app0stop); fflush(confilep);
fprintf(confilep,"lh3app0test %s\n",shm->lh3app0test); fflush(confilep);
fprintf(confilep,"lh3app0timeo %d\n",shm->lh3app0timeo); fflush(confilep);
fprintf(confilep,"lh3app0testtimeo %d\n",shm->lh3app0testtimeo); fflush(confilep);
fprintf(confilep,"lh3app0remtest %s\n",shm->lh3app0remtest); fflush(confilep);
fprintf(confilep,"lh3app0remtesttimeo %d\n",shm->lh3app0remtesttimeo); fflush(confilep);
fprintf(confilep,"lh3app0remnode %d\n",shm->lh3app0remnode); fflush(confilep);
fprintf(confilep,"lh3app0secs %d\n",shm->lh3app0secs); fflush(confilep);
fprintf(confilep,"lh3app0pause %d\n",shm->lh3app0pause); fflush(confilep);
fprintf(confilep,"lh3app0crit %d\n",shm->lh3app0crit); fflush(confilep);
fprintf(confilep,"lh3app1name %s\n",shm->lh3app1name); fflush(confilep);
fprintf(confilep,"lh3app1stat %d\n",shm->lh3app1stat); fflush(confilep);
fprintf(confilep,"lh3app1start %s\n",shm->lh3app1start); fflush(confilep);
fprintf(confilep,"lh3app1stop %s\n",shm->lh3app1stop); fflush(confilep);
fprintf(confilep,"lh3app1test %s\n",shm->lh3app1test); fflush(confilep);
fprintf(confilep,"lh3app1timeo %d\n",shm->lh3app1timeo); fflush(confilep);
fprintf(confilep,"lh3app1testtimeo %d\n",shm->lh3app1testtimeo); fflush(confilep);
fprintf(confilep,"lh3app1remtest %s\n",shm->lh3app1remtest); fflush(confilep);
fprintf(confilep,"lh3app1remtesttimeo %d\n",shm->lh3app1remtesttimeo); fflush(confilep);
fprintf(confilep,"lh3app1remnode %d\n",shm->lh3app1remnode); fflush(confilep);
fprintf(confilep,"lh3app1secs %d\n",shm->lh3app1secs); fflush(confilep);
fprintf(confilep,"lh3app1pause %d\n",shm->lh3app1pause); fflush(confilep);
fprintf(confilep,"lh3app1crit %d\n",shm->lh3app1crit); fflush(confilep);
fprintf(confilep,"lh3app2name %s\n",shm->lh3app2name); fflush(confilep);
fprintf(confilep,"lh3app2stat %d\n",shm->lh3app2stat); fflush(confilep);
fprintf(confilep,"lh3app2start %s\n",shm->lh3app2start); fflush(confilep);
fprintf(confilep,"lh3app2stop %s\n",shm->lh3app2stop); fflush(confilep);
fprintf(confilep,"lh3app2test %s\n",shm->lh3app2test); fflush(confilep);
fprintf(confilep,"lh3app2timeo %d\n",shm->lh3app2timeo); fflush(confilep);
fprintf(confilep,"lh3app2testtimeo %d\n",shm->lh3app2testtimeo); fflush(confilep);
fprintf(confilep,"lh3app2remtest %s\n",shm->lh3app2remtest); fflush(confilep);
fprintf(confilep,"lh3app2remtesttimeo %d\n",shm->lh3app2remtesttimeo); fflush(confilep);
fprintf(confilep,"lh3app2remnode %d\n",shm->lh3app2remnode); fflush(confilep);
fprintf(confilep,"lh3app2secs %d\n",shm->lh3app2secs); fflush(confilep);
fprintf(confilep,"lh3app2pause %d\n",shm->lh3app2pause); fflush(confilep);
fprintf(confilep,"lh3app2crit %d\n",shm->lh3app2crit); fflush(confilep);
fprintf(confilep,"lh3app3name %s\n",shm->lh3app3name); fflush(confilep);
fprintf(confilep,"lh3app3stat %d\n",shm->lh3app3stat); fflush(confilep);
fprintf(confilep,"lh3app3start %s\n",shm->lh3app3start); fflush(confilep);
fprintf(confilep,"lh3app3stop %s\n",shm->lh3app3stop); fflush(confilep);
fprintf(confilep,"lh3app3test %s\n",shm->lh3app3test); fflush(confilep);
fprintf(confilep,"lh3app3timeo %d\n",shm->lh3app3timeo); fflush(confilep);
fprintf(confilep,"lh3app3testtimeo %d\n",shm->lh3app3testtimeo); fflush(confilep);
fprintf(confilep,"lh3app3remtest %s\n",shm->lh3app3remtest); fflush(confilep);
fprintf(confilep,"lh3app3remtesttimeo %d\n",shm->lh3app3remtesttimeo); fflush(confilep);
fprintf(confilep,"lh3app3remnode %d\n",shm->lh3app3remnode); fflush(confilep);
fprintf(confilep,"lh3app3secs %d\n",shm->lh3app3secs); fflush(confilep);
fprintf(confilep,"lh3app3pause %d\n",shm->lh3app3pause); fflush(confilep);
fprintf(confilep,"lh3app3crit %d\n",shm->lh3app3crit); fflush(confilep);

	

	
        fprintf(confilep,"%d",now); fflush(confilep);
        fclose(confilep);
	sprintf(s,"mv %s.adv %s",shm->cf,shm->cf);
	system(s);
}

  
fstlhtrans (int lhnum,int *lhnode,int *lhstat,int *lhtimeo,char *lhname,int *lhfailing,
	int *lhapp0stat,
	int *lhapp0crit,
	int *lhapp0testres,
	char *lhapp0name,
	int *lhapp0timeo,
	int *lhapp0testtimeo,
	int *lhapp0remtesttimeo,
	int *lhapp0remnode,
	int *lhapp1stat,
	int *lhapp1crit,
	int *lhapp1testres,
	char *lhapp1name,
	int *lhapp1timeo,
	int *lhapp1testtimeo,
	int *lhapp1remtesttimeo,
	int *lhapp1remnode,
	int *lhapp2stat,
	int *lhapp2crit,
	int *lhapp2testres,
	char *lhapp2name,
	int *lhapp2timeo,
	int *lhapp2testtimeo,
	int *lhapp2remtesttimeo,
	int *lhapp2remnode,
	int *lhapp3stat,
	int *lhapp3crit,
	int *lhapp3testres,
	char *lhapp3name,
	int *lhapp3timeo,
	int *lhapp3testtimeo,
	int *lhapp3remtesttimeo,
	int *lhapp3remnode
	) {
char msg[BUFSIZ]="";
time_t now;

if (*lhnode == shm->nodeN ) {
	/***** monitor lh transition time stamp ******/
	time(&now);
	if ( (*lhstat == GUP ||  *lhstat == GDOWN )
	&&	*lhtimeo != 0
	&&	(now - shm->lhfailtimeo > *lhtimeo ) ){
		sprintf(msg,"%s %d at %d failed",
			lhname,*lhtimeo,now);
		logit(msg);
		sprintf(msg2,"/opt/FSTha/bin/notify Virtualhost Transition Failure: %s",msg);
		system(msg2);
		sprintf(msg,"lh%d%d",lhnum,*lhtimeo);
		fstkill(msg);
		sprintf(msg,"lh%dstat %d",lhnum,FAILED);
		fsthahere(msg);
		sprintf(msg,"lh%dtimeo 0",lhnum);
		fsthahere(msg);
		sprintf(msg,"dirty 0",YES);
		fsthahere(msg);

	}
	
	if (*lhstat == UP) {
		/***** monitor lhapp test ******/

               if (*lhapp0crit != 0 && *lhapp0stat == ON &&
                       *lhapp0testres >= *lhapp0crit) {
			sprintf(msg,"%s of %s  failed",
				lhapp0name,lhname);
			logit(msg);
			sprintf(msg2,"/opt/FSTha/bin/notify Application Failure: %s",msg);
			system(msg2);
			sprintf(msg,"lh%dfailing %d",lhnum,FAILED);
			fsthahere(msg);
			sprintf(msg,"lh%dstat %d",lhnum,GDOWN);
			fsthahere(msg);
			sprintf(msg,"dirty 0",YES);
			fsthahere(msg);
		}
		/******* app on/off script timeout ******/
               if (*lhapp0timeo != 0 && (now  - shm->appfailtimeo > *lhapp0timeo)) {
			sprintf(msg,"%s of %s  failed (timeout)",
				lhapp0name,lhname);
			logit(msg);
			sprintf(msg2,"/opt/FSTha/bin/notify Application Script Time-out: %s",msg);
			system(msg2);
			sprintf(msg,"/tmp/lh%dapp0%d",lhnum,*lhapp0timeo);
			fstkill(msg);
			sprintf(msg,"lh%dapp0timeo 0",lhnum);
			fsthahere(msg);
			sprintf(msg,"dirty 0",YES);
			fsthahere(msg);
		}
		/***** app test script timeout ***/
               if (*lhapp0testtimeo != 0 && (now  - shm->appfailtesttimeo > *lhapp0testtimeo)) {
			sprintf(msg,"Testing %s of %s  failed (timeout)",
				lhapp0name,lhname);
			logit(msg);
			sprintf(msg2,"/opt/FSTha/bin/notify Application Test Script Time-out: %s",msg);
			system(msg2);
			sprintf(msg,"/tmp/lh%dapp0test%d",lhnum,*lhapp0testtimeo);
			fstkill(msg);
			sprintf(msg,"lh%dapp0testtimeo 0",lhnum);
			fsthahere(msg);
			sprintf(msg,"dirty 0",YES);
			fsthahere(msg);
		}

               if (*lhapp1crit != 0 && *lhapp1stat == ON &&
                       *lhapp1testres >= *lhapp1crit) {
			sprintf(msg,"%s of %s  failed",
				lhapp1name,lhname);
			logit(msg);
			sprintf(msg2,"/opt/FSTha/bin/notify Application Failure: %s",msg);
			system(msg2);
			sprintf(msg,"lh%dfailing %d",lhnum,FAILED);
			fsthahere(msg);
			sprintf(msg,"lh%dstat %d",lhnum,GDOWN);
			fsthahere(msg);
			sprintf(msg,"dirty 0",YES);
			fsthahere(msg);
		}
		/******* app on/off script timeout ******/
               if (*lhapp1timeo != 0 && (now  - shm->appfailtimeo > *lhapp1timeo)) {
			sprintf(msg,"%s of %s  failed (timeout)",
				lhapp1name,lhname);
			logit(msg);
                        sprintf(msg2,"/opt/FSTha/bin/notify Application Script Time-out: %s",msg);
                        system(msg2);
			sprintf(msg,"/tmp/lh%dapp1%d",lhnum,*lhapp1timeo);
			fstkill(msg);
			sprintf(msg,"lh%dapp1timeo 0",lhnum);
			fsthahere(msg);
			sprintf(msg,"dirty 0",YES);
			fsthahere(msg);
		}
		/***** app test script timeout ***/
               if (*lhapp1testtimeo != 0 && (now  - shm->appfailtesttimeo > *lhapp1testtimeo)) {
			sprintf(msg,"Testing %s of %s  failed (timeout)",
				lhapp1name,lhname);
			logit(msg);
                        sprintf(msg2,"/opt/FSTha/bin/notify Application Test Script Time-out: %s",msg);
                        system(msg2);
			sprintf(msg,"/tmp/lh%dapp1test%d",lhnum,*lhapp1testtimeo);
			fstkill(msg);
			sprintf(msg,"lh%dapp1testtimeo 0",lhnum);
			fsthahere(msg);
			sprintf(msg,"dirty 0",YES);
			fsthahere(msg);
		}

               if (*lhapp2crit != 0 && *lhapp2stat == ON &&
                       *lhapp2testres >= *lhapp2crit) {
			sprintf(msg,"%s of %s  failed",
				lhapp2name,lhname);
			logit(msg);
                        sprintf(msg2,"/opt/FSTha/bin/notify Application Failure: %s",msg);
                        system(msg2);
			sprintf(msg,"lh%dfailing %d",lhnum,FAILED);
			fsthahere(msg);
			sprintf(msg,"lh%dstat %d",lhnum,GDOWN);
			fsthahere(msg);
			sprintf(msg,"dirty 0",YES);
			fsthahere(msg);
		}
		/******* app on/off script timeout ******/
               if (*lhapp2timeo != 0 && (now  - shm->appfailtimeo > *lhapp2timeo)) {
			sprintf(msg,"%s of %s  failed (timeout)",
				lhapp2name,lhname);
			logit(msg);
                        sprintf(msg2,"/opt/FSTha/bin/notify Application Script Time-out: %s",msg);
                        system(msg2);
			sprintf(msg,"/tmp/lh%dapp2%d",lhnum,*lhapp2timeo);
			fstkill(msg);
			sprintf(msg,"lh%dapp2timeo 0",lhnum);
			fsthahere(msg);
			sprintf(msg,"dirty 0",YES);
			fsthahere(msg);
		}
		/***** app test script timeout ***/
               if (*lhapp2testtimeo != 0 && (now  - shm->appfailtesttimeo > *lhapp2testtimeo)) {
			sprintf(msg,"Testing %s of %s  failed (timeout)",
				lhapp2name,lhname);
			logit(msg);
                        sprintf(msg2,"/opt/FSTha/bin/notify Application Test Script Time-out: %s",msg);
                        system(msg2);
			sprintf(msg,"/tmp/lh%dapp2test%d",lhnum,*lhapp2testtimeo);
			fstkill(msg);
			sprintf(msg,"lh%dapp2testtimeo 0",lhnum);
			fsthahere(msg);
			sprintf(msg,"dirty 0",YES);
			fsthahere(msg);
		}

               if (*lhapp3crit != 0 && *lhapp3stat == ON &&
                       *lhapp3testres >= *lhapp3crit) {
			sprintf(msg,"%s of %s  failed",
				lhapp3name,lhname);
			logit(msg);
                        sprintf(msg2,"/opt/FSTha/bin/notify Application Failure: %s",msg);
                        system(msg2);
			sprintf(msg,"lh%dfailing %d",lhnum,FAILED);
			fsthahere(msg);
			sprintf(msg,"lh%dstat %d",lhnum,GDOWN);
			fsthahere(msg);
			sprintf(msg,"dirty 0",YES);
			fsthahere(msg);
		}
		/******* app on/off script timeout ******/
               if (*lhapp3timeo != 0 && (now  - shm->appfailtimeo > *lhapp3timeo)) {
			sprintf(msg,"%s of %s  failed (timeout)",
				lhapp3name,lhname);
			logit(msg);
                        sprintf(msg2,"/opt/FSTha/bin/notify Application Script Time-out: %s",msg);
                        system(msg2);
			sprintf(msg,"/tmp/lh%dapp3%d",lhnum,*lhapp3timeo);
			fstkill(msg);
			sprintf(msg,"lh%dapp3timeo 0",lhnum);
			fsthahere(msg);
			sprintf(msg,"dirty 0",YES);
			fsthahere(msg);
		}
		/***** app test script timeout ***/
               if (*lhapp3testtimeo != 0 && (now  - shm->appfailtesttimeo > *lhapp3testtimeo)) {
			sprintf(msg,"Testing %s of %s  failed (timeout)",
				lhapp3name,lhname);
			logit(msg);
                        sprintf(msg2,"/opt/FSTha/bin/notify Application Test Script Time-out: %s",msg);
                        system(msg2);
			sprintf(msg,"/tmp/lh%dapp3test%d",lhnum,*lhapp3testtimeo);
			fstkill(msg);
			sprintf(msg,"lh%dapp3testtimeo 0",lhnum);
			fsthahere(msg);
			sprintf(msg,"dirty 0",YES);
			fsthahere(msg);
		}


		/***** end monitor lhapp test ******/

	} /***** if (lhstat == UP) *****/

} /**** end if (lhnode == shm->nodeN ) ***/

if (*lhnode != shm->nodeN ) {
        time(&now);

        if (*lhstat == UP) {
                /***** app test script timeout ***/
               if (*lhapp0remtesttimeo != 0 
			&& (*lhapp0remnode == shm->nodeN || *lhapp0remnode == 8) 
			&& (now  - shm->appfailtesttimeo > *lhapp0remtesttimeo)) {
                        sprintf(msg,"Remote Testing %s of %s  failed (timeout)",
                                lhapp0name,lhname);
                        logit(msg);
                        sprintf(msg2,"/opt/FSTha/bin/notify REMOTE Application Test Script Time-out: %s",msg);
                        system(msg2);
                        sprintf(msg,"/tmp/lh%dapp0remtest%d",lhnum,*lhapp0testtimeo);
                        fstkill(msg);
                        sprintf(msg,"lh%dapp0remtesttimeo 0",lhnum);
                        fsthahere(msg);
                        sprintf(msg,"dirty 0",YES);
                        fsthahere(msg);
                }

               if (*lhapp1remtesttimeo != 0 
			&& (*lhapp1remnode == shm->nodeN || *lhapp1remnode == 8) 
			&& (now  - shm->appfailtesttimeo > *lhapp1remtesttimeo)) {
                        sprintf(msg,"Remote Testing %s of %s  failed (timeout)",
                                lhapp1name,lhname);
                        logit(msg);
                        sprintf(msg2,"/opt/FSTha/bin/notify REMOTE Application Test Script Time-out: %s",msg);
                        system(msg2);
                        sprintf(msg,"/tmp/lh%dapp1remtest%d",lhnum,*lhapp1testtimeo);
                        fstkill(msg);
                        sprintf(msg,"lh%dapp1remtesttimeo 0",lhnum);
                        fsthahere(msg);
                        sprintf(msg,"dirty 0",YES);
                        fsthahere(msg);
                }

               if (*lhapp2remtesttimeo != 0 
			&& (*lhapp2remnode == shm->nodeN || *lhapp2remnode == 8) 
			&& (now  - shm->appfailtesttimeo > *lhapp2remtesttimeo)) {
                        sprintf(msg,"Remote Testing %s of %s  failed (timeout)",
                                lhapp2name,lhname);
                        logit(msg);
                        sprintf(msg2,"/opt/FSTha/bin/notify REMOTE Application Test Script Time-out: %s",msg);
                        system(msg2);
                        sprintf(msg,"/tmp/lh%dapp2remtest%d",lhnum,*lhapp2testtimeo);
                        fstkill(msg);
                        sprintf(msg,"lh%dapp2remtesttimeo 0",lhnum);
                        fsthahere(msg);
                        sprintf(msg,"dirty 0",YES);
                        fsthahere(msg);
                }

               if (*lhapp3remtesttimeo != 0 
			&& (*lhapp3remnode == shm->nodeN || *lhapp3remnode == 8) 
			&& (now  - shm->appfailtesttimeo > *lhapp3remtesttimeo)) {
                        sprintf(msg,"Remote Testing %s of %s  failed (timeout)",
                                lhapp3name,lhname);
                        logit(msg);
                        sprintf(msg2,"/opt/FSTha/bin/notify REMOTE Application Test Script Time-out: %s",msg);
                        system(msg2);
                        sprintf(msg,"/tmp/lh%dapp3remtest%d",lhnum,*lhapp3testtimeo);
                        fstkill(msg);
                        sprintf(msg,"lh%dapp3remtesttimeo 0",lhnum);
                        fsthahere(msg);
                        sprintf(msg,"dirty 0",YES);
                        fsthahere(msg);
                }

        } /***** if (lhstat == UP) *****/

} /**** end if (lhnode != shm->nodeN ) ***/

}


chk_heartbeats(){
time_t now;
		/***** monitor heartbeat time stamp ******/
		time(&now);
		if ((now - shm->hbtimeout) > atoi(shm->hb0tm0) ) {
			if (shm->hb0stat0 != DOWN && shm->insync == NO) {
				sprintf(msg,"%s = %s  communication failed",
					shm->hb0pg0,shm->hb0tm0);
				logit(msg);
				sprintf(msg,"hb0stat0 %d",DOWN);
				fsthahere(msg);
				sprintf(msg,"dirty %d",YES);
				fsthahere(msg);
			}
		}
		else {
			if (shm->hb0stat0 != UP ) {
				sprintf(msg,"hb0stat0 %d",UP);
				fsthahere(msg);
				sprintf(msg,"%s = %s  communication ok",
					shm->hb0pg0,shm->hb0tm0);
				sprintf(msg,"dirty %d",YES);
				fsthahere(msg);
			}
		}
		if ((now - shm->hbtimeout) > atoi(shm->hb0tm1) ) {
			if (shm->hb0stat1 != DOWN && shm->insync == NO) {
				sprintf(msg,"%s = %s  communication failed",
					shm->hb0pg1,shm->hb0tm1);
				logit(msg);
				sprintf(msg,"hb0stat1 %d",DOWN);
				fsthahere(msg);
				sprintf(msg,"dirty %d",YES);
				fsthahere(msg);
			}
		}
		else {
			if (shm->hb0stat1 != UP ) {
				sprintf(msg,"hb0stat1 %d",UP);
				fsthahere(msg);
				sprintf(msg,"%s = %s  communication ok",
					shm->hb0pg1,shm->hb0tm1);
				sprintf(msg,"dirty %d",YES);
				fsthahere(msg);
			}
		}
		if ((now - shm->hbtimeout) > atoi(shm->hb0tm2) ) {
			if (shm->hb0stat2 != DOWN && shm->insync == NO) {
				sprintf(msg,"%s = %s  communication failed",
					shm->hb0pg2,shm->hb0tm2);
				logit(msg);
				sprintf(msg,"hb0stat2 %d",DOWN);
				fsthahere(msg);
				sprintf(msg,"dirty %d",YES);
				fsthahere(msg);
			}
		}
		else {
			if (shm->hb0stat2 != UP ) {
				sprintf(msg,"hb0stat2 %d",UP);
				fsthahere(msg);
				sprintf(msg,"%s = %s  communication ok",
					shm->hb0pg2,shm->hb0tm2);
				sprintf(msg,"dirty %d",YES);
				fsthahere(msg);
			}
		}
		if ((now - shm->hbtimeout) > atoi(shm->hb0tm3) ) {
			if (shm->hb0stat3 != DOWN && shm->insync == NO) {
				sprintf(msg,"%s = %s  communication failed",
					shm->hb0pg3,shm->hb0tm3);
				logit(msg);
				sprintf(msg,"hb0stat3 %d",DOWN);
				fsthahere(msg);
				sprintf(msg,"dirty %d",YES);
				fsthahere(msg);
			}
		}
		else {
			if (shm->hb0stat3 != UP ) {
				sprintf(msg,"hb0stat3 %d",UP);
				fsthahere(msg);
				sprintf(msg,"%s = %s  communication ok",
					shm->hb0pg3,shm->hb0tm3);
				sprintf(msg,"dirty %d",YES);
				fsthahere(msg);
			}
		}

		if ((now - shm->hbtimeout) > atoi(shm->hb1tm0) ) {
			if (shm->hb1stat0 != DOWN && shm->insync == NO) {
				sprintf(msg,"%s = %s  communication failed",
					shm->hb1pg0,shm->hb1tm0);
				logit(msg);
				sprintf(msg,"hb1stat0 %d",DOWN);
				fsthahere(msg);
				sprintf(msg,"dirty %d",YES);
				fsthahere(msg);
			}
		}
		else {
			if (shm->hb1stat0 != UP ) {
				sprintf(msg,"hb1stat0 %d",UP);
				fsthahere(msg);
				sprintf(msg,"%s = %s  communication ok",
					shm->hb1pg0,shm->hb1tm0);
				sprintf(msg,"dirty %d",YES);
				fsthahere(msg);
			}
		}
		if ((now - shm->hbtimeout) > atoi(shm->hb1tm1) ) {
			if (shm->hb1stat1 != DOWN && shm->insync == NO) {
				sprintf(msg,"%s = %s  communication failed",
					shm->hb1pg1,shm->hb1tm1);
				logit(msg);
				sprintf(msg,"hb1stat1 %d",DOWN);
				fsthahere(msg);
				sprintf(msg,"dirty %d",YES);
				fsthahere(msg);
			}
		}
		else {
			if (shm->hb1stat1 != UP ) {
				sprintf(msg,"hb1stat1 %d",UP);
				fsthahere(msg);
				sprintf(msg,"%s = %s  communication ok",
					shm->hb1pg1,shm->hb1tm1);
				sprintf(msg,"dirty %d",YES);
				fsthahere(msg);
			}
		}
		if ((now - shm->hbtimeout) > atoi(shm->hb1tm2) ) {
			if (shm->hb1stat2 != DOWN && shm->insync == NO) {
				sprintf(msg,"%s = %s  communication failed",
					shm->hb1pg2,shm->hb1tm2);
				logit(msg);
				sprintf(msg,"hb1stat2 %d",DOWN);
				fsthahere(msg);
				sprintf(msg,"dirty %d",YES);
				fsthahere(msg);
			}
		}
		else {
			if (shm->hb1stat2 != UP ) {
				sprintf(msg,"hb1stat2 %d",UP);
				fsthahere(msg);
				sprintf(msg,"%s = %s  communication ok",
					shm->hb1pg2,shm->hb1tm2);
				sprintf(msg,"dirty %d",YES);
				fsthahere(msg);
			}
		}
		if ((now - shm->hbtimeout) > atoi(shm->hb1tm3) ) {
			if (shm->hb1stat3 != DOWN && shm->insync == NO) {
				sprintf(msg,"%s = %s  communication failed",
					shm->hb1pg3,shm->hb1tm3);
				logit(msg);
				sprintf(msg,"hb1stat3 %d",DOWN);
				fsthahere(msg);
				sprintf(msg,"dirty %d",YES);
				fsthahere(msg);
			}
		}
		else {
			if (shm->hb1stat3 != UP ) {
				sprintf(msg,"hb1stat3 %d",UP);
				fsthahere(msg);
				sprintf(msg,"%s = %s  communication ok",
					shm->hb1pg3,shm->hb1tm3);
				sprintf(msg,"dirty %d",YES);
				fsthahere(msg);
			}
		}

		if (shm->nodeN != 0 ) {
			if ( (now - shm->hbtimeout) > atoi(shm->pbtm0) ) {
				if (shm->node0stat == UP && shm->cstat == UP) {
					if (shm->hb1stat0 != UP && shm->hb0stat0 != UP) {
						sprintf(msg,"node0stat %d",GDOWN);
						fsthahere(msg);
						sprintf(msg2,"/opt/FSTha/bin/notify Cluster Node-0 Going Down");
						system(msg2);
					}
				}
			}
			else {
				if (shm->node0stat != UP ) {
					sprintf(msg,"node0stat %d",UP);
					fsthahere(msg);
					sprintf(msg,"node0 = %s  communication ok", shm->pbtm0);
					logit(msg);
					sprintf(msg,"dirty %d",YES);
					fsthahere(msg);
				}
			}
		}
		if (shm->nodeN != 1) {
			if ( (now - shm->hbtimeout) > atoi(shm->pbtm1) ) {
				if (shm->node1stat == UP && shm->cstat == UP) {
					if (shm->hb1stat1 != UP && shm->hb0stat1 != UP) {
						sprintf(msg,"node1stat %d",GDOWN);
						fsthahere(msg);
						sprintf(msg2,"/opt/FSTha/bin/notify Cluster Node-1 Going Down");
						system(msg2);
					}
				}
			}
			else {
				if (shm->node1stat != UP ) {
					sprintf(msg,"node1stat %d",UP);
					fsthahere(msg);
					sprintf(msg,"node1 = %s  communication ok", shm->pbtm1);
					logit(msg);
					sprintf(msg,"dirty %d",YES);
					fsthahere(msg);
				}
			}
		}
		if (shm->nodeN != 2) {
			if ( (now - shm->hbtimeout) > atoi(shm->pbtm2) ) {
				if (shm->node2stat == UP && shm->cstat == UP) {
					if (shm->hb1stat2 != UP && shm->hb0stat2 != UP) {
						sprintf(msg,"node2stat %d",GDOWN);
						fsthahere(msg);
						sprintf(msg2,"/opt/FSTha/bin/notify Cluster Node-2 Going Down");
						system(msg2);
					}
				}
			}
			else {
				if (shm->node2stat != UP ) {
					sprintf(msg,"node2stat %d",UP);
					fsthahere(msg);
					sprintf(msg,"node2 = %s  communication ok", shm->pbtm2);
					logit(msg);
					sprintf(msg,"dirty %d",YES);
					fsthahere(msg);
				}
			}
		}
		if (shm->nodeN != 3) {
			if ( (now - shm->hbtimeout) > atoi(shm->pbtm3) ) {
				if (shm->node3stat == UP && shm->cstat == UP) {
					if (shm->hb1stat3 != UP && shm->hb0stat3 != UP) {
						sprintf(msg,"node3stat %d",GDOWN);
						fsthahere(msg);
						sprintf(msg2,"/opt/FSTha/bin/notify Cluster Node-2 Going Down");
						system(msg2);
					
					}
				}
			}
			else {
				if (shm->node3stat != UP ) {
					sprintf(msg,"node3stat %d",UP);
					fsthahere(msg);
					sprintf(msg,"node3 = %s  communication ok", shm->pbtm3);
					logit(msg);
					sprintf(msg,"dirty %d",YES);
					fsthahere(msg);
				}
			}
		}
}

		/***** end monitor heatbeat time stamp ******/
main(int argc, char *argv[]) {
int	rcode,prvstat,x,syncfromnode=-1;
char	s[BUFSIZ]="",hb[32]="",dv[32]="",ip[32]="",dummy[128]="";
FILE	*f;
time_t	now;
char	buf[MAX_MSG];


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

f=fopen("/opt/FSTha/etc/fstha.version","r");
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

if (get_shm(0) != 0) 
	die("Failed to Attach");

sprintf(msg,"fsthbm_pid %d",mypid);
fsthahere(msg);

shm->backup = NO;
if (shm->verbose == YES) 
	logit("Changing State.");

if (argc > 1) {
	syncfromnode=atoi(argv[1]);
} else {
	syncfromnode=-1;
}

while (1) {
	if (shm->cstat == JOIN || shm->cstat == GUP) {

		shm->myloadf = getmyload(0);

		/***** start hb monitoring ******/
	
                sprintf(msg,"/opt/FSTha/bin/`uname -s`/fstif UP %s %s %s %s",
		shm->hb0dv,shm->hb0ip,shm->hb0mask,shm->hb0bcast);
		if (shm->verbose == YES) logit(msg);
		if (system(msg) != 0) 
			die(msg);
                sprintf(msg,"/opt/FSTha/bin/`uname -s`/fstif UP %s %s %s %s",
		shm->hb1dv,shm->hb1ip,shm->hb1mask,shm->hb1bcast);
		if (shm->verbose == YES) logit(msg);
		if (system(msg) != 0) 
			die(msg);
		fstkill("fsthbserv");
		fstkill("fstsvr");
		fstkill("fsthbb");
	
		/***** monitor existence of fstsvr ******/
		if (isrunning("fstsvr") != YES) {
			logit("Starting fstsvr.");
			sprintf(msg,
				"nohup fstsvr >/dev/null 2>&1 &");	
			if (shm->verbose == YES) logit(msg);
			if (system(msg) != 0) 
				die(msg);
		}
		else
			logit("Warning: fstserv already running.");
		/***** end monitor existence of fstsvr ******/

		/*****/
		logit("Starting fsthbb.");
		sprintf(msg,"fsthbb&");	
		if (shm->verbose == YES) logit(msg);
		if (system(msg) != 0) 
			die(msg);
		/***** monitor existence of fsthbserv *****/
		if (isrunning("fsthbserv") != YES) {
			logit("Starting fsthbserv.");
			sprintf(msg,
				"nohup fsthbserv >/dev/null 2>&1 &");	
			if (shm->verbose == YES) logit(msg);
			if (system(msg) != 0) 
				die(msg);
			sleep(shm->hbtimeout);
		}
		else
			logit("Warning: fsthbserv already running.");
		/***** end monitor existence of fsthbserv ******/
		if (shm->cstat == JOIN) {
			while (amalone()==0 )
				chk_heartbeats();
			shm->cstat = WAIT;
			prvverbose = shm->verbose;
			shm->verbose = 0;
			while ((rcode=fsthadump(syncfromnode)) != 0) {
			}
			while (shm->cstat!=UP)
				sleep(1);
			if (shm->other_strict != shm->reserve_strict) {
				sprintf(msg,"Reservation Policy mismatch (%d.%d). Both nodes must use same policy. Aborted.",shm->reserve_strict,shm->other_strict);
				logit(msg);
				shm->cstat = DOWN;
				//remove_shm();
				//exit(1);
                       	}

		}
		if (shm->cstat == GUP) {
			if (!(shm->reserve_strict == PLUS || shm->reserve_strict == HA)) {
				printf("Checking Heartbeat 1 of 8\n");
				logit("Checking Heartbeats 1 of 8");
				sprintf(msg,"/usr/sbin/ping %s 3",shm->hb0pg0);
				if (shm->nodeN != 0 && system(msg) == 0) {
					sprintf(msg,"ERROR: %s appears to be UP.",
						shm->hb0pg0);
					logit(msg);
					fprintf(stderr,msg);
					sprintf(msg,"Bootup Aborting.");
					logit(msg);
					fprintf(stderr,msg);
					shm->cstat = DOWN;
					remove_shm();
				}
				printf("Checking Heartbeat 2 of 8\n");
				logit("Checking Heartbeat 2 of 8");
				sprintf(msg,"/usr/sbin/ping %s 3",shm->hb1pg0);
				if (shm->nodeN != 0 && system(msg) == 0) {
					sprintf(msg,"ERROR: %s appears to be UP.",
						shm->hb1pg0);
					logit(msg);
					fprintf(stderr,msg);
					sprintf(msg,"Bootup Aborting.");
					logit(msg);
					fprintf(stderr,msg);
					shm->cstat = DOWN;
					remove_shm();
				}
				printf("Checking Heartbeat 3 of 8\n");
				logit("Checking Heartbeat 3 of 8");
				sprintf(msg,"/usr/sbin/ping %s 3",shm->hb0pg1);
				if (shm->nodeN != 1 && system(msg) == 0) {
					sprintf(msg,"ERROR: %s appears to be UP.",
						shm->hb0pg1);
					logit(msg);
					fprintf(stderr,msg);
					sprintf(msg,"Bootup Aborting.");
					logit(msg);
					fprintf(stderr,msg);
					shm->cstat = DOWN;
					remove_shm();
				}
				printf("Checking Heartbeat 4 of 8\n");
				logit("Checking Heartbeat 4 of 8");
				sprintf(msg,"/usr/sbin/ping %s 3",shm->hb1pg1);
				if (shm->nodeN != 1 && system(msg) == 0) {
					sprintf(msg,"ERROR: %s appears to be UP.",
						shm->hb1pg1);
					logit(msg);
					fprintf(stderr,msg);
					sprintf(msg,"Bootup Aborting.");
					logit(msg);
					fprintf(stderr,msg);
					shm->cstat = DOWN;
					remove_shm();
				}
				printf("Checking Heartbeat 5 of 8\n");
				logit("Checking Heartbeat 5 of 8");
				sprintf(msg,"/usr/sbin/ping %s 3",shm->hb0pg2);
				if (shm->nodeN != 2 && system(msg) == 0) {
					sprintf(msg,"ERROR: %s appears to be UP.",
						shm->hb0pg2);
					logit(msg);
					fprintf(stderr,msg);
					sprintf(msg,"Bootup Aborting.");
					logit(msg);
					fprintf(stderr,msg);
					shm->cstat = DOWN;
					remove_shm();
				}
				printf("Checking Heartbeat 6 of 8\n");
				logit("Checking Heartbeat 6 of 8");
				sprintf(msg,"/usr/sbin/ping %s 3",shm->hb1pg2);
				if (shm->nodeN != 2 && system(msg) == 0) {
					sprintf(msg,"ERROR: %s appears to be UP.",
						shm->hb1pg2);
					logit(msg);
					fprintf(stderr,msg);
					sprintf(msg,"Bootup Aborting.");
					logit(msg);
					fprintf(stderr,msg);
					shm->cstat = DOWN;
					remove_shm();
				}
				printf("Checking Heartbeat 7 of 8\n");
				logit("Checking Heartbeat 7 of 8");
				sprintf(msg,"/usr/sbin/ping %s 3",shm->hb0pg3);
				if (shm->nodeN != 3 && system(msg) == 0) {
					sprintf(msg,"ERROR: %s appears to be UP.",
						shm->hb0pg3);
					logit(msg);
					fprintf(stderr,msg);
					sprintf(msg,"Bootup Aborting.");
					logit(msg);
					fprintf(stderr,msg);
					shm->cstat = DOWN;
					remove_shm();
				}
				printf("Checking Heartbeat 8 of 8\n");
				logit("Checking Heartbeat 8 of 8");
				sprintf(msg,"/usr/sbin/ping %s 3",shm->hb1pg3);
				if (shm->nodeN != 3 && system(msg) == 0) {
					sprintf(msg,"ERROR: %s appears to be UP.",
						shm->hb1pg3);
					logit(msg);
					fprintf(stderr,msg);
					sprintf(msg,"Bootup Aborting.");
					logit(msg);
					fprintf(stderr,msg);
					shm->cstat = DOWN;
					remove_shm();
				}
			}
			if (shm->cstat != DOWN) {
				shm->cstat = UP;
				logit("Cluster Is UP");
			}
		}

		/***** end start hb monitoring ******/

		shm->dirty=YES;
		continue;
	}
	chk_heartbeats();
	if (shm->cstat == UP ) {
		if (shm->nodeN == 0) {
			sprintf(msg,"node0stat %d",UP);
			fsthahere(msg);
		}
		else if (shm->nodeN == 1) {
			sprintf(msg,"node1stat %d",UP);
			fsthahere(msg);
		}
		else if (shm->nodeN == 2) {
			sprintf(msg,"node2stat %d",UP);
			fsthahere(msg);
		}
		else if (shm->nodeN == 3) {
			sprintf(msg,"node3stat %d",UP);
			fsthahere(msg);
		}

		/***** monitor existence of fsthbb ****/
		if (isrunning("fsthbb") != YES) {
			logit("Starting  fsthbb.");
			sprintf(msg,
				"nohup fsthbb >/dev/null 2>&1 &");	
			if (shm->verbose == YES) logit(msg);
			if (system(msg) != 0) 
				die(msg);
			sleep(1);
		}
		/***** end monitor existence of fsthbb ******/
		/***** monitor existence of fsthbserv ****/
		if (isrunning("fsthbserv") != YES) {
			logit("Starting fsthbserv.");
			sprintf(msg,
				"nohup fsthbserv >/dev/null 2>&1 &");	
			if (shm->verbose == YES) logit(msg);
			if (system(msg) != 0) 
				die(msg);
			sleep(1);
		}
		/***** end monitor existence of fsthbserv ******/
		/***** monitor existence of fstsvr ******/
		if (isrunning("fstsvr") != YES) {
			logit("Starting fstsvr.");
			sprintf(msg,
				"nohup fstsvr >/dev/null 2>&1 &");	
			if (shm->verbose == YES) logit(msg);
			if (system(msg) != 0) 
				die(msg);
			sleep(1);
		}
		/***** end monitor existence of fstsvr ******/

		/***** monitor existence of fstasm ******/
		if (isrunning("fstasm") != YES) {
			logit("Starting fstasm.");
			sprintf(msg,
				"nohup fstasm >/dev/null 2>&1 &");	
			if (shm->verbose == YES) logit(msg);
			if (system(msg) != 0) 
				die(msg);
			sleep(1);
		}
		/***** end monitor existence of fstasm ******/

		/* keep it reserved by any node in the cluster at all times*/
		if (shm->reserve_strict == PLUS || shm->reserve_strict == HA) {
			if (reserve_disk()!=0) {
/***************************
				rcode = NO;
				if (shm->qnode == 0 && shm->node0stat == DOWN)
					rcode = YES;
				else if (shm->qnode == 1 && shm->node1stat == DOWN)
					rcode = YES;
				else if (shm->qnode == 2 && shm->node2stat == DOWN)
					rcode = YES;
				else if (shm->qnode == 3 && shm->node3stat == DOWN)
					rcode = YES;
				if (rcode == YES){
					if (shm->reserve_strict == PLUS) {
						sprintf(msg,
						"Reservation of disk FAILED. NODE DOWN.");
						logit(msg);
						sleep(1);
						sync();
						sync();
						sync();
						sync();
						uadmin(A_SHUTDOWN,AD_HALT,0);
					} else {
						sprintf(msg,
						"Reservation of disk FAILED. DOWN HA.");
						logit(msg);
                                		shm->cstat = DOWN;
					}
				}
*************************************/
			} else {
				if (shm->qnode != shm->nodeN) {
					sprintf(msg,"This Node (%d) Has Now Reserved Disk",shm->nodeN);
					fsthahere(msg);
					sprintf(msg,"qnode %d",shm->nodeN);
					fsthahere(msg);
				}
			}
		}

		/***** monitor existence of other nodes ******/
		/* if prev not alone but am now alone */
		if (shm->alone== NO && amalone()==0 ) {
				sprintf(msg,"ALL NODES MISSING. Waiting %d seconds to reserve disk",SCSI_TIMEOUT);
				logit(msg);
				if (shm->reserve_strict == PLUS || shm->reserve_strict == HA) {
					if (reserve_disk_force() != 0) {
						if (shm->reserve_strict == PLUS) {
							sprintf(msg,
							"Reservation of disk FAILED. NODE DOWN.");
							logit(msg);
							sleep(1);
							sync();
							sync();
							sync();
							sync();
							uadmin(A_SHUTDOWN,AD_HALT,0);
						} else {
							sprintf(msg,
							"Forced Reservation of disk FAILED. DOWN HA.");
							logit(msg);
                                			shm->cstat = DOWN;
						}
					}
				}
		} else if (shm->nodeN != 0 && shm->node0stat == GDOWN)  {
			sprintf(msg,"node0stat %d",DOWN);
			fsthahere(msg);
			
			if (shm->cstat == UP) {
				logit("NODE-0 MISSING");
				sprintf(msg,"dirty %d",YES);
				fsthahere(msg);
				sprintf(msg,"/opt/FSTha/bin/notify NODE-0 MISSING");
				system(msg);

				if (shm->lh0node == 0) {
					if (shm->lh0stat != DOWN)  {
						if (shm->nodeN == 1 ||
							(shm->nodeN == 2 && shm->node1stat != UP) ||
							(shm->nodeN == 3 && shm->node1stat != UP && 
										shm->node2stat != UP)
							) {
							if (isup(shm->lh0failto) == UP) 
								rcode=shm->lh0failto;
							else 
								rcode=lowload(shm->nodeN);
							sprintf(msg,"VH0 was not Down. Moving to Node-%d",rcode);
							logit(msg);
							system(msg);
							sprintf(msg,"lh0stat %d",GUP);
							fsthasend(msg);
							sprintf(msg,"lh0node %d",rcode);
							fsthasend(msg);
							sprintf(msg,"dirty %d",YES);
							fsthahere(msg);
						}

					}
				}
				if (shm->lh1node == 0) {
					if (shm->lh1stat != DOWN)  {
						if (shm->nodeN == 1 ||
							(shm->nodeN == 2 && shm->node1stat != UP) ||
							(shm->nodeN == 3 && shm->node1stat != UP && 
										shm->node2stat != UP)
							) {
							if (isup(shm->lh1failto) == UP) 
								rcode=shm->lh1failto;
							else 
								rcode=lowload(shm->nodeN);
							sprintf(msg,"VH1 was not Down. Moving to Node-%d",rcode);
							logit(msg);
							system(msg);
							sprintf(msg,"lh1stat %d",GUP);
							fsthasend(msg);
							sprintf(msg,"lh1node %d",rcode);
							fsthasend(msg);
							sprintf(msg,"dirty %d",YES);
							fsthahere(msg);
						}
					}
				}
				if (shm->lh2node ==0) {
					if (shm->lh2stat != DOWN)  {
						if (shm->nodeN == 1 ||
							(shm->nodeN == 2 && shm->node1stat != UP) ||
							(shm->nodeN == 3 && shm->node1stat != UP && 
										shm->node2stat != UP)
							) {
							if (isup(shm->lh2failto) == UP) 
								rcode=shm->lh2failto;
							else 
								rcode=lowload(shm->nodeN);
							sprintf(msg,"VH2 was not Down. Moving to Node-%d",rcode);
							logit(msg);
							system(msg);
							sprintf(msg,"lh2stat %d",GUP);
							fsthasend(msg);
							sprintf(msg,"lh2node %d",rcode);
							fsthasend(msg);
							sprintf(msg,"dirty %d",YES);
							fsthahere(msg);
						}
					}
				}
				if (shm->lh3node ==0) {
					if (shm->lh3stat != DOWN)  {
						if (shm->nodeN == 1 ||
							(shm->nodeN == 2 && shm->node1stat != UP) ||
							(shm->nodeN == 3 && shm->node1stat != UP && 
										shm->node2stat != UP)
							) {
							if (isup(shm->lh3failto) == UP) 
								rcode=shm->lh3failto;
							else 
								rcode=lowload(shm->nodeN);
							sprintf(msg,"VH3 was not Down. Moving to Node-%d",rcode);
							logit(msg);
							system(msg);
							sprintf(msg,"lh3stat %d",GUP);
							fsthasend(msg);
							sprintf(msg,"lh3node %d",rcode);
							fsthasend(msg);
							sprintf(msg,"dirty %d",YES);
							fsthahere(msg);
						}
					}
				}
			}
		} else if (shm->nodeN != 1 &&shm->node1stat == GDOWN)  {
			sprintf(msg,"node1stat %d",DOWN);
			fsthahere(msg);
			
			if (shm->cstat == UP) {
				logit("NODE-1 MISSING");
				sprintf(msg,"dirty %d",YES);
				fsthahere(msg);
				sprintf(msg,"/opt/FSTha/bin/notify NODE-1 MISSING");
				system(msg);

				if (shm->lh0node == 1) {
					if (shm->lh0stat != DOWN)  {
						if (shm->nodeN == 0 ||
							(shm->nodeN == 2 && shm->node0stat != UP) ||
							(shm->nodeN == 3 && shm->node0stat != UP && 
										shm->node2stat != UP)
							) {
							if (isup(shm->lh0failto) == UP) 
								rcode=shm->lh0failto;
							else 
								rcode=lowload(shm->nodeN);
							sprintf(msg,"VH0 was not Down. Moving to Node-%d",rcode);
							logit(msg);
							system(msg);
							sprintf(msg,"lh0stat %d",GUP);
							fsthasend(msg);
							sprintf(msg,"lh0node %d",rcode);
							fsthasend(msg);
							sprintf(msg,"dirty %d",YES);
							fsthahere(msg);
						}
					}
				}
				if (shm->lh1node == 1) {
					if (shm->lh1stat != DOWN)  {
						if (shm->nodeN == 0 ||
							(shm->nodeN == 2 && shm->node0stat != UP) ||
							(shm->nodeN == 3 && shm->node0stat != UP && 
										shm->node2stat != UP)
							) {
							if (isup(shm->lh1failto) == UP) 
								rcode=shm->lh1failto;
							else 
								rcode=lowload(shm->nodeN);
							sprintf(msg,"VH1 was not Down. Moving to Node-%d",rcode);
							logit(msg);
							system(msg);
							sprintf(msg,"lh1stat %d",GUP);
							fsthasend(msg);
							sprintf(msg,"lh1node %d",rcode);
							fsthasend(msg);
							sprintf(msg,"dirty %d",YES);
							fsthahere(msg);
						}
					}
				}
				if (shm->lh2node ==1) {
					if (shm->lh2stat != DOWN)  {
						if (shm->nodeN == 0 ||
							(shm->nodeN == 2 && shm->node0stat != UP) ||
							(shm->nodeN == 3 && shm->node0stat != UP && 
										shm->node2stat != UP)
							) {
							if (isup(shm->lh2failto) == UP) 
								rcode=shm->lh2failto;
							else 
								rcode=lowload(shm->nodeN);
							sprintf(msg,"VH2 was not Down. Moving to Node-%d",rcode);
							logit(msg);
							system(msg);
							sprintf(msg,"lh2stat %d",GUP);
							fsthasend(msg);
							sprintf(msg,"lh2node %d",rcode);
							fsthasend(msg);
							sprintf(msg,"dirty %d",YES);
							fsthahere(msg);
						}
					}
				}
				if (shm->lh3node ==1) {
					if (shm->lh3stat != DOWN)  {
						if (shm->nodeN == 0 ||
							(shm->nodeN == 2 && shm->node0stat != UP) ||
							(shm->nodeN == 3 && shm->node0stat != UP && 
										shm->node2stat != UP)
							) {
							if (isup(shm->lh3failto) == UP) 
								rcode=shm->lh3failto;
							else 
								rcode=lowload(shm->nodeN);
							sprintf(msg,"VH3 was not Down. Moving to Node-%d",rcode);
							logit(msg);
							system(msg);
							sprintf(msg,"lh3stat %d",GUP);
							fsthasend(msg);
							sprintf(msg,"lh3node %d",rcode);
							fsthasend(msg);
							sprintf(msg,"dirty %d",YES);
							fsthahere(msg);
						}
					}
				}
			}
		} else if (shm->nodeN != 2 &&shm->node2stat == GDOWN)  {
			sprintf(msg,"node2stat %d",DOWN);
			fsthahere(msg);
			
			if (shm->cstat == UP) {
				logit("NODE-2 MISSING");
				sprintf(msg,"dirty %d",YES);
				fsthahere(msg);
				sprintf(msg,"/opt/FSTha/bin/notify NODE-2 MISSING");
				system(msg);

				if (shm->lh0node == 2) {
					if (shm->lh0stat != DOWN)  {
						if (shm->nodeN == 0 ||
							(shm->nodeN == 1 && shm->node0stat != UP) ||
							(shm->nodeN == 3 && shm->node0stat != UP && 
										shm->node1stat != UP)
							) {
							if (isup(shm->lh0failto) == UP) 
								rcode=shm->lh0failto;
							else 
								rcode=lowload(shm->nodeN);
							sprintf(msg,"VH0 was not Down. Moving to Node-%d",rcode);
							logit(msg);
							system(msg);
							sprintf(msg,"lh0stat %d",GUP);
							fsthasend(msg);
							sprintf(msg,"lh0node %d",rcode);
							fsthasend(msg);
							sprintf(msg,"dirty %d",YES);
							fsthahere(msg);
						}
					}
				}
				if (shm->lh1node == 2) {
					if (shm->lh1stat != DOWN)  {
						if (shm->nodeN == 0 ||
							(shm->nodeN == 1 && shm->node0stat != UP) ||
							(shm->nodeN == 3 && shm->node0stat != UP && 
										shm->node1stat != UP)
							) {
							if (isup(shm->lh1failto) == UP) 
								rcode=shm->lh1failto;
							else 
								rcode=lowload(shm->nodeN);
							sprintf(msg,"VH1 was not Down. Moving to Node-%d",rcode);
							logit(msg);
							system(msg);
							sprintf(msg,"lh1stat %d",GUP);
							fsthasend(msg);
							sprintf(msg,"lh1node %d",rcode);
							fsthasend(msg);
							sprintf(msg,"dirty %d",YES);
							fsthahere(msg);
						}
					}
				}
				if (shm->lh2node ==2) {
					if (shm->lh2stat != DOWN)  {
						if (shm->nodeN == 0 ||
							(shm->nodeN == 1 && shm->node0stat != UP) ||
							(shm->nodeN == 3 && shm->node0stat != UP && 
										shm->node1stat != UP)
							) {
							if (isup(shm->lh2failto) == UP) 
								rcode=shm->lh2failto;
							else 
								rcode=lowload(shm->nodeN);
							sprintf(msg,"VH2 was not Down. Moving to Node-%d",rcode);
							logit(msg);
							system(msg);
							sprintf(msg,"lh2stat %d",GUP);
							fsthasend(msg);
							sprintf(msg,"lh2node %d",rcode);
							fsthasend(msg);
							sprintf(msg,"dirty %d",YES);
							fsthahere(msg);
						}
					}
				}
				if (shm->lh3node ==2) {
					if (shm->lh3stat != DOWN)  {
						if (shm->nodeN == 0 ||
							(shm->nodeN == 1 && shm->node0stat != UP) ||
							(shm->nodeN == 3 && shm->node0stat != UP && 
										shm->node1stat != UP)
							) {
							if (isup(shm->lh3failto) == UP) 
								rcode=shm->lh3failto;
							else 
								rcode=lowload(shm->nodeN);
							sprintf(msg,"VH3 was not Down. Moving to Node-%d",rcode);
							logit(msg);
							system(msg);
							sprintf(msg,"lh3stat %d",GUP);
							fsthasend(msg);
							sprintf(msg,"lh3node %d",rcode);
							fsthasend(msg);
							sprintf(msg,"dirty %d",YES);
							fsthahere(msg);
						}
					}
				}
			}
		} else if (shm->nodeN != 3 &&shm->node3stat == GDOWN)  {
			sprintf(msg,"node3stat %d",DOWN);
			fsthahere(msg);
			
			if (shm->cstat == UP) {
				logit("NODE-3 MISSING");
				sprintf(msg,"dirty %d",YES);
				fsthahere(msg);
				sprintf(msg,"/opt/FSTha/bin/notify NODE-3 MISSING");
				system(msg);

				if (shm->lh0node == 3) {
					if (shm->lh0stat != DOWN)  {
						if (shm->nodeN == 0 ||
							(shm->nodeN == 1 && shm->node0stat != UP) ||
							(shm->nodeN == 2 && shm->node0stat != UP && 
										shm->node1stat != UP)
							) {
							if (isup(shm->lh0failto) == UP) 
								rcode=shm->lh0failto;
							else 
								rcode=lowload(shm->nodeN);
							sprintf(msg,"VH0 was not Down. Moving to Node-%d",rcode);
							logit(msg);
							system(msg);
							sprintf(msg,"lh0stat %d",GUP);
							fsthasend(msg);
							sprintf(msg,"lh0node %d",rcode);
							fsthasend(msg);
							sprintf(msg,"dirty %d",YES);
							fsthahere(msg);
						}
					}
				}
				if (shm->lh1node == 3) {
					if (shm->lh1stat != DOWN)  {
						if (shm->nodeN == 0 ||
							(shm->nodeN == 1 && shm->node0stat != UP) ||
							(shm->nodeN == 2 && shm->node0stat != UP && 
										shm->node1stat != UP)
							) {
							if (isup(shm->lh1failto) == UP) 
								rcode=shm->lh1failto;
							else 
								rcode=lowload(shm->nodeN);
							sprintf(msg,"VH1 was not Down. Moving to Node-%d",rcode);
							logit(msg);
							system(msg);
							sprintf(msg,"lh1stat %d",GUP);
							fsthasend(msg);
							sprintf(msg,"lh1node %d",rcode);
							fsthasend(msg);
							sprintf(msg,"dirty %d",YES);
							fsthahere(msg);
						}
					}
				}
				if (shm->lh2node ==3) {
					if (shm->lh2stat != DOWN)  {
						if (shm->nodeN == 0 ||
							(shm->nodeN == 1 && shm->node0stat != UP) ||
							(shm->nodeN == 2 && shm->node0stat != UP && 
										shm->node1stat != UP)
							) {
							if (isup(shm->lh2failto) == UP) 
								rcode=shm->lh2failto;
							else 
								rcode=lowload(shm->nodeN);
							sprintf(msg,"VH2 was not Down. Moving to Node-%d",rcode);
							logit(msg);
							system(msg);
							sprintf(msg,"lh2stat %d",GUP);
							fsthasend(msg);
							sprintf(msg,"lh2node %d",rcode);
							fsthasend(msg);
							sprintf(msg,"dirty %d",YES);
							fsthahere(msg);
						}
					}
				}
				if (shm->lh3node ==3) {
					if (shm->lh3stat != DOWN)  {
						if (shm->nodeN == 0 ||
							(shm->nodeN == 1 && shm->node0stat != UP) ||
							(shm->nodeN == 2 && shm->node0stat != UP && 
										shm->node1stat != UP)
							) {
							if (isup(shm->lh3failto) == UP) 
								rcode=shm->lh3failto;
							else 
								rcode=lowload(shm->nodeN);
							sprintf(msg,"VH3 was not Down. Moving to Node-%d",rcode);
							logit(msg);
							system(msg);
							sprintf(msg,"lh3stat %d",GUP);
							fsthasend(msg);
							sprintf(msg,"lh3node %d",rcode);
							fsthasend(msg);
							sprintf(msg,"dirty %d",YES);
							fsthahere(msg);
						}
					}
				}
			}
		}


	chk_heartbeats();

		/***** end monitor existence of other node ******/


		/***monitor loical host transitions ***/
		fstlhtrans (0, &(shm->lh0node),&(shm->lh0stat),&(shm->lh0timeo),
				shm->lh0name,&(shm->lh0failing),
				&(shm->lh0app0stat),
				&(shm->lh0app0crit),
				&(shm->lh0app0testres),
				shm->lh0app0name,
				&(shm->lh0app0timeo),
				&(shm->lh0app0testtimeo),
				&(shm->lh0app0remtesttimeo),
				&(shm->lh0app0remnode),
				&(shm->lh0app1stat),
				&(shm->lh0app1crit),
				&(shm->lh0app1testres),
				shm->lh0app1name,
				&(shm->lh0app1timeo),
				&(shm->lh0app1testtimeo),
				&(shm->lh0app1remtesttimeo),
				&(shm->lh0app1remnode),
				&(shm->lh0app2stat),
				&(shm->lh0app2crit),
				&(shm->lh0app2testres),
				shm->lh0app2name,
				&(shm->lh0app2timeo),
				&(shm->lh0app2testtimeo),
				&(shm->lh0app2remtesttimeo),
				&(shm->lh0app2remnode),
				&(shm->lh0app3stat),
				&(shm->lh0app3crit),
				&(shm->lh0app3testres),
				shm->lh0app3name,
				&(shm->lh0app3timeo),
				&(shm->lh0app3testtimeo),
				&(shm->lh0app3remtesttimeo),
				&(shm->lh0app3remnode)
				);


		fstlhtrans (1, &(shm->lh1node),&(shm->lh1stat),&(shm->lh1timeo),
				shm->lh1name,&(shm->lh1failing),
				&(shm->lh1app0stat),
				&(shm->lh1app0crit),
				&(shm->lh1app0testres),
				shm->lh1app0name,
				&(shm->lh1app0timeo),
				&(shm->lh1app0testtimeo),
				&(shm->lh1app0remtesttimeo),
				&(shm->lh1app0remnode),
				&(shm->lh1app1stat),
				&(shm->lh1app1crit),
				&(shm->lh1app1testres),
				shm->lh1app1name,
				&(shm->lh1app1timeo),
				&(shm->lh1app1testtimeo),
				&(shm->lh1app1remtesttimeo),
				&(shm->lh1app1remnode),
				&(shm->lh1app2stat),
				&(shm->lh1app2crit),
				&(shm->lh1app2testres),
				shm->lh1app2name,
				&(shm->lh1app2timeo),
				&(shm->lh1app2testtimeo),
				&(shm->lh1app2remtesttimeo),
				&(shm->lh1app2remnode),
				&(shm->lh1app3stat),
				&(shm->lh1app3crit),
				&(shm->lh1app3testres),
				shm->lh1app3name,
				&(shm->lh1app3timeo),
				&(shm->lh1app3testtimeo),
				&(shm->lh1app3remtesttimeo),
				&(shm->lh1app3remnode)
				);


		fstlhtrans (2, &(shm->lh2node),&(shm->lh2stat),&(shm->lh2timeo),
				shm->lh2name,&(shm->lh2failing),
				&(shm->lh2app0stat),
				&(shm->lh2app0crit),
				&(shm->lh2app0testres),
				shm->lh2app0name,
				&(shm->lh2app0timeo),
				&(shm->lh2app0testtimeo),
				&(shm->lh2app0remtesttimeo),
				&(shm->lh2app0remnode),
				&(shm->lh2app1stat),
				&(shm->lh2app1crit),
				&(shm->lh2app1testres),
				shm->lh2app1name,
				&(shm->lh2app1timeo),
				&(shm->lh2app1testtimeo),
				&(shm->lh2app1remtesttimeo),
				&(shm->lh2app1remnode),
				&(shm->lh2app2stat),
				&(shm->lh2app2crit),
				&(shm->lh2app2testres),
				shm->lh2app2name,
				&(shm->lh2app2timeo),
				&(shm->lh2app2testtimeo),
				&(shm->lh2app2remtesttimeo),
				&(shm->lh2app2remnode),
				&(shm->lh2app3stat),
				&(shm->lh2app3crit),
				&(shm->lh2app3testres),
				shm->lh2app3name,
				&(shm->lh2app3timeo),
				&(shm->lh2app3testtimeo),
				&(shm->lh2app3remtesttimeo),
				&(shm->lh2app3remnode)
				);


		fstlhtrans (3, &(shm->lh3node),&(shm->lh3stat),&(shm->lh3timeo),
				shm->lh3name,&(shm->lh3failing),
				&(shm->lh3app0stat),
				&(shm->lh3app0crit),
				&(shm->lh3app0testres),
				shm->lh3app0name,
				&(shm->lh3app0timeo),
				&(shm->lh3app0testtimeo),
				&(shm->lh3app0remtesttimeo),
				&(shm->lh3app0remnode),
				&(shm->lh3app1stat),
				&(shm->lh3app1crit),
				&(shm->lh3app1testres),
				shm->lh3app1name,
				&(shm->lh3app1timeo),
				&(shm->lh3app1testtimeo),
				&(shm->lh3app1remtesttimeo),
				&(shm->lh3app1remnode),
				&(shm->lh3app2stat),
				&(shm->lh3app2crit),
				&(shm->lh3app2testres),
				shm->lh3app2name,
				&(shm->lh3app2timeo),
				&(shm->lh3app2testtimeo),
				&(shm->lh3app2remtesttimeo),
				&(shm->lh3app2remnode),
				&(shm->lh3app3stat),
				&(shm->lh3app3crit),
				&(shm->lh3app3testres),
				shm->lh3app3name,
				&(shm->lh3app3timeo),
				&(shm->lh3app3testtimeo),
				&(shm->lh3app3remtesttimeo),
				&(shm->lh3app3remnode)
				);



		/***end monitor loical host transitions ***/

	}
	else if (shm->cstat == DOWN) {
		release_disk();
		logit("fsthbm is Shutting Down");
		logit("killing fst processes...");
		fstkill("fsthbb");
		fstkill("fsthbserv");
		fstkill("fstsvr");
		fstkill("fstasm");
		fstkill("fstast");
                sprintf(msg,"/opt/FSTha/bin/`uname -s`/fstif DOWN %s",shm->hb0dv);
		if (shm->verbose == YES) logit(msg);
		system(msg);
                sprintf(msg,"/opt/FSTha/bin/`uname -s`/fstif DOWN %s",shm->hb1dv);
		if (shm->verbose == YES) logit(msg);
		system(msg);
		logit("fsthbm is Down");
		remove_shm();
		exit(0);
	}
	
	time(&now);
	if ( (now-300) > loadtime) {
		shm->myloadf = getmyload(0);
		loadtime = now;
	}
		
	if ( shm->dirty == YES && shm->cstat == UP ) {
		shm->myloadf = getmyload(0);
		put_conf();
		sprintf(msg,"dirty %d",NO);
		fsthahere(msg);
		
	}
	if ( shm->backup == YES ) {
		sprintf(msg,"backup %d",NO);
		fsthahere(msg);
		sprintf(msg,"cp %s %s.`date|tr ' ' '_'`-%d",shm->cf,shm->cf,getpid());
		system(msg);
	}
}


}
