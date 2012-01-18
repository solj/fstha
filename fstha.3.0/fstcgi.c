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
 * File Name: fstcgi.c
 * 		produce html status page
 * 
 * File Authors:
 * 		Reggie Beavers (reggiebeavers@fstha.com)
 */
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <libgen.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "fstha.h"

#define TEXTROWS 10
#define TEXTCOLS 120

void getmessages () {
off_t filesize, offset_done, offset;
int f,ret,x;
char s[BUFSIZ]="",b[BUFSIZ]="";

if ( (f=open("/var/adm/messages",O_RDONLY|O_LARGEFILE)) == -1) {
        fprintf(stderr,"Open of /var/adm/messages failed.\n");
        exit(1);
}

filesize=lseek(f,0,SEEK_END);

for (offset_done=0; offset_done < 1000; offset_done++) {
        lseek(f,filesize-offset_done,SEEK_SET);
        read(f,s,1);
        if (s[0] == 10 || offset_done == filesize) {
                ret=read(f,b,BUFSIZ);
                for (x=0,s[0]=0; b[x]!=10 && x<=ret; x++) {
                        putchar(b[x]);
                }
                if (x>0) puts("<br>");
        }
}
close(f);

}


main(int argc, char *argv[]) {
	FILE *vf; 
	char versionstr[BUFSIZ]="";
	int	htmlallowexec,refreshrate,cstat;
	time_t	now;
	char nodename[MAXHOSTNAMELEN]="";
	char querystring[BUFSIZ],requestmethod[BUFSIZ];
	char *firstarg,*secondarg,*lh,*node,*app,*mode;
	
	strcpy(fsthaprogname,argv[0]);
	mypid=getpid();
	
	gethostname(nodename,MAXHOSTNAMELEN-2);
	nodename[MAXHOSTNAMELEN-1]=0;
	setvbuf(stdout, NULL, _IONBF,0);

	vf=fopen("/opt/FSTha/etc/fstha.version","r");
	if (vf != NULL) {
		fgets(versionstr,BUFSIZ,vf);
	}
	fclose(vf);
	/*
 	* attach shm
 	*/
	shmid = shmget(shmkey,SHMSZ,IPC_ALLOC|0666);
	if ((shm = (struct cstates *)shmat(shmid, NULL, 0)) == (struct cstates *) -1)  {
		refreshrate = 30;
		cstat = DOWN;
		htmlallowexec = NO;
	}
	else {
		refreshrate = shm->htmlrefreshsecs;
		cstat = shm->cstat;
		htmlallowexec = shm->htmlallowexec;
	}

	printf("Content-Type: text/html\r\n\r\n");
	printf("<HTML> <HEAD>\r\n");
	printf("<META HTTP-EQUIV=\"Refresh\" CONTENT=\"%d\">\r\n",
		(refreshrate == 0) ? 30 : refreshrate );
	printf("<TITLE> %s - %s </TITLE>\r\n",versionstr,nodename);
	printf("</HEAD>\r\n<BODY BGCOLOR=\"AliceBlue\" TEXT=\"black\"> \r\n");
	printf("<CENTER>\r\n");
	printf("<img src=\"/fstimages/header.gif\">",nodename);
	printf("<TABLE WIDTH=402 BORDER=1>\r\n");
	printf("<TR>");
	printf("<TD WIDTH=200><FONT COLOR=\"green\">");
if (cstat == DOWN) {
printf("<CENTER> Cluster is Down</CENTER></BODY></HTML>");
exit(1);
}
	printf(" <a href=\"http://www.fstha.com/donate.html\"><center>Make a Donation</center></a> </TD>");
	printf("<TD WIDTH=200 ></FONT>");
	time(&now);
	printf("<P>%s</P>\r\n",ctime(&now));


	printf("</TABLE></CENTER>\r\n");
	printf("<BR>\r\n");

	printf("<BR>\r\n");
	printf("<CENTER> <TABLE WIDTH=400  BORDER=2 CELLPADDING=10>\r\n");
	printf("<TR>\r\n");
	printf("<TH></TH>");
	printf("<TH  WIDTH=50>Node</TH>");
	printf("<TH>10.1.1.x</TH>");
	printf("<TH>10.1.2.x</TH>");
	printf("<TH WIDTH=50><B>%s</B></TH>",shm->lh0name);
	printf("<TH WIDTH=50><B>%s</B></TH>",shm->lh1name);
	printf("<TH WIDTH=50><B>%s</B></TH>",shm->lh2name);
	printf("<TH WIDTH=50><B>%s</B></TH>",shm->lh3name);
	printf("</TR>\r\n");

	printf("<TR>\r\n");
	if (shm->node0stat == UP)
		printf("<TD><img src=\"/fstimages/green-cone.gif\"></TD>");
	else
		printf("<TD><img src=\"/fstimages/lgrey-cone.gif\"></TD>");
	printf("<TD><B>%s</B></TD>",shm->node0);
	if (shm->hb0stat0 == UP)
		printf("<TD><img src=\"/fstimages/green-dot.gif\"></TD>");
	else
		printf("<TD><img src=\"/fstimages/lgrey-dot.gif\"></TD>");
	if (shm->hb1stat0 == UP)
		printf("<TD><img src=\"/fstimages/green-dot.gif\"></TD>");
	else
		printf("<TD><img src=\"/fstimages/lgrey-dot.gif\"></TD>");

	if (shm->lh0node == 0) {
		if (shm->lh0stat == UP)
			if (shm->lh0app0stat == DOWN || shm->lh0app0stat == FAILED  || shm->lh0app0stat == NOCHECK
			|| shm->lh0app1stat == DOWN || shm->lh0app1stat == FAILED  || shm->lh0app1stat == NOCHECK
			|| shm->lh0app2stat == DOWN || shm->lh0app2stat == FAILED  || shm->lh0app2stat == NOCHECK
			|| shm->lh0app3stat == DOWN || shm->lh0app3stat == FAILED  || shm->lh0app3stat == NOCHECK)
				printf("<TD><center><img src=\"/fstimages/yellow-dot.gif\"></center></TD>");
			else
				printf("<TD><center><img src=\"/fstimages/green-dot.gif\"></center></TD>");
		else if (shm->lh0stat == DOWN)
			printf("<TD><center><img src=\"/fstimages/brown-dot.gif\"></center></TD>");
		else if (shm->lh0stat == FAILED)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else if (shm->lh0stat == GUP || shm->lh0stat == GDOWN)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else 
			printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	} else
		printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	if (shm->lh1node == 0) {
		if (shm->lh1stat == UP)
			if (shm->lh1app0stat == DOWN || shm->lh1app0stat == FAILED  || shm->lh1app0stat == NOCHECK
			|| shm->lh1app1stat == DOWN || shm->lh1app1stat == FAILED  || shm->lh1app1stat == NOCHECK
			|| shm->lh1app2stat == DOWN || shm->lh1app2stat == FAILED  || shm->lh1app2stat == NOCHECK
			|| shm->lh1app3stat == DOWN || shm->lh1app3stat == FAILED  || shm->lh1app3stat == NOCHECK)
				printf("<TD><center><img src=\"/fstimages/yellow-dot.gif\"></center></TD>");
			else
				printf("<TD><center><img src=\"/fstimages/green-dot.gif\"></center></TD>");
		else if (shm->lh1stat == DOWN)
			printf("<TD><center><img src=\"/fstimages/brown-dot.gif\"></center></TD>");
		else if (shm->lh1stat == FAILED)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else if (shm->lh1stat == GUP || shm->lh1stat == GDOWN)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else 
			printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	} else
		printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	if (shm->lh2node == 0) {
		if (shm->lh2stat == UP)
			if (shm->lh2app0stat == DOWN || shm->lh2app0stat == FAILED  || shm->lh2app0stat == NOCHECK
			|| shm->lh2app1stat == DOWN || shm->lh2app1stat == FAILED  || shm->lh2app1stat == NOCHECK
			|| shm->lh2app2stat == DOWN || shm->lh2app2stat == FAILED  || shm->lh2app2stat == NOCHECK
			|| shm->lh2app3stat == DOWN || shm->lh2app3stat == FAILED  || shm->lh2app3stat == NOCHECK)
				printf("<TD><center><img src=\"/fstimages/yellow-dot.gif\"></center></TD>");
			else
				printf("<TD><center><img src=\"/fstimages/green-dot.gif\"></center></TD>");
		else if (shm->lh2stat == DOWN)
			printf("<TD><center><img src=\"/fstimages/brown-dot.gif\"></center></TD>");
		else if (shm->lh2stat == FAILED)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else if (shm->lh2stat == GUP || shm->lh2stat == GDOWN)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else 
			printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	} else
		printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	if (shm->lh3node == 0) {
		if (shm->lh3stat == UP)
			if (shm->lh3app0stat == DOWN || shm->lh3app0stat == FAILED  || shm->lh3app0stat == NOCHECK
			|| shm->lh3app1stat == DOWN || shm->lh3app1stat == FAILED  || shm->lh3app1stat == NOCHECK
			|| shm->lh3app2stat == DOWN || shm->lh3app2stat == FAILED  || shm->lh3app2stat == NOCHECK
			|| shm->lh3app3stat == DOWN || shm->lh3app3stat == FAILED  || shm->lh3app3stat == NOCHECK)
				printf("<TD><center><img src=\"/fstimages/yellow-dot.gif\"></center></TD>");
			else
				printf("<TD><center><img src=\"/fstimages/green-dot.gif\"></center></TD>");
		else if (shm->lh3stat == DOWN)
			printf("<TD><center><img src=\"/fstimages/brown-dot.gif\"></center></TD>");
		else if (shm->lh3stat == FAILED)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else if (shm->lh3stat == GUP || shm->lh3stat == GDOWN)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else 
			printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	} else
		printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	printf("</TR>\r\n");

	printf("<TR>\r\n");
	if (shm->node1stat == UP)
		printf("<TD><img src=\"/fstimages/green-cone.gif\"></TD>");
	else
		printf("<TD><img src=\"/fstimages/lgrey-cone.gif\"></TD>");
	printf("<TD><B>%s</B></TD>",shm->node1);
	if (shm->hb0stat1 == UP)
		printf("<TD><img src=\"/fstimages/green-dot.gif\"></TD>");
	else
		printf("<TD><img src=\"/fstimages/lgrey-dot.gif\"></TD>");
	if (shm->hb1stat1 == UP)
		printf("<TD><img src=\"/fstimages/green-dot.gif\"></TD>");
	else
		printf("<TD><img src=\"/fstimages/lgrey-dot.gif\"></TD>");

	if (shm->lh0node == 1) {
		if (shm->lh0stat == UP)
			if (shm->lh0app0stat == DOWN || shm->lh0app0stat == FAILED  || shm->lh0app0stat == NOCHECK
			|| shm->lh0app1stat == DOWN || shm->lh0app1stat == FAILED  || shm->lh0app1stat == NOCHECK
			|| shm->lh0app2stat == DOWN || shm->lh0app2stat == FAILED  || shm->lh0app2stat == NOCHECK
			|| shm->lh0app3stat == DOWN || shm->lh0app3stat == FAILED  || shm->lh0app3stat == NOCHECK)
				printf("<TD><center><img src=\"/fstimages/yellow-dot.gif\"></center></TD>");
			else
				printf("<TD><center><img src=\"/fstimages/green-dot.gif\"></center></TD>");
		else if (shm->lh0stat == DOWN)
			printf("<TD><center><img src=\"/fstimages/brown-dot.gif\"></center></TD>");
		else if (shm->lh0stat == FAILED)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else if (shm->lh0stat == GUP || shm->lh0stat == GDOWN)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else 
			printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	} else
		printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	if (shm->lh1node == 1) {
		if (shm->lh1stat == UP)
			if (shm->lh1app0stat == DOWN || shm->lh1app0stat == FAILED  || shm->lh1app0stat == NOCHECK
			|| shm->lh1app1stat == DOWN || shm->lh1app1stat == FAILED  || shm->lh1app1stat == NOCHECK
			|| shm->lh1app2stat == DOWN || shm->lh1app2stat == FAILED  || shm->lh1app2stat == NOCHECK
			|| shm->lh1app3stat == DOWN || shm->lh1app3stat == FAILED  || shm->lh1app3stat == NOCHECK)
				printf("<TD><center><img src=\"/fstimages/yellow-dot.gif\"></center></TD>");
			else
				printf("<TD><center><img src=\"/fstimages/green-dot.gif\"></center></TD>");
		else if (shm->lh1stat == DOWN)
			printf("<TD><center><img src=\"/fstimages/brown-dot.gif\"></center></TD>");
		else if (shm->lh1stat == FAILED)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else if (shm->lh1stat == GUP || shm->lh1stat == GDOWN)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else 
			printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	} else
		printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	if (shm->lh2node == 1) {
		if (shm->lh2stat == UP)
			if (shm->lh2app0stat == DOWN || shm->lh2app0stat == FAILED  || shm->lh2app0stat == NOCHECK
			|| shm->lh2app1stat == DOWN || shm->lh2app1stat == FAILED  || shm->lh2app1stat == NOCHECK
			|| shm->lh2app2stat == DOWN || shm->lh2app2stat == FAILED  || shm->lh2app2stat == NOCHECK
			|| shm->lh2app3stat == DOWN || shm->lh2app3stat == FAILED  || shm->lh2app3stat == NOCHECK)
				printf("<TD><center><img src=\"/fstimages/yellow-dot.gif\"></center></TD>");
			else
				printf("<TD><center><img src=\"/fstimages/green-dot.gif\"></center></TD>");
		else if (shm->lh2stat == DOWN)
			printf("<TD><center><img src=\"/fstimages/brown-dot.gif\"></center></TD>");
		else if (shm->lh2stat == FAILED)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else if (shm->lh2stat == GUP || shm->lh2stat == GDOWN)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else 
			printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	} else
		printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	if (shm->lh3node == 1) {
		if (shm->lh3stat == UP)
			if (shm->lh3app0stat == DOWN || shm->lh3app0stat == FAILED  || shm->lh3app0stat == NOCHECK
			|| shm->lh3app1stat == DOWN || shm->lh3app1stat == FAILED  || shm->lh3app1stat == NOCHECK
			|| shm->lh3app2stat == DOWN || shm->lh3app2stat == FAILED  || shm->lh3app2stat == NOCHECK
			|| shm->lh3app3stat == DOWN || shm->lh3app3stat == FAILED  || shm->lh3app3stat == NOCHECK)
				printf("<TD><center><img src=\"/fstimages/yellow-dot.gif\"></center></TD>");
			else
				printf("<TD><center><img src=\"/fstimages/green-dot.gif\"></center></TD>");
		else if (shm->lh3stat == DOWN)
			printf("<TD><center><img src=\"/fstimages/brown-dot.gif\"></center></TD>");
		else if (shm->lh3stat == FAILED)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else if (shm->lh3stat == GUP || shm->lh3stat == GDOWN)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else 
			printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	} else
		printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	printf("</TR>\r\n");

	printf("<TR>\r\n");
	if (shm->node2stat == UP)
		printf("<TD><img src=\"/fstimages/green-cone.gif\"></TD>");
	else
		printf("<TD><img src=\"/fstimages/lgrey-cone.gif\"></TD>");
	printf("<TD><B>%s</B></TD>",shm->node2);
	if (shm->hb0stat2 == UP)
		printf("<TD><img src=\"/fstimages/green-dot.gif\"></TD>");
	else
		printf("<TD><img src=\"/fstimages/lgrey-dot.gif\"></TD>");
	if (shm->hb1stat2 == UP)
		printf("<TD><img src=\"/fstimages/green-dot.gif\"></TD>");
	else
		printf("<TD><img src=\"/fstimages/lgrey-dot.gif\"></TD>");

	if (shm->lh0node == 2) {
		if (shm->lh0stat == UP)
			if (shm->lh0app0stat == DOWN || shm->lh0app0stat == FAILED  || shm->lh0app0stat == NOCHECK
			|| shm->lh0app1stat == DOWN || shm->lh0app1stat == FAILED  || shm->lh0app1stat == NOCHECK
			|| shm->lh0app2stat == DOWN || shm->lh0app2stat == FAILED  || shm->lh0app2stat == NOCHECK
			|| shm->lh0app3stat == DOWN || shm->lh0app3stat == FAILED  || shm->lh0app3stat == NOCHECK)
				printf("<TD><center><img src=\"/fstimages/yellow-dot.gif\"></center></TD>");
			else
				printf("<TD><center><img src=\"/fstimages/green-dot.gif\"></center></TD>");
		else if (shm->lh0stat == DOWN)
			printf("<TD><center><img src=\"/fstimages/brown-dot.gif\"></center></TD>");
		else if (shm->lh0stat == FAILED)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else if (shm->lh0stat == GUP || shm->lh0stat == GDOWN)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else 
			printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	} else
		printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	if (shm->lh1node == 2) {
		if (shm->lh1stat == UP)
			if (shm->lh1app0stat == DOWN || shm->lh1app0stat == FAILED  || shm->lh1app0stat == NOCHECK
			|| shm->lh1app1stat == DOWN || shm->lh1app1stat == FAILED  || shm->lh1app1stat == NOCHECK
			|| shm->lh1app2stat == DOWN || shm->lh1app2stat == FAILED  || shm->lh1app2stat == NOCHECK
			|| shm->lh1app3stat == DOWN || shm->lh1app3stat == FAILED  || shm->lh1app3stat == NOCHECK)
				printf("<TD><center><img src=\"/fstimages/yellow-dot.gif\"></center></TD>");
			else
				printf("<TD><center><img src=\"/fstimages/green-dot.gif\"></center></TD>");
		else if (shm->lh1stat == DOWN)
			printf("<TD><center><img src=\"/fstimages/brown-dot.gif\"></center></TD>");
		else if (shm->lh1stat == FAILED)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else if (shm->lh1stat == GUP || shm->lh1stat == GDOWN)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else 
			printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	} else
		printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	if (shm->lh2node == 2) {
		if (shm->lh2stat == UP)
			if (shm->lh2app0stat == DOWN || shm->lh2app0stat == FAILED  || shm->lh2app0stat == NOCHECK
			|| shm->lh2app1stat == DOWN || shm->lh2app1stat == FAILED  || shm->lh2app1stat == NOCHECK
			|| shm->lh2app2stat == DOWN || shm->lh2app2stat == FAILED  || shm->lh2app2stat == NOCHECK
			|| shm->lh2app3stat == DOWN || shm->lh2app3stat == FAILED  || shm->lh2app3stat == NOCHECK)
				printf("<TD><center><img src=\"/fstimages/yellow-dot.gif\"></center></TD>");
			else
				printf("<TD><center><img src=\"/fstimages/green-dot.gif\"></center></TD>");
		else if (shm->lh2stat == DOWN)
			printf("<TD><center><img src=\"/fstimages/brown-dot.gif\"></center></TD>");
		else if (shm->lh2stat == FAILED)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else if (shm->lh2stat == GUP || shm->lh2stat == GDOWN)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else 
			printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	} else
		printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	if (shm->lh3node == 2) {
		if (shm->lh3stat == UP)
			if (shm->lh3app0stat == DOWN || shm->lh3app0stat == FAILED  || shm->lh3app0stat == NOCHECK
			|| shm->lh3app1stat == DOWN || shm->lh3app1stat == FAILED  || shm->lh3app1stat == NOCHECK
			|| shm->lh3app2stat == DOWN || shm->lh3app2stat == FAILED  || shm->lh3app2stat == NOCHECK
			|| shm->lh3app3stat == DOWN || shm->lh3app3stat == FAILED  || shm->lh3app3stat == NOCHECK)
				printf("<TD><center><img src=\"/fstimages/yellow-dot.gif\"></center></TD>");
			else
				printf("<TD><center><img src=\"/fstimages/green-dot.gif\"></center></TD>");
		else if (shm->lh3stat == DOWN)
			printf("<TD><center><img src=\"/fstimages/brown-dot.gif\"></center></TD>");
		else if (shm->lh3stat == FAILED)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else if (shm->lh3stat == GUP || shm->lh3stat == GDOWN)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else 
			printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	} else
		printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	printf("</TR>\r\n");

	printf("<TR>\r\n");
	if (shm->node3stat == UP)
		printf("<TD><img src=\"/fstimages/green-cone.gif\"></TD>");
	else
		printf("<TD><img src=\"/fstimages/lgrey-cone.gif\"></TD>");
	printf("<TD><B>%s</B></TD>",shm->node3);
	if (shm->hb0stat3 == UP)
		printf("<TD><img src=\"/fstimages/green-dot.gif\"></TD>");
	else
		printf("<TD><img src=\"/fstimages/lgrey-dot.gif\"></TD>");
	if (shm->hb1stat3 == UP)
		printf("<TD><img src=\"/fstimages/green-dot.gif\"></TD>");
	else
		printf("<TD><img src=\"/fstimages/lgrey-dot.gif\"></TD>");

	if (shm->lh0node == 3) {
		if (shm->lh0stat == UP)
			if (shm->lh0app0stat == DOWN || shm->lh0app0stat == FAILED  || shm->lh0app0stat == NOCHECK
			|| shm->lh0app1stat == DOWN || shm->lh0app1stat == FAILED  || shm->lh0app1stat == NOCHECK
			|| shm->lh0app2stat == DOWN || shm->lh0app2stat == FAILED  || shm->lh0app2stat == NOCHECK
			|| shm->lh0app3stat == DOWN || shm->lh0app3stat == FAILED  || shm->lh0app3stat == NOCHECK)
				printf("<TD><center><img src=\"/fstimages/yellow-dot.gif\"></center></TD>");
			else
				printf("<TD><center><img src=\"/fstimages/green-dot.gif\"></center></TD>");
		else if (shm->lh0stat == DOWN)
			printf("<TD><center><img src=\"/fstimages/brown-dot.gif\"></center></TD>");
		else if (shm->lh0stat == FAILED)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else if (shm->lh0stat == GUP || shm->lh0stat == GDOWN)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else 
			printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	} else
		printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	if (shm->lh1node == 3) {
		if (shm->lh1stat == UP)
			if (shm->lh1app0stat == DOWN || shm->lh1app0stat == FAILED  || shm->lh1app0stat == NOCHECK
			|| shm->lh1app1stat == DOWN || shm->lh1app1stat == FAILED  || shm->lh1app1stat == NOCHECK
			|| shm->lh1app2stat == DOWN || shm->lh1app2stat == FAILED  || shm->lh1app2stat == NOCHECK
			|| shm->lh1app3stat == DOWN || shm->lh1app3stat == FAILED  || shm->lh1app3stat == NOCHECK)
				printf("<TD><center><img src=\"/fstimages/yellow-dot.gif\"></center></TD>");
			else
				printf("<TD><center><img src=\"/fstimages/green-dot.gif\"></center></TD>");
		else if (shm->lh1stat == DOWN)
			printf("<TD><center><img src=\"/fstimages/brown-dot.gif\"></center></TD>");
		else if (shm->lh1stat == FAILED)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else if (shm->lh1stat == GUP || shm->lh1stat == GDOWN)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else 
			printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	} else
		printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	if (shm->lh2node == 3) {
		if (shm->lh2stat == UP)
			if (shm->lh2app0stat == DOWN || shm->lh2app0stat == FAILED  || shm->lh2app0stat == NOCHECK
			|| shm->lh2app1stat == DOWN || shm->lh2app1stat == FAILED  || shm->lh2app1stat == NOCHECK
			|| shm->lh2app2stat == DOWN || shm->lh2app2stat == FAILED  || shm->lh2app2stat == NOCHECK
			|| shm->lh2app3stat == DOWN || shm->lh2app3stat == FAILED  || shm->lh2app3stat == NOCHECK)
				printf("<TD><center><img src=\"/fstimages/yellow-dot.gif\"></center></TD>");
			else
				printf("<TD><center><img src=\"/fstimages/green-dot.gif\"></center></TD>");
		else if (shm->lh2stat == DOWN)
			printf("<TD><center><img src=\"/fstimages/brown-dot.gif\"></center></TD>");
		else if (shm->lh2stat == FAILED)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else if (shm->lh2stat == GUP || shm->lh2stat == GDOWN)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else 
			printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	} else
		printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	if (shm->lh3node == 3) {
		if (shm->lh3stat == UP)
			if (shm->lh3app0stat == DOWN || shm->lh3app0stat == FAILED  || shm->lh3app0stat == NOCHECK
			|| shm->lh3app1stat == DOWN || shm->lh3app1stat == FAILED  || shm->lh3app1stat == NOCHECK
			|| shm->lh3app2stat == DOWN || shm->lh3app2stat == FAILED  || shm->lh3app2stat == NOCHECK
			|| shm->lh3app3stat == DOWN || shm->lh3app3stat == FAILED  || shm->lh3app3stat == NOCHECK)
				printf("<TD><center><img src=\"/fstimages/yellow-dot.gif\"></center></TD>");
			else
				printf("<TD><center><img src=\"/fstimages/green-dot.gif\"></center></TD>");
		else if (shm->lh3stat == DOWN)
			printf("<TD><center><img src=\"/fstimages/brown-dot.gif\"></center></TD>");
		else if (shm->lh3stat == FAILED)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else if (shm->lh3stat == GUP || shm->lh3stat == GDOWN)
			printf("<TD><center><img src=\"/fstimages/red-dot.gif\"></center></TD>");
		else 
			printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	} else
		printf("<TD><center><img src=\"/fstimages/lgrey-dot.gif\"></center></TD>");
	printf("</TR>\r\n");

	printf("</TABLE> </CENTER>\r\n");


	printf("<BR>\r\n");
	printf("<BR>\r\n");
        printf("<CENTER>");
printf("<FONT italics>");
printf("<img src=\"/fstimages/green-dot.gif\">  Up w/all Apps");
printf("<img src=\"/fstimages/yellow-dot.gif\"> App(s) Down ");
printf("<img src=\"/fstimages/blue-dot.gif\">   In Transition ");
printf("<img src=\"/fstimages/red-dot.gif\">    Failure ");
printf("<img src=\"/fstimages/brown-dot.gif\">  Down");
printf("<BR>\r\n");
printf("<BR>\r\n");
printf("Copyright 2005 Foundation Software Tools, LLC");
printf("</FONT>");
        printf("</CENTER>");
	printf("<BR>\r\n");
	printf("<CENTER><SUB>%s</SUB></CENTER>",versionstr);
	printf("</BODY></HTML>\r\n");
	exit(0);
}
