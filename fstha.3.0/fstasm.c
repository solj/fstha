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
 * File Name: fstasm.c
 * 	        Application Agent Monitor
 * 
 * File Authors:
 * 		Reggie Beavers (reggiebeavers@fstha.com)
 */
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <libgen.h>
#include <time.h>
#include "fsthanet.h"


fstasm(int lhnum,int *lhfailing, int *lhstat,int *lhnode,char *lhpath,int *lhtimeo,
	char *lhapp0name,
	int *lhapp0stat,
	char *lhapp0start,
	char *lhapp0stop,
	int *lhapp0timeo,
	int *lhapp0testres,
	int *lhapp0testtimeo,
	int *lhapp0remtesttimeo,
	int *lhapp0remnode,
	char *lhapp1name,
	int *lhapp1stat,
	char *lhapp1start,
	char *lhapp1stop,
	int *lhapp1timeo,
	int *lhapp1testres,
	int *lhapp1testtimeo,
	int *lhapp1remtesttimeo,
	int *lhapp1remnode,
	char *lhapp2name,
	int *lhapp2stat,
	char *lhapp2start,
	char *lhapp2stop,
	int *lhapp2timeo,
	int *lhapp2testres,
	int *lhapp2testtimeo,
	int *lhapp2remtesttimeo,
	int *lhapp2remnode,
	char *lhapp3name,
	int *lhapp3stat,
	char *lhapp3start,
	char *lhapp3stop,
	int *lhapp3timeo,
	int *lhapp3testres,
	int *lhapp3testtimeo,
	int *lhapp3remtesttimeo,
	int *lhapp3remnode,
	int *lhfailto
	) {
int	rcode;
time_t	now;
FILE	*f;
char    msg2[BUFSIZ]="";
char	msg[BUFSIZ]="";

time(&now);
if ( *lhfailing == FAILED && (*lhstat == FAILED || *lhstat == DOWN) ) {
	sprintf(msg,"lh%dfailing 0",lhnum);
	fsthathere(msg);
	fsthahere(msg);
	while (*lhfailing != 0)
		sleep(1);

	if (*lhfailto != shm->nodeN && isup(*lhfailto) == UP) {
		sprintf(msg,"Relocating vh%d to preferred node-%d", *lhnode, *lhfailto);
		logit(msg);
		sprintf(msg,"fstha locate %d %d",lhnum,*lhfailto);
	} else {
		sprintf(msg,"Relocating vh%d to node with lowest load", *lhnode);
		logit(msg);
		sprintf(msg,"fstha locate %d %d",lhnum,lowload(shm->nodeN));
	}
	system(msg);
		
}
/******* monitor data service monitors ******/
else if ( *lhstat == UP && *lhnode == shm->nodeN) {
		if (lhapp0name[0]!=0 && *lhapp0stat == GUP ) {
			sprintf(msg,"Turning ON %s of VH%d",lhapp0name,lhnum);
			logit(msg);
			time(&now);
			sprintf(msg,"/tmp/lh%dapp0%d",lhnum,now);
			f=fopen(msg,"w");
			if (f == NULL) {
        			sprintf(msg2,"Unable to open %s",msg);
				logit(msg2);
        			return;
			}
			sprintf(msg2,"lh%dapp0timeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg2,"lh%dapp0testres 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg2,"lh%dapp0testtimeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			fprintf(f,"%s",lhapp0start);
			fclose(f);
			sprintf(msg2,"lh%dapp0timeo %d",lhnum,now);
			fsthahere(msg2);
			sprintf(msg2,". %s",msg);
			if (system(msg2) !=0 ) {
				sprintf(msg,"%s Start Failed.",lhapp0name);
				logit(msg);
				sprintf(msg2,"lh%dapp0stat %d",lhnum,OFF);
				fsthathere(msg2);
				fsthahere(msg2);
			} else {
				sprintf(msg,"%s Start Complete.",lhapp0name);
				logit(msg);
				sprintf(msg2,"lh%dapp0stat %d",lhnum,ON);
				fsthathere(msg2);
				fsthahere(msg2);
			}
			sprintf(msg2,"lh%dapp0timeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg,"/tmp/lh%dapp0%d",lhnum,now);
			unlink(msg);
		}

		if (lhapp1name[0]!=0 && *lhapp1stat == GUP ) {
			sprintf(msg,"Turning ON %s of VH%d",lhapp1name,lhnum);
			logit(msg);
			time(&now);
			sprintf(msg,"/tmp/lh%dapp1%d",lhnum,now);
			f=fopen(msg,"w");
			if (f == NULL) {
        			sprintf(msg2,"Unable to open %s",msg);
				logit(msg2);
        			return;
			}
			sprintf(msg2,"lh%dapp1timeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg2,"lh%dapp1testres 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg2,"lh%dapp1testtimeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			fprintf(f,"%s",lhapp1start);
			fclose(f);
			sprintf(msg2,"lh%dapp1timeo %d",lhnum,now);
			fsthahere(msg2);
			sprintf(msg2,". %s",msg);
			if (system(msg2) !=0 ) {
				sprintf(msg,"%s Start Failed.",lhapp1name);
				logit(msg);
				sprintf(msg2,"lh%dapp1stat %d",lhnum,OFF);
				fsthathere(msg2);
				fsthahere(msg2);
			} else {
				sprintf(msg,"%s Start Complete.",lhapp1name);
				logit(msg);
				sprintf(msg2,"lh%dapp1stat %d",lhnum,ON);
				fsthathere(msg2);
				fsthahere(msg2);
			}
			sprintf(msg2,"lh%dapp1timeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg,"/tmp/lh%dapp1%d",lhnum,now);
			unlink(msg);
		}

		if (lhapp2name[0]!=0 && *lhapp2stat == GUP ) {
			sprintf(msg,"Turning ON %s of VH%d",lhapp2name,lhnum);
			logit(msg);
			time(&now);
			sprintf(msg,"/tmp/lh%dapp2%d",lhnum,now);
			f=fopen(msg,"w");
			if (f == NULL) {
        			sprintf(msg2,"Unable to open %s",msg);
				logit(msg2);
        			return;
			}
			sprintf(msg2,"lh%dapp2timeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg2,"lh%dapp2testres 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg2,"lh%dapp2testtimeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			fprintf(f,"%s",lhapp2start);
			fclose(f);
			sprintf(msg2,"lh%dapp2timeo %d",lhnum,now);
			fsthahere(msg2);
			sprintf(msg2,". %s",msg);
			if (system(msg2) !=0 ) {
				sprintf(msg,"%s Start Failed.",lhapp2name);
				logit(msg);
				sprintf(msg2,"lh%dapp2stat %d",lhnum,OFF);
				fsthathere(msg2);
				fsthahere(msg2);
			} else {
				sprintf(msg,"%s Start Complete.",lhapp2name);
				logit(msg);
				sprintf(msg2,"lh%dapp2stat %d",lhnum,ON);
				fsthathere(msg2);
				fsthahere(msg2);
			}
			sprintf(msg2,"lh%dapp2timeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg,"/tmp/lh%dapp2%d",lhnum,now);
			unlink(msg);
		}

		if (lhapp3name[0]!=0 && *lhapp3stat == GUP ) {
			sprintf(msg,"Turning ON %s of VH%d",lhapp3name,lhnum);
			logit(msg);
			time(&now);
			sprintf(msg,"/tmp/lh%dapp3%d",lhnum,now);
			f=fopen(msg,"w");
			if (f == NULL) {
        			sprintf(msg2,"Unable to open %s",msg);
				logit(msg2);
        			return;
			}
			sprintf(msg2,"lh%dapp3timeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg2,"lh%dapp3testres 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg2,"lh%dapp3testtimeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			fprintf(f,"%s",lhapp3start);
			fclose(f);
			sprintf(msg2,"lh%dapp3timeo %d",lhnum,now);
			fsthahere(msg2);
			sprintf(msg2,". %s",msg);
			if (system(msg2) !=0 ) {
				sprintf(msg,"%s Start Failed.",lhapp3name);
				logit(msg);
				sprintf(msg2,"lh%dapp3stat %d",lhnum,OFF);
				fsthathere(msg2);
				fsthahere(msg2);
			} else {
				sprintf(msg,"%s Start Complete.",lhapp3name);
				logit(msg);
				sprintf(msg2,"lh%dapp3stat %d",lhnum,ON);
				fsthathere(msg2);
				fsthahere(msg2);
			}
			sprintf(msg2,"lh%dapp3timeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg,"/tmp/lh%dapp3%d",lhnum,now);
			unlink(msg);
		}




	if (lhapp0name[0]!=0 && *lhapp0stat == GDOWN ) {

		sprintf(msg,"fstast lh%dapp0",lhnum);
		fstkill(msg);

		sprintf(msg,"Turning OFF %s of VH%d",lhapp0name,lhnum);
		logit(msg);
		time(&now);
		sprintf(msg,"/tmp/lh%dapp0%d",lhnum,now);
		f=fopen(msg,"w");
		if (f == NULL) {
        		sprintf(msg2,"Unable to open %s",msg);
        		logit(msg2);
			return;
		}
		sprintf(msg2,"lh%dapp0testres 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		sprintf(msg2,"lh%dapp0testtimeo 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		fprintf(f,"%s",lhapp0stop);
		fclose(f);
		sprintf(msg2,"lh%dapp0timeo %d",lhnum,now);
		fsthahere(msg2);
		sprintf(msg2,". %s",msg);
		if (system(msg2) !=0 ) {
			sprintf(msg,"%s Stop Failed.",lhapp0name);
			logit(msg);
		} else {
			sprintf(msg,"%s Stop Complete.",lhapp0name);
			logit(msg);
		}
		sprintf(msg2,"lh%dapp0timeo 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		sprintf(msg2,"lh%dapp0stat %d",lhnum,OFF);
		fsthathere(msg2);
		fsthahere(msg2);
		sprintf(msg,"/tmp/lh%dapp0%d",lhnum,now);
		unlink(msg);
	}

	if (lhapp1name[0]!=0 && *lhapp1stat == GDOWN ) {

		sprintf(msg,"fstast lh%dapp1",lhnum);
		fstkill(msg);

		sprintf(msg,"Turning OFF %s of VH%d",lhapp1name,lhnum);
		logit(msg);
		time(&now);
		sprintf(msg,"/tmp/lh%dapp1%d",lhnum,now);
		f=fopen(msg,"w");
		if (f == NULL) {
        		sprintf(msg2,"Unable to open %s",msg);
        		logit(msg2);
			return;
		}
		sprintf(msg2,"lh%dapp1testres 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		sprintf(msg2,"lh%dapp1testtimeo 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		fprintf(f,"%s",lhapp1stop);
		fclose(f);
		sprintf(msg2,"lh%dapp1timeo %d",lhnum,now);
		fsthahere(msg2);
		sprintf(msg2,". %s",msg);
		if (system(msg2) !=0 ) {
			sprintf(msg,"%s Stop Failed.",lhapp1name);
			logit(msg);
		} else {
			sprintf(msg,"%s Stop Complete.",lhapp1name);
			logit(msg);
		}
		sprintf(msg2,"lh%dapp1timeo 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		sprintf(msg2,"lh%dapp1stat %d",lhnum,OFF);
		fsthathere(msg2);
		fsthahere(msg2);
		sprintf(msg,"/tmp/lh%dapp1%d",lhnum,now);
		unlink(msg);
	}

	if (lhapp2name[0]!=0 && *lhapp2stat == GDOWN ) {

		sprintf(msg,"fstast lh%dapp2",lhnum);
		fstkill(msg);

		sprintf(msg,"Turning OFF %s of VH%d",lhapp2name,lhnum);
		logit(msg);
		time(&now);
		sprintf(msg,"/tmp/lh%dapp2%d",lhnum,now);
		f=fopen(msg,"w");
		if (f == NULL) {
        		sprintf(msg2,"Unable to open %s",msg);
        		logit(msg2);
			return;
		}
		sprintf(msg2,"lh%dapp2testres 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		sprintf(msg2,"lh%dapp2testtimeo 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		fprintf(f,"%s",lhapp2stop);
		fclose(f);
		sprintf(msg2,"lh%dapp2timeo %d",lhnum,now);
		fsthahere(msg2);
		sprintf(msg2,". %s",msg);
		if (system(msg2) !=0 ) {
			sprintf(msg,"%s Stop Failed.",lhapp2name);
			logit(msg);
		} else {
			sprintf(msg,"%s Stop Complete.",lhapp2name);
			logit(msg);
		}
		sprintf(msg2,"lh%dapp2timeo 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		sprintf(msg2,"lh%dapp2stat %d",lhnum,OFF);
		fsthathere(msg2);
		fsthahere(msg2);
		sprintf(msg,"/tmp/lh%dapp2%d",lhnum,now);
		unlink(msg);
	}

	if (lhapp3name[0]!=0 && *lhapp3stat == GDOWN ) {

		sprintf(msg,"fstast lh%dapp3",lhnum);
		fstkill(msg);

		sprintf(msg,"Turning OFF %s of VH%d",lhapp3name,lhnum);
		logit(msg);
		time(&now);
		sprintf(msg,"/tmp/lh%dapp3%d",lhnum,now);
		f=fopen(msg,"w");
		if (f == NULL) {
        		sprintf(msg2,"Unable to open %s",msg);
        		logit(msg2);
			return;
		}
		sprintf(msg2,"lh%dapp3testres 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		sprintf(msg2,"lh%dapp3testtimeo 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		fprintf(f,"%s",lhapp3stop);
		fclose(f);
		sprintf(msg2,"lh%dapp3timeo %d",lhnum,now);
		fsthahere(msg2);
		sprintf(msg2,". %s",msg);
		if (system(msg2) !=0 ) {
			sprintf(msg,"%s Stop Failed.",lhapp3name);
			logit(msg);
		} else {
			sprintf(msg,"%s Stop Complete.",lhapp3name);
			logit(msg);
		}
		sprintf(msg2,"lh%dapp3timeo 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		sprintf(msg2,"lh%dapp3stat %d",lhnum,OFF);
		fsthathere(msg2);
		fsthahere(msg2);
		sprintf(msg,"/tmp/lh%dapp3%d",lhnum,now);
		unlink(msg);
	}


	if (lhapp0name[0]!=0 && *lhapp0stat == ON ) {
		sprintf(msg,"fstast lh%dapp0",lhnum);
		if (isrunning(msg) != YES) {
			sprintf(msg,"Starting Application %s Monitor.",lhapp0name);
			logit(msg);
                       	sprintf(msg,"fstast lh%dapp0&",lhnum);
                       	if (system(msg) != 0) 
                               	die(msg);
               	}
	}
	if (lhapp1name[0]!=0 && *lhapp1stat == ON ) {
		sprintf(msg,"fstast lh%dapp1",lhnum);
		if (isrunning(msg) != YES) {
			sprintf(msg,"Starting Application %s Monitor.",lhapp1name);
			logit(msg);
                       	sprintf(msg,"fstast lh%dapp1&",lhnum);
                       	if (system(msg) != 0) 
                               	die(msg);
               	}
	}
	if (lhapp2name[0]!=0 && *lhapp2stat == ON ) {
		sprintf(msg,"fstast lh%dapp2",lhnum);
		if (isrunning(msg) != YES) {
			sprintf(msg,"Starting Application %s Monitor.",lhapp2name);
			logit(msg);
                       	sprintf(msg,"fstast lh%dapp2&",lhnum);
                       	if (system(msg) != 0) 
                               	die(msg);
               	}
	}
	if (lhapp3name[0]!=0 && *lhapp3stat == ON ) {
		sprintf(msg,"fstast lh%dapp3",lhnum);
		if (isrunning(msg) != YES) {
			sprintf(msg,"Starting Application %s Monitor.",lhapp3name);
			logit(msg);
                       	sprintf(msg,"fstast lh%dapp3&",lhnum);
                       	if (system(msg) != 0) 
                               	die(msg);
               	}
	}
}

else if ( *lhstat == UP && *lhnode != shm->nodeN && (*lhapp0remnode == shm->nodeN || *lhapp0remnode == 8)) {
        if (lhapp0name[0]!=0 && *lhapp0stat == ON ) {
                sprintf(msg,"fstast lh%dapp0",lhnum);
                if (isrunning(msg) != YES) {
                        sprintf(msg,"Starting Application %s Remote Monitor.",lhapp0name);
                        logit(msg);
                        sprintf(msg,"fstast lh%dapp0&",lhnum);
                        if (system(msg) != 0)
                                die(msg);
                }
        }
        if (lhapp1name[0]!=0 && *lhapp1stat == ON && (*lhapp1remnode == shm->nodeN || *lhapp1remnode == 8)) {
                sprintf(msg,"fstast lh%dapp1",lhnum);
                if (isrunning(msg) != YES) {
                        sprintf(msg,"Starting Application %s Remote Monitor.",lhapp1name);
                        logit(msg);
                        sprintf(msg,"fstast lh%dapp1&",lhnum);
                        if (system(msg) != 0)
                                die(msg);
                }
        }
        if (lhapp2name[0]!=0 && *lhapp2stat == ON && (*lhapp2remnode == shm->nodeN || *lhapp2remnode == 8)) {
                sprintf(msg,"fstast lh%dapp2",lhnum);
                if (isrunning(msg) != YES) {
                        sprintf(msg,"Starting Application %s Remote Monitor.",lhapp2name);
                        logit(msg);
                        sprintf(msg,"fstast lh%dapp2&",lhnum);
                        if (system(msg) != 0)
                                die(msg);
                }
        }
        if (lhapp3name[0]!=0 && *lhapp3stat == ON && (*lhapp3remnode == shm->nodeN || *lhapp3remnode == 8)) {
                sprintf(msg,"fstast lh%dapp3",lhnum);
                if (isrunning(msg) != YES) {
                        sprintf(msg,"Starting Application %s Remote Monitor.",lhapp3name);
                        logit(msg);
                        sprintf(msg,"fstast lh%dapp3&",lhnum);
                        if (system(msg) != 0)
                                die(msg);
                }
        }
} else if ( *lhstat != GDOWN && *lhnode != shm->nodeN && (*lhapp0remnode == shm->nodeN || *lhapp0remnode == 8)) {
        sprintf(msg,"fstast lh%dapp0",lhnum);
	fstkill(msg);
        sprintf(msg,"fstast lh%dapp1",lhnum);
	fstkill(msg);
        sprintf(msg,"fstast lh%dapp2",lhnum);
	fstkill(msg);
        sprintf(msg,"fstast lh%dapp3",lhnum);
	fstkill(msg);

} else if ( *lhstat == GUP && *lhnode == shm->nodeN) {

		/***********run *.up scripts************************/
	time(&now);

	sprintf(msg,"lh%dtimeo %d",lhnum,now);
	fsthathere(msg);
	fsthahere(msg);

	sprintf(msg,"/tmp/lh%d%d",lhnum,now);
	f=fopen(msg,"w");
	if (f==NULL) {
		/* do error stuff HERE */
	}
	fprintf(f,"for i in %s/S*.up\ndo\n$i\ndone\n",lhpath);
	fclose(f);
	sprintf(msg2,". %s",msg);
	rcode = system(msg2);
	if (shm->verbose == YES) {
		sprintf(msg2,"S.up Scripts returned %d",rcode);
		logit(msg2);
	}
	if (rcode != 0) {
		sprintf(msg2,"S.up Scripts returned %d",rcode);
		logit(msg2);
		sprintf(msg,"lh%dstat %d",lhnum,FAILED);
		fsthathere(msg);
		fsthahere(msg);
		sprintf(msg,"dirty %d",YES);
		fsthahere(msg);
	}
	else if (*lhstat != FAILED) {
		sprintf(msg,"lh%dstat %d",lhnum,UP);
		fsthathere(msg);
		fsthahere(msg);
		sprintf(msg,"dirty %d",YES);
		fsthahere(msg);
	}
	sprintf(msg,"/tmp/lh%d%d",lhnum,now);
	unlink(msg);
	sprintf(msg,"lh%dtimeo 0",lhnum);
	fsthathere(msg);
	fsthahere(msg);
	if (*lhstat == UP) {
		if (lhapp0name[0]!=0 && *lhapp0stat == ON ) {

			time(&now);
			sprintf(msg,"/tmp/lh%dapp0%d",lhnum,now);
			f=fopen(msg,"w");
			if (f == NULL) {
        			sprintf(msg2,"Unable to open %s",msg);
				logit(msg2);
        			return;
			}
			sprintf(msg2,"lh%dapp0timeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg2,"lh%dapp0testres 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg2,"lh%dapp0testtimeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			fprintf(f,"%s",lhapp0start);
			fclose(f);
			sprintf(msg2,"lh%dapp0timeo %d",lhnum,now);
			fsthahere(msg2);
			sprintf(msg2,". %s",msg);
			if (system(msg2) !=0 ) {
				sprintf(msg,"%s Start Failed.",lhapp0name);
				logit(msg);
			} else {
				sprintf(msg,"%s Start Complete.",lhapp0name);
				logit(msg);
			}
			sprintf(msg2,"lh%dapp0timeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg,"/tmp/lh%dapp0%d",lhnum,now);
			unlink(msg);
		}

		if (lhapp1name[0]!=0 && *lhapp1stat == ON ) {

			time(&now);
			sprintf(msg,"/tmp/lh%dapp1%d",lhnum,now);
			f=fopen(msg,"w");
			if (f == NULL) {
        			sprintf(msg2,"Unable to open %s",msg);
				logit(msg2);
        			return;
			}
			sprintf(msg2,"lh%dapp1timeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg2,"lh%dapp1testres 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg2,"lh%dapp1testtimeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			fprintf(f,"%s",lhapp1start);
			fclose(f);
			sprintf(msg2,"lh%dapp1timeo %d",lhnum,now);
			fsthahere(msg2);
			sprintf(msg2,". %s",msg);
			if (system(msg2) !=0 ) {
				sprintf(msg,"%s Start Failed.",lhapp1name);
				logit(msg);
			} else {
				sprintf(msg,"%s Start Complete.",lhapp1name);
				logit(msg);
			}
			sprintf(msg2,"lh%dapp1timeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg,"/tmp/lh%dapp1%d",lhnum,now);
			unlink(msg);
		}

		if (lhapp2name[0]!=0 && *lhapp2stat == ON ) {

			time(&now);
			sprintf(msg,"/tmp/lh%dapp2%d",lhnum,now);
			f=fopen(msg,"w");
			if (f == NULL) {
        			sprintf(msg2,"Unable to open %s",msg);
				logit(msg2);
        			return;
			}
			sprintf(msg2,"lh%dapp2timeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg2,"lh%dapp2testres 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg2,"lh%dapp2testtimeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			fprintf(f,"%s",lhapp2start);
			fclose(f);
			sprintf(msg2,"lh%dapp2timeo %d",lhnum,now);
			fsthahere(msg2);
			sprintf(msg2,". %s",msg);
			if (system(msg2) !=0 ) {
				sprintf(msg,"%s Start Failed.",lhapp2name);
				logit(msg);
			} else {
				sprintf(msg,"%s Start Complete.",lhapp2name);
				logit(msg);
			}
			sprintf(msg2,"lh%dapp2timeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg,"/tmp/lh%dapp2%d",lhnum,now);
			unlink(msg);
		}

		if (lhapp3name[0]!=0 && *lhapp3stat == ON ) {

			time(&now);
			sprintf(msg,"/tmp/lh%dapp3%d",lhnum,now);
			f=fopen(msg,"w");
			if (f == NULL) {
        			sprintf(msg2,"Unable to open %s",msg);
				logit(msg2);
        			return;
			}
			sprintf(msg2,"lh%dapp3timeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg2,"lh%dapp3testres 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg2,"lh%dapp3testtimeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			fprintf(f,"%s",lhapp3start);
			fclose(f);
			sprintf(msg2,"lh%dapp3timeo %d",lhnum,now);
			fsthahere(msg2);
			sprintf(msg2,". %s",msg);
			if (system(msg2) !=0 ) {
				sprintf(msg,"%s Start Failed.",lhapp3name);
				logit(msg);
			} else {
				sprintf(msg,"%s Start Complete.",lhapp3name);
				logit(msg);
			}
			sprintf(msg2,"lh%dapp3timeo 0",lhnum);
			fsthathere(msg2);
			fsthahere(msg2);
			sprintf(msg,"/tmp/lh%dapp3%d",lhnum,now);
			unlink(msg);
		}



	}
}


else if ( *lhstat == GDOWN && *lhnode == shm->nodeN) {

	/*********shutdown loigcalgroup apps*************/
	if (lhapp0name[0]!=0 && *lhapp0stat == ON ) {

		sprintf(msg,"fstast lh%dapp0",lhnum);
		fstkill(msg);

		time(&now);
		sprintf(msg,"/tmp/lh%dapp0%d",lhnum,now);
		f=fopen(msg,"w");
		if (f == NULL) {
        		sprintf(msg2,"Unable to open %s",msg);
        		logit(msg2);
			return;
		}
		sprintf(msg2,"lh%dapp0testres 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		sprintf(msg2,"lh%dapp0testtimeo 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		fprintf(f,"%s",lhapp0stop);
		fclose(f);
		sprintf(msg2,"lh%dapp0timeo %d",lhnum,now);
		fsthahere(msg2);
		sprintf(msg2,". %s",msg);
		if (system(msg2) !=0 ) {
			sprintf(msg,"%s Stop Failed.",lhapp0name);
			logit(msg);
		} else {
			sprintf(msg,"%s Stop Complete.",lhapp0name);
			logit(msg);
		}
		sprintf(msg2,"lh%dapp0timeo 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		sprintf(msg,"/tmp/lh%dapp0%d",lhnum,now);
		unlink(msg);
	}

	if (lhapp1name[0]!=0 && *lhapp1stat == ON ) {

		sprintf(msg,"fstast lh%dapp1",lhnum);
		fstkill(msg);

		time(&now);
		sprintf(msg,"/tmp/lh%dapp1%d",lhnum,now);
		f=fopen(msg,"w");
		if (f == NULL) {
        		sprintf(msg2,"Unable to open %s",msg);
        		logit(msg2);
			return;
		}
		sprintf(msg2,"lh%dapp1testres 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		sprintf(msg2,"lh%dapp1testtimeo 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		fprintf(f,"%s",lhapp1stop);
		fclose(f);
		sprintf(msg2,"lh%dapp1timeo %d",lhnum,now);
		fsthahere(msg2);
		sprintf(msg2,". %s",msg);
		if (system(msg2) !=0 ) {
			sprintf(msg,"%s Stop Failed.",lhapp1name);
			logit(msg);
		} else {
			sprintf(msg,"%s Stop Complete.",lhapp1name);
			logit(msg);
		}
		sprintf(msg2,"lh%dapp1timeo 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		sprintf(msg,"/tmp/lh%dapp1%d",lhnum,now);
		unlink(msg);
	}

	if (lhapp2name[0]!=0 && *lhapp2stat == ON ) {

		sprintf(msg,"fstast lh%dapp2",lhnum);
		fstkill(msg);

		time(&now);
		sprintf(msg,"/tmp/lh%dapp2%d",lhnum,now);
		f=fopen(msg,"w");
		if (f == NULL) {
        		sprintf(msg2,"Unable to open %s",msg);
        		logit(msg2);
			return;
		}
		sprintf(msg2,"lh%dapp2testres 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		sprintf(msg2,"lh%dapp2testtimeo 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		fprintf(f,"%s",lhapp2stop);
		fclose(f);
		sprintf(msg2,"lh%dapp2timeo %d",lhnum,now);
		fsthahere(msg2);
		sprintf(msg2,". %s",msg);
		if (system(msg2) !=0 ) {
			sprintf(msg,"%s Stop Failed.",lhapp2name);
			logit(msg);
		} else {
			sprintf(msg,"%s Stop Complete.",lhapp2name);
			logit(msg);
		}
		sprintf(msg2,"lh%dapp2timeo 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		sprintf(msg,"/tmp/lh%dapp2%d",lhnum,now);
		unlink(msg);
	}

	if (lhapp3name[0]!=0 && *lhapp3stat == ON ) {

		sprintf(msg,"fstast lh%dapp3",lhnum);
		fstkill(msg);

		time(&now);
		sprintf(msg,"/tmp/lh%dapp3%d",lhnum,now);
		f=fopen(msg,"w");
		if (f == NULL) {
        		sprintf(msg2,"Unable to open %s",msg);
        		logit(msg2);
			return;
		}
		sprintf(msg2,"lh%dapp3testres 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		sprintf(msg2,"lh%dapp3testtimeo 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		fprintf(f,"%s",lhapp3stop);
		fclose(f);
		sprintf(msg2,"lh%dapp3timeo %d",lhnum,now);
		fsthahere(msg2);
		sprintf(msg2,". %s",msg);
		if (system(msg2) !=0 ) {
			sprintf(msg,"%s Stop Failed.",lhapp3name);
			logit(msg);
		} else {
			sprintf(msg,"%s Stop Complete.",lhapp3name);
			logit(msg);
		}
		sprintf(msg2,"lh%dapp3timeo 0",lhnum);
		fsthathere(msg2);
		fsthahere(msg2);
		sprintf(msg,"/tmp/lh%dapp3%d",lhnum,now);
		unlink(msg);
	}




	time(&now);

	sprintf(msg,"lh%dtimeo %d",lhnum,now);
	fsthathere(msg);
	fsthahere(msg);
	sprintf(msg,"/tmp/lh%d%d",lhnum,now);
	f=fopen(msg,"w");
	if (f==NULL) {
		/* do error stuff HERE */
	}

	/***********run *.down scripts************************/

	fprintf(f,"for i in %s/S*.down\ndo\n$i\ndone\n",lhpath);
	fclose(f);
	sprintf(msg2,". %s",msg);

	rcode = system(msg2);
	if (shm->verbose == YES) {
		sprintf(msg2,"S.down Scripts returned %d",rcode);
		logit(msg2);
	}
	if (rcode != 0) {
		sprintf(msg,"LH%d Shutdown FAILED",lhnum);
		logit(msg);
		sprintf(msg,"lh%dstat %d",lhnum,FAILED);
		fsthathere(msg);
		fsthahere(msg);
	} else 
	sprintf(msg,"lh%dstat %d",lhnum,DOWN);
	fsthathere(msg);
	fsthahere(msg);
	sprintf(msg,"/tmp/lh%d%d",lhnum,now);
	unlink(msg);
	sprintf(msg,"lh%dtimeo 0",lhnum);
	fsthathere(msg);
	fsthahere(msg);
	sprintf(msg,"dirty %d",YES);
	fsthahere(msg);
}
}


main(int argc, char *argv[]) {
int justcomeup=YES;

strcpy(fsthaprogname,argv[0]);
mypid=getpid();
fclose(stdin);
fclose(stdout);
fclose(stderr);
setsid();
umask(077);

sprintf(msg,"%s STARTUP %s ", fsthaprogname,BBCVERSION);
logit(msg);


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

sprintf(msg,"fstasm_pid %d",mypid);
fsthahere(msg);

while (1) {
	if (shm->cstat != DOWN) {
	
	if (justcomeup == YES) {
		justcomeup = NO;
		do_fallback();
	}

	/******* monitor existence of fsthbm **********/
		if (isrunning("fsthbm") != YES) {
                	sprintf(msg,"fsthbm Is Missing. Cluster Aborted.");
                	logit(msg);
			shm->cstat = DOWN;
			release_disk();
			remove_shm();
			exit(1);
        	}
	/******* end monitor existence of fsthbm **********/


	/******* monitor data services (Call test scripts) ******/
	
	fstasm(0,&(shm->lh0failing),&(shm->lh0stat),&(shm->lh0node),
		shm->lh0path,&(shm->lh0timeo),
		shm->lh0app0name,
		&(shm->lh0app0stat),
		shm->lh0app0start,
		shm->lh0app0stop,
		&(shm->lh0app0timeo),
		&(shm->lh0app0testres),
		&(shm->lh0app0testtimeo),
		&(shm->lh0app0remtesttimeo),
		&(shm->lh0app0remnode),
		shm->lh0app1name,
		&(shm->lh0app1stat),
		shm->lh0app1start,
		shm->lh0app1stop,
		&(shm->lh0app1timeo),
		&(shm->lh0app1testres),
		&(shm->lh0app1testtimeo),
		&(shm->lh0app1remtesttimeo),
		&(shm->lh0app1remnode),
		shm->lh0app2name,
		&(shm->lh0app2stat),
		shm->lh0app2start,
		shm->lh0app2stop,
		&(shm->lh0app2timeo),
		&(shm->lh0app2testres),
		&(shm->lh0app2testtimeo),
		&(shm->lh0app2remtesttimeo),
		&(shm->lh0app2remnode),
		shm->lh0app3name,
		&(shm->lh0app3stat),
		shm->lh0app3start,
		shm->lh0app3stop,
		&(shm->lh0app3timeo),
		&(shm->lh0app3testres),
		&(shm->lh0app3testtimeo),
		&(shm->lh0app3remtesttimeo),
		&(shm->lh0app3remnode),
		&(shm->lh0failto)
		); 

	fstasm(1,&(shm->lh1failing),&(shm->lh1stat),&(shm->lh1node),
		shm->lh1path,&(shm->lh1timeo),
		shm->lh1app0name,
		&(shm->lh1app0stat),
		shm->lh1app0start,
		shm->lh1app0stop,
		&(shm->lh1app0timeo),
		&(shm->lh1app0testres),
		&(shm->lh1app0testtimeo),
		&(shm->lh1app0remtesttimeo),
		&(shm->lh1app0remnode),
		shm->lh1app1name,
		&(shm->lh1app1stat),
		shm->lh1app1start,
		shm->lh1app1stop,
		&(shm->lh1app1timeo),
		&(shm->lh1app1testres),
		&(shm->lh1app1testtimeo),
		&(shm->lh1app1remtesttimeo),
		&(shm->lh1app1remnode),
		shm->lh1app2name,
		&(shm->lh1app2stat),
		shm->lh1app2start,
		shm->lh1app2stop,
		&(shm->lh1app2timeo),
		&(shm->lh1app2testres),
		&(shm->lh1app2testtimeo),
		&(shm->lh1app2remtesttimeo),
		&(shm->lh1app2remnode),
		shm->lh1app3name,
		&(shm->lh1app3stat),
		shm->lh1app3start,
		shm->lh1app3stop,
		&(shm->lh1app3timeo),
		&(shm->lh1app3testres),
		&(shm->lh1app3testtimeo),
		&(shm->lh1app3remtesttimeo),
		&(shm->lh1app3remnode),
		&(shm->lh1failto)
		); 

	fstasm(2,&(shm->lh2failing),&(shm->lh2stat),&(shm->lh2node),
		shm->lh2path,&(shm->lh2timeo),
		shm->lh2app0name,
		&(shm->lh2app0stat),
		shm->lh2app0start,
		shm->lh2app0stop,
		&(shm->lh2app0timeo),
		&(shm->lh2app0testres),
		&(shm->lh2app0testtimeo),
		&(shm->lh2app0remtesttimeo),
		&(shm->lh2app0remnode),
		shm->lh2app1name,
		&(shm->lh2app1stat),
		shm->lh2app1start,
		shm->lh2app1stop,
		&(shm->lh2app1timeo),
		&(shm->lh2app1testres),
		&(shm->lh2app1testtimeo),
		&(shm->lh2app1remtesttimeo),
		&(shm->lh2app1remnode),
		shm->lh2app2name,
		&(shm->lh2app2stat),
		shm->lh2app2start,
		shm->lh2app2stop,
		&(shm->lh2app2timeo),
		&(shm->lh2app2testres),
		&(shm->lh2app2testtimeo),
		&(shm->lh2app2remtesttimeo),
		&(shm->lh2app2remnode),
		shm->lh2app3name,
		&(shm->lh2app3stat),
		shm->lh2app3start,
		shm->lh2app3stop,
		&(shm->lh2app3timeo),
		&(shm->lh2app3testres),
		&(shm->lh2app3testtimeo),
		&(shm->lh2app3remtesttimeo),
		&(shm->lh2app3remnode),
		&(shm->lh2failto)
		); 

	fstasm(3,&(shm->lh3failing),&(shm->lh3stat),&(shm->lh3node),
		shm->lh3path,&(shm->lh3timeo),
		shm->lh3app0name,
		&(shm->lh3app0stat),
		shm->lh3app0start,
		shm->lh3app0stop,
		&(shm->lh3app0timeo),
		&(shm->lh3app0testres),
		&(shm->lh3app0testtimeo),
		&(shm->lh3app0remtesttimeo),
		&(shm->lh3app0remnode),
		shm->lh3app1name,
		&(shm->lh3app1stat),
		shm->lh3app1start,
		shm->lh3app1stop,
		&(shm->lh3app1timeo),
		&(shm->lh3app1testres),
		&(shm->lh3app1testtimeo),
		&(shm->lh3app1remtesttimeo),
		&(shm->lh3app1remnode),
		shm->lh3app2name,
		&(shm->lh3app2stat),
		shm->lh3app2start,
		shm->lh3app2stop,
		&(shm->lh3app2timeo),
		&(shm->lh3app2testres),
		&(shm->lh3app2testtimeo),
		&(shm->lh3app2remtesttimeo),
		&(shm->lh3app2remnode),
		shm->lh3app3name,
		&(shm->lh3app3stat),
		shm->lh3app3start,
		shm->lh3app3stop,
		&(shm->lh3app3timeo),
		&(shm->lh3app3testres),
		&(shm->lh3app3testtimeo),
		&(shm->lh3app3remtesttimeo),
		&(shm->lh3app3remnode),
		&(shm->lh3failto)
		); 


	/******* end monitor data services (Call test scripts) ******/
	}
	else if (shm->cstat == DOWN) {
		logit("fstasm is Down");
		exit(0);
	}
	sleep(1);
}

}

