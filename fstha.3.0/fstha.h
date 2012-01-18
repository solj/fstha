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
 * File Name: fstha.h
 * 		
 * 
 * File Authors:
 * 		Reggie Beavers (reggiebeavers@fstha.com)
 */
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>
#include <sys/systeminfo.h>
#include <sys/sem.h>
#include <sys/mhd.h>
#include <sys/utsname.h>

#define SCSI_TIMEOUT 40 /* default + 10 */
#define RESERVE_DELAY 40 /* time to wait to see if another reserves */
#define MAX_MSG 100
#define ETCDIR "/opt/FSTha/etc"

#define BBCVERSION "3.0.1"
#define PLUS 2
#define HA 1
#define MANUAL 0
#define YES 0
#define sYES "0"
#define NO 1
#define sNO "1"
#define UP 0
#define sUP "0"
#define ON 0
#define DOWN 1
#define sDOWN "1"
#define OFF 1
#define REMOTE 2
#define sREMOTE "2"
#define FAIL 3
#define sFAIL "3"
#define FAILED 3
#define sFAILED "3"
#define GDOWN 4
#define sGDOWN "4"
#define GUP 5
#define sGUP "5"
#define JOIN 6
#define NOCHECK 8
#define sNOCHECK "8"
#define WAIT 9
#define sWAIT "9"
#define UNUSED 9
#define FALLANY 8

key_t   shmkey='b'+'b'+'h'+'b';     /* key passed to shmget() */
int     shmflg=SHM_R|SHM_W,         /* permisions flags passed to shmget() */
        shmid,                      /* id returned from shmget()*/
	semflag=IPC_CREAT|0666,     /* ermisions flags passed to semget() */
        nsems=1,                    /* num sems to pass to semget() */
        nops,                       /* number of operations to do */
        semid;      

struct shmid_ds shmbuf;
union semnum {
        int val;
        struct semid_ds *buf;
        ushort *array;
        };

struct sembuf sops[2];

struct utsname myuname;

int prvverbose=1; /* used to return to desired verbose after sync*/

time_t loadtime;  /* my load average timer*/

struct cstates {
int	fsthbb_pid;
int	fsthbm_pid;
int	fsthbserv_pid;
int	fstsvr_pid;
int	fstasm_pid;
int	fstastlh0app0_pid;
int	fstastlh0app1_pid;
int	fstastlh0app2_pid;
int	fstastlh0app3_pid;
int	fstastlh1app0_pid;
int	fstastlh1app1_pid;
int	fstastlh1app2_pid;
int	fstastlh1app3_pid;
int	fstastlh2app0_pid;
int	fstastlh2app1_pid;
int	fstastlh2app2_pid;
int	fstastlh2app3_pid;
int	fstastlh3app0_pid;
int	fstastlh3app1_pid;
int	fstastlh3app2_pid;
int	fstastlh3app3_pid;
int	unfreeze;
int	hbtimeout;
int	hbdelay;
float	myloadf;  /* my load average */
int	insync;
int	qnode;  /* which node has disk reserved */
int	verbose;
int	backup;
char    passwd[16];  /* future use */
int	version;
int	htmlrefreshsecs;
int	htmlallowexec;

int	cstat;      /* cluster status 0=up 1=down */
int	dirty;       /* if dirty=YES then written memory to disk */
int	alone;       /* only node in cluster? */
int	nodeN;        /* my node number */
char	node0[20];   /* ip addr of node0 */
char	node1[20];
char	node2[20];
char	node3[20];
int	node0stat;        /* other node status */
int	node1stat;        /* other node status */
int	node2stat;        /* other node status */
int	node3stat;        /* other node status */
char	cf[128];     /* location of config file */
char	logfile[128];     /* location of log file */
char	reserve_disk[128];  /* quorum disk */
int	reserve_strict;     /* strict disk policy */
int	other_strict;  /* policy of node syncing from */
char	syslogfac[28];     /*  as in user.notice */
char	port[12];     /* tcp port */
char	hbport[12];     /* udp port */

float	node0load;
float	node1load;
float	node2load;
float	node3load;

char	hb0tm0[20];  /* ping timestamp */
char	hb0tm1[20];
char	hb0tm2[20];
char	hb0tm3[20];
char	hb0ip[16];   /* ip */
char	hb0dv[10];   /* interface */
char	hb0pg0[16];   /* ip to ping on other node */
char	hb0pg1[16];   /* ip to ping on other node */
char	hb0pg2[16];   /* ip to ping on other node */
char	hb0pg3[16];   /* ip to ping on other node */
int	hb0stat0;   
int	hb0stat1;   
int	hb0stat2;   
int	hb0stat3;   
char    hb0mask[16];
char    hb0bcast[16];

char	hb1tm0[20];
char	hb1tm1[20];
char	hb1tm2[20];
char	hb1tm3[20];
char	hb1ip[16];
char	hb1dv[10];
char	hb1pg0[16];   /* ip to ping on other node */
char	hb1pg1[16];   /* ip to ping on other node */
char	hb1pg2[16];   /* ip to ping on other node */
char	hb1pg3[16];   /* ip to ping on other node */
int	hb1stat0;   
int	hb1stat1;   
int	hb1stat2;   
int	hb1stat3;   
char    hb1mask[16];
char    hb1bcast[16];

char	pbtm0[20];
char	pbtm1[20];
char	pbtm2[20];
char	pbtm3[20];
int	pbstat0;
int	pbstat1;
int	pbstat2;
int	pbstat3;

int	lhfailtimeo;	/* secs for transition to complete b4 failed */
int	appfailtimeo;	/* secs for transition to complete b4 failed */
int	appfailtesttimeo;	/* secs for transition to complete b4 failed */

char	lh0name[128];           /* logical hostname */
char	lh0path[128];       /* basedir */
int	lh0node;            /* node 0 or 1*/
int	lh0fallback;        /* node to fallback to */
int	lh0failto;        /* node to fail to */
int	lh0stat;            /* 0=served-by-node0 2=served-by-node1 1=down 3=failed 4=going-down 5=going-up*/
int	lh0timeo;		/* secs since transition began */
int	lh0failing;		/* is this failing */
char	lh0app0name[128];       /* app name */
int	lh0app0stat;        /* 0=up 2=maint 1=down 3=failed */
char	lh0app0start[128];  /* start script */
char	lh0app0stop[128];   /* stop script */
char	lh0app0test[128];   /* test script */
int	lh0app0testres;     /* test script result*/
char	lh0app0testtim[20]; /* test script time stamp upon return*/
char    lh0app0remtest[128];   /* remote test script */
int     lh0app0remtestres;     /* test script result*/
int     lh0app0remtesttimeo; /* test script time stamp upon return*/
int	lh0app0remnode;  /* which remote node to monitor 8=any 9=none*/
int	lh0app0timeo;  /* script timeout to fail 0=never-timeout */
int	lh0app0testtimeo;  /* script timeout to fail 0=never-timeout */
int	lh0app0secs;   /* time passed since last test */
int	lh0app0pause;   /* time to pause b4 testing begins */
int	lh0app0crit;   /* number of fails b4 failover 0=never-failover */
char	lh0app1name[128];
int	lh0app1stat;
char	lh0app1start[128];
char	lh0app1stop[128];
char	lh0app1test[128];
int	lh0app1testres;     /* test script result*/
char	lh0app1testtim[20]; /* test script time stamp upon return*/
char    lh0app1remtest[128];   /* remote test script */
int     lh0app1remtestres;     /* test script result*/
int     lh0app1remtesttimeo; /* test script time stamp upon return*/
int	lh0app1remnode;  /* which remote node to monitor 8=any 9=none*/
int	lh0app1timeo;
int	lh0app1testtimeo;
int	lh0app1secs;
int	lh0app1pause;   /* time to pause b4 testing begins */
int	lh0app1crit;
char	lh0app2name[128];
int	lh0app2stat;
char	lh0app2start[128];
char	lh0app2stop[128];
char	lh0app2test[128];
int	lh0app2testres;     /* test script result*/
char	lh0app2testtim[20]; /* test script time stamp upon return*/
char    lh0app2remtest[128];   /* remote test script */
int     lh0app2remtestres;     /* test script result*/
int     lh0app2remtesttimeo; /* test script time stamp upon return*/
int	lh0app2remnode;  /* which remote node to monitor 8=any 9=none*/
int	lh0app2timeo;
int	lh0app2testtimeo;
int	lh0app2secs;
int	lh0app2pause;   /* time to pause b4 testing begins */
int	lh0app2crit;
char	lh0app3name[128];
int	lh0app3stat;
char	lh0app3start[128];
char	lh0app3stop[128];
char	lh0app3test[128];
int	lh0app3testres;     /* test script result*/
char	lh0app3testtim[20]; /* test script time stamp upon return*/
char    lh0app3remtest[128];   /* remote test script */
int     lh0app3remtestres;     /* test script result*/
int     lh0app3remtesttimeo; /* test script time stamp upon return*/
int	lh0app3remnode;
int	lh0app3timeo;
int	lh0app3testtimeo;
int	lh0app3secs;
int	lh0app3pause;   /* time to pause b4 testing begins */
int	lh0app3crit;



char	lh1name[128];           /* logical hostname */
char	lh1path[128];       /* basedir */
int	lh1node;            /* node 0 or 1*/
int	lh1fallback;        /* node to fallback to */
int	lh1failto;        /* node to fail to */
int	lh1stat;            /* 0=served-by-node0 2=served-by-node1 1=down 3=failed 4=going-down 5=going-up*/
int	lh1timeo;		/* secs since transition began */
int	lh1failing;		/* is this failing */
char	lh1app0name[128];       /* app name */
int	lh1app0stat;        /* 0=up 2=maint 1=down 3=failed */
char	lh1app0start[128];  /* start script */
char	lh1app0stop[128];   /* stop script */
char	lh1app0test[128];   /* test script */
int	lh1app0testres;     /* test script result*/
char	lh1app0testtim[20]; /* test script time stamp upon return*/
char    lh1app0remtest[128];   /* remote test script */
int     lh1app0remtestres;     /* test script result*/
int     lh1app0remtesttimeo; /* test script time stamp upon return*/
int	lh1app0remnode;
int	lh1app0timeo;  /* script timeout to fail 0=never-timeout */
int	lh1app0testtimeo;  /* script timeout to fail 0=never-timeout */
int	lh1app0secs;   /* time passed since last test */
int	lh1app0pause;   /* time to pause b4 testing begins */
int	lh1app0crit;   /* number of fails b4 failover 0=never-failover */
char	lh1app1name[128];
int	lh1app1stat;
char	lh1app1start[128];
char	lh1app1stop[128];
char	lh1app1test[128];
int	lh1app1testres;     /* test script result*/
char	lh1app1testtim[20]; /* test script time stamp upon return*/
char    lh1app1remtest[128];   /* remote test script */
int     lh1app1remtestres;     /* test script result*/
int     lh1app1remtesttimeo; /* test script time stamp upon return*/
int	lh1app1remnode;
int	lh1app1timeo;
int	lh1app1testtimeo;
int	lh1app1secs;
int	lh1app1pause;   /* time to pause b4 testing begins */
int	lh1app1crit;
char	lh1app2name[128];
int	lh1app2stat;
char	lh1app2start[128];
char	lh1app2stop[128];
char	lh1app2test[128];
int	lh1app2testres;     /* test script result*/
char	lh1app2testtim[20]; /* test script time stamp upon return*/
char    lh1app2remtest[128];   /* remote test script */
int     lh1app2remtestres;     /* test script result*/
int     lh1app2remtesttimeo; /* test script time stamp upon return*/
int	lh1app2remnode;
int	lh1app2timeo;
int	lh1app2testtimeo;
int	lh1app2secs;
int	lh1app2pause;   /* time to pause b4 testing begins */
int	lh1app2crit;
char	lh1app3name[128];
int	lh1app3stat;
char	lh1app3start[128];
char	lh1app3stop[128];
char	lh1app3test[128];
int	lh1app3testres;     /* test script result*/
char	lh1app3testtim[20]; /* test script time stamp upon return*/
char    lh1app3remtest[128];   /* remote test script */
int     lh1app3remtestres;     /* test script result*/
int     lh1app3remtesttimeo; /* test script time stamp upon return*/
int	lh1app3remnode;
int	lh1app3timeo;
int	lh1app3testtimeo;
int	lh1app3secs;
int	lh1app3pause;   /* time to pause b4 testing begins */
int	lh1app3crit;



char	lh2name[128];           /* logical hostname */
char	lh2path[128];       /* basedir */
int	lh2node;            /* node 0 or 1*/
int	lh2fallback;        /* node to fallback to */
int	lh2failto;        /* node to fail to */
int	lh2stat;            /* 0=served-by-node0 2=served-by-node1 1=down 3=failed 4=going-down 5=going-up*/
int	lh2timeo;		/* secs since transition began */
int	lh2failing;		/* is this failing */
char	lh2app0name[128];       /* app name */
int	lh2app0stat;        /* 0=up 2=maint 1=down 3=failed */
char	lh2app0start[128];  /* start script */
char	lh2app0stop[128];   /* stop script */
char	lh2app0test[128];   /* test script */
int	lh2app0testres;     /* test script result*/
char	lh2app0testtim[20]; /* test script time stamp upon return*/
char    lh2app0remtest[128];   /* remote test script */
int     lh2app0remtestres;     /* test script result*/
int     lh2app0remtesttimeo; /* test script time stamp upon return*/
int	lh2app0remnode;
int	lh2app0timeo;  /* script timeout to fail 0=never-timeout */
int	lh2app0testtimeo;  /* script timeout to fail 0=never-timeout */
int	lh2app0secs;   /* time passed since last test */
int	lh2app0pause;   /* time to pause b4 testing begins */
int	lh2app0crit;   /* number of fails b4 failover 0=never-failover */
char	lh2app1name[128];
int	lh2app1stat;
char	lh2app1start[128];
char	lh2app1stop[128];
char	lh2app1test[128];
int	lh2app1testres;     /* test script result*/
char	lh2app1testtim[20]; /* test script time stamp upon return*/
char    lh2app1remtest[128];   /* remote test script */
int     lh2app1remtestres;     /* test script result*/
int     lh2app1remtesttimeo; /* test script time stamp upon return*/
int	lh2app1remnode;
int	lh2app1timeo;
int	lh2app1testtimeo;
int	lh2app1secs;
int	lh2app1pause;   /* time to pause b4 testing begins */
int	lh2app1crit;
char	lh2app2name[128];
int	lh2app2stat;
char	lh2app2start[128];
char	lh2app2stop[128];
char	lh2app2test[128];
int	lh2app2testres;     /* test script result*/
char	lh2app2testtim[20]; /* test script time stamp upon return*/
char    lh2app2remtest[128];   /* remote test script */
int     lh2app2remtestres;     /* test script result*/
int     lh2app2remtesttimeo; /* test script time stamp upon return*/
int	lh2app2remnode;
int	lh2app2timeo;
int	lh2app2testtimeo;
int	lh2app2secs;
int	lh2app2pause;   /* time to pause b4 testing begins */
int	lh2app2crit;
char	lh2app3name[128];
int	lh2app3stat;
char	lh2app3start[128];
char	lh2app3stop[128];
char	lh2app3test[128];
int	lh2app3testres;     /* test script result*/
char	lh2app3testtim[20]; /* test script time stamp upon return*/
char    lh2app3remtest[128];   /* remote test script */
int     lh2app3remtestres;     /* test script result*/
int     lh2app3remtesttimeo; /* test script time stamp upon return*/
int	lh2app3remnode;
int	lh2app3timeo;
int	lh2app3testtimeo;
int	lh2app3secs;
int	lh2app3pause;   /* time to pause b4 testing begins */
int	lh2app3crit;



char	lh3name[128];           /* logical hostname */
char	lh3path[128];       /* basedir */
int	lh3node;            /* node 0 or 1*/
int	lh3fallback;        /* node to fallback to */
int	lh3failto;        /* node to fail to */
int	lh3stat;            /* 0=served-by-node0 2=served-by-node1 1=down 3=failed 4=going-down 5=going-up*/
int	lh3timeo;		/* secs since transition began */
int	lh3failing;		/* is this failing */
char	lh3app0name[128];       /* app name */
int	lh3app0stat;        /* 0=up 2=maint 1=down 3=failed */
char	lh3app0start[128];  /* start script */
char	lh3app0stop[128];   /* stop script */
char	lh3app0test[128];   /* test script */
int	lh3app0testres;     /* test script result*/
char	lh3app0testtim[20]; /* test script time stamp upon return*/
char    lh3app0remtest[128];   /* remote test script */
int     lh3app0remtestres;     /* test script result*/
int     lh3app0remtesttimeo; /* test script time stamp upon return*/
int	lh3app0remnode;
int	lh3app0timeo;  /* script timeout to fail 0=never-timeout */
int	lh3app0testtimeo;  /* script timeout to fail 0=never-timeout */
int	lh3app0secs;   /* time passed since last test */
int	lh3app0pause;   /* time to pause b4 testing begins */
int	lh3app0crit;   /* number of fails b4 failover 0=never-failover */
char	lh3app1name[128];
int	lh3app1stat;
char	lh3app1start[128];
char	lh3app1stop[128];
char	lh3app1test[128];
int	lh3app1testres;     /* test script result*/
char	lh3app1testtim[20]; /* test script time stamp upon return*/
char    lh3app1remtest[128];   /* remote test script */
int     lh3app1remtestres;     /* test script result*/
int     lh3app1remtesttimeo; /* test script time stamp upon return*/
int	lh3app1remnode;
int	lh3app1timeo;
int	lh3app1testtimeo;
int	lh3app1secs;
int	lh3app1pause;   /* time to pause b4 testing begins */
int	lh3app1crit;
char	lh3app2name[128];
int	lh3app2stat;
char	lh3app2start[128];
char	lh3app2stop[128];
char	lh3app2test[128];
int	lh3app2testres;     /* test script result*/
char	lh3app2testtim[20]; /* test script time stamp upon return*/
char    lh3app2remtest[128];   /* remote test script */
int     lh3app2remtestres;     /* test script result*/
int     lh3app2remtesttimeo; /* test script time stamp upon return*/
int	lh3app2remnode;
int	lh3app2timeo;
int	lh3app2testtimeo;
int	lh3app2secs;
int	lh3app2pause;   /* time to pause b4 testing begins */
int	lh3app2crit;
char	lh3app3name[128];
int	lh3app3stat;
char	lh3app3start[128];
char	lh3app3stop[128];
char	lh3app3test[128];
int	lh3app3testres;     /* test script result*/
char	lh3app3testtim[20]; /* test script time stamp upon return*/
char    lh3app3remtest[128];   /* remote test script */
int     lh3app3remtestres;     /* test script result*/
int     lh3app3remtesttimeo; /* test script time stamp upon return*/
int	lh3app3remnode;
int	lh3app3timeo;
int	lh3app3testtimeo;
int	lh3app3secs;
int	lh3app3pause;   /* time to pause b4 testing begins */
int	lh3app3crit;



};

struct cstates *shm;

int SHMSZ=sizeof(struct cstates);

FILE *confilep;
char fsthaprogname[128]="";
char syslogfac[128]="daemon.notice";
char msg[1024];
pid_t   mypid;



void logit(char *s) {

        time_t now;
        char st[BUFSIZ];

	sprintf(st,"/usr/bin/logger -t %d:%s -p %s %s",shmid,fsthaprogname,syslogfac,s);
	system(st);
}

void die(char *s) {
        time_t now;
        char st[BUFSIZ];

        time(&now);
        strcpy(st,ctime(&now));
        st[strlen(st)-1]=0;
        fprintf(stderr, "%s: %s (%d): %s\n",st,fsthaprogname,mypid,s);
	fflush(stderr);
        exit(1);
}



get_shm(int c) {
	char msg[128];

	/*
 	* get shared memory segment
 	*/
	if (c==0) {
		while ((shmid = shmget(shmkey,SHMSZ,IPC_ALLOC|0666)) <0 ) {
        	}
	}
	else
		shmid = shmget(shmkey,SHMSZ,IPC_ALLOC|0666);

	sleep(1);
	/*
 	* attach shm
 	*/
	if ((shm = (struct cstates *)shmat(shmid, NULL, 0))
		== (struct cstates *) -1) {
        	fprintf(stderr,"Shared Memory Segment Not Previously Attached.\n");
        	sprintf(msg,"logger -p %s Shared Memory Segment Not Previously Attached.",syslogfac);
		system(msg);
		return(1);
	}
	if ((semid = semget('i'+'n'+'t',nsems,0666)) < 0 )  {
                fprintf(stderr,"Failed to Create Semaphore\n");
                sprintf(msg,"logger -p %s Failed to Create Semaphore",syslogfac);
                system(msg);
                return(1);
        }
/****************
	if ((semid_tcp = semget('t'+'c'+'p',nsems,0666)) < 0 )  {
                fprintf(stderr,"Failed to Create Semaphore\n");
                sprintf(msg,"logger -p %s Failed to Create Semaphore",syslogfac);
                system(msg);
                return(1);
        }
*****************/

	return(0);
}


create_shm() {
	char msg[128];

	printf("Checking Instance State.\n");
	if (get_shm(1) == 0) {
		fprintf(stderr,"Cluster Node Already Initialized. Command Aborted.\n");
        	sprintf(msg,"logger -p %s Cluster Node Already Initialized. Command Aborted.",syslogfac);
		system(msg);
		return(1);
	}
	/*
 	* create shared memory segment
 	*/
	if ((shmid = shmget(shmkey,SHMSZ,IPC_CREAT|0666)) <0 )  {
        	fprintf(stderr,"Failed to Create Shared Memory Segment\n");
        	sprintf(msg,"logger -p %s Failed to Create Shared Memory Segment",syslogfac);
		system(msg);
		return(1);
	}
	/*
 	* attach shm
 	*/
	if ((shm = (struct cstates *)shmat(shmid, NULL, 0)) == (struct cstates *) -1)  {
        	fprintf(stderr,"Failed to Attach Shared Memory Segment\n");
        	sprintf(msg,"logger -p %s Failed to Attach Shared Memory Segment",syslogfac);
		system(msg);
		return(1);
	}
	printf("Shared Memory %d OK.\n",shmid);
        /*
        * create semaphore
        */
        if ((semid = semget('i'+'n'+'t',nsems,semflag)) < 0 )  {
                fprintf(stderr,"Failed to Create Semaphore\n");
                sprintf(msg,"logger -p %s Failed to Create Semaphore",syslogfac);
                system(msg);
                return(1);
        }
        printf("Semaphore %d OK.\n",semid);
/**********************
        if ((semid_tcp = semget('t'+'c'+'p',nsems,semflag)) < 0 )  {
                fprintf(stderr,"Failed to Create Semaphore\n");
                sprintf(msg,"logger -p %s Failed to Create Semaphore",syslogfac);
                system(msg);
                return(1);
        }
        printf("Semaphore %d OK.\n",semid_tcp);
******************/

	return(0);
}

int reserve_sem() {
nops=2;
sops[0].sem_num=0;
sops[0].sem_op=0; /*wait for semaphore flag to become zero*/
sops[0].sem_flg=SEM_UNDO; /* take off semaphore asynchronous */
sops[1].sem_num=0;
sops[1].sem_op=1; /* increment semaphore RESERVE IT*/
sops[1].sem_flg=SEM_UNDO|IPC_NOWAIT; /* take off semaphore */
if (semop(semid, sops, nops) == -1) {
        return(1);
}
else
        return(0);
}
int release_sem() {
nops=1;
sops[0].sem_num=0;
sops[0].sem_op=-1; /* decrement semaphore RELEASE IT*/
sops[0].sem_flg=SEM_UNDO|IPC_NOWAIT; /* take off semaphore */
if (semop(semid, sops, nops) == -1) {
        fprintf(stderr,"Failed to Release Semaphore\n");
        sprintf(msg,"logger -p %s Failed to Release Semaphore",syslogfac);
        system(msg);
        return(1);
}
else
        return(0);
}


remove_shm() {
shmctl(shmid,IPC_RMID,&shmbuf);
semctl(semid,0,IPC_RMID);
}

update_conf (char *s) {
	char dummy[128]="";
	int x=-1;
	
	while(reserve_sem() != 0);
        if (strncmp(s,"unfreeze ",9) == 0 ) {
                sscanf(s,"%s %d",dummy,&(shm->unfreeze));
		if (shm->unfreeze != YES)
			logit("FROZEN");
		else
			logit("THAWED");
        }

	if (shm->unfreeze != YES) {
		release_sem();
		return 0;
	}

                if (strncmp(s,"port ",5) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->port);
                        sprintf(dummy,"port = %s",shm->port);
                        if (shm->verbose == YES) logit(dummy);
                        }
                else if (strncmp(s,"hbport ",7) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->hbport);
                        sprintf(dummy,"hbport = %s",shm->hbport);
                        if (shm->verbose == YES) logit(dummy);
                        }
                else if (strncmp(s,"reserve_disk ",13) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->reserve_disk);
                        sprintf(dummy,"reserve_disk = %s",shm->reserve_disk);
                        if (shm->verbose == YES) logit(dummy);
                        }
                else if (strncmp(s,"qnode ",6) == 0 ) {
                        sscanf(s,"%s %d",dummy,&x);
			if (x != shm->qnode) {
				sprintf(dummy,"Node %d Now Has Disk Reserved",x);
				logit(dummy);
			}
			shm->qnode=x;
                        }
                else if (strncmp(s,"insync ",7) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->insync));
                        }
                else if (strncmp(s,"other_strict ",13) == 0 )  {
                        sscanf(s,"%s %d",dummy,&(shm->other_strict));
			}
                else if (strncmp(s,"syslogfac ",10) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->syslogfac);
                        strcpy(syslogfac,shm->syslogfac);
                        sprintf(dummy,"syslogfac = %s",shm->syslogfac);
                        if (shm->verbose == YES) logit(dummy);
                        }
                else if (strncmp(s,"dirty ",6) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->dirty));
                        }
                else if (strncmp(s,"fsthbb_pid ",11) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->fsthbb_pid));
                        }
                else if (strncmp(s,"fsthbm_pid ",11) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->fsthbm_pid));
                        }
                else if (strncmp(s,"fstasm_pid ",11) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->fstasm_pid));
                        }
                else if (strncmp(s,"fstsvr_pid ",11) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->fstsvr_pid));
                        }
                else if (strncmp(s,"fsthbserv_pid ",14) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->fsthbserv_pid));
                        }
                else if (strncmp(s,"fstastlh0app0_pid ",18) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->fstastlh0app0_pid));
                        }
                else if (strncmp(s,"fstastlh0app1_pid ",18) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->fstastlh0app1_pid));
                        }
                else if (strncmp(s,"fstastlh0app2_pid ",18) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->fstastlh0app2_pid));
                        }
                else if (strncmp(s,"fstastlh0app3_pid ",18) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->fstastlh0app3_pid));
                        }
                else if (strncmp(s,"fstastlh1app0_pid ",18) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->fstastlh1app0_pid));
                        }
                else if (strncmp(s,"fstastlh1app1_pid ",18) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->fstastlh1app1_pid));
                        }
                else if (strncmp(s,"fstastlh1app2_pid ",18) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->fstastlh1app2_pid));
                        }
                else if (strncmp(s,"fstastlh1app3_pid ",18) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->fstastlh1app3_pid));
                        }
                else if (strncmp(s,"fstastlh2app0_pid ",18) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->fstastlh2app0_pid));
                        }
                else if (strncmp(s,"fstastlh2app1_pid ",18) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->fstastlh2app1_pid));
                        }
                else if (strncmp(s,"fstastlh2app2_pid ",18) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->fstastlh2app2_pid));
                        }
                else if (strncmp(s,"fstastlh2app3_pid ",18) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->fstastlh2app3_pid));
                        }
                else if (strncmp(s,"fstastlh3app0_pid ",18) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->fstastlh3app0_pid));
                        }
                else if (strncmp(s,"fstastlh3app1_pid ",18) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->fstastlh3app1_pid));
                        }
                else if (strncmp(s,"fstastlh3app2_pid ",18) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->fstastlh3app2_pid));
                        }
                else if (strncmp(s,"fstastlh3app3_pid ",18) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->fstastlh3app3_pid));
                        }
                else if (strncmp(s,"reserve_strict ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->reserve_strict));
                        }
                else if (strncmp(s,"backup ",7) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->backup));
                        }
                else if (strncmp(s,"logfile ",8) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->logfile);
                        sprintf(dummy,"logfile = %s",shm->logfile);
                        if (shm->verbose == YES) logit(dummy);
                        }
                else if (strncmp(s,"cstat ",6) == 0 ) {
			if (shm->cstat == WAIT) 
				shm->verbose = prvverbose;
                        sscanf(s,"%s %d",
				dummy,&(shm->cstat));
                        sprintf(dummy,"cstat = %d",
				shm->cstat);
                        if (shm->verbose == YES) logit(dummy);
                        }
                else if (strncmp(s,"verbose ",8) == 0 ) {
                        sscanf(s,"%s %d",
				dummy,&(shm->verbose));
                        sprintf(dummy,"verbose = %d",
				shm->verbose);
                        if (shm->verbose == YES) logit(dummy);
                        }
                else if (strncmp(s,"passwd ",7) == 0 ) {
                        sscanf(s,"%s %s",
				dummy,&(shm->passwd));
                        sprintf(dummy,"passwd = %s",
				shm->passwd);
                        if (shm->verbose == YES) logit(dummy);
                        }
                else if (strncmp(s,"hbtimeout ",10) == 0 ) {
                        sscanf(s,"%s %d",
				dummy,&(shm->hbtimeout));
                        sprintf(dummy,"hbtimeout = %d",
				shm->hbtimeout);
                        if (shm->verbose == YES) logit(dummy);
                        }
                else if (strncmp(s,"hbdelay ",8) == 0 ) {
                        sscanf(s,"%s %d",
				dummy,&(shm->hbdelay));
                        sprintf(dummy,"hbdelay = %d",
				shm->hbdelay);
                        if (shm->verbose == YES) logit(dummy);
                        }
                else if (strncmp(s,"version ",8) == 0 ) {
                        sscanf(s,"%s %d",
				dummy,&(shm->version));
                        sprintf(dummy,"version = %d",
				shm->version);
                        if (shm->verbose == YES) logit(dummy);
                        }
                else if (strncmp(s,"htmlrefreshsecs ",16) == 0 ) {
                        sscanf(s,"%s %d",
				dummy,&(shm->htmlrefreshsecs));
                        sprintf(dummy,"htmlrefreshsecs = %d",
				shm->htmlrefreshsecs);
                        if (shm->verbose == YES) logit(dummy);
                        }
                else if (strncmp(s,"htmlallowexec ",8) == 0 ) {
                        sscanf(s,"%s %d",
				dummy,&(shm->htmlallowexec));
                        sprintf(dummy,"htmlallowexec = %d",
				shm->htmlallowexec);
                        if (shm->verbose == YES) logit(dummy);
                        }
/**
                else if (strncmp(s,"pbfreq ",7) == 0 ) {
                        sscanf(s,"%s %d ",
				dummy,&(shm->pbfreq));
                        sprintf(dummy,"pbfreq = %d",
				shm->pbfreq);
                        if (shm->verbose == YES) logit(dummy);
                        }
                else if (strncmp(s,"pbsecs ",7) == 0 ) {
                        sscanf(s,"%s %d ",
				dummy,&(shm->pbsecs));
                        sprintf(dummy,"pbsecs = %d",
				shm->pbsecs);
                        if (shm->verbose == YES) logit(dummy);
                        }
**/
                else if (strncmp(s,"pbtm0 ",6) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->pbtm0);
                        }
                else if (strncmp(s,"pbtm1 ",6) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->pbtm1);
                        }
                else if (strncmp(s,"pbtm2 ",6) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->pbtm2);
                        }
                else if (strncmp(s,"pbtm3 ",6) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->pbtm3);
                        }
                else if (strncmp(s,"hb0 ",4) == 0 ) {
                        sscanf(s,"%s %s %s %s %s",
				dummy,shm->hb0dv,shm->hb0ip,shm->hb0mask,shm->hb0bcast);
                        sprintf(dummy,"hb0 = %s on %s netmask %s bcast %s",
				shm->hb0ip,shm->hb0dv,shm->hb0mask,shm->hb0bcast);
                        if (shm->verbose == YES) logit(dummy);
                        }
                else if (strncmp(s,"hb0pg0 ",7) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->hb0pg0);
                        }
                else if (strncmp(s,"hb0pg1 ",7) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->hb0pg1);
                        }
                else if (strncmp(s,"hb0pg2 ",7) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->hb0pg2);
                        }
                else if (strncmp(s,"hb0pg3 ",7) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->hb0pg3);
                        }
                else if (strncmp(s,"hb0tm0 ",7) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->hb0tm0);
                        }
                else if (strncmp(s,"hb0tm1 ",7) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->hb0tm1);
                        }
                else if (strncmp(s,"hb0tm2 ",7) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->hb0tm2);
                        }
                else if (strncmp(s,"hb0tm3 ",7) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->hb0tm3);
                        }
                else if (strncmp(s,"hb0stat0 ",9) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->hb0stat0));
                        }
                else if (strncmp(s,"hb0stat1 ",9) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->hb0stat1));
                        }
                else if (strncmp(s,"hb0stat2 ",9) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->hb0stat2));
                        }
                else if (strncmp(s,"hb0stat3 ",9) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->hb0stat3));
                        }
                else if (strncmp(s,"hb1 ",4) == 0 ) {
                        sscanf(s,"%s %s %s %s %s",
				dummy,shm->hb1dv,shm->hb1ip,shm->hb1mask,shm->hb1bcast);
                        sprintf(dummy,"hb1 = %s  on %s netmask %s bcast %s",
				shm->hb1ip,shm->hb1dv,shm->hb1mask,shm->hb1bcast);
                        if (shm->verbose == YES) logit(dummy);
                        }
                else if (strncmp(s,"hb1pg0 ",7) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->hb1pg0);
                        }
                else if (strncmp(s,"hb1pg1 ",7) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->hb1pg1);
                        }
                else if (strncmp(s,"hb1pg2 ",7) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->hb1pg2);
                        }
                else if (strncmp(s,"hb1pg3 ",7) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->hb1pg3);
                        }
                else if (strncmp(s,"hb1tm0 ",7) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->hb1tm0);
                        }
                else if (strncmp(s,"hb1tm1 ",7) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->hb1tm1);
                        }
                else if (strncmp(s,"hb1tm2 ",7) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->hb1tm2);
                        }
                else if (strncmp(s,"hb1tm3 ",7) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->hb1tm3);
                        }
                else if (strncmp(s,"hb1stat0 ",9) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->hb1stat0));
                        }
                else if (strncmp(s,"hb1stat1 ",9) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->hb1stat1));
                        }
                else if (strncmp(s,"hb1stat2 ",9) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->hb1stat2));
                        }
                else if (strncmp(s,"hb1stat3 ",9) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->hb1stat3));
                        }
                else if (strncmp(s,"nodeN ",6) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->nodeN));
                        sprintf(dummy,"node = %d",shm->nodeN);
                        if (shm->verbose == YES) logit(dummy);
                        }
                else if (strncmp(s,"node0 ",6) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->node0);
                        sprintf(dummy,"node0 = %s",shm->node0);
                        if (shm->verbose == YES) logit(dummy);
                        }
                else if (strncmp(s,"node1 ",6) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->node1);
                        sprintf(dummy,"node1 = %s",shm->node1);
                        if (shm->verbose == YES) logit(dummy);
                        }
                else if (strncmp(s,"node2 ",6) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->node2);
                        sprintf(dummy,"node2 = %s",shm->node2);
                        if (shm->verbose == YES) logit(dummy);
                        }
                else if (strncmp(s,"node3 ",6) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->node3);
                        sprintf(dummy,"node3 = %s",shm->node3);
                        if (shm->verbose == YES) logit(dummy);
                        }
                else if (strncmp(s,"node0load ",10) == 0 ) {
                        sscanf(s,"%s %f",dummy,&(shm->node0load));
                        }
                else if (strncmp(s,"node1load ",10) == 0 ) {
                        sscanf(s,"%s %f",dummy,&(shm->node1load));
                        }
                else if (strncmp(s,"node2load ",10) == 0 ) {
                        sscanf(s,"%s %f",dummy,&(shm->node2load));
                        }
                else if (strncmp(s,"node3load ",10) == 0 ) {
                        sscanf(s,"%s %f",dummy,&(shm->node3load));
                        }
                else if (strncmp(s,"node0stat ",10) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->node0stat));
                        }
                else if (strncmp(s,"node1stat ",10) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->node1stat));
                        }
                else if (strncmp(s,"node2stat ",10) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->node2stat));
                        }
                else if (strncmp(s,"node3stat ",10) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->node3stat));
                        }
                else if (strncmp(s,"lhfailtimeo ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lhfailtimeo));
                        }
                else if (strncmp(s,"appfailtimeo ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->appfailtimeo));
                        }
                else if (strncmp(s,"appfailtesttimeo ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->appfailtesttimeo));
                        }




                else if (strncmp(s,"lh0name ",8) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh0name);
                        }
                else if (strncmp(s,"lh0path ",8) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh0path);
                        }
                else if (strncmp(s,"lh0node ",8) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0node));
                        }
                else if (strncmp(s,"lh0fallback ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0fallback));
                        }
                else if (strncmp(s,"lh0failto ",10) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0failto));
                        }
                else if (strncmp(s,"lh0stat ",8) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0stat));
			if (shm->lh0stat == UNUSED) {
				shm->lh0name[0]=0; 
				shm->lh0app0name[0]=0;
				shm->lh0app1name[0]=0;
				shm->lh0app2name[0]=0;
				shm->lh0app3name[0]=0;
			}
                        }
                else if (strncmp(s,"lh0timeo ",9) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0timeo));
                        }
                else if (strncmp(s,"lh0failing ",11) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0failing));
                        }
		/***********************/
                else if (strncmp(s,"lh0app0name ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh0app0name);
                        }
                else if (strncmp(s,"lh0app0stat ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app0stat));
			if (shm->lh0app0stat == UNUSED)
				shm->lh0app0name[0]=0;
                        }
                else if (strncmp(s,"lh0app0start ",13) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh0app0start);
                        }
                else if (strncmp(s,"lh0app0stop ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh0app0stop);
                        }
                else if (strncmp(s,"lh0app0test ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh0app0test);
                        }
                else if (strncmp(s,"lh0app0timeo ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app0timeo));
                        }
                else if (strncmp(s,"lh0app0remnode ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app0remnode));
                        }
                else if (strncmp(s,"lh0app0remtest ",15) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh0app0remtest);
                        }
                else if (strncmp(s,"lh0app0testtimeo ",17) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app0testtimeo));
                        }
                else if (strncmp(s,"lh0app0crit ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app0crit));
                        }
                else if (strncmp(s,"lh0app0secs ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app0secs));
                        }
                else if (strncmp(s,"lh0app0pause ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app0pause));
                        }
                else if (strncmp(s,"lh0app0testres ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app0testres));
                        }
		/***********************/
                else if (strncmp(s,"lh0app1name ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh0app1name);
                        }
                else if (strncmp(s,"lh0app1remnode ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app1remnode));
                        }
                else if (strncmp(s,"lh0app1remtest ",15) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh0app1remtest);
                        }
                else if (strncmp(s,"lh0app1stat ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app1stat));
			if (shm->lh0app1stat == UNUSED)
				shm->lh0app1name[0]=0;
                        }
                else if (strncmp(s,"lh0app1start ",13) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh0app1start);
                        }
                else if (strncmp(s,"lh0app1stop ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh0app1stop);
                        }
                else if (strncmp(s,"lh0app1test ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh0app1test);
                        }
                else if (strncmp(s,"lh0app1timeo ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app1timeo));
                        }
                else if (strncmp(s,"lh0app1testtimeo ",17) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app1testtimeo));
                        }
                else if (strncmp(s,"lh0app1crit ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app1crit));
                        }
                else if (strncmp(s,"lh0app1secs ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app1secs));
                        }
                else if (strncmp(s,"lh0app1pause ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app1pause));
                        }
                else if (strncmp(s,"lh0app1testres ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app1testres));
                        }
		/***********************/
                else if (strncmp(s,"lh0app2name ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh0app2name);
                        }
                else if (strncmp(s,"lh0app2remnode ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app2remnode));
                        }
                else if (strncmp(s,"lh0app2remtest ",15) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh0app2remtest);
                        }
                else if (strncmp(s,"lh0app2stat ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app2stat));
			if (shm->lh0app2stat == UNUSED)
				shm->lh0app2name[0]=0;
                        }
                else if (strncmp(s,"lh0app2start ",13) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh0app2start);
                        }
                else if (strncmp(s,"lh0app2stop ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh0app2stop);
                        }
                else if (strncmp(s,"lh0app2test ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh0app2test);
                        }
                else if (strncmp(s,"lh0app2timeo ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app2timeo));
                        }
                else if (strncmp(s,"lh0app2testtimeo ",17) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app2testtimeo));
                        }
                else if (strncmp(s,"lh0app2crit ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app2crit));
                        }
                else if (strncmp(s,"lh0app2secs ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app2secs));
                        }
                else if (strncmp(s,"lh0app2pause ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app2pause));
                        }
                else if (strncmp(s,"lh0app2testres ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app2testres));
                        }
		/***********************/
                else if (strncmp(s,"lh0app3name ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh0app3name);
                        }
                else if (strncmp(s,"lh0app3remnode ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app3remnode));
                        }
                else if (strncmp(s,"lh0app3remtest ",15) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh0app3remtest);
                        }
                else if (strncmp(s,"lh0app3stat ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app3stat));
			if (shm->lh0app3stat == UNUSED)
				shm->lh0app3name[0]=0;
                        }
                else if (strncmp(s,"lh0app3start ",13) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh0app3start);
                        }
                else if (strncmp(s,"lh0app3stop ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh0app3stop);
                        }
                else if (strncmp(s,"lh0app3test ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh0app3test);
                        }
                else if (strncmp(s,"lh0app3timeo ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app3timeo));
                        }
                else if (strncmp(s,"lh0app3testtimeo ",17) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app3testtimeo));
                        }
                else if (strncmp(s,"lh0app3crit ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app3crit));
                        }
                else if (strncmp(s,"lh0app3secs ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app3secs));
                        }
                else if (strncmp(s,"lh0app3pause ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app3pause));
                        }
                else if (strncmp(s,"lh0app3testres ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh0app3testres));
                        }
		/***********************/



                else if (strncmp(s,"lh1name ",8) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh1name);
                        }
                else if (strncmp(s,"lh1path ",8) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh1path);
                        }
                else if (strncmp(s,"lh1node ",8) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1node));
                        }
                else if (strncmp(s,"lh1fallback ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1fallback));
                        }
                else if (strncmp(s,"lh1failto ",10) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1failto));
                        }
                else if (strncmp(s,"lh1stat ",8) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1stat));
			if (shm->lh1stat == UNUSED) {
				shm->lh1name[0]=0; 
				shm->lh1app0name[0]=0;
				shm->lh1app1name[0]=0;
				shm->lh1app2name[0]=0;
				shm->lh1app3name[0]=0;
			}
                        }
                else if (strncmp(s,"lh1timeo ",9) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1timeo));
                        }
                else if (strncmp(s,"lh1failing ",11) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1failing));
                        }
		/***********************/
                else if (strncmp(s,"lh1app0name ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh1app0name);
                        }
                else if (strncmp(s,"lh1app0remnode ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app0remnode));
                        }
                else if (strncmp(s,"lh1app0remtest ",15) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh1app0remtest);
                        }
                else if (strncmp(s,"lh1app0stat ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app0stat));
			if (shm->lh1app0stat == UNUSED)
				shm->lh1app0name[0]=0;
                        }
                else if (strncmp(s,"lh1app0start ",13) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh1app0start);
                        }
                else if (strncmp(s,"lh1app0stop ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh1app0stop);
                        }
                else if (strncmp(s,"lh1app0test ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh1app0test);
                        }
                else if (strncmp(s,"lh1app0timeo ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app0timeo));
                        }
                else if (strncmp(s,"lh1app0testtimeo ",17) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app0testtimeo));
                        }
                else if (strncmp(s,"lh1app0crit ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app0crit));
                        }
                else if (strncmp(s,"lh1app0secs ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app0secs));
                        }
                else if (strncmp(s,"lh1app0pause ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app0pause));
                        }
                else if (strncmp(s,"lh1app0testres ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app0testres));
                        }
		/***********************/
                else if (strncmp(s,"lh1app1name ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh1app1name);
                        }
                else if (strncmp(s,"lh1app1remnode ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app1remnode));
                        }
                else if (strncmp(s,"lh1app1remtest ",15) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh1app1remtest);
                        }
                else if (strncmp(s,"lh1app1stat ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app1stat));
			if (shm->lh1app1stat == UNUSED)
				shm->lh1app1name[0]=0;
                        }
                else if (strncmp(s,"lh1app1start ",13) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh1app1start);
                        }
                else if (strncmp(s,"lh1app1stop ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh1app1stop);
                        }
                else if (strncmp(s,"lh1app1test ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh1app1test);
                        }
                else if (strncmp(s,"lh1app1timeo ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app1timeo));
                        }
                else if (strncmp(s,"lh1app1testtimeo ",17) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app1testtimeo));
                        }
                else if (strncmp(s,"lh1app1crit ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app1crit));
                        }
                else if (strncmp(s,"lh1app1secs ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app1secs));
                        }
                else if (strncmp(s,"lh1app1pause ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app1pause));
                        }
                else if (strncmp(s,"lh1app1testres ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app1testres));
                        }
		/***********************/
                else if (strncmp(s,"lh1app2name ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh1app2name);
                        }
                else if (strncmp(s,"lh1app2remnode ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app2remnode));
                        }
                else if (strncmp(s,"lh1app2remtest ",15) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh1app2remtest);
                        }
                else if (strncmp(s,"lh1app2stat ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app2stat));
			if (shm->lh1app2stat == UNUSED)
				shm->lh1app2name[0]=0;
                        }
                else if (strncmp(s,"lh1app2start ",13) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh1app2start);
                        }
                else if (strncmp(s,"lh1app2stop ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh1app2stop);
                        }
                else if (strncmp(s,"lh1app2test ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh1app2test);
                        }
                else if (strncmp(s,"lh1app2timeo ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app2timeo));
                        }
                else if (strncmp(s,"lh1app2testtimeo ",17) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app2testtimeo));
                        }
                else if (strncmp(s,"lh1app2crit ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app2crit));
                        }
                else if (strncmp(s,"lh1app2secs ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app2secs));
                        }
                else if (strncmp(s,"lh1app2pause ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app2pause));
                        }
                else if (strncmp(s,"lh1app2testres ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app2testres));
                        }
		/***********************/
                else if (strncmp(s,"lh1app3name ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh1app3name);
                        }
                else if (strncmp(s,"lh1app3remnode ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app3remnode));
                        }
                else if (strncmp(s,"lh1app3remtest ",15) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh1app3remtest);
                        }
                else if (strncmp(s,"lh1app3stat ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app3stat));
			if (shm->lh1app3stat == UNUSED)
				shm->lh1app3name[0]=0;
                        }
                else if (strncmp(s,"lh1app3start ",13) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh1app3start);
                        }
                else if (strncmp(s,"lh1app3stop ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh1app3stop);
                        }
                else if (strncmp(s,"lh1app3test ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh1app3test);
                        }
                else if (strncmp(s,"lh1app3timeo ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app3timeo));
                        }
                else if (strncmp(s,"lh1app3testtimeo ",17) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app3testtimeo));
                        }
                else if (strncmp(s,"lh1app3crit ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app3crit));
                        }
                else if (strncmp(s,"lh1app3secs ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app3secs));
                        }
                else if (strncmp(s,"lh1app3pause ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app3pause));
                        }
                else if (strncmp(s,"lh1app3testres ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh1app3testres));
                        }
		/***********************/



                else if (strncmp(s,"lh2name ",8) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh2name);
                        }
                else if (strncmp(s,"lh2path ",8) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh2path);
                        }
                else if (strncmp(s,"lh2node ",8) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2node));
                        }
                else if (strncmp(s,"lh2fallback ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2fallback));
                        }
                else if (strncmp(s,"lh2failto ",10) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2failto));
                        }
                else if (strncmp(s,"lh2stat ",8) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2stat));
			if (shm->lh2stat == UNUSED) {
				shm->lh2name[0]=0; 
				shm->lh2app0name[0]=0;
				shm->lh2app1name[0]=0;
				shm->lh2app2name[0]=0;
				shm->lh2app3name[0]=0;
			}
                        }
                else if (strncmp(s,"lh2timeo ",9) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2timeo));
                        }
                else if (strncmp(s,"lh2failing ",11) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2failing));
                        }
		/***********************/
                else if (strncmp(s,"lh2app0name ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh2app0name);
                        }
                else if (strncmp(s,"lh2app0remnode ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app0remnode));
                        }
                else if (strncmp(s,"lh2app0remtest ",15) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh2app0remtest);
                        }
                else if (strncmp(s,"lh2app0stat ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app0stat));
			if (shm->lh2app0stat == UNUSED)
				shm->lh2app0name[0]=0;
                        }
                else if (strncmp(s,"lh2app0start ",13) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh2app0start);
                        }
                else if (strncmp(s,"lh2app0stop ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh2app0stop);
                        }
                else if (strncmp(s,"lh2app0test ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh2app0test);
                        }
                else if (strncmp(s,"lh2app0timeo ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app0timeo));
                        }
                else if (strncmp(s,"lh2app0testtimeo ",17) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app0testtimeo));
                        }
                else if (strncmp(s,"lh2app0crit ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app0crit));
                        }
                else if (strncmp(s,"lh2app0secs ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app0secs));
                        }
                else if (strncmp(s,"lh2app0pause ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app0pause));
                        }
                else if (strncmp(s,"lh2app0testres ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app0testres));
                        }
		/***********************/
                else if (strncmp(s,"lh2app1name ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh2app1name);
                        }
                else if (strncmp(s,"lh2app1remnode ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app1remnode));
                        }
                else if (strncmp(s,"lh2app1remtest ",15) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh2app1remtest);
                        }
                else if (strncmp(s,"lh2app1stat ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app1stat));
			if (shm->lh2app1stat == UNUSED)
				shm->lh2app1name[0]=0;
                        }
                else if (strncmp(s,"lh2app1start ",13) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh2app1start);
                        }
                else if (strncmp(s,"lh2app1stop ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh2app1stop);
                        }
                else if (strncmp(s,"lh2app1test ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh2app1test);
                        }
                else if (strncmp(s,"lh2app1timeo ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app1timeo));
                        }
                else if (strncmp(s,"lh2app1testtimeo ",17) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app1testtimeo));
                        }
                else if (strncmp(s,"lh2app1crit ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app1crit));
                        }
                else if (strncmp(s,"lh2app1secs ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app1secs));
                        }
                else if (strncmp(s,"lh2app1pause ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app1pause));
                        }
                else if (strncmp(s,"lh2app1testres ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app1testres));
                        }
		/***********************/
                else if (strncmp(s,"lh2app2name ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh2app2name);
                        }
                else if (strncmp(s,"lh2app2remnode ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app2remnode));
                        }
                else if (strncmp(s,"lh2app2remtest ",15) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh2app2remtest);
                        }
                else if (strncmp(s,"lh2app2stat ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app2stat));
			if (shm->lh2app2stat == UNUSED)
				shm->lh2app2name[0]=0;
                        }
                else if (strncmp(s,"lh2app2start ",13) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh2app2start);
                        }
                else if (strncmp(s,"lh2app2stop ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh2app2stop);
                        }
                else if (strncmp(s,"lh2app2test ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh2app2test);
                        }
                else if (strncmp(s,"lh2app2timeo ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app2timeo));
                        }
                else if (strncmp(s,"lh2app2testtimeo ",17) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app2testtimeo));
                        }
                else if (strncmp(s,"lh2app2crit ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app2crit));
                        }
                else if (strncmp(s,"lh2app2secs ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app2secs));
                        }
                else if (strncmp(s,"lh2app2pause ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app2pause));
                        }
                else if (strncmp(s,"lh2app2testres ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app2testres));
                        }
		/***********************/
                else if (strncmp(s,"lh2app3name ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh2app3name);
                        }
                else if (strncmp(s,"lh2app3remnode ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app3remnode));
                        }
                else if (strncmp(s,"lh2app3remtest ",15) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh2app3remtest);
                        }
                else if (strncmp(s,"lh2app3stat ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app3stat));
			if (shm->lh2app3stat == UNUSED)
				shm->lh2app3name[0]=0;
                        }
                else if (strncmp(s,"lh2app3start ",13) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh2app3start);
                        }
                else if (strncmp(s,"lh2app3stop ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh2app3stop);
                        }
                else if (strncmp(s,"lh2app3test ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh2app3test);
                        }
                else if (strncmp(s,"lh2app3timeo ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app3timeo));
                        }
                else if (strncmp(s,"lh2app3testtimeo ",17) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app3testtimeo));
                        }
                else if (strncmp(s,"lh2app3crit ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app3crit));
                        }
                else if (strncmp(s,"lh2app3secs ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app3secs));
                        }
                else if (strncmp(s,"lh2app3pause ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app3pause));
                        }
                else if (strncmp(s,"lh2app3testres ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh2app3testres));
                        }
		/***********************/



                else if (strncmp(s,"lh3name ",8) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh3name);
                        }
                else if (strncmp(s,"lh3path ",8) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh3path);
                        }
                else if (strncmp(s,"lh3node ",8) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3node));
                        }
                else if (strncmp(s,"lh3fallback ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3fallback));
                        }
                else if (strncmp(s,"lh3failto ",10) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3failto));
                        }
                else if (strncmp(s,"lh3stat ",8) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3stat));
			if (shm->lh3stat == UNUSED) {
				shm->lh3name[0]=0; 
				shm->lh3app0name[0]=0;
				shm->lh3app1name[0]=0;
				shm->lh3app2name[0]=0;
				shm->lh3app3name[0]=0;
			}
                        }
                else if (strncmp(s,"lh3timeo ",9) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3timeo));
                        }
                else if (strncmp(s,"lh3failing ",11) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3failing));
                        }
		/***********************/
                else if (strncmp(s,"lh3app0name ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh3app0name);
                        }
                else if (strncmp(s,"lh3app0remnode ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app0remnode));
                        }
                else if (strncmp(s,"lh3app0remtest ",15) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh3app0remtest);
                        }
                else if (strncmp(s,"lh3app0stat ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app0stat));
			if (shm->lh3app0stat == UNUSED)
				shm->lh3app0name[0]=0;
                        }
                else if (strncmp(s,"lh3app0start ",13) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh3app0start);
                        }
                else if (strncmp(s,"lh3app0stop ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh3app0stop);
                        }
                else if (strncmp(s,"lh3app0test ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh3app0test);
                        }
                else if (strncmp(s,"lh3app0timeo ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app0timeo));
                        }
                else if (strncmp(s,"lh3app0testtimeo ",17) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app0testtimeo));
                        }
                else if (strncmp(s,"lh3app0crit ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app0crit));
                        }
                else if (strncmp(s,"lh3app0secs ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app0secs));
                        }
                else if (strncmp(s,"lh3app0pause ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app0pause));
                        }
                else if (strncmp(s,"lh3app0testres ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app0testres));
                        }
		/***********************/
                else if (strncmp(s,"lh3app1name ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh3app1name);
                        }
                else if (strncmp(s,"lh3app1remnode ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app1remnode));
                        }
                else if (strncmp(s,"lh3app1remtest ",15) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh3app1remtest);
                        }
                else if (strncmp(s,"lh3app1stat ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app1stat));
			if (shm->lh3app1stat == UNUSED)
				shm->lh3app1name[0]=0;
                        }
                else if (strncmp(s,"lh3app1start ",13) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh3app1start);
                        }
                else if (strncmp(s,"lh3app1stop ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh3app1stop);
                        }
                else if (strncmp(s,"lh3app1test ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh3app1test);
                        }
                else if (strncmp(s,"lh3app1timeo ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app1timeo));
                        }
                else if (strncmp(s,"lh3app1testtimeo ",17) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app1testtimeo));
                        }
                else if (strncmp(s,"lh3app1crit ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app1crit));
                        }
                else if (strncmp(s,"lh3app1secs ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app1secs));
                        }
                else if (strncmp(s,"lh3app1pause ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app1pause));
                        }
                else if (strncmp(s,"lh3app1testres ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app1testres));
                        }
		/***********************/
                else if (strncmp(s,"lh3app2name ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh3app2name);
                        }
                else if (strncmp(s,"lh3app2remnode ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app2remnode));
                        }
                else if (strncmp(s,"lh3app2remtest ",15) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh3app2remtest);
                        }
                else if (strncmp(s,"lh3app2stat ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app2stat));
			if (shm->lh3app2stat == UNUSED)
				shm->lh3app2name[0]=0;
                        }
                else if (strncmp(s,"lh3app2start ",13) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh3app2start);
                        }
                else if (strncmp(s,"lh3app2stop ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh3app2stop);
                        }
                else if (strncmp(s,"lh3app2test ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh3app2test);
                        }
                else if (strncmp(s,"lh3app2timeo ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app2timeo));
                        }
                else if (strncmp(s,"lh3app2testtimeo ",17) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app2testtimeo));
                        }
                else if (strncmp(s,"lh3app2crit ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app2crit));
                        }
                else if (strncmp(s,"lh3app2secs ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app2secs));
                        }
                else if (strncmp(s,"lh3app2pause ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app2pause));
                        }
                else if (strncmp(s,"lh3app2testres ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app2testres));
                        }
		/***********************/
                else if (strncmp(s,"lh3app3name ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh3app3name);
                        }
                else if (strncmp(s,"lh3app3remnode ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app3remnode));
                        }
                else if (strncmp(s,"lh3app3remtest ",15) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh3app3remtest);
                        }
                else if (strncmp(s,"lh3app3stat ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app3stat));
			if (shm->lh3app3stat == UNUSED)
				shm->lh3app3name[0]=0;
                        }
                else if (strncmp(s,"lh3app3start ",13) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh3app3start);
                        }
                else if (strncmp(s,"lh3app3stop ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh3app3stop);
                        }
                else if (strncmp(s,"lh3app3test ",12) == 0 ) {
                        sscanf(s,"%s %s",dummy,shm->lh3app3test);
                        }
                else if (strncmp(s,"lh3app3timeo ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app3timeo));
                        }
                else if (strncmp(s,"lh3app3testtimeo ",17) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app3testtimeo));
                        }
                else if (strncmp(s,"lh3app3crit ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app3crit));
                        }
                else if (strncmp(s,"lh3app3secs ",12) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app3secs));
                        }
                else if (strncmp(s,"lh3app3pause ",13) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app3pause));
                        }
                else if (strncmp(s,"lh3app3testres ",15) == 0 ) {
                        sscanf(s,"%s %d",dummy,&(shm->lh3app3testres));
                        }
		/***********************/





	release_sem();

	if (dummy[0] == 0) 
		return 1;
	else
		return 0;
}


int fsthahere(char *msg) {
update_conf(msg);
}


int isrunning(char *s) {
char fname[128]="";
int fd,pid=0;


sleep(1);
if (strcmp(s,"fsthbm") == 0) 
	pid=shm->fsthbm_pid;
else if (strcmp(s,"fsthbserv") == 0)
	pid=shm->fsthbserv_pid;
else if (strcmp(s,"fsthbb") == 0)
	pid=shm->fsthbb_pid;
else if (strcmp(s,"fstsvr") == 0)
	pid=shm->fstsvr_pid;
else if (strcmp(s,"fstasm") == 0)
	pid=shm->fstasm_pid;
else if (strcmp(s,"fstast lh0app0") == 0)
	pid=shm->fstastlh0app0_pid;
else if (strcmp(s,"fstast lh0app1") == 0)
	pid=shm->fstastlh0app1_pid;
else if (strcmp(s,"fstast lh0app2") == 0)
	pid=shm->fstastlh0app2_pid;
else if (strcmp(s,"fstast lh0app3") == 0)
	pid=shm->fstastlh0app3_pid;
else if (strcmp(s,"fstast lh1app0") == 0)
	pid=shm->fstastlh1app0_pid;
else if (strcmp(s,"fstast lh1app1") == 0)
	pid=shm->fstastlh1app1_pid;
else if (strcmp(s,"fstast lh1app2") == 0)
	pid=shm->fstastlh1app2_pid;
else if (strcmp(s,"fstast lh1app3") == 0)
	pid=shm->fstastlh1app3_pid;
else if (strcmp(s,"fstast lh2app0") == 0)
	pid=shm->fstastlh2app0_pid;
else if (strcmp(s,"fstast lh2app1") == 0)
	pid=shm->fstastlh2app1_pid;
else if (strcmp(s,"fstast lh2app2") == 0)
	pid=shm->fstastlh2app2_pid;
else if (strcmp(s,"fstast lh2app3") == 0)
	pid=shm->fstastlh2app3_pid;
else if (strcmp(s,"fstast lh3app0") == 0)
	pid=shm->fstastlh3app0_pid;
else if (strcmp(s,"fstast lh3app1") == 0)
	pid=shm->fstastlh3app1_pid;
else if (strcmp(s,"fstast lh3app2") == 0)
	pid=shm->fstastlh3app2_pid;
else if (strcmp(s,"fstast lh3app3") == 0)
	pid=shm->fstastlh3app3_pid;
if (pid == 0)
	return(NO);

sprintf(fname,"/proc/%d/psinfo",pid);
if((fd=open(fname,O_RDONLY)) >= 0) {
	close(fd);
	return(YES);
}
else
	return(NO);

}


int fstkill(char *s) {
char msg[BUFSIZ]="";

sprintf(msg,"Killing %s",s);
if (shm->verbose == YES) logit(msg);
sprintf(msg,"/usr/bin/ps -ef|/usr/bin/grep -v grep|/usr/bin/grep '%s' |/usr/bin/awk '{print $2}'|/usr/bin/xargs kill > /dev/null 2>&1",s);
return (system(msg));
}


char *fstnodestr(int n) {
if (n == 0)
	return shm->node0;
else if ( n == 1)
	return shm->node1;
else if ( n == 2)
	return shm->node2;
else if ( n == 3)
	return shm->node3;
else if ( n == 8)
	return "Any";
else if ( n == 9)
	return "None";
else
	return "UNKNOWN";
}
char *fsthastat(int n) {
if (n == 0)
	return "UP";
else if ( n == 1)
	return "DOWN";
else if ( n == 2)
	return "REMOTE";
else if ( n == 3)
	return "FAILED";
else if ( n == 4)
	return "GOING-DOWN";
else if ( n == 5)
	return "COMING-UP";
else if ( n == 9)
	return "WAIT";
else
	return "UNKNOWN";
}
char *fstappstat(int n) {
if (n == 0)
	return "ON";
else if ( n == 1)
	return "OFF";
else if ( n == 3)
	return "FAILED";
else if ( n == 4)
	return "GOING-OFF";
else if ( n == 5)
	return "COMING-ON";
else if ( n == 8)
	return "NOCHECK";
else if ( n == 9)
	return "WAIT";
else
	return "UNKNOWN";
}



int do_fallback() {
	char dummy[128]="";

	if (shm->lh0name[0] != 0 &&
		(shm->lh0fallback == shm->nodeN ||
			(shm->lh0fallback == FALLANY && 
				(shm->lh0stat == DOWN || shm->lh0stat == FAIL)
		)	)	
	   ) {
		sprintf(dummy,"LH0 %s Falling Back.",shm->lh0name);
		logit(dummy);
		sprintf(dummy,"fstha locate 0 %d",shm->nodeN);
		system(dummy);
	}
	if (shm->lh1name[0] != 0 &&
		(shm->lh1fallback == shm->nodeN ||
			(shm->lh1fallback == FALLANY && 
				(shm->lh1stat == DOWN || shm->lh1stat == FAIL)
		)	)	
	   ) {
		sprintf(dummy,"LH1 %s Falling Back.",shm->lh1name);
		logit(dummy);
		sprintf(dummy,"fstha locate 1 %d",shm->nodeN);
		system(dummy);
	}
	if (shm->lh2name[0] != 0 &&
		(shm->lh2fallback == shm->nodeN ||
			(shm->lh2fallback == FALLANY && 
				(shm->lh2stat == DOWN || shm->lh2stat == FAIL)
		)	)	
	   ) {
		sprintf(dummy,"LH2 %s Falling Back.",shm->lh2name);
		logit(dummy);
		sprintf(dummy,"fstha locate 2 %d",shm->nodeN);
		system(dummy);
	}
	if (shm->lh3name[0] != 0 &&
		(shm->lh3fallback == shm->nodeN ||
			(shm->lh3fallback == FALLANY && 
				(shm->lh3stat == DOWN || shm->lh3stat == FAIL)
		)	)	
	   ) {
		sprintf(dummy,"LH3 %s Falling Back.",shm->lh3name);
		logit(dummy);
		sprintf(dummy,"fstha locate 3 %d",shm->nodeN);
		system(dummy);
	}
}

int reserve_disk() {
	int hdl,rcode;
	
	if (strcmp(shm->reserve_disk,"none") == 0) 
		return(0);
	hdl=open(shm->reserve_disk,O_NDELAY);
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
int reserve_disk_force() {
	int hdl,rcode;
	
	if (strcmp(shm->reserve_disk,"none") == 0) 
		return(0);
	hdl=open(shm->reserve_disk,O_NDELAY);
        if (hdl == -1) 
		return(-1);
	rcode=ioctl(hdl,MHIOCTKOWN);
	if (rcode != 0) {
		sleep(SCSI_TIMEOUT);
		rcode=ioctl(hdl,MHIOCTKOWN);
	}
	sleep(RESERVE_DELAY);
	rcode=ioctl(hdl,MHIOCQRESERVE);
	if (rcode == 0) {
		ioctl(hdl,MHIOCSTATUS);
		ioctl(hdl,MHIOCTKOWN);
	}
		
	close(hdl);
	return(rcode);
}
int release_disk() {
	int hdl,rcode;
	
	if (strcmp(shm->reserve_disk,"none") == 0) 
		return(0);
	hdl=open(shm->reserve_disk,O_NDELAY);
        if (hdl == -1) 
		return(-1);
	rcode=ioctl(hdl,MHIOCRELEASE);
	close(hdl);
	return(rcode);
}

int lowload(int thisNode) {

sleep(1);
if (shm->node0stat == UP && thisNode != 0 &&
        shm->node0load <= shm->node1load &&
        shm->node0load <= shm->node2load &&
        shm->node0load <= shm->node3load )
        return 0;
if (shm->node1stat == UP && thisNode != 1 &&
        shm->node1load <= shm->node0load &&
        shm->node1load <= shm->node2load &&
        shm->node1load <= shm->node3load )
        return 1;
if (shm->node2stat == UP && thisNode != 2 &&
        shm->node2load <= shm->node1load &&
        shm->node2load <= shm->node0load &&
        shm->node2load <= shm->node3load )
        return 2;
if (shm->node3stat == UP && thisNode != 3 &&
        shm->node3load <= shm->node1load &&
        shm->node3load <= shm->node2load &&
        shm->node3load <= shm->node0load )
        return 3;
return thisNode;
}

int isup(int vh) {
if (vh == 0 && shm->node0stat == UP )
        return UP;
if (vh == 1 && shm->node1stat == UP )
        return UP;
if (vh == 2 && shm->node2stat == UP )
        return UP;
if (vh == 3 && shm->node3stat == UP )
        return UP;
return DOWN;
}

int amalone() {
shm->alone=NO;
if (shm->nodeN == 0 &&
        shm->node1stat == DOWN &&
        shm->node2stat == DOWN &&
        shm->node3stat == DOWN)
        shm->alone=YES;
if (shm->nodeN == 1 &&
        shm->node0stat == DOWN &&
        shm->node2stat == DOWN &&
        shm->node3stat == DOWN)
        shm->alone=YES;
if (shm->nodeN == 2 &&
        shm->node1stat == DOWN &&
        shm->node0stat == DOWN &&
        shm->node3stat == DOWN)
        shm->alone=YES;
if (shm->nodeN == 3 &&
        shm->node1stat == DOWN &&
        shm->node2stat == DOWN &&
        shm->node0stat == DOWN)
        shm->alone=YES;
return shm->alone;
}

int amaloneip() {
if (shm->nodeN == 0) {
	sprintf(msg,"Checking %s",shm->hb0pg1);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb0pg1);
	if (system(msg) == 0)
		return 1;
	sprintf(msg,"Checking %s",shm->hb0pg2);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb0pg2);
	if (system(msg) == 0)
		return 1;
	sprintf(msg,"Checking %s",shm->hb0pg3);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb0pg3);
	if (system(msg) == 0)
		return 1;
	sprintf(msg,"Checking %s",shm->hb1pg1);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb1pg1);
	if (system(msg) == 0)
		return 1;
	sprintf(msg,"Checking %s",shm->hb1pg2);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb1pg2);
	if (system(msg) == 0)
		return 1;
	sprintf(msg,"Checking %s",shm->hb1pg3);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb1pg3);
	if (system(msg) == 0)
		return 1;

} else if (shm->nodeN == 1) {
	sprintf(msg,"Checking %s",shm->hb0pg0);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb0pg0);
	if (system(msg) == 0)
		return 1;
	sprintf(msg,"Checking %s",shm->hb0pg2);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb0pg2);
	if (system(msg) == 0)
		return 1;
	sprintf(msg,"Checking %s",shm->hb0pg3);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb0pg3);
	if (system(msg) == 0)
		return 1;
	sprintf(msg,"Checking %s",shm->hb1pg0);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb1pg0);
	if (system(msg) == 0)
		return 1;
	sprintf(msg,"Checking %s",shm->hb1pg2);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb1pg2);
	if (system(msg) == 0)
		return 1;
	sprintf(msg,"Checking %s",shm->hb1pg3);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb1pg3);
	if (system(msg) == 0)
		return 1;

} else if (shm->nodeN == 2) {
	sprintf(msg,"Checking %s",shm->hb0pg0);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb0pg0);
	if (system(msg) == 0)
		return 1;
	sprintf(msg,"Checking %s",shm->hb0pg1);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb0pg1);
	if (system(msg) == 0)
		return 1;
	sprintf(msg,"Checking %s",shm->hb0pg3);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb0pg3);
	if (system(msg) == 0)
		return 1;
	sprintf(msg,"Checking %s",shm->hb1pg0);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb1pg0);
	if (system(msg) == 0)
		return 1;
	sprintf(msg,"Checking %s",shm->hb1pg1);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb1pg1);
	if (system(msg) == 0)
		return 1;
	sprintf(msg,"Checking %s",shm->hb1pg3);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb1pg3);
	if (system(msg) == 0)
		return 1;

} else if (shm->nodeN == 3) {
	sprintf(msg,"Checking %s",shm->hb0pg0);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb0pg0);
	if (system(msg) == 0)
		return 1;
	sprintf(msg,"Checking %s",shm->hb0pg2);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb0pg2);
	if (system(msg) == 0)
		return 1;
	sprintf(msg,"Checking %s",shm->hb0pg1);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb0pg1);
	if (system(msg) == 0)
		return 1;
	sprintf(msg,"Checking %s",shm->hb1pg0);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb1pg0);
	if (system(msg) == 0)
		return 1;
	sprintf(msg,"Checking %s",shm->hb1pg2);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb1pg2);
	if (system(msg) == 0)
		return 1;
	sprintf(msg,"Checking %s",shm->hb1pg1);
	logit(msg);
	printf("%s\n",msg);
	sprintf(msg,"/usr/sbin/ping %s 2",shm->hb1pg1);
	if (system(msg) == 0)
		return 1;

}

return 0;
}

