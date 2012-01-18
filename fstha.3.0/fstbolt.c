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
 * File Name: fstbolt.c
 * 		Return status of cluster items to fsthatool
 * 
 * File Authors:
 * 		Reggie Beavers (reggiebeavers@fstha.com)
 */
/*
 * fstbolt.c
 *
 *
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
#include <sys/systeminfo.h>
#include "fstha.h"



main(int argc, char *argv[]) {

get_shm(0);
if (strcmp(argv[1],"showstat") == 0 ) {
printf("node0 \"%s\" ",shm->node0);
printf("node1 \"%s\" ",shm->node1);
printf("node2 \"%s\" ",shm->node2);
printf("node3 \"%s\" ",shm->node3);
printf("nodeN %d ",shm->nodeN);
printf("node0stat %d ",shm->node0stat);
printf("node1stat %d ",shm->node1stat);
printf("node2stat %d ",shm->node2stat);
printf("node3stat %d ",shm->node3stat);
printf("hb0stat0 %d ",shm->hb0stat0);
printf("hb1stat0 %d ",shm->hb1stat0);
printf("hb0stat1 %d ",shm->hb0stat1);
printf("hb1stat1 %d ",shm->hb1stat1);
printf("hb0stat2 %d ",shm->hb0stat2);
printf("hb1stat2 %d ",shm->hb1stat2);
printf("hb0stat3 %d ",shm->hb0stat3);
printf("hb1stat3 %d ",shm->hb1stat3);

printf("vh0name \"%s\" ",shm->lh0name);
printf("vh0node %d ",shm->lh0node);
printf("vh0stat %d ",shm->lh0stat);

printf("vh1name \"%s\" ",shm->lh1name);
printf("vh1node %d ",shm->lh1node);
printf("vh1stat %d ",shm->lh1stat);

printf("vh2name \"%s\" ",shm->lh2name);
printf("vh2node %d ",shm->lh2node);
printf("vh2stat %d ",shm->lh2stat);

printf("vh3name \"%s\" ",shm->lh3name);
printf("vh3node %d ",shm->lh3node);
printf("vh3stat %d ",shm->lh3stat);

if ( strcmp(shm->lh0app0name,"") == 0 )
	printf("vh0app0stat 9 ");
else
	printf("vh0app0stat %d ",shm->lh0app0stat);
if ( strcmp(shm->lh0app1name,"") == 0 )
	printf("vh0app1stat 9 ");
else
	printf("vh0app1stat %d ",shm->lh0app1stat);
if ( strcmp(shm->lh0app2name,"") == 0 )
	printf("vh0app2stat 9 ");
else
	printf("vh0app2stat %d ",shm->lh0app2stat);
if ( strcmp(shm->lh0app3name,"") == 0 )
	printf("vh0app3stat 9 ");
else
	printf("vh0app3stat %d ",shm->lh0app3stat);



if ( strcmp(shm->lh1app0name,"") == 0 )
	printf("vh1app0stat 9 ");
else
	printf("vh1app0stat %d ",shm->lh1app0stat);

if ( strcmp(shm->lh1app1name,"") == 0 )
	printf("vh1app1stat 9 ");
else
	printf("vh1app1stat %d ",shm->lh1app1stat);
if ( strcmp(shm->lh1app2name,"") == 0 )
	printf("vh1app2stat 9 ");
else
	printf("vh1app2stat %d ",shm->lh1app2stat);
if ( strcmp(shm->lh1app3name,"") == 0 )
	printf("vh1app3stat 9 ");
else
	printf("vh1app3stat %d ",shm->lh1app3stat);



if ( strcmp(shm->lh2app0name,"") == 0 )
	printf("vh2app0stat 9 ");
else
	printf("vh2app0stat %d ",shm->lh2app0stat);
if ( strcmp(shm->lh2app1name,"") == 0 )
	printf("vh2app1stat 9 ");
else
	printf("vh2app1stat %d ",shm->lh2app1stat);
if ( strcmp(shm->lh2app2name,"") == 0 )
	printf("vh2app2stat 9 ");
else
	printf("vh2app2stat %d ",shm->lh2app2stat);
if ( strcmp(shm->lh2app3name,"") == 0 )
	printf("vh2app3stat 9 ");
else
	printf("vh2app3stat %d ",shm->lh2app3stat);



if ( strcmp(shm->lh3app0name,"") == 0 )
	printf("vh3app0stat 9 ");
else
	printf("vh3app0stat %d ",shm->lh3app0stat);

if ( strcmp(shm->lh3app1name,"") == 0 )
	printf("vh3app1stat 9 ");
else
	printf("vh3app1stat %d ",shm->lh3app1stat);
if ( strcmp(shm->lh3app2name,"") == 0 )
	printf("vh3app2stat 9 ");
else
	printf("vh3app2stat %d ",shm->lh3app2stat);
if ( strcmp(shm->lh3app3name,"") == 0 )
	printf("vh3app3stat 9 ");
else
	printf("vh3app3stat %d ",shm->lh3app3stat);

exit (0);
}

else if (strcmp(argv[1],"vhstat") == 0 ) {

printf("node0 \"%s\" ",shm->node0);
printf("node1 \"%s\" ",shm->node1);
printf("node2 \"%s\" ",shm->node2);
printf("node3 \"%s\" ",shm->node3);

printf("vh0name \"%s\" ",shm->lh0name);
printf("vh0node %d ",shm->lh0node);
printf("vh0stat %d ",shm->lh0stat);
if ( strcmp(shm->lh0name,"") == 0 || shm->lh0node == 9)
	printf("vh0nodename \"\" vh0stat \"\" ");
else {
	if (shm->lh0node == 0)
		printf("vh0nodename \"%s\" ",shm->node0);
	else
		printf("vh0nodename \"%s\" ",shm->node1);
	switch (shm->lh0stat) {
	case 0 : printf("vh0stat \"UP\" "); break;
	case 1 : printf("vh0stat \"DOWN\" "); break;
	case 3 : printf("vh0stat \"FAILED\" "); break;
	case 4 : printf("vh0stat \"@DOWN\" "); break;
	case 5 : printf("vh0stat \"@UP\" "); break;
	}
}


printf("vh1name \"%s\" ",shm->lh1name);
printf("vh1node %d ",shm->lh1node);
printf("vh1stat %d ",shm->lh1stat);
if ( strcmp(shm->lh1name,"") == 0 || shm->lh1node == 9)
	printf("vh1nodename \"\" vh1stat \"\" ");
else {
	if (shm->lh1node == 0)
		printf("vh1nodename \"%s\" ",shm->node0);
	else
		printf("vh1nodename \"%s\" ",shm->node1);
	switch (shm->lh1stat) {
	case 0 : printf("vh1stat \"UP\" "); break;
	case 1 : printf("vh1stat \"DOWN\" "); break;
	case 3 : printf("vh1stat \"FAILED\" "); break;
	case 4 : printf("vh1stat \"@DOWN\" "); break;
	case 5 : printf("vh1stat \"@UP\" "); break;
	}
}


printf("vh2name \"%s\" ",shm->lh2name);
printf("vh2node %d ",shm->lh2node);
printf("vh2stat %d ",shm->lh2stat);
if ( strcmp(shm->lh2name,"") == 0 || shm->lh2node == 9)
	printf("vh2nodename \"\" vh2stat \"\" ");
else {
	if (shm->lh2node == 0)
		printf("vh2nodename \"%s\" ",shm->node0);
	else
		printf("vh2nodename \"%s\" ",shm->node1);
	switch (shm->lh2stat) {
	case 0 : printf("vh2stat \"UP\" "); break;
	case 1 : printf("vh2stat \"DOWN\" "); break;
	case 3 : printf("vh2stat \"FAILED\" "); break;
	case 4 : printf("vh2stat \"@DOWN\" "); break;
	case 5 : printf("vh2stat \"@UP\" "); break;
	}
}


printf("vh3name \"%s\" ",shm->lh3name);
printf("vh3node %d ",shm->lh3node);
printf("vh3stat %d ",shm->lh3stat);
if ( strcmp(shm->lh3name,"") == 0 || shm->lh3node == 9)
	printf("vh3nodename \"\" vh3stat \"\" ");
else {
	if (shm->lh3node == 0)
		printf("vh3nodename \"%s\" ",shm->node0);
	else
		printf("vh3nodename \"%s\" ",shm->node1);
	switch (shm->lh3stat) {
	case 0 : printf("vh3stat \"UP\" "); break;
	case 1 : printf("vh3stat \"DOWN\" "); break;
	case 3 : printf("vh3stat \"FAILED\" "); break;
	case 4 : printf("vh3stat \"@DOWN\" "); break;
	case 5 : printf("vh3stat \"@UP\" "); break;
	}
}
}

else if (strcmp(argv[1],"vh0stat") == 0 ) {

printf("vhname \"%s\" ",shm->lh0name);
printf("vhstat %d ",shm->lh0stat);
printf("vhpath \"%s\" ",shm->lh0path);

}
else if (strcmp(argv[1],"vh1stat") == 0 ) {

printf("vhname \"%s\" ",shm->lh1name);
printf("vhstat %d ",shm->lh1stat);
printf("vhpath \"%s\" ",shm->lh1path);

}
else if (strcmp(argv[1],"vh2stat") == 0 ) {

printf("vhname \"%s\" ",shm->lh2name);
printf("vhstat %d ",shm->lh2stat);
printf("vhpath \"%s\" ",shm->lh2path);

}
else if (strcmp(argv[1],"vh3stat") == 0 ) {

printf("vhname \"%s\" ",shm->lh3name);
printf("vhstat %d ",shm->lh3stat);
printf("vhpath \"%s\" ",shm->lh3path);

}
/****/
else if (strcmp(argv[1],"vh0app0stat") == 0 ) {

printf("apname \"%s\" ",shm->lh0app0name);
if (strcmp(shm->lh0app0name,"") == 0 )
	printf("apstat 9 ");
else
printf("apstat %d ",shm->lh0app0stat);
printf("apstart \"%s\" ",shm->lh0app0start);
printf("apstop \"%s\" ",shm->lh0app0stop);
printf("aptest \"%s\" ",shm->lh0app0test);
printf("aprtest \"%s\" ",shm->lh0app0remtest);
printf("ap_c %d ",shm->lh0app0crit);
printf("ap_m %d ",shm->lh0app0timeo);
printf("ap_p %d ",shm->lh0app0pause);

}
else if (strcmp(argv[1],"vh0app1stat") == 0 ) {

printf("apname \"%s\" ",shm->lh0app1name);
if (strcmp(shm->lh0app1name,"") == 0 )
	printf("apstat 9 ");
else
printf("apstat %d ",shm->lh0app1stat);
printf("apstart \"%s\" ",shm->lh0app1start);
printf("apstop \"%s\" ",shm->lh0app1stop);
printf("aptest \"%s\" ",shm->lh0app1test);
printf("aprtest \"%s\" ",shm->lh0app1remtest);
printf("ap_c %d ",shm->lh0app1crit);
printf("ap_m %d ",shm->lh0app1timeo);
printf("ap_p %d ",shm->lh0app1pause);

}
else if (strcmp(argv[1],"vh0app2stat") == 0 ) {

printf("apname \"%s\" ",shm->lh0app2name);
if (strcmp(shm->lh0app2name,"") == 0 )
	printf("apstat 9 ");
else
printf("apstat %d ",shm->lh0app2stat);
printf("apstart \"%s\" ",shm->lh0app2start);
printf("apstop \"%s\" ",shm->lh0app2stop);
printf("aptest \"%s\" ",shm->lh0app2test);
printf("aprtest \"%s\" ",shm->lh0app2remtest);
printf("ap_c %d ",shm->lh0app2crit);
printf("ap_m %d ",shm->lh0app2timeo);
printf("ap_p %d ",shm->lh0app2pause);

}
else if (strcmp(argv[1],"vh0app3stat") == 0 ) {

printf("apname \"%s\" ",shm->lh0app3name);
if (strcmp(shm->lh0app3name,"") == 0 )
	printf("apstat 9 ");
else
printf("apstat %d ",shm->lh0app3stat);
printf("apstart \"%s\" ",shm->lh0app3start);
printf("apstop \"%s\" ",shm->lh0app3stop);
printf("aptest \"%s\" ",shm->lh0app3test);
printf("aprtest \"%s\" ",shm->lh0app3remtest);
printf("ap_c %d ",shm->lh0app3crit);
printf("ap_m %d ",shm->lh0app3timeo);
printf("ap_p %d ",shm->lh0app3pause);

}
/****/
else if (strcmp(argv[1],"vh1app0stat") == 0 ) {

printf("apname \"%s\" ",shm->lh1app0name);
if (strcmp(shm->lh1app0name,"") == 0 )
	printf("apstat 9 ");
else
printf("apstat %d ",shm->lh1app0stat);
printf("apstart \"%s\" ",shm->lh1app0start);
printf("apstop \"%s\" ",shm->lh1app0stop);
printf("aptest \"%s\" ",shm->lh1app0test);
printf("aprtest \"%s\" ",shm->lh1app0remtest);
printf("ap_c %d ",shm->lh1app0crit);
printf("ap_m %d ",shm->lh1app0timeo);
printf("ap_p %d ",shm->lh1app0pause);

}
else if (strcmp(argv[1],"vh1app1stat") == 0 ) {

printf("apname \"%s\" ",shm->lh1app1name);
if (strcmp(shm->lh1app1name,"") == 0 )
	printf("apstat 9 ");
else
printf("apstat %d ",shm->lh1app1stat);
printf("apstart \"%s\" ",shm->lh1app1start);
printf("apstop \"%s\" ",shm->lh1app1stop);
printf("aptest \"%s\" ",shm->lh1app1test);
printf("aprtest \"%s\" ",shm->lh1app1remtest);
printf("ap_c %d ",shm->lh1app1crit);
printf("ap_m %d ",shm->lh1app1timeo);
printf("ap_p %d ",shm->lh1app1pause);

}
else if (strcmp(argv[1],"vh1app2stat") == 0 ) {

printf("apname \"%s\" ",shm->lh1app2name);
if (strcmp(shm->lh1app2name,"") == 0 )
	printf("apstat 9 ");
else
printf("apstat %d ",shm->lh1app2stat);
printf("apstart \"%s\" ",shm->lh1app2start);
printf("apstop \"%s\" ",shm->lh1app2stop);
printf("aptest \"%s\" ",shm->lh1app2test);
printf("aprtest \"%s\" ",shm->lh1app2remtest);
printf("ap_c %d ",shm->lh1app2crit);
printf("ap_m %d ",shm->lh1app2timeo);
printf("ap_p %d ",shm->lh1app2pause);

}
else if (strcmp(argv[1],"vh1app3stat") == 0 ) {

printf("apname \"%s\" ",shm->lh1app3name);
if (strcmp(shm->lh1app3name,"") == 0 )
	printf("apstat 9 ");
else
printf("apstat %d ",shm->lh1app3stat);
printf("apstart \"%s\" ",shm->lh1app3start);
printf("apstop \"%s\" ",shm->lh1app3stop);
printf("aptest \"%s\" ",shm->lh1app3test);
printf("aprtest \"%s\" ",shm->lh1app3remtest);
printf("ap_c %d ",shm->lh1app3crit);
printf("ap_m %d ",shm->lh1app3timeo);
printf("ap_p %d ",shm->lh1app3pause);

}
/****/
else if (strcmp(argv[1],"vh2app0stat") == 0 ) {

printf("apname \"%s\" ",shm->lh2app0name);
if (strcmp(shm->lh2app0name,"") == 0 )
	printf("apstat 9 ");
else
printf("apstat %d ",shm->lh2app0stat);
printf("apstart \"%s\" ",shm->lh2app0start);
printf("apstop \"%s\" ",shm->lh2app0stop);
printf("aptest \"%s\" ",shm->lh2app0test);
printf("aprtest \"%s\" ",shm->lh2app0remtest);
printf("ap_c %d ",shm->lh2app0crit);
printf("ap_m %d ",shm->lh2app0timeo);
printf("ap_p %d ",shm->lh2app0pause);

}
else if (strcmp(argv[1],"vh2app1stat") == 0 ) {

printf("apname \"%s\" ",shm->lh2app1name);
if (strcmp(shm->lh2app1name,"") == 0 )
	printf("apstat 9 ");
else
printf("apstat %d ",shm->lh2app1stat);
printf("apstart \"%s\" ",shm->lh2app1start);
printf("apstop \"%s\" ",shm->lh2app1stop);
printf("aptest \"%s\" ",shm->lh2app1test);
printf("aprtest \"%s\" ",shm->lh2app1remtest);
printf("ap_c %d ",shm->lh2app1crit);
printf("ap_m %d ",shm->lh2app1timeo);
printf("ap_p %d ",shm->lh2app1pause);

}
else if (strcmp(argv[1],"vh2app2stat") == 0 ) {

printf("apname \"%s\" ",shm->lh2app2name);
if (strcmp(shm->lh2app2name,"") == 0 )
	printf("apstat 9 ");
else
printf("apstat %d ",shm->lh2app2stat);
printf("apstart \"%s\" ",shm->lh2app2start);
printf("apstop \"%s\" ",shm->lh2app2stop);
printf("aptest \"%s\" ",shm->lh2app2test);
printf("aprtest \"%s\" ",shm->lh2app2remtest);
printf("ap_c %d ",shm->lh2app2crit);
printf("ap_m %d ",shm->lh2app2timeo);
printf("ap_p %d ",shm->lh2app2pause);

}
else if (strcmp(argv[1],"vh2app3stat") == 0 ) {

printf("apname \"%s\" ",shm->lh2app3name);
if (strcmp(shm->lh2app3name,"") == 0 )
	printf("apstat 9 ");
else
printf("apstat %d ",shm->lh2app3stat);
printf("apstart \"%s\" ",shm->lh2app3start);
printf("apstop \"%s\" ",shm->lh2app3stop);
printf("aptest \"%s\" ",shm->lh2app3test);
printf("aprtest \"%s\" ",shm->lh2app3remtest);
printf("ap_c %d ",shm->lh2app3crit);
printf("ap_m %d ",shm->lh2app3timeo);
printf("ap_p %d ",shm->lh2app3pause);

}
/****/
else if (strcmp(argv[1],"vh3app0stat") == 0 ) {

printf("apname \"%s\" ",shm->lh3app0name);
if (strcmp(shm->lh3app0name,"") == 0 )
	printf("apstat 9 ");
else
printf("apstat %d ",shm->lh3app0stat);
printf("apstart \"%s\" ",shm->lh3app0start);
printf("apstop \"%s\" ",shm->lh3app0stop);
printf("aptest \"%s\" ",shm->lh3app0test);
printf("aprtest \"%s\" ",shm->lh3app0remtest);
printf("ap_c %d ",shm->lh3app0crit);
printf("ap_m %d ",shm->lh3app0timeo);
printf("ap_p %d ",shm->lh3app0pause);

}
else if (strcmp(argv[1],"vh3app1stat") == 0 ) {

printf("apname \"%s\" ",shm->lh3app1name);
if (strcmp(shm->lh3app1name,"") == 0 )
	printf("apstat 9 ");
else
printf("apstat %d ",shm->lh3app1stat);
printf("apstart \"%s\" ",shm->lh3app1start);
printf("apstop \"%s\" ",shm->lh3app1stop);
printf("aptest \"%s\" ",shm->lh3app1test);
printf("aprtest \"%s\" ",shm->lh3app1remtest);
printf("ap_c %d ",shm->lh3app1crit);
printf("ap_m %d ",shm->lh3app1timeo);
printf("ap_p %d ",shm->lh3app1pause);

}
else if (strcmp(argv[1],"vh3app2stat") == 0 ) {

printf("apname \"%s\" ",shm->lh3app2name);
if (strcmp(shm->lh3app2name,"") == 0 )
	printf("apstat 9 ");
else
printf("apstat %d ",shm->lh3app2stat);
printf("apstart \"%s\" ",shm->lh3app2start);
printf("apstop \"%s\" ",shm->lh3app2stop);
printf("aptest \"%s\" ",shm->lh3app2test);
printf("aprtest \"%s\" ",shm->lh3app2remtest);
printf("ap_c %d ",shm->lh3app2crit);
printf("ap_m %d ",shm->lh3app2timeo);
printf("ap_p %d ",shm->lh3app2pause);

}
else if (strcmp(argv[1],"vh3app3stat") == 0 ) {

printf("apname \"%s\" ",shm->lh3app3name);
if (strcmp(shm->lh3app3name,"") == 0 )
	printf("apstat 9 ");
else
printf("apstat %d ",shm->lh3app3stat);
printf("apstart \"%s\" ",shm->lh3app3start);
printf("apstop \"%s\" ",shm->lh3app3stop);
printf("aptest \"%s\" ",shm->lh3app3test);
printf("aprtest \"%s\" ",shm->lh3app3remtest);
printf("ap_c %d ",shm->lh3app3crit);
printf("ap_m %d ",shm->lh3app3timeo);
printf("ap_p %d ",shm->lh3app3pause);

}

else if (strcmp(argv[1],"vhapp") == 0 ) {


printf("vh0name \"%s\" ",shm->lh0name);
printf("vh0node %d ",shm->lh0node);
if ( strcmp(shm->lh0name,"") == 0 || shm->lh0node == 9)
	printf("vh0nodename \"\" vh0stat \"\" ");
else {
	printf("vh0app0name \"%s\" ",shm->lh0app0name);
if ( strcmp(shm->lh0app0name,"") == 0 )
	printf("vh0app0stat 9 ");
else
	printf("vh0app0stat %d ",shm->lh0app0stat);
	printf("vh0app1name \"%s\" ",shm->lh0app1name);
if ( strcmp(shm->lh0app1name,"") == 0 )
	printf("vh0app1stat 9 ");
else
	printf("vh0app1stat %d ",shm->lh0app1stat);
	printf("vh0app2name \"%s\" ",shm->lh0app2name);
if ( strcmp(shm->lh0app2name,"") == 0 )
	printf("vh0app2stat 9 ");
else
	printf("vh0app2stat %d ",shm->lh0app2stat);
	printf("vh0app3name \"%s\" ",shm->lh0app3name);
if ( strcmp(shm->lh0app3name,"") == 0 )
	printf("vh0app3stat 9 ");
else
	printf("vh0app3stat %d ",shm->lh0app3stat);

	}
printf("vh1name \"%s\" ",shm->lh1name);
printf("vh1node %d ",shm->lh1node);
if ( strcmp(shm->lh1name,"") == 0 || shm->lh1node == 9)
	printf("vh1nodename \"\" vh1stat \"\" ");
else {
	printf("vh1app0name \"%s\" ",shm->lh1app0name);
if ( strcmp(shm->lh1app0name,"") == 0 )
	printf("vh1app0stat 9 ");
else
	printf("vh1app0stat %d ",shm->lh1app0stat);
	printf("vh1app1name \"%s\" ",shm->lh1app1name);
if ( strcmp(shm->lh1app1name,"") == 0 )
	printf("vh1app1stat 9 ");
else
	printf("vh1app1stat %d ",shm->lh1app1stat);
	printf("vh1app2name \"%s\" ",shm->lh1app2name);
if ( strcmp(shm->lh1app2name,"") == 0 )
	printf("vh1app2stat 9 ");
else
	printf("vh1app2stat %d ",shm->lh1app2stat);
	printf("vh1app3name \"%s\" ",shm->lh1app3name);
if ( strcmp(shm->lh1app3name,"") == 0 )
	printf("vh1app3stat 9 ");
else
	printf("vh1app3stat %d ",shm->lh1app3stat);

	}
printf("vh2name \"%s\" ",shm->lh2name);
printf("vh2node %d ",shm->lh2node);
if ( strcmp(shm->lh2name,"") == 0 || shm->lh2node == 9)
	printf("vh2nodename \"\" vh2stat \"\" ");
else {
	printf("vh2app0name \"%s\" ",shm->lh2app0name);
if ( strcmp(shm->lh2app0name,"") == 0 )
	printf("vh2app0stat 9 ");
else
	printf("vh2app0stat %d ",shm->lh2app0stat);
	printf("vh2app1name \"%s\" ",shm->lh2app1name);
if ( strcmp(shm->lh2app1name,"") == 0 )
	printf("vh2app1stat 9 ");
else
	printf("vh2app1stat %d ",shm->lh2app1stat);
	printf("vh2app2name \"%s\" ",shm->lh2app2name);
if ( strcmp(shm->lh2app2name,"") == 0 )
	printf("vh2app2stat 9 ");
else
	printf("vh2app2stat %d ",shm->lh2app2stat);
	printf("vh2app3name \"%s\" ",shm->lh2app3name);
if ( strcmp(shm->lh2app3name,"") == 0 )
	printf("vh2app3stat 9 ");
else
	printf("vh2app3stat %d ",shm->lh2app3stat);

	}
printf("vh3name \"%s\" ",shm->lh3name);
printf("vh3node %d ",shm->lh3node);
if ( strcmp(shm->lh3name,"") == 0 || shm->lh3node == 9)
	printf("vh3nodename \"\" vh3stat \"\" ");
else {
	printf("vh3app0name \"%s\" ",shm->lh3app0name);
if ( strcmp(shm->lh3app0name,"") == 0 )
	printf("vh3app0stat 9 ");
else
	printf("vh3app0stat %d ",shm->lh3app0stat);
	printf("vh3app1name \"%s\" ",shm->lh3app1name);
if ( strcmp(shm->lh3app1name,"") == 0 )
	printf("vh3app1stat 9 ");
else
	printf("vh3app1stat %d ",shm->lh3app1stat);
	printf("vh3app2name \"%s\" ",shm->lh3app2name);
if ( strcmp(shm->lh3app2name,"") == 0 )
	printf("vh3app2stat 9 ");
else
	printf("vh3app2stat %d ",shm->lh3app2stat);
	printf("vh3app3name \"%s\" ",shm->lh3app3name);
if ( strcmp(shm->lh3app3name,"") == 0 )
	printf("vh3app3stat 9 ");
else
	printf("vh3app3stat %d ",shm->lh3app3stat);

	}
}


else  {
}


exit(0);
}
