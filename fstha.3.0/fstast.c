/*
 * HA/FST - High Availability Cluster
 * Copyright (C) 2003-2006 Reggie Beavers
 * 
 * Licensed under the terms of the GNU Lesser General Public License:
 * 		http://www.opensource.org/licenses/gpl-license.php
 * 
 * For further information visit:
 * 		http://www.fstha.com/
 * 
 * File Name: fstast.c
 *		Run application agent test scripts
 * 
 * File Authors:
 *		Reggie Beavers (reggiebeavers@fstha.com)
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "fstha.h"

time_t	now;

fstast(int lhnum,int appnum,int *node,int *stat,char *remtestscript,int *remnode,char *testscript,int *result,int *crit,char *name,
	int *timeo,int sleeptime,int pause) {
FILE *f;
char msg2[128]="";
int lhstat;

sleep(pause);
while (1) {
	if (isrunning("fsthbm") != YES) {
		exit(0);
	}
	if (isrunning("fstsvr") != YES) {
		exit(0);
	}

	if (*node != shm->nodeN && (*remnode == shm->nodeN || *remnode == 8)) {
		if (*stat != NOCHECK) {
			time(&now);
			sprintf(msg2,"lh%dapp%dremtesttimeo %d",lhnum,appnum,now);
			fsthahere(msg2);
		
			sprintf(msg,"/tmp/lh%dapp%dremtest%d",lhnum,appnum,now);
			f=fopen(msg,"w");
			if (f == NULL) {
				sprintf(msg2,"Unable to open %s",msg);
				logit(msg2);
			}
			fprintf(f,"%s",remtestscript);
			fclose(f);
			sprintf(msg2,". %s",msg);
			if (system(msg2) != 0 ) {
				sprintf(msg2,"lh%dapp%dremtestres %d",lhnum,appnum,(*result+1));
				fsthahere(msg2);
				sprintf(msg,"Application %s Failed Remote Test",name);
				logit(msg);
 				sprintf(msg2,"/opt/FSTha/bin/notify REMOTE Application Test Failed: %s",msg);
                                system(msg2);
			}
			unlink(msg);
		}
	} else if (*node == shm->nodeN) {
		if (*stat == NOCHECK) {
			sprintf(msg2,"lh%dapp%dtestres 0",lhnum,appnum);
			fsthahere(msg2);
		} else  {
			time(&now);
			sprintf(msg2,"lh%dapp%dtesttimeo %d",lhnum,appnum,now);
			fsthahere(msg2);
		
			sprintf(msg,"/tmp/lh%dapp%dtest%d",lhnum,appnum,now);
			f=fopen(msg,"w");
			if (f == NULL) {
				sprintf(msg2,"Unable to open %s",msg);
				logit(msg2);
			}
			fprintf(f,"%s",testscript);
			fclose(f);
			sprintf(msg2,". %s",msg);
			if (system(msg2) != 0 ) {
				sprintf(msg2,"lh%dapp%dtestres %d",lhnum,appnum,(*result+1));
				fsthahere(msg2);
				if ( *crit == 0 ) {
					sprintf(msg,"Application %s Failed Test",name);
					logit(msg);
				}
				else if (*result <= *crit || *crit == 0 ) {
					sprintf(msg,"Application %s Failed Test %d out of %d times",
						name,*result,*crit);
					logit(msg);
				}
			}
			else {
				if (*result != 0) {
					sprintf(msg,"Application %s Test Passed Again",
					name);
					logit(msg);
				}
				sprintf(msg2,"lh%dapp%dtestres 0",lhnum,appnum);
				fsthahere(msg2);
			}
			sprintf(msg2,"lh%dapp%dtesttimeo 0",lhnum,appnum);
			fsthahere(msg2);
			sprintf(msg,"/tmp/lh%dapp%dtest%d",lhnum,appnum,now);
			unlink(msg);
		}
		sleep(sleeptime);
	}

}
}


main(int argc, char *argv[]) {
char appname[128]="";

strcpy(fsthaprogname,argv[0]);
mypid=getpid();
fclose(stdin);
fclose(stdout);
fclose(stderr);
setsid();
umask(077);

if ( argc != 2 || atoi(argv[1]) < 0 || atoi(argv[1]) > 1 ) {
	sprintf(msg,"Usage:  %s  <lh#app#>\n", argv[0]);
	die(msg);
}

strcpy(appname,argv[1]);

if (get_shm(0) != 0) 
	die("Failed to Attach");


if ( strcmp(appname,"lh0app0") == 0 )  {
	sprintf(msg,"fstastlh0app0_pid %d",mypid);
	fsthahere(msg);
	fstast(0,0,&(shm->lh0node),&(shm->lh0app0stat),shm->lh0app0remtest,&(shm->lh0app0remnode),shm->lh0app0test,&(shm->lh0app0testres), &(shm->lh0app0crit),shm->lh0app0name,&(shm->lh0app0testtimeo),shm->lh0app0secs,shm->lh0app0pause); 
}
else if ( strcmp(appname,"lh0app1") == 0 ) {
	sprintf(msg,"fstastlh0app1_pid %d",mypid);
	fsthahere(msg);
	fstast(0,1,&(shm->lh0node),&(shm->lh0app1stat),shm->lh0app1remtest,&(shm->lh0app1remnode),shm->lh0app1test,&(shm->lh0app1testres), &(shm->lh0app1crit),shm->lh0app1name,&(shm->lh0app1testtimeo),shm->lh0app1secs,shm->lh0app1pause); 
}
else if ( strcmp(appname,"lh0app2") == 0 ) {
	sprintf(msg,"fstastlh0app2_pid %d",mypid);
	fsthahere(msg);
	fstast(0,2,&(shm->lh0node),&(shm->lh0app2stat),shm->lh0app2remtest,&(shm->lh0app2remnode),shm->lh0app2test,&(shm->lh0app2testres), &(shm->lh0app2crit),shm->lh0app2name,&(shm->lh0app2testtimeo),shm->lh0app2secs,shm->lh0app2pause); 
}
else if ( strcmp(appname,"lh0app3") == 0 ) {
	sprintf(msg,"fstastlh0app3_pid %d",mypid);
	fsthahere(msg);
	fstast(0,3,&(shm->lh0node),&(shm->lh0app3stat),shm->lh0app0remtest,&(shm->lh0app3remnode),shm->lh0app3test,&(shm->lh0app3testres), &(shm->lh0app3crit),shm->lh0app3name,&(shm->lh0app3testtimeo),shm->lh0app3secs,shm->lh0app3pause); 
}


else if ( strcmp(appname,"lh1app0") == 0 )  {
	sprintf(msg,"fstastlh1app0_pid %d",mypid);
	fsthahere(msg);
	fstast(1,0,&(shm->lh1node),&(shm->lh1app0stat),shm->lh1app0remtest,&(shm->lh1app0remnode),shm->lh1app0test,&(shm->lh1app0testres), &(shm->lh1app0crit),shm->lh1app0name,&(shm->lh1app0testtimeo),shm->lh1app0secs,shm->lh1app0pause); 
}
else if ( strcmp(appname,"lh1app1") == 0 ) {
	sprintf(msg,"fstastlh1app1_pid %d",mypid);
	fsthahere(msg);
	fstast(1,1,&(shm->lh1node),&(shm->lh1app1stat),shm->lh1app1remtest,&(shm->lh1app1remnode),shm->lh1app1test,&(shm->lh1app1testres), &(shm->lh1app1crit),shm->lh1app1name,&(shm->lh1app1testtimeo),shm->lh1app1secs,shm->lh1app1pause); 
}
else if ( strcmp(appname,"lh1app2") == 0 ) {
	sprintf(msg,"fstastlh1app2_pid %d",mypid);
	fsthahere(msg);
	fstast(1,2,&(shm->lh1node),&(shm->lh1app2stat),shm->lh1app2remtest,&(shm->lh1app2remnode),shm->lh1app2test,&(shm->lh1app2testres), &(shm->lh1app2crit),shm->lh1app2name,&(shm->lh1app2testtimeo),shm->lh1app2secs,shm->lh1app2pause); 
}
else if ( strcmp(appname,"lh1app3") == 0 ) {
	sprintf(msg,"fstastlh1app3_pid %d",mypid);
	fsthahere(msg);
	fstast(1,3,&(shm->lh1node),&(shm->lh1app3stat),shm->lh1app3remtest,&(shm->lh1app3remnode),shm->lh1app3test,&(shm->lh1app3testres), &(shm->lh1app3crit),shm->lh1app3name,&(shm->lh1app3testtimeo),shm->lh1app3secs,shm->lh1app3pause); 
}


else if ( strcmp(appname,"lh2app0") == 0 )  {
	sprintf(msg,"fstastlh2app0_pid %d",mypid);
	fsthahere(msg);
	fstast(2,0,&(shm->lh2node),&(shm->lh2app0stat),shm->lh2app0remtest,&(shm->lh2app0remnode),shm->lh2app0test,&(shm->lh2app0testres), &(shm->lh2app0crit),shm->lh2app0name,&(shm->lh2app0testtimeo),shm->lh2app0secs,shm->lh2app0pause); 
}
else if ( strcmp(appname,"lh2app1") == 0 ) {
	sprintf(msg,"fstastlh2app1_pid %d",mypid);
	fsthahere(msg);
	fstast(2,1,&(shm->lh2node),&(shm->lh2app1stat),shm->lh2app1remtest,&(shm->lh2app1remnode),shm->lh2app1test,&(shm->lh2app1testres), &(shm->lh2app1crit),shm->lh2app1name,&(shm->lh2app1testtimeo),shm->lh2app1secs,shm->lh2app1pause); 
}
else if ( strcmp(appname,"lh2app2") == 0 ) {
	sprintf(msg,"fstastlh2app2_pid %d",mypid);
	fsthahere(msg);
	fstast(2,2,&(shm->lh2node),&(shm->lh2app2stat),shm->lh2app2remtest,&(shm->lh2app2remnode),shm->lh2app2test,&(shm->lh2app2testres), &(shm->lh2app2crit),shm->lh2app2name,&(shm->lh2app2testtimeo),shm->lh2app2secs,shm->lh2app2pause); 
}
else if ( strcmp(appname,"lh2app3") == 0 ) {
	sprintf(msg,"fstastlh2app3_pid %d",mypid);
	fsthahere(msg);
	fstast(2,3,&(shm->lh2node),&(shm->lh2app3stat),shm->lh2app3remtest,&(shm->lh2app3remnode),shm->lh2app3test,&(shm->lh2app3testres), &(shm->lh2app3crit),shm->lh2app3name,&(shm->lh2app3testtimeo),shm->lh2app3secs,shm->lh2app3pause); 
}


else if ( strcmp(appname,"lh3app0") == 0 )  {
	sprintf(msg,"fstastlh3app0_pid %d",mypid);
	fsthahere(msg);
	fstast(3,0,&(shm->lh3node),&(shm->lh3app0stat),shm->lh3app0remtest,&(shm->lh3app0remnode),shm->lh3app0test,&(shm->lh3app0testres), &(shm->lh3app0crit),shm->lh3app0name,&(shm->lh3app0testtimeo),shm->lh3app0secs,shm->lh3app0pause); 
}
else if ( strcmp(appname,"lh3app1") == 0 ) {
	sprintf(msg,"fstastlh3app1_pid %d",mypid);
	fsthahere(msg);
	fstast(3,1,&(shm->lh3node),&(shm->lh3app1stat),shm->lh3app1remtest,&(shm->lh3app1remnode),shm->lh3app1test,&(shm->lh3app1testres), &(shm->lh3app1crit),shm->lh3app1name,&(shm->lh3app1testtimeo),shm->lh3app1secs,shm->lh3app1pause); 
}
else if ( strcmp(appname,"lh3app2") == 0 ) {
	sprintf(msg,"fstastlh3app2_pid %d",mypid);
	fsthahere(msg);
	fstast(3,2,&(shm->lh3node),&(shm->lh3app2stat),shm->lh3app2remtest,&(shm->lh3app2remnode),shm->lh3app2test,&(shm->lh3app2testres), &(shm->lh3app2crit),shm->lh3app2name,&(shm->lh3app2testtimeo),shm->lh3app2secs,shm->lh3app2pause); 
}
else if ( strcmp(appname,"lh3app3") == 0 ) {
	sprintf(msg,"fstastlh3app3_pid %d",mypid);
	fsthahere(msg);
	fstast(3,3,&(shm->lh3node),&(shm->lh3app3stat),shm->lh3app3remtest,&(shm->lh3app3remnode),shm->lh3app3test,&(shm->lh3app3testres), &(shm->lh3app3crit),shm->lh3app3name,&(shm->lh3app3testtimeo),shm->lh3app3secs,shm->lh3app3pause); 
}

sprintf(msg,"%s %s is Down.",basename(fsthaprogname),argv[1]);
logit(msg);
}
