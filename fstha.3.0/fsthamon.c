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
 * File Name: fsthamon.c
 *            Text based (curses) cluster operation tool.
 * 
 * File Authors:
 * 		Reggie Beavers (reggiebeavers@fstha.com)
 */
#include <curses.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <netdb.h>
#include <values.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <libgen.h>
#include "fstha.h"

#define MAXCOLS 79
#define MAXLH 4
#define MAXDS 4
#define MWy 7           /* x y of message window */
#define MWx 60
#define DEFAULT_SLEEP 1


void showtime(), getmessages();
void fsthatitle(), mydie(), showcmdline();
int fsthagethost, fsthamon();

/*
 * globals
 */

char nodename[MAXHOSTNAMELEN]="";
char appendlog[BUFSIZ]="";
char commandlog[BUFSIZ]="";
FILE *lf;
uid_t user,euser;
char username[80];
char currtime[80];
unsigned sleep_time=DEFAULT_SLEEP;
time_t prev_filetime=0;
int X=0;
char HAVERSION[BUFSIZ]="";

enum area { top, bottom } whichwindow=top;
WINDOW *lhw,*mgw,*dsw, *lgw, *csw;
struct lhline {
        char name[21];
        char mastered[21];
        char state[40];
	int  prev_currds;
	int  currds;
	int  numds;
	int  lhnum;
}; 
struct lhline logicalhost[MAXLH];
#define LHSIZE  ((21*3)+(sizeof(X)*4))    /* pure and simple laziness */

struct dsline {
        char name[21];
        char state[2];
	char basedir[BUFSIZ];
};
struct dsline dataservice[MAXDS*MAXLH];
#define DSSIZE  (21+2+BUFSIZ)            /* more laziness */

int prev_currlh=1,currlh=0;                     /* the current logical hosts */
short numlh=0;                                  /* number of logical hosts */
short prevnumlhs=0;                             /* number of logical hosts */



/* view only mode */
bool RO = FALSE;

void argerr(char *s,int x) {
        fprintf(stderr,"\n%s\n",s);
        exit(x);
}

/* get username */
char *usrname(char *username,int uid) {
        FILE *f;
        char s[BUFSIZ]="";
        char u[BUFSIZ]="";

        sprintf(u,":x:%d:",uid);
        f = fopen("/etc/passwd","r");
        if ( f == NULL )
                argerr("Can't open /etc/passwd",1);
        while (fgets(s,BUFSIZ,f) != NULL)
                if ( (char *) strstr(s,u) !=  NULL ) {
                        s[(int) strstr(s,u) - (int) s] = 0;
                        break;
                }
        fclose(f);
	strcpy(username,s);
        return (username);
}


main(int argc, char **argv) {

        short i;
        char s[BUFSIZ]="";
	FILE *f;
	time_t now;


	sprintf(s,"%s/fstha.version",ETCDIR);
	f=fopen(s,"r");
	if (f == NULL) {
		strcpy(HAVERSION,"http://www.fstha.com/donate");
	} else {
		fgets(HAVERSION,BUFSIZ,f);
		HAVERSION[strlen(HAVERSION)-1]=0;
	fclose(f);
	}
	strcpy(fsthaprogname,argv[0]);
	sprintf(s," 2>&1 | logger -p %s -t %s &",syslogfac,fsthaprogname);
	strcat(appendlog,s);

        sprintf(s,"%s   [ -v  -s <sleep time> -l <command-logfile> -n <nice value> -h ]",argv[0]);

        for ( i=1; i <argc; i++ ) {
                switch (argv[i][0]) {
                case '-': 
                        switch (argv[i][1]) {
                        case 's' :
                                if ( (i+1) <= argc )  {
                                	sleep_time = atoi(argv[++i]);
				}
                                else {
                                        argerr(s,1);
				}
                                break;
                        case 'h' :
                                argerr(s,0);
                                break;
                        case 'l':
                                if ( (i+1) <= argc )  {
					strcpy(commandlog,argv[++i]);
				}
                                else {
                                        argerr(s,1);
				}
				break;
                        case 'n':
                                if ( (i+1) <= argc )  {
                                        if (nice(atoi(argv[++i])) == -1) {
						argerr(s,1);
					}
				}
                                else {
                                        argerr(s,1);
				}
                                break;
                        case 'v':
				RO = TRUE;
                                break;
                        default: 
                                argerr(s,1);
                                break;
                        }
                        break;
                default: 
                        argerr(s,1);
                        break;
                }
        }
        signal(SIGINT, mydie);
	gethostname(nodename,MAXHOSTNAMELEN-2);
	nodename[MAXHOSTNAMELEN-1]=0;

	if ((shmid = shmget(shmkey,SHMSZ,IPC_ALLOC|0666)) <0 ) {
		fprintf(stderr,"Cluser Not Initialized.\n");
		exit(1);
        }
	else {
        	sleep(1);
		if ((shm = (struct cstates *)shmat(shmid, NULL, 0)) == (struct cstates *) -1)  {
			fprintf(stderr,"Cluser Not IN Known State.\n");
			exit(1);
		}
	}
        initscr();
	curs_set(0);

	user = getuid();
	euser = geteuid();
	usrname(username,user);
	if (commandlog[0] == 0 ) {
		now=time(NULL);
		strcpy(currtime,ctime(&now));
		currtime[strlen(currtime)-1]=0;
		for (i=0; currtime[i] != 0; i++ )
			if (currtime[i] == ' ')
				currtime[i] = '-';
//		sprintf(commandlog,"/var/tmp/fsthamon_%s.log",currtime);
		sprintf(commandlog,"/dev/null");
	}
	lf = fopen(commandlog,"a");
	if (lf == NULL) 
                argerr((char *)strcat("Can't open ",commandlog),1);


        fsthamon();
        mydie("Done.");
}

void mydie(char *s) {

        signal(SIGINT, SIG_IGN);
        mvcur(0,COLS -1, LINES - 1, 0);
        clrtobot();
	delwin(lhw); delwin(mgw); delwin(dsw); delwin(lgw);
        endwin();
        printf("\r\n%s\n",s);
        exit(0);
}

int fsthamon() {

        char c, s[BUFSIZ]="";
	char phyhost_data[36], *phyhost_ptr;
        char cmdline[BUFSIZ+BUFSIZ]="";
	int x;



        fsthatitle();
        lhw = subwin(stdscr,MAXLH+1,MAXCOLS,4,0);
        if ( lhw == NULL ) 
                mydie("Unable to create window for Virtualhosts. Try re-sizing your terminal to 80x24");


        dsw = subwin(stdscr,MAXDS+1,18,11,0);
        if ( dsw == NULL )
                mydie("Unable to create window for Appmonitoring. Try re-sizing your terminal to 80x24");

        mgw = subwin(stdscr,MWy,MWx,11,19);
        if ( mgw == NULL )
                mydie("Unable to create window for Messages. Try re-sizing your terminal to 80x24");
        scrollok(mgw,TRUE);

        lgw = subwin(stdscr,1,64,24,14);
        if ( lgw == NULL )
                mydie("Unable to create window for Log. Try re-sizing your terminal to 80x24");

        csw = subwin(stdscr,1,8,0,((MAXCOLS/2)-((strlen(nodename)+9)/2)+strlen(nodename)));
        if ( csw == NULL )
                mydie("Unable to create window for Cluster Status. Try re-sizing your terminal to 80x24");

	/* init ds holders */
	for (c=0; c <  MAXLH; c++) {
		logicalhost[c].currds = 0;
		logicalhost[c].prev_currds = 1;
	}
		


        crmode(); noecho(); nodelay(stdscr, TRUE);
        fsthagethosts();
	prevnumlhs = numlh;
        c = 0;
        while ( c != 'q' && c != 'Q' ) {

                c = getch();
		if ( prevnumlhs != numlh)
			c='q';
                switch (c) {

					/* arrow key movement  */
			case 27:
					switch (getch()) {
						case '[':
							switch (getch()) {
								case 'A':
                                        				if (whichwindow == top)
                                                				if ( currlh > 0 ) 
                                                        				currlh--;
                                                				else
                                                        				currlh = numlh;
                                        				else
                                                				if ( logicalhost[currlh].currds > 0 ) 
                                                        				logicalhost[currlh].currds--;
                                                				else
                                                        				logicalhost[currlh].currds = logicalhost[currlh].numds;
                                        				break;
								case 'B':
                                        				if (whichwindow == top)
                                                				if ( currlh < numlh ) 
                                                        				currlh++;
                                                				else
                                                        				currlh = 0;
                                        				else
                                                				if ( logicalhost[currlh].currds < logicalhost[currlh].numds ) 
                                                        				logicalhost[currlh].currds++;
                                                				else
                                                        				logicalhost[currlh].currds = 0;
                                        				break;
								default:
									break;
							}
						default:
							break;
					}
					break;






					/*  tab key  */

                        case '\011':
					if (whichwindow == top) {
                                                whichwindow = bottom;
						wmove(lhw,MAXLH,0);
						wclrtoeol(lhw);
						wrefresh(lhw);
						wmove(dsw,MAXDS,0);
						waddstr(dsw,"^");
						wrefresh(dsw);
					}
                                        else {
                                                whichwindow = top;
						wmove(dsw,MAXDS,0);
						wclrtoeol(dsw);
						wrefresh(dsw);
						wmove(lhw,MAXLH,0);
						waddstr(lhw,"^");
						wrefresh(lhw);
					}
                                        break;





					/*  CTRL-R   


			case 18:
*/

			case 'v':
			case 'V':
					RO = TRUE;
			case 'R':
			case 'r':
					clear();
					prev_filetime = 0;
        				fsthatitle();
					break;
			case 'e':
			case 'E':
					RO = FALSE;
					prev_filetime = 0;
					clear();
        				fsthatitle();
					break;


                        case 's':
                        case 'S':
                                        cmdline[0]=c; cmdline[1]=0;
					c=atoi(cmdline);
					if ( RO )
						break;
					sprintf(cmdline,"fstha shutdown");
                                        showcmdline(cmdline);
                                        system(strcat(cmdline,appendlog));
                                        break;

                        case '0':
                        case '1':
                        case '2':
                        case '3':
                                        cmdline[0]=c; cmdline[1]=0;
					c=atoi(cmdline);
					if ( RO )
						break;
					sprintf(cmdline,"fstha locate %d %d",logicalhost[currlh].lhnum,c);
                                        showcmdline(cmdline);
                                        system(strcat(cmdline,appendlog));
                                        break;

                        case 'n':
                        case 'N':
					if ( RO )
						break;
					sprintf(cmdline,"fstha locate %d %c",logicalhost[currlh].lhnum,c);
                                        showcmdline(cmdline);
                                        system(strcat(cmdline,appendlog));
                                        break;
                        case 'f':
                        case 'F':
					if ( RO )
						break;
					sprintf(cmdline,"fstha off %d %d",logicalhost[currlh].currds,logicalhost[currlh].lhnum);
                                        showcmdline(cmdline);
                                        system(strcat(cmdline,appendlog));
					cmdline[0]=0;
                                        break;

                        case 'o':
                        case 'O':
					if ( RO )
						break;
					sprintf(cmdline,"fstha on %d %d",logicalhost[currlh].currds,logicalhost[currlh].lhnum);
                                        showcmdline(cmdline);
                                        system(strcat(cmdline,appendlog));
					cmdline[0]=0;
                                        break;
                        case 'm':
                        case 'M':
					if ( RO )
						break;
					sprintf(cmdline,"fstha nocheck %d %d",logicalhost[currlh].currds,logicalhost[currlh].lhnum);
                                        showcmdline(cmdline);
                                        system(strcat(cmdline,appendlog));
					cmdline[0]=0;
                                        break;

			case 'k':
			case 'K':
                        case 'u':
                        case 'U':
                                        if (whichwindow == top)
                                                if ( currlh > 0 ) 
                                                        currlh--;
                                                else
                                                        currlh = numlh;
                                        else
                                                if ( logicalhost[currlh].currds > 0 ) 
                                                        logicalhost[currlh].currds--;
                                                else
                                                        logicalhost[currlh].currds = logicalhost[currlh].numds;
                                        break;
			case 'j':
			case 'J':
                        case 'd':
                        case 'D':
                                        if (whichwindow == top)
                                                if ( currlh < numlh ) 
                                                        currlh++;
                                                else
                                                        currlh = 0;
                                        else
                                                if ( logicalhost[currlh].currds < logicalhost[currlh].numds ) 
                                                        logicalhost[currlh].currds++;
                                                else
                                                        logicalhost[currlh].currds = 0;
                                        break;

                        case 'q':
                        case 'Q':       showcmdline("Quiting...");
                        default :
                                        showtime();
					fsthatitle();
                                        fsthagethosts();
                                        getmessages();
					sleep(sleep_time);
                                        break;
                }
        }
	return (1);

}

void fsthatitle() {
        char s[BUFSIZ]="";

        move(0,0);
	if ( RO ) standout();
        addstr(HAVERSION);

        move(0,(MAXCOLS/2)-((strlen(nodename)+9)/2));
        standout(); addstr(nodename); standend();


        move(2,0);
        printw("%15.15-s  %20.20-s  %40.40-s" ,
                "Virtual Host","Mastered", "Status - FallbackNode - FailtoNode");
        move(3,0);
        printw("%15.15-s  %20.20-s  %40.40-s" ,
                "---------------","--------------------",
                "----------------------------------------");
        move(9,0);
        printw("%12.12-s %5.5-s %60.60-s",
                "App Agents  ","State","Messages");
        move(10,0);
        printw("%12.12-s %5.5-s %60.60-s" ,
                "------------","-----",
                "------------------------------------------------------------");
        move(19,0);
        standout(); addstr("R"); standend(); addstr("efresh ");
        standout(); 
	if ( RO ) {
		addch('E'); standend();
        	addstr("xecMode ");
	} else {	
		addch('V'); standend();
        	addstr("iewMode ");
	}
        standout(); addstr("TAB"); standend(); addstr(" ");
        standout(); addstr("S"); standend(); addstr("hutdown ");
        standout(); addch('Q'); standend();
        addstr("uit     ");

        move(19,45);
        addstr("Turn App Agent   ");
        standout(); addch('O'); standend();
        addstr("n of");
        standout(); addch('F'); standend();
        addstr(" or ");
        standout(); addch('M'); standend();
        addstr("aint");

	move(21,0);
	sprintf(s,"Private/Public: %c%c%c%c%c%c%c%c / %c%c%c%c",
	(shm->hb0stat0 == UP) ? '+': '-',
	(shm->hb0stat1 == UP) ? '+': '-',
	(shm->hb0stat2 == UP) ? '+': '-',
	(shm->hb0stat3 == UP) ? '+': '-',
	(shm->hb1stat0 == UP) ? '+': '-',
	(shm->hb1stat1 == UP) ? '+': '-',
	(shm->hb1stat2 == UP) ? '+': '-',
	(shm->hb1stat3 == UP) ? '+': '-',
	(shm->node0stat == UP) ? '+': '-',
	(shm->node1stat == UP) ? '+': '-',
	(shm->node2stat == UP) ? '+': '-',
	(shm->node3stat == UP) ? '+': '-');
	addstr(s);
	move(22,0);
	addstr("          Node: 01230123 / 0123 ");

        move(21,45);
        addstr("Locate->node ");
        standout(); addch('0'); standend();
        addstr(" ");
        standout(); addch('1'); standend();
        addstr(" ");
        standout(); addch('2'); standend();
        addstr(" ");
        standout(); addch('3'); standend();
        addstr(" ");
        standout(); addch('N'); standend();
        addstr("one");


        move(24,0);
	if ( RO ) {
        	standout(); 
        	addstr("View Only     ");
	}
	else {
        	addstr("Exec Command: ");
	}
        standend();
        refresh();
}

void showtime() {

	time_t now;

	now=time(NULL);
	move(0,54);
	strcpy(currtime,ctime(&now));
	currtime[strlen(currtime)-1]=0;
	if ( RO ) standout();
	addstr(currtime);
	standend();
        refresh();
}

void getmessages () {

        FILE *f;
        struct stat fileinfo;
        char s[BUFSIZ]="";
	char *p,s2[BUFSIZ];

        stat(shm->logfile,&fileinfo);    /*only update if modified*/
        if ( prev_filetime != fileinfo.st_mtime ) {
                f=fopen(shm->logfile,"r");
                fseek(f,((MWy * MWx) * -1),SEEK_END);
                fread(s,(MWy * MWx),1,f);
                s[(MWy * MWx)-1]=0;             /* remove last cr */
		wstandend(mgw);
               	waddstr(mgw,s);

                fclose(f);
                wrefresh(mgw);
                prev_filetime = fileinfo.st_mtime;
                wclear(lgw);
                wrefresh(lgw);
        }
}

void showcmdline(char *s) {
        wclear(lgw);
        wstandout(lgw);
        waddstr(lgw,s); 
        wstandend(lgw); 
        wrefresh(lgw);
}


int fsthagethosts() {

        char	line[BUFSIZ]="",s[BUFSIZ+1],
		dummy[BUFSIZ+1];
        short	i,n,x;



	/* get cluster status */

	wmove(csw,0,0); wclrtoeol(csw);
	waddch(csw,':');
	wstandout(csw);
	if (shm->unfreeze != YES) {
		waddstr(csw,"FROZEN");
		RO = TRUE;
	} else if (shm->cstat == UP)
		waddstr(csw,"UP");
	else if (shm->cstat == GUP)
		waddstr(csw,"-->UP");
	else if (shm->cstat == GDOWN)
		waddstr(csw,"-->DOWN");
	else if (shm->cstat == DOWN)
		waddstr(csw,"DOWN");
	else if (shm->cstat == JOIN)
		waddstr(csw,"-->JOIN");
	else if (shm->cstat == WAIT) {
		waddstr(csw,"SYNC");
		RO = TRUE;
	}
	if ( ! RO ) wstandend(csw);
	wrefresh(csw);
	if (shm->cstat == DOWN)
		if ( ! RO ) return(0);
	

	/* cleanup first 

	for (numlh=0; numlh <MAXLH; numlh++) {
		logicalhost[numlh].name[0]=0;
		logicalhost[numlh].mastered[0]=0;
		logicalhost[numlh].state[0]=0;
	}
	for (numlh=0; numlh <MAXDS; numlh++) {
		dataservice[numlh].name[0]=0;
		dataservice[numlh].state[0]=0;
	}
	***/

	/* get logical hosts */

	numlh=-1;
	if (strcmp(shm->lh0name,"") != 0 && shm->lh0node != UNUSED) {
		numlh++;
		strcpy(logicalhost[numlh].name,shm->lh0name);
		logicalhost[numlh].lhnum = 0;
		sprintf(logicalhost[numlh].mastered,"%d",shm->lh0node);
		if (shm->lh0stat == 0)
			sprintf(logicalhost[numlh].state,"%s","UP");
		else if (shm->lh0stat == 1)
			sprintf(logicalhost[numlh].state,"%s","DOWN");
		else if (shm->lh0stat == 2)
			sprintf(logicalhost[numlh].state,"%s","UP ON REMOTE NODE");
		else if (shm->lh0stat == 3)
			sprintf(logicalhost[numlh].state,"%s","FAILED!!");
		else if (shm->lh0stat == 4)
			sprintf(logicalhost[numlh].state,"%s","GOING-DOWN");
		else if (shm->lh0stat == 5)
			sprintf(logicalhost[numlh].state,"%s","COMING-UP");
		else if (shm->lh0stat == 9)
			sprintf(logicalhost[numlh].state,"%s","WAIT");
		else
			sprintf(logicalhost[numlh].state,"%s","UNKNOWN");
		if (shm->lh0timeo != 0)
			strcat(logicalhost[numlh].state," ...");

sprintf(logicalhost[numlh].state,"%s - %s - %s",logicalhost[numlh].state,
fstnodestr(shm->lh0fallback),
fstnodestr(shm->lh0failto));
		logicalhost[numlh].numds=-1;
		if (strlen(shm->lh0app0name) > 0) {
			++logicalhost[numlh].numds;
			strcpy(dataservice[(numlh*4)+0].name,shm->lh0app0name);
			sprintf(dataservice[(numlh*4)+0].state,"%d",shm->lh0app0stat);
		}
		if (strlen(shm->lh0app1name) > 0) {
			++logicalhost[numlh].numds;
			strcpy(dataservice[(numlh*4)+1].name,shm->lh0app1name);
			sprintf(dataservice[(numlh*4)+1].state,"%d",shm->lh0app1stat);
		}
		if (strlen(shm->lh0app2name) > 0) {
			++logicalhost[numlh].numds;
			strcpy(dataservice[(numlh*4)+2].name,shm->lh0app2name);
			sprintf(dataservice[(numlh*4)+2].state,"%d",shm->lh0app2stat);
		}
		if (strlen(shm->lh0app3name) > 0) {
			++logicalhost[numlh].numds;
			strcpy(dataservice[(numlh*4)+3].name,shm->lh0app3name);
			sprintf(dataservice[(numlh*4)+3].state,"%d",shm->lh0app3stat);
		}
	}


	if (strcmp(shm->lh1name,"") != 0 && shm->lh1node != UNUSED) {
		numlh++;
		strcpy(logicalhost[numlh].name,shm->lh1name);
		logicalhost[numlh].lhnum = 1;
		sprintf(logicalhost[numlh].mastered,"%d",shm->lh1node);
		if (shm->lh1stat == 0)
			sprintf(logicalhost[numlh].state,"%s","UP");
		else if (shm->lh1stat == 1)
			sprintf(logicalhost[numlh].state,"%s","DOWN");
		else if (shm->lh1stat == 2)
			sprintf(logicalhost[numlh].state,"%s","UP ON REMOTE NODE");
		else if (shm->lh1stat == 3)
			sprintf(logicalhost[numlh].state,"%s","FAILED!!");
		else if (shm->lh1stat == 4)
			sprintf(logicalhost[numlh].state,"%s","GOING-DOWN");
		else if (shm->lh1stat == 5)
			sprintf(logicalhost[numlh].state,"%s","COMING-UP");
		else if (shm->lh1stat == 9)
			sprintf(logicalhost[numlh].state,"%s","WAIT");
		else
			sprintf(logicalhost[numlh].state,"%s","UNKNOWN");
		if (shm->lh1timeo != 0)
			strcat(logicalhost[numlh].state," ...");
sprintf(logicalhost[numlh].state,"%s - %s - %s",logicalhost[numlh].state,
fstnodestr(shm->lh1fallback),
fstnodestr(shm->lh1failto));
		logicalhost[numlh].numds=-1;
		if (strlen(shm->lh1app0name) > 0) {
			++logicalhost[numlh].numds;
			strcpy(dataservice[(numlh*4)+0].name,shm->lh1app0name);
			sprintf(dataservice[(numlh*4)+0].state,"%d",shm->lh1app0stat);
		}
		if (strlen(shm->lh1app1name) > 0) {
			++logicalhost[numlh].numds;
			strcpy(dataservice[(numlh*4)+1].name,shm->lh1app1name);
			sprintf(dataservice[(numlh*4)+1].state,"%d",shm->lh1app1stat);
		}
		if (strlen(shm->lh1app2name) > 0) {
			++logicalhost[numlh].numds;
			strcpy(dataservice[(numlh*4)+2].name,shm->lh1app2name);
			sprintf(dataservice[(numlh*4)+2].state,"%d",shm->lh1app2stat);
		}
		if (strlen(shm->lh1app3name) > 0) {
			++logicalhost[numlh].numds;
			strcpy(dataservice[(numlh*4)+3].name,shm->lh1app3name);
			sprintf(dataservice[(numlh*4)+3].state,"%d",shm->lh1app3stat);
		}
	}


	if (strcmp(shm->lh2name,"") != 0 && shm->lh2node != UNUSED) {
		numlh++;
		strcpy(logicalhost[numlh].name,shm->lh2name);
		logicalhost[numlh].lhnum = 2;
		sprintf(logicalhost[numlh].mastered,"%d",shm->lh2node);
		if (shm->lh2stat == 0)
			sprintf(logicalhost[numlh].state,"%s","UP");
		else if (shm->lh2stat == 1)
			sprintf(logicalhost[numlh].state,"%s","DOWN");
		else if (shm->lh2stat == 2)
			sprintf(logicalhost[numlh].state,"%s","UP ON REMOTE NODE");
		else if (shm->lh2stat == 3)
			sprintf(logicalhost[numlh].state,"%s","FAILED!!");
		else if (shm->lh2stat == 4)
			sprintf(logicalhost[numlh].state,"%s","GOING-DOWN");
		else if (shm->lh2stat == 5)
			sprintf(logicalhost[numlh].state,"%s","COMING-UP");
		else if (shm->lh2stat == 9)
			sprintf(logicalhost[numlh].state,"%s","WAIT");
		else
			sprintf(logicalhost[numlh].state,"%s","UNKNOWN");
		if (shm->lh2timeo != 0)
			strcat(logicalhost[numlh].state," ...");
sprintf(logicalhost[numlh].state,"%s - %s - %s",logicalhost[numlh].state,
fstnodestr(shm->lh2fallback),
fstnodestr(shm->lh2failto));
		logicalhost[numlh].numds=-1;
		if (strlen(shm->lh2app0name) > 0) {
			++logicalhost[numlh].numds;
			strcpy(dataservice[(numlh*4)+0].name,shm->lh2app0name);
			sprintf(dataservice[(numlh*4)+0].state,"%d",shm->lh2app0stat);
		}
		if (strlen(shm->lh2app1name) > 0) {
			++logicalhost[numlh].numds;
			strcpy(dataservice[(numlh*4)+1].name,shm->lh2app1name);
			sprintf(dataservice[(numlh*4)+1].state,"%d",shm->lh2app1stat);
		}
		if (strlen(shm->lh2app2name) > 0) {
			++logicalhost[numlh].numds;
			strcpy(dataservice[(numlh*4)+2].name,shm->lh2app2name);
			sprintf(dataservice[(numlh*4)+2].state,"%d",shm->lh2app2stat);
		}
		if (strlen(shm->lh2app3name) > 0) {
			++logicalhost[numlh].numds;
			strcpy(dataservice[(numlh*4)+3].name,shm->lh2app3name);
			sprintf(dataservice[(numlh*4)+3].state,"%d",shm->lh2app3stat);
		}
	}


	if (strcmp(shm->lh3name,"") !=0 && shm->lh3node != UNUSED) {
		numlh++;
		strcpy(logicalhost[numlh].name,shm->lh3name);
		logicalhost[numlh].lhnum = 3;
		sprintf(logicalhost[numlh].mastered,"%d",shm->lh3node);
		if (shm->lh3stat == 0)
			sprintf(logicalhost[numlh].state,"%s","UP");
		else if (shm->lh3stat == 1)
			sprintf(logicalhost[numlh].state,"%s","DOWN");
		else if (shm->lh3stat == 2)
			sprintf(logicalhost[numlh].state,"%s","UP ON REMOTE NODE");
		else if (shm->lh3stat == 3)
			sprintf(logicalhost[numlh].state,"%s","FAILED!!");
		else if (shm->lh3stat == 4)
			sprintf(logicalhost[numlh].state,"%s","GOING-DOWN");
		else if (shm->lh3stat == 5)
			sprintf(logicalhost[numlh].state,"%s","COMING-UP");
		else if (shm->lh3stat == 9)
			sprintf(logicalhost[numlh].state,"%s","WAIT");
		else
			sprintf(logicalhost[numlh].state,"%s","UNKNOWN");
		if (shm->lh3timeo != 0)
			strcat(logicalhost[numlh].state," ...");
sprintf(logicalhost[numlh].state,"%s - %s - %s",logicalhost[numlh].state,
fstnodestr(shm->lh3fallback),
fstnodestr(shm->lh3failto));
		logicalhost[numlh].numds=-1;
		if (strlen(shm->lh3app0name) > 0) {
			++logicalhost[numlh].numds;
			strcpy(dataservice[(numlh*4)+0].name,shm->lh3app0name);
			sprintf(dataservice[(numlh*4)+0].state,"%d",shm->lh3app0stat);
		}
		if (strlen(shm->lh3app1name) > 0) {
			++logicalhost[numlh].numds;
			strcpy(dataservice[(numlh*4)+1].name,shm->lh3app1name);
			sprintf(dataservice[(numlh*4)+1].state,"%d",shm->lh3app1stat);
		}
		if (strlen(shm->lh3app2name) > 0) {
			++logicalhost[numlh].numds;
			strcpy(dataservice[(numlh*4)+2].name,shm->lh3app2name);
			sprintf(dataservice[(numlh*4)+2].state,"%d",shm->lh3app2stat);
		}
		if (strlen(shm->lh3app3name) > 0) {
			++logicalhost[numlh].numds;
			strcpy(dataservice[(numlh*4)+3].name,shm->lh3app3name);
			sprintf(dataservice[(numlh*4)+3].state,"%d",shm->lh3app3stat);
		}
	}


	/* show logical hosts */

//        if ( prev_currlh != currlh ) {
{
                wmove(lhw,0,0); wclrtobot(lhw);
                for (i=0; i<= numlh; i++ ) {
			if (strlen(logicalhost[i].name) > 0) {
                                if (strcmp(logicalhost[i].mastered,"0") == 0)
					sprintf(dummy,"%s (0)",shm->node0);
				else if (strcmp(logicalhost[i].mastered,"1") == 0)
					sprintf(dummy,"%s (1)",shm->node1);
				else if (strcmp(logicalhost[i].mastered,"2") == 0)
					sprintf(dummy,"%s (2)",shm->node2);
				else if (strcmp(logicalhost[i].mastered,"3") == 0)
					sprintf(dummy,"%s (3)",shm->node3);
				else
					strcpy(dummy,"Internal Error");
                        	sprintf(line,"%15.15-s  %20.20-s  %40.40-s",
                                	logicalhost[i].name, 
                                	dummy,
					logicalhost[i].state);
                        	if ( currlh == i )
                                	wstandout(lhw);
                        	waddstr(lhw,line);
                        	wstandend(lhw);
			}
                }
		if (whichwindow == top) {
			wmove(lhw,MAXLH,0);
			waddstr(lhw,"^");
		}
                wrefresh(lhw);
        }


	/* show dataservices */

//       if ( prev_currlh != currlh || logicalhost[currlh].prev_currds != logicalhost[currlh].currds) {
{
               	wmove(dsw,0,0); wclrtobot(dsw);
               	for (x=0,n=currlh*MAXDS; x< MAXDS; x++ ) {
			if (strlen(dataservice[(n)+x].name) > 0) {
               			sprintf(line,"%12.12-s ",dataservice[(n)+x].name);
				if ( strcmp(dataservice[(n)+x].state,"0")== 0)
               				strcat(line,"On\n");
				else if ( strcmp(dataservice[(n)+x].state,"1")== 0)
               				strcat(line,"Off\n");
				else if ( strcmp(dataservice[(n)+x].state,"8")== 0)
               				strcat(line,"Maint");
				else 
               				continue;
               			if ( logicalhost[currlh].currds  == x )
                       			wstandout(dsw);
               			waddstr(dsw,line);
               			wstandend(dsw);
			}
         	}
		if (whichwindow == bottom) {
			wmove(dsw,MAXDS,0);
			waddstr(dsw,"^");
		}
               	wrefresh(dsw);
	}



       	logicalhost[currlh].prev_currds = logicalhost[currlh].currds;
        prev_currlh = currlh;
}
