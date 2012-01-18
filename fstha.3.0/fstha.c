/*
 * HA/FST - High Availability Cluster
 * Copyright (C) 2003-2006 Reggie Beavers
 * 
 * Licensed under the terms of the GNU General Public License:
 * 		http://www.opensource.org/licenses/gpl-license.php
 * 
 * For further information visit:
 * 		http://www.fstha.com/
 * 
 * 
 * File Name: fstha.c
 * 		Main command line interface. 
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
#include <sys/systeminfo.h>
#include <sys/stat.h>
#include <sys/mhd.h>
#include "fsthanet.h"


get_conf() {
	char s[BUFSIZ],dummy[128];

	/*
        * populate shm segment from conf file
        */
	
        confilep=fopen(shm->cf,"r");
        if ( confilep  == NULL) {
		sprintf(msg,"/usr/bin/ipcrm -m %d > /dev/null 2>&1",shmid);
		system(msg);
                sprintf(msg,"open failed: %s",shm->cf);
                die(msg);
        }
        while (fgets(s,BUFSIZ,confilep) != NULL ) {
		update_conf(s);
	}
        fclose(confilep);


}

export_conf(char *s) {

if (strlen(s) == 0 || strcmp(s,"fstastlh0app0_pid")==0)
printf("fstastlh0app0_pid %d\n",shm->fstastlh0app0_pid);
if (strlen(s) == 0 || strcmp(s,"fstastlh0app1_pid")==0)
printf("fstastlh0app1_pid %d\n",shm->fstastlh0app1_pid);
if (strlen(s) == 0 || strcmp(s,"fstastlh0app2_pid")==0)
printf("fstastlh0app2_pid %d\n",shm->fstastlh0app2_pid);
if (strlen(s) == 0 || strcmp(s,"fstastlh0app3_pid")==0)
printf("fstastlh0app3_pid %d\n",shm->fstastlh0app3_pid);
if (strlen(s) == 0 || strcmp(s,"fstastlh1app0_pid")==0)
printf("fstastlh1app0_pid %d\n",shm->fstastlh1app0_pid);
if (strlen(s) == 0 || strcmp(s,"fstastlh1app1_pid")==0)
printf("fstastlh1app1_pid %d\n",shm->fstastlh1app1_pid);
if (strlen(s) == 0 || strcmp(s,"fstastlh1app2_pid")==0)
printf("fstastlh1app2_pid %d\n",shm->fstastlh1app2_pid);
if (strlen(s) == 0 || strcmp(s,"fstastlh1app3_pid")==0)
printf("fstastlh1app3_pid %d\n",shm->fstastlh1app3_pid);
if (strlen(s) == 0 || strcmp(s,"fstastlh2app0_pid")==0)
printf("fstastlh2app0_pid %d\n",shm->fstastlh2app0_pid);
if (strlen(s) == 0 || strcmp(s,"fstastlh2app1_pid")==0)
printf("fstastlh2app1_pid %d\n",shm->fstastlh2app1_pid);
if (strlen(s) == 0 || strcmp(s,"fstastlh2app2_pid")==0)
printf("fstastlh2app2_pid %d\n",shm->fstastlh2app2_pid);
if (strlen(s) == 0 || strcmp(s,"fstastlh2app3_pid")==0)
printf("fstastlh2app3_pid %d\n",shm->fstastlh2app3_pid);
if (strlen(s) == 0 || strcmp(s,"fstastlh3app0_pid")==0)
printf("fstastlh3app0_pid %d\n",shm->fstastlh3app0_pid);
if (strlen(s) == 0 || strcmp(s,"fstastlh3app1_pid")==0)
printf("fstastlh3app1_pid %d\n",shm->fstastlh3app1_pid);
if (strlen(s) == 0 || strcmp(s,"fstastlh3app2_pid")==0)
printf("fstastlh3app2_pid %d\n",shm->fstastlh3app2_pid);
if (strlen(s) == 0 || strcmp(s,"fstastlh3app3_pid")==0)
printf("fstastlh3app3_pid %d\n",shm->fstastlh3app3_pid);

if (strlen(s) == 0 || strcmp(s,"fsthbserv_pid")==0)
printf("fsthbserv_pid %d\n",shm->fsthbserv_pid);
if (strlen(s) == 0 || strcmp(s,"fstsvr_pid")==0)
printf("fstsvr_pid %d\n",shm->fstsvr_pid);
if (strlen(s) == 0 || strcmp(s,"fstasm_pid")==0)
printf("fstasm_pid %d\n",shm->fstasm_pid);
if (strlen(s) == 0 || strcmp(s,"fsthbb_pid")==0)
printf("fsthbb_pid %d\n",shm->fsthbb_pid);
if (strlen(s) == 0 || strcmp(s,"fsthbm_pid")==0)
printf("fsthbm_pid %d\n",shm->fsthbm_pid);

if (strlen(s) == 0 || strcmp(s,"cf")==0)
printf("cf %s\n",shm->cf);
if (strlen(s) == 0 || strcmp(s,"verbose")==0)
printf("verbose %d\n",shm->verbose);
if (strlen(s) == 0 || strcmp(s,"hbtimeout")==0)
printf("hbtimeout %d\n",shm->hbtimeout);
if (strlen(s) == 0 || strcmp(s,"hbdelay")==0)
printf("hbdelay %d\n",shm->hbdelay);
if (strlen(s) == 0 || strcmp(s,"version")==0)
printf("version %d\n",shm->version);
if (strlen(s) == 0 || strcmp(s,"htmlrefreshsecs")==0)
printf("htmlrefreshsecs %d\n",shm->htmlrefreshsecs);
if (strlen(s) == 0 || strcmp(s,"htmlallowexec")==0)
printf("htmlallowexec %d\n",shm->htmlallowexec);
if (strlen(s) == 0 || strcmp(s,"nodeN")==0)
printf("nodeN %d\n",shm->nodeN);

if (strlen(s) == 0 || strcmp(s,"node0stat")==0)
printf("node0stat %d\n",shm->node0stat);
if (strlen(s) == 0 || strcmp(s,"node1stat")==0)
printf("node1stat %d\n",shm->node1stat);
if (strlen(s) == 0 || strcmp(s,"node2stat")==0)
printf("node2stat %d\n",shm->node2stat);
if (strlen(s) == 0 || strcmp(s,"node3stat")==0)
printf("node3stat %d\n",shm->node3stat);

if (strlen(s) == 0 || strcmp(s,"reserve_strict")==0)
printf("reserve_strict %d\n",shm->reserve_strict);
if (strlen(s) == 0 || strcmp(s,"reserve_disk")==0)
printf("reserve_disk %s\n",shm->reserve_disk);
if (strlen(s) == 0 || strcmp(s,"logfile")==0)
printf("logfile %s\n",shm->logfile);
if (strlen(s) == 0 || strcmp(s,"syslogfac")==0)
printf("syslogfac %s\n",shm->syslogfac);
if (strlen(s) == 0 || strcmp(s,"port")==0)
printf("port %s\n",shm->port);
if (strlen(s) == 0 || strcmp(s,"hbport")==0)
printf("hbport %s\n",shm->hbport);
if (strlen(s) == 0 || strcmp(s,"hb0stat0")==0)
printf("hb0stat0 %d\n",shm->hb0stat0);
if (strlen(s) == 0 || strcmp(s,"hb0stat1")==0)
printf("hb0stat1 %d\n",shm->hb0stat1);
if (strlen(s) == 0 || strcmp(s,"hb0stat2")==0)
printf("hb0stat2 %d\n",shm->hb0stat2);
if (strlen(s) == 0 || strcmp(s,"hb0stat3")==0)
printf("hb0stat3 %d\n",shm->hb0stat3);
if (strlen(s) == 0 || strcmp(s,"hb1stat0")==0)
printf("hb1stat0 %d\n",shm->hb1stat0);
if (strlen(s) == 0 || strcmp(s,"hb1stat1")==0)
printf("hb1stat1 %d\n",shm->hb1stat1);
if (strlen(s) == 0 || strcmp(s,"hb1stat2")==0)
printf("hb1stat2 %d\n",shm->hb1stat2);
if (strlen(s) == 0 || strcmp(s,"hb1stat3")==0)
printf("hb1stat3 %d\n",shm->hb1stat3);
if (strlen(s) == 0 || strcmp(s,"hb0ip")==0)
printf("hb0ip %s\n",shm->hb0ip);
if (strlen(s) == 0 || strcmp(s,"hb0dv")==0)
printf("hb0dv %s\n",shm->hb0dv);
if (strlen(s) == 0 || strcmp(s,"hb0pg0")==0)
printf("hb0pg0 %s\n",shm->hb0pg0);
if (strlen(s) == 0 || strcmp(s,"hb0pg1")==0)
printf("hb0pg1 %s\n",shm->hb0pg1);
if (strlen(s) == 0 || strcmp(s,"hb0pg2")==0)
printf("hb0pg2 %s\n",shm->hb0pg2);
if (strlen(s) == 0 || strcmp(s,"hb0pg3")==0)
printf("hb0pg3 %s\n",shm->hb0pg3);

if (strlen(s) == 0 || strcmp(s,"hb1ip")==0)
printf("hb1ip %s\n",shm->hb1ip);
if (strlen(s) == 0 || strcmp(s,"hb1dv")==0)
printf("hb1dv %s\n",shm->hb1dv);

if (strlen(s) == 0 || strcmp(s,"hb1pg0")==0)
printf("hb1pg0 %s\n",shm->hb1pg0);
if (strlen(s) == 0 || strcmp(s,"hb1pg1")==0)
printf("hb1pg1 %s\n",shm->hb1pg1);
if (strlen(s) == 0 || strcmp(s,"hb1pg2")==0)
printf("hb1pg2 %s\n",shm->hb1pg2);
if (strlen(s) == 0 || strcmp(s,"hb1pg3")==0)
printf("hb1pg3 %s\n",shm->hb1pg3);

if (strlen(s) == 0 || strcmp(s,"node0")==0)
printf("node0 %s\n",shm->node0);
if (strlen(s) == 0 || strcmp(s,"node1")==0)
printf("node1 %s\n",shm->node1);
if (strlen(s) == 0 || strcmp(s,"node2")==0)
printf("node2 %s\n",shm->node2);
if (strlen(s) == 0 || strcmp(s,"node3")==0)
printf("node3 %s\n",shm->node3);

if (strlen(s) == 0 || strcmp(s,"lhfailtimeo")==0)
printf("lhfailtimeo %d\n",shm->lhfailtimeo); 
if (strlen(s) == 0 || strcmp(s,"appfailtimeo")==0)
printf("appfailtimeo %d\n",shm->appfailtimeo); 
if (strlen(s) == 0 || strcmp(s,"appfailtesttimeo")==0)
printf("appfailtesttimeo %d\n",shm->appfailtesttimeo); 

if (strlen(s) == 0 || strcmp(s,"lh0name")==0)
printf("lh0name %s\n",shm->lh0name); 
if (strlen(s) == 0 || strcmp(s,"lh0path")==0)
printf("lh0path %s\n",shm->lh0path); 
if (strlen(s) == 0 || strcmp(s,"lh0stat")==0)
printf("lh0stat %d\n",shm->lh0stat); 
if (strlen(s) == 0 || strcmp(s,"lh0node")==0)
printf("lh0node %d\n",shm->lh0node); 
if (strlen(s) == 0 || strcmp(s,"lh0fallback")==0)
printf("lh0fallback %d\n",shm->lh0fallback); 
if (strlen(s) == 0 || strcmp(s,"lh0failto")==0)
printf("lh0failto %d\n",shm->lh0failto); 
if (strlen(s) == 0 || strcmp(s,"lh0timeo")==0)
printf("lh0timeo %d\n",shm->lh0timeo); 
if (strlen(s) == 0 || strcmp(s,"lh0failing")==0)
printf("lh0failing %d\n",shm->lh0failing); 
if (strlen(s) == 0 || strcmp(s,"lh0app0name")==0)
printf("lh0app0name %s\n",shm->lh0app0name); 
if (strlen(s) == 0 || strcmp(s,"lh0app0stat")==0)
printf("lh0app0stat %d\n",shm->lh0app0stat); 
if (strlen(s) == 0 || strcmp(s,"lh0app0start")==0)
printf("lh0app0start %s\n",shm->lh0app0start); 
if (strlen(s) == 0 || strcmp(s,"lh0app0stop")==0)
printf("lh0app0stop %s\n",shm->lh0app0stop); 
if (strlen(s) == 0 || strcmp(s,"lh0app0test")==0)
printf("lh0app0test %s\n",shm->lh0app0test); 
if (strlen(s) == 0 || strcmp(s,"lh0app0timeo")==0)
printf("lh0app0timeo %d\n",shm->lh0app0timeo); 
if (strlen(s) == 0 || strcmp(s,"lh0app0testtimeo")==0)
printf("lh0app0testtimeo %d\n",shm->lh0app0testtimeo); 
if (strlen(s) == 0 || strcmp(s,"lh0app0remtest")==0)
printf("lh0app0remtest %s\n",shm->lh0app0remtest);
if (strlen(s) == 0 || strcmp(s,"lh0app0remtesttimeo")==0)
printf("lh0app0remtesttimeo %d\n",shm->lh0app0remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh0app0remtesttimeo")==0)
printf("lh0app0remtesttimeo %d\n",shm->lh0app0remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh0app0remnode")==0)
printf("lh0app0remnode %d\n",shm->lh0app0remnode);
if (strlen(s) == 0 || strcmp(s,"lh0app0secs")==0)
printf("lh0app0secs %d\n",shm->lh0app0secs); 
if (strlen(s) == 0 || strcmp(s,"lh0app0pause")==0)
printf("lh0app0pause %d\n",shm->lh0app0pause); 
if (strlen(s) == 0 || strcmp(s,"lh0app0crit")==0)
printf("lh0app0crit %d\n",shm->lh0app0crit); 
if (strlen(s) == 0 || strcmp(s,"lh0app1name")==0)
printf("lh0app1name %s\n",shm->lh0app1name); 
if (strlen(s) == 0 || strcmp(s,"lh0app1stat")==0)
printf("lh0app1stat %d\n",shm->lh0app1stat); 
if (strlen(s) == 0 || strcmp(s,"lh0app1start")==0)
printf("lh0app1start %s\n",shm->lh0app1start); 
if (strlen(s) == 0 || strcmp(s,"lh0app1stop")==0)
printf("lh0app1stop %s\n",shm->lh0app1stop); 
if (strlen(s) == 0 || strcmp(s,"lh0app1test")==0)
printf("lh0app1test %s\n",shm->lh0app1test); 
if (strlen(s) == 0 || strcmp(s,"lh0app1timeo")==0)
printf("lh0app1timeo %d\n",shm->lh0app1timeo); 
if (strlen(s) == 0 || strcmp(s,"lh0app1testtimeo")==0)
printf("lh0app1testtimeo %d\n",shm->lh0app1testtimeo); 
if (strlen(s) == 0 || strcmp(s,"lh0app1remtest")==0)
printf("lh0app1remtest %s\n",shm->lh0app1remtest);
if (strlen(s) == 0 || strcmp(s,"lh0app1remtesttimeo")==0)
printf("lh0app1remtesttimeo %d\n",shm->lh0app1remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh0app1remtesttimeo")==0)
printf("lh0app1remtesttimeo %d\n",shm->lh0app1remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh0app1remnode")==0)
printf("lh0app1remnode %d\n",shm->lh0app1remnode);
if (strlen(s) == 0 || strcmp(s,"lh0app1secs")==0)
printf("lh0app1secs %d\n",shm->lh0app1secs); 
if (strlen(s) == 0 || strcmp(s,"lh0app1pause")==0)
printf("lh0app1pause %d\n",shm->lh0app1pause); 
if (strlen(s) == 0 || strcmp(s,"lh0app1crit")==0)
printf("lh0app1crit %d\n",shm->lh0app1crit); 
if (strlen(s) == 0 || strcmp(s,"lh0app2name")==0)
printf("lh0app2name %s\n",shm->lh0app2name); 
if (strlen(s) == 0 || strcmp(s,"lh0app2stat")==0)
printf("lh0app2stat %d\n",shm->lh0app2stat); 
if (strlen(s) == 0 || strcmp(s,"lh0app2start")==0)
printf("lh0app2start %s\n",shm->lh0app2start); 
if (strlen(s) == 0 || strcmp(s,"lh0app2stop")==0)
printf("lh0app2stop %s\n",shm->lh0app2stop); 
if (strlen(s) == 0 || strcmp(s,"lh0app2test")==0)
printf("lh0app2test %s\n",shm->lh0app2test); 
if (strlen(s) == 0 || strcmp(s,"lh0app2timeo")==0)
printf("lh0app2timeo %d\n",shm->lh0app2timeo); 
if (strlen(s) == 0 || strcmp(s,"lh0app2testtimeo")==0)
printf("lh0app2testtimeo %d\n",shm->lh0app2testtimeo); 
if (strlen(s) == 0 || strcmp(s,"lh0app2remtest")==0)
printf("lh0app2remtest %s\n",shm->lh0app2remtest);
if (strlen(s) == 0 || strcmp(s,"lh0app2remtesttimeo")==0)
printf("lh0app2remtesttimeo %d\n",shm->lh0app2remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh0app2remtesttimeo")==0)
printf("lh0app2remtesttimeo %d\n",shm->lh0app2remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh0app2remnode")==0)
printf("lh0app2remnode %d\n",shm->lh0app2remnode);
if (strlen(s) == 0 || strcmp(s,"lh0app2secs")==0)
printf("lh0app2secs %d\n",shm->lh0app2secs); 
if (strlen(s) == 0 || strcmp(s,"lh0app2pause")==0)
printf("lh0app2pause %d\n",shm->lh0app2pause); 
if (strlen(s) == 0 || strcmp(s,"lh0app2crit")==0)
printf("lh0app2crit %d\n",shm->lh0app2crit); 
if (strlen(s) == 0 || strcmp(s,"lh0app3name")==0)
printf("lh0app3name %s\n",shm->lh0app3name); 
if (strlen(s) == 0 || strcmp(s,"lh0app3stat")==0)
printf("lh0app3stat %d\n",shm->lh0app3stat); 
if (strlen(s) == 0 || strcmp(s,"lh0app3start")==0)
printf("lh0app3start %s\n",shm->lh0app3start); 
if (strlen(s) == 0 || strcmp(s,"lh0app3stop")==0)
printf("lh0app3stop %s\n",shm->lh0app3stop); 
if (strlen(s) == 0 || strcmp(s,"lh0app3test")==0)
printf("lh0app3test %s\n",shm->lh0app3test); 
if (strlen(s) == 0 || strcmp(s,"lh0app3timeo")==0)
printf("lh0app3timeo %d\n",shm->lh0app3timeo); 
if (strlen(s) == 0 || strcmp(s,"lh0app3testtimeo")==0)
printf("lh0app3testtimeo %d\n",shm->lh0app3testtimeo); 
if (strlen(s) == 0 || strcmp(s,"lh0app3secs")==0)
printf("lh0app3secs %d\n",shm->lh0app3secs); 
if (strlen(s) == 0 || strcmp(s,"lh0app3remtest")==0)
printf("lh0app3remtest %s\n",shm->lh0app3remtest);
if (strlen(s) == 0 || strcmp(s,"lh0app3remtesttimeo")==0)
printf("lh0app3remtesttimeo %d\n",shm->lh0app3remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh0app3remtesttimeo")==0)
printf("lh0app3remtesttimeo %d\n",shm->lh0app3remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh0app3remnode")==0)
printf("lh0app3remnode %d\n",shm->lh0app3remnode);
if (strlen(s) == 0 || strcmp(s,"lh0app3pause")==0)
printf("lh0app3pause %d\n",shm->lh0app3pause); 
if (strlen(s) == 0 || strcmp(s,"lh0app3crit")==0)
printf("lh0app3crit %d\n",shm->lh0app3crit); 


if (strlen(s) == 0 || strcmp(s,"lh1name")==0)
printf("lh1name %s\n",shm->lh1name); 
if (strlen(s) == 0 || strcmp(s,"lh1path")==0)
printf("lh1path %s\n",shm->lh1path); 
if (strlen(s) == 0 || strcmp(s,"lh1stat")==0)
printf("lh1stat %d\n",shm->lh1stat); 
if (strlen(s) == 0 || strcmp(s,"lh1node")==0)
printf("lh1node %d\n",shm->lh1node); 
if (strlen(s) == 0 || strcmp(s,"lh1fallback")==0)
printf("lh1fallback %d\n",shm->lh1fallback); 
if (strlen(s) == 0 || strcmp(s,"lh1failto")==0)
printf("lh1failto %d\n",shm->lh1failto); 
if (strlen(s) == 0 || strcmp(s,"lh1timeo")==0)
printf("lh1timeo %d\n",shm->lh1timeo); 
if (strlen(s) == 0 || strcmp(s,"lh1failing")==0)
printf("lh1failing %d\n",shm->lh1failing); 
if (strlen(s) == 0 || strcmp(s,"lh1app0name")==0)
printf("lh1app0name %s\n",shm->lh1app0name); 
if (strlen(s) == 0 || strcmp(s,"lh1app0stat")==0)
printf("lh1app0stat %d\n",shm->lh1app0stat); 
if (strlen(s) == 0 || strcmp(s,"lh1app0start")==0)
printf("lh1app0start %s\n",shm->lh1app0start); 
if (strlen(s) == 0 || strcmp(s,"lh1app0stop")==0)
printf("lh1app0stop %s\n",shm->lh1app0stop); 
if (strlen(s) == 0 || strcmp(s,"lh1app0test")==0)
printf("lh1app0test %s\n",shm->lh1app0test); 
if (strlen(s) == 0 || strcmp(s,"lh1app0timeo")==0)
printf("lh1app0timeo %d\n",shm->lh1app0timeo); 
if (strlen(s) == 0 || strcmp(s,"lh1app0testtimeo")==0)
printf("lh1app0testtimeo %d\n",shm->lh1app0testtimeo); 
if (strlen(s) == 0 || strcmp(s,"lh1app0remtest")==0)
printf("lh1app0remtest %s\n",shm->lh1app0remtest);
if (strlen(s) == 0 || strcmp(s,"lh1app0remtesttimeo")==0)
printf("lh1app0remtesttimeo %d\n",shm->lh1app0remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh1app0remtesttimeo")==0)
printf("lh1app0remtesttimeo %d\n",shm->lh1app0remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh1app0remnode")==0)
printf("lh1app0remnode %d\n",shm->lh1app0remnode);
if (strlen(s) == 0 || strcmp(s,"lh1app0secs")==0)
printf("lh1app0secs %d\n",shm->lh1app0secs); 
if (strlen(s) == 0 || strcmp(s,"lh1app0pause")==0)
printf("lh1app0pause %d\n",shm->lh1app0pause); 
if (strlen(s) == 0 || strcmp(s,"lh1app0crit")==0)
printf("lh1app0crit %d\n",shm->lh1app0crit); 
if (strlen(s) == 0 || strcmp(s,"lh1app1name")==0)
printf("lh1app1name %s\n",shm->lh1app1name); 
if (strlen(s) == 0 || strcmp(s,"lh1app1stat")==0)
printf("lh1app1stat %d\n",shm->lh1app1stat); 
if (strlen(s) == 0 || strcmp(s,"lh1app1start")==0)
printf("lh1app1start %s\n",shm->lh1app1start); 
if (strlen(s) == 0 || strcmp(s,"lh1app1stop")==0)
printf("lh1app1stop %s\n",shm->lh1app1stop); 
if (strlen(s) == 0 || strcmp(s,"lh1app1test")==0)
printf("lh1app1test %s\n",shm->lh1app1test); 
if (strlen(s) == 0 || strcmp(s,"lh1app1timeo")==0)
printf("lh1app1timeo %d\n",shm->lh1app1timeo); 
if (strlen(s) == 0 || strcmp(s,"lh1app1testtimeo")==0)
printf("lh1app1testtimeo %d\n",shm->lh1app1testtimeo); 
if (strlen(s) == 0 || strcmp(s,"lh1app1remtest")==0)
printf("lh1app1remtest %s\n",shm->lh1app1remtest);
if (strlen(s) == 0 || strcmp(s,"lh1app1remtesttimeo")==0)
printf("lh1app1remtesttimeo %d\n",shm->lh1app1remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh1app1remtesttimeo")==0)
printf("lh1app1remtesttimeo %d\n",shm->lh1app1remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh1app1remnode")==0)
printf("lh1app1remnode %d\n",shm->lh1app1remnode);
if (strlen(s) == 0 || strcmp(s,"lh1app1secs")==0)
printf("lh1app1secs %d\n",shm->lh1app1secs); 
if (strlen(s) == 0 || strcmp(s,"lh1app1pause")==0)
printf("lh1app1pause %d\n",shm->lh1app1pause); 
if (strlen(s) == 0 || strcmp(s,"lh1app1crit")==0)
printf("lh1app1crit %d\n",shm->lh1app1crit); 
if (strlen(s) == 0 || strcmp(s,"lh1app2name")==0)
printf("lh1app2name %s\n",shm->lh1app2name); 
if (strlen(s) == 0 || strcmp(s,"lh1app2stat")==0)
printf("lh1app2stat %d\n",shm->lh1app2stat); 
if (strlen(s) == 0 || strcmp(s,"lh1app2start")==0)
printf("lh1app2start %s\n",shm->lh1app2start); 
if (strlen(s) == 0 || strcmp(s,"lh1app2stop")==0)
printf("lh1app2stop %s\n",shm->lh1app2stop); 
if (strlen(s) == 0 || strcmp(s,"lh1app2test")==0)
printf("lh1app2test %s\n",shm->lh1app2test); 
if (strlen(s) == 0 || strcmp(s,"lh1app2timeo")==0)
printf("lh1app2timeo %d\n",shm->lh1app2timeo); 
if (strlen(s) == 0 || strcmp(s,"lh1app2testtimeo")==0)
printf("lh1app2testtimeo %d\n",shm->lh1app2testtimeo); 
if (strlen(s) == 0 || strcmp(s,"lh1app2remtest")==0)
printf("lh1app2remtest %s\n",shm->lh1app2remtest);
if (strlen(s) == 0 || strcmp(s,"lh1app2remtesttimeo")==0)
printf("lh1app2remtesttimeo %d\n",shm->lh1app2remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh1app2remtesttimeo")==0)
printf("lh1app2remtesttimeo %d\n",shm->lh1app2remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh1app2remnode")==0)
printf("lh1app2remnode %d\n",shm->lh1app2remnode);
if (strlen(s) == 0 || strcmp(s,"lh1app2secs")==0)
printf("lh1app2secs %d\n",shm->lh1app2secs); 
if (strlen(s) == 0 || strcmp(s,"lh1app2pause")==0)
printf("lh1app2pause %d\n",shm->lh1app2pause); 
if (strlen(s) == 0 || strcmp(s,"lh1app2crit")==0)
printf("lh1app2crit %d\n",shm->lh1app2crit); 
if (strlen(s) == 0 || strcmp(s,"lh1app3name")==0)
printf("lh1app3name %s\n",shm->lh1app3name); 
if (strlen(s) == 0 || strcmp(s,"lh1app3stat")==0)
printf("lh1app3stat %d\n",shm->lh1app3stat); 
if (strlen(s) == 0 || strcmp(s,"lh1app3start")==0)
printf("lh1app3start %s\n",shm->lh1app3start); 
if (strlen(s) == 0 || strcmp(s,"lh1app3stop")==0)
printf("lh1app3stop %s\n",shm->lh1app3stop); 
if (strlen(s) == 0 || strcmp(s,"lh1app3test")==0)
printf("lh1app3test %s\n",shm->lh1app3test); 
if (strlen(s) == 0 || strcmp(s,"lh1app3timeo")==0)
printf("lh1app3timeo %d\n",shm->lh1app3timeo); 
if (strlen(s) == 0 || strcmp(s,"lh1app3testtimeo")==0)
printf("lh1app3testtimeo %d\n",shm->lh1app3testtimeo); 
if (strlen(s) == 0 || strcmp(s,"lh1app3remtest")==0)
printf("lh1app3remtest %s\n",shm->lh1app3remtest);
if (strlen(s) == 0 || strcmp(s,"lh1app3remtesttimeo")==0)
printf("lh1app3remtesttimeo %d\n",shm->lh1app3remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh1app3remtesttimeo")==0)
printf("lh1app3remtesttimeo %d\n",shm->lh1app3remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh1app3remnode")==0)
printf("lh1app3remnode %d\n",shm->lh1app3remnode);
if (strlen(s) == 0 || strcmp(s,"lh1app3secs")==0)
printf("lh1app3secs %d\n",shm->lh1app3secs); 
if (strlen(s) == 0 || strcmp(s,"lh1app3pause")==0)
printf("lh1app3pause %d\n",shm->lh1app3pause); 
if (strlen(s) == 0 || strcmp(s,"lh1app3crit")==0)
printf("lh1app3crit %d\n",shm->lh1app3crit); 


if (strlen(s) == 0 || strcmp(s,"lh2name")==0)
printf("lh2name %s\n",shm->lh2name); 
if (strlen(s) == 0 || strcmp(s,"lh2path")==0)
printf("lh2path %s\n",shm->lh2path); 
if (strlen(s) == 0 || strcmp(s,"lh2stat")==0)
printf("lh2stat %d\n",shm->lh2stat); 
if (strlen(s) == 0 || strcmp(s,"lh2node")==0)
printf("lh2node %d\n",shm->lh2node); 
if (strlen(s) == 0 || strcmp(s,"lh2fallback")==0)
printf("lh2fallback %d\n",shm->lh2fallback); 
if (strlen(s) == 0 || strcmp(s,"lh2failto")==0)
printf("lh2failto %d\n",shm->lh2failto); 
if (strlen(s) == 0 || strcmp(s,"lh2timeo")==0)
printf("lh2timeo %d\n",shm->lh2timeo); 
if (strlen(s) == 0 || strcmp(s,"lh2failing")==0)
printf("lh2failing %d\n",shm->lh2failing); 
if (strlen(s) == 0 || strcmp(s,"lh2app0name")==0)
printf("lh2app0name %s\n",shm->lh2app0name); 
if (strlen(s) == 0 || strcmp(s,"lh2app0stat")==0)
printf("lh2app0stat %d\n",shm->lh2app0stat); 
if (strlen(s) == 0 || strcmp(s,"lh2app0start")==0)
printf("lh2app0start %s\n",shm->lh2app0start); 
if (strlen(s) == 0 || strcmp(s,"lh2app0stop")==0)
printf("lh2app0stop %s\n",shm->lh2app0stop); 
if (strlen(s) == 0 || strcmp(s,"lh2app0test")==0)
printf("lh2app0test %s\n",shm->lh2app0test); 
if (strlen(s) == 0 || strcmp(s,"lh2app0timeo")==0)
printf("lh2app0timeo %d\n",shm->lh2app0timeo); 
if (strlen(s) == 0 || strcmp(s,"lh2app0testtimeo")==0)
printf("lh2app0testtimeo %d\n",shm->lh2app0testtimeo); 
if (strlen(s) == 0 || strcmp(s,"lh2app0remtest")==0)
printf("lh2app0remtest %s\n",shm->lh2app0remtest);
if (strlen(s) == 0 || strcmp(s,"lh2app0remtesttimeo")==0)
printf("lh2app0remtesttimeo %d\n",shm->lh2app0remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh2app0remtesttimeo")==0)
printf("lh2app0remtesttimeo %d\n",shm->lh2app0remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh2app0remnode")==0)
printf("lh2app0remnode %d\n",shm->lh2app0remnode);
if (strlen(s) == 0 || strcmp(s,"lh2app0secs")==0)
printf("lh2app0secs %d\n",shm->lh2app0secs); 
if (strlen(s) == 0 || strcmp(s,"lh2app0pause")==0)
printf("lh2app0pause %d\n",shm->lh2app0pause); 
if (strlen(s) == 0 || strcmp(s,"lh2app0crit")==0)
printf("lh2app0crit %d\n",shm->lh2app0crit); 
if (strlen(s) == 0 || strcmp(s,"lh2app1name")==0)
printf("lh2app1name %s\n",shm->lh2app1name); 
if (strlen(s) == 0 || strcmp(s,"lh2app1stat")==0)
printf("lh2app1stat %d\n",shm->lh2app1stat); 
if (strlen(s) == 0 || strcmp(s,"lh2app1start")==0)
printf("lh2app1start %s\n",shm->lh2app1start); 
if (strlen(s) == 0 || strcmp(s,"lh2app1stop")==0)
printf("lh2app1stop %s\n",shm->lh2app1stop); 
if (strlen(s) == 0 || strcmp(s,"lh2app1test")==0)
printf("lh2app1test %s\n",shm->lh2app1test); 
if (strlen(s) == 0 || strcmp(s,"lh2app1timeo")==0)
printf("lh2app1timeo %d\n",shm->lh2app1timeo); 
if (strlen(s) == 0 || strcmp(s,"lh2app1testtimeo")==0)
printf("lh2app1testtimeo %d\n",shm->lh2app1testtimeo); 
if (strlen(s) == 0 || strcmp(s,"lh2app1remtest")==0)
printf("lh2app1remtest %s\n",shm->lh2app1remtest);
if (strlen(s) == 0 || strcmp(s,"lh2app1remtesttimeo")==0)
printf("lh2app1remtesttimeo %d\n",shm->lh2app1remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh2app1remtesttimeo")==0)
printf("lh2app1remtesttimeo %d\n",shm->lh2app1remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh2app1remnode")==0)
printf("lh2app1remnode %d\n",shm->lh2app1remnode);
if (strlen(s) == 0 || strcmp(s,"lh2app1secs")==0)
printf("lh2app1secs %d\n",shm->lh2app1secs); 
if (strlen(s) == 0 || strcmp(s,"lh2app1pause")==0)
printf("lh2app1pause %d\n",shm->lh2app1pause); 
if (strlen(s) == 0 || strcmp(s,"lh2app1crit")==0)
printf("lh2app1crit %d\n",shm->lh2app1crit); 
if (strlen(s) == 0 || strcmp(s,"lh2app2name")==0)
printf("lh2app2name %s\n",shm->lh2app2name); 
if (strlen(s) == 0 || strcmp(s,"lh2app2stat")==0)
printf("lh2app2stat %d\n",shm->lh2app2stat); 
if (strlen(s) == 0 || strcmp(s,"lh2app2start")==0)
printf("lh2app2start %s\n",shm->lh2app2start); 
if (strlen(s) == 0 || strcmp(s,"lh2app2stop")==0)
printf("lh2app2stop %s\n",shm->lh2app2stop); 
if (strlen(s) == 0 || strcmp(s,"lh2app2test")==0)
printf("lh2app2test %s\n",shm->lh2app2test); 
if (strlen(s) == 0 || strcmp(s,"lh2app2timeo")==0)
printf("lh2app2timeo %d\n",shm->lh2app2timeo); 
if (strlen(s) == 0 || strcmp(s,"lh2app2testtimeo")==0)
printf("lh2app2testtimeo %d\n",shm->lh2app2testtimeo); 
if (strlen(s) == 0 || strcmp(s,"lh2app2remtest")==0)
printf("lh2app2remtest %s\n",shm->lh2app2remtest);
if (strlen(s) == 0 || strcmp(s,"lh2app2remtesttimeo")==0)
printf("lh2app2remtesttimeo %d\n",shm->lh2app2remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh2app2remtesttimeo")==0)
printf("lh2app2remtesttimeo %d\n",shm->lh2app2remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh2app2remnode")==0)
printf("lh2app2remnode %d\n",shm->lh2app2remnode);
if (strlen(s) == 0 || strcmp(s,"lh2app2secs")==0)
printf("lh2app2secs %d\n",shm->lh2app2secs); 
if (strlen(s) == 0 || strcmp(s,"lh2app2pause")==0)
printf("lh2app2pause %d\n",shm->lh2app2pause); 
if (strlen(s) == 0 || strcmp(s,"lh2app2crit")==0)
printf("lh2app2crit %d\n",shm->lh2app2crit); 
if (strlen(s) == 0 || strcmp(s,"lh2app3name")==0)
printf("lh2app3name %s\n",shm->lh2app3name); 
if (strlen(s) == 0 || strcmp(s,"lh2app3stat")==0)
printf("lh2app3stat %d\n",shm->lh2app3stat); 
if (strlen(s) == 0 || strcmp(s,"lh2app3start")==0)
printf("lh2app3start %s\n",shm->lh2app3start); 
if (strlen(s) == 0 || strcmp(s,"lh2app3stop")==0)
printf("lh2app3stop %s\n",shm->lh2app3stop); 
if (strlen(s) == 0 || strcmp(s,"lh2app3test")==0)
printf("lh2app3test %s\n",shm->lh2app3test); 
if (strlen(s) == 0 || strcmp(s,"lh2app3timeo")==0)
printf("lh2app3timeo %d\n",shm->lh2app3timeo); 
if (strlen(s) == 0 || strcmp(s,"lh2app3testtimeo")==0)
printf("lh2app3testtimeo %d\n",shm->lh2app3testtimeo); 
if (strlen(s) == 0 || strcmp(s,"lh2app3remtest")==0)
printf("lh2app3remtest %s\n",shm->lh2app3remtest);
if (strlen(s) == 0 || strcmp(s,"lh2app3remtesttimeo")==0)
printf("lh2app3remtesttimeo %d\n",shm->lh2app3remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh2app3remtesttimeo")==0)
printf("lh2app3remtesttimeo %d\n",shm->lh2app3remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh2app3remnode")==0)
printf("lh2app3remnode %d\n",shm->lh2app3remnode);
if (strlen(s) == 0 || strcmp(s,"lh2app3secs")==0)
printf("lh2app3secs %d\n",shm->lh2app3secs); 
if (strlen(s) == 0 || strcmp(s,"lh2app3pause")==0)
printf("lh2app3pause %d\n",shm->lh2app3pause); 
if (strlen(s) == 0 || strcmp(s,"lh2app3crit")==0)
printf("lh2app3crit %d\n",shm->lh2app3crit); 


if (strlen(s) == 0 || strcmp(s,"lh3name")==0)
printf("lh3name %s\n",shm->lh3name); 
if (strlen(s) == 0 || strcmp(s,"lh3path")==0)
printf("lh3path %s\n",shm->lh3path); 
if (strlen(s) == 0 || strcmp(s,"lh3stat")==0)
printf("lh3stat %d\n",shm->lh3stat); 
if (strlen(s) == 0 || strcmp(s,"lh3node")==0)
printf("lh3node %d\n",shm->lh3node); 
if (strlen(s) == 0 || strcmp(s,"lh3fallback")==0)
printf("lh3fallback %d\n",shm->lh3fallback); 
if (strlen(s) == 0 || strcmp(s,"lh3failto")==0)
printf("lh3failto %d\n",shm->lh3failto); 
if (strlen(s) == 0 || strcmp(s,"lh3timeo")==0)
printf("lh3timeo %d\n",shm->lh3timeo); 
if (strlen(s) == 0 || strcmp(s,"lh3failing")==0)
printf("lh3failing %d\n",shm->lh3failing); 
if (strlen(s) == 0 || strcmp(s,"lh3app0name")==0)
printf("lh3app0name %s\n",shm->lh3app0name); 
if (strlen(s) == 0 || strcmp(s,"lh3app0stat")==0)
printf("lh3app0stat %d\n",shm->lh3app0stat); 
if (strlen(s) == 0 || strcmp(s,"lh3app0start")==0)
printf("lh3app0start %s\n",shm->lh3app0start); 
if (strlen(s) == 0 || strcmp(s,"lh3app0stop")==0)
printf("lh3app0stop %s\n",shm->lh3app0stop); 
if (strlen(s) == 0 || strcmp(s,"lh3app0test")==0)
printf("lh3app0test %s\n",shm->lh3app0test); 
if (strlen(s) == 0 || strcmp(s,"lh3app0timeo")==0)
printf("lh3app0timeo %d\n",shm->lh3app0timeo); 
if (strlen(s) == 0 || strcmp(s,"lh3app0testtimeo")==0)
printf("lh3app0testtimeo %d\n",shm->lh3app0testtimeo); 
if (strlen(s) == 0 || strcmp(s,"lh3app0remtest")==0)
printf("lh3app0remtest %s\n",shm->lh3app0remtest);
if (strlen(s) == 0 || strcmp(s,"lh3app0remtesttimeo")==0)
printf("lh3app0remtesttimeo %d\n",shm->lh3app0remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh3app0remtesttimeo")==0)
printf("lh3app0remtesttimeo %d\n",shm->lh3app0remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh3app0remnode")==0)
printf("lh3app0remnode %d\n",shm->lh3app0remnode);
if (strlen(s) == 0 || strcmp(s,"lh3app0secs")==0)
printf("lh3app0secs %d\n",shm->lh3app0secs); 
if (strlen(s) == 0 || strcmp(s,"lh3app0pause")==0)
printf("lh3app0pause %d\n",shm->lh3app0pause); 
if (strlen(s) == 0 || strcmp(s,"lh3app0crit")==0)
printf("lh3app0crit %d\n",shm->lh3app0crit); 
if (strlen(s) == 0 || strcmp(s,"lh3app1name")==0)
printf("lh3app1name %s\n",shm->lh3app1name); 
if (strlen(s) == 0 || strcmp(s,"lh3app1stat")==0)
printf("lh3app1stat %d\n",shm->lh3app1stat); 
if (strlen(s) == 0 || strcmp(s,"lh3app1start")==0)
printf("lh3app1start %s\n",shm->lh3app1start); 
if (strlen(s) == 0 || strcmp(s,"lh3app1stop")==0)
printf("lh3app1stop %s\n",shm->lh3app1stop); 
if (strlen(s) == 0 || strcmp(s,"lh3app1test")==0)
printf("lh3app1test %s\n",shm->lh3app1test); 
if (strlen(s) == 0 || strcmp(s,"lh3app1timeo")==0)
printf("lh3app1timeo %d\n",shm->lh3app1timeo); 
if (strlen(s) == 0 || strcmp(s,"lh3app1testtimeo")==0)
printf("lh3app1testtimeo %d\n",shm->lh3app1testtimeo); 
if (strlen(s) == 0 || strcmp(s,"lh3app1remtest")==0)
printf("lh3app1remtest %s\n",shm->lh3app1remtest);
if (strlen(s) == 0 || strcmp(s,"lh3app1remtesttimeo")==0)
printf("lh3app1remtesttimeo %d\n",shm->lh3app1remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh3app1remtesttimeo")==0)
printf("lh3app1remtesttimeo %d\n",shm->lh3app1remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh3app1remnode")==0)
printf("lh3app1remnode %d\n",shm->lh3app1remnode);
if (strlen(s) == 0 || strcmp(s,"lh3app1secs")==0)
printf("lh3app1secs %d\n",shm->lh3app1secs); 
if (strlen(s) == 0 || strcmp(s,"lh3app1pause")==0)
printf("lh3app1pause %d\n",shm->lh3app1pause); 
if (strlen(s) == 0 || strcmp(s,"lh3app1crit")==0)
printf("lh3app1crit %d\n",shm->lh3app1crit); 
if (strlen(s) == 0 || strcmp(s,"lh3app2name")==0)
printf("lh3app2name %s\n",shm->lh3app2name); 
if (strlen(s) == 0 || strcmp(s,"lh3app2stat")==0)
printf("lh3app2stat %d\n",shm->lh3app2stat); 
if (strlen(s) == 0 || strcmp(s,"lh3app2start")==0)
printf("lh3app2start %s\n",shm->lh3app2start); 
if (strlen(s) == 0 || strcmp(s,"lh3app2stop")==0)
printf("lh3app2stop %s\n",shm->lh3app2stop); 
if (strlen(s) == 0 || strcmp(s,"lh3app2test")==0)
printf("lh3app2test %s\n",shm->lh3app2test); 
if (strlen(s) == 0 || strcmp(s,"lh3app2timeo")==0)
printf("lh3app2timeo %d\n",shm->lh3app2timeo); 
if (strlen(s) == 0 || strcmp(s,"lh3app2testtimeo")==0)
printf("lh3app2testtimeo %d\n",shm->lh3app2testtimeo); 
if (strlen(s) == 0 || strcmp(s,"lh3app2remtest")==0)
printf("lh3app2remtest %s\n",shm->lh3app2remtest);
if (strlen(s) == 0 || strcmp(s,"lh3app2remtesttimeo")==0)
printf("lh3app2remtesttimeo %d\n",shm->lh3app2remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh3app2remtesttimeo")==0)
printf("lh3app2remtesttimeo %d\n",shm->lh3app2remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh3app2remnode")==0)
printf("lh3app2remnode %d\n",shm->lh3app2remnode);
if (strlen(s) == 0 || strcmp(s,"lh3app2secs")==0)
printf("lh3app2secs %d\n",shm->lh3app2secs); 
if (strlen(s) == 0 || strcmp(s,"lh3app2pause")==0)
printf("lh3app2pause %d\n",shm->lh3app2pause); 
if (strlen(s) == 0 || strcmp(s,"lh3app2crit")==0)
printf("lh3app2crit %d\n",shm->lh3app2crit); 
if (strlen(s) == 0 || strcmp(s,"lh3app3name")==0)
printf("lh3app3name %s\n",shm->lh3app3name); 
if (strlen(s) == 0 || strcmp(s,"lh3app3stat")==0)
printf("lh3app3stat %d\n",shm->lh3app3stat); 
if (strlen(s) == 0 || strcmp(s,"lh3app3start")==0)
printf("lh3app3start %s\n",shm->lh3app3start); 
if (strlen(s) == 0 || strcmp(s,"lh3app3stop")==0)
printf("lh3app3stop %s\n",shm->lh3app3stop); 
if (strlen(s) == 0 || strcmp(s,"lh3app3test")==0)
printf("lh3app3test %s\n",shm->lh3app3test); 
if (strlen(s) == 0 || strcmp(s,"lh3app3timeo")==0)
printf("lh3app3timeo %d\n",shm->lh3app3timeo); 
if (strlen(s) == 0 || strcmp(s,"lh3app3testtimeo")==0)
printf("lh3app3testtimeo %d\n",shm->lh3app3testtimeo); 
if (strlen(s) == 0 || strcmp(s,"lh3app3remtest")==0)
printf("lh3app3remtest %s\n",shm->lh3app3remtest);
if (strlen(s) == 0 || strcmp(s,"lh3app3remtesttimeo")==0)
printf("lh3app3remtesttimeo %d\n",shm->lh3app3remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh3app3remtesttimeo")==0)
printf("lh3app3remtesttimeo %d\n",shm->lh3app3remtesttimeo);
if (strlen(s) == 0 || strcmp(s,"lh3app3remnode")==0)
printf("lh3app3remnode %d\n",shm->lh3app3remnode);
if (strlen(s) == 0 || strcmp(s,"lh3app3secs")==0)
printf("lh3app3secs %d\n",shm->lh3app3secs); 
if (strlen(s) == 0 || strcmp(s,"lh3app3pause")==0)
printf("lh3app3pause %d\n",shm->lh3app3pause); 
if (strlen(s) == 0 || strcmp(s,"lh3app3crit")==0)
printf("lh3app3crit %d\n",shm->lh3app3crit); 


if (strlen(s) == 0 || strcmp(s,"cf")==0)
printf("cstat %d\n",shm->cstat); 
}


/*
 * turn apps on or off
 */
fstappswitch(int lhnum,int appnum, int *appstat, char *appname, 
		int modein, char *startscript,char *stopscript,
		int *apptestres, int *apptesttimeo, int *apptimeo
		){
FILE *f;
char fname[BUFSIZ]="";
char msg2[BUFSIZ]="";
time_t now;
int mode;

mode=modein;

if (*appstat == mode) {
	sprintf(msg,"%s Already Done. Ignored.",appname);
	logit(msg);
	exit(1);
}
/*
time(&now);
sprintf(msg,"/tmp/lh%dapp%d%d",lhnum,appnum,now);
f=fopen(msg,"w");
if (f == NULL) {
	fprintf(stderr,"Unable to open %s",msg);
	exit(1);
}
*/
if (mode == ON) {
	sprintf(msg2,"lh%dapp%dtestres 0",lhnum,appnum);
	fsthathere(msg2);
	fsthahere(msg2);
	sprintf(msg2,"lh%dapp%dtesttimeo 0",lhnum,appnum);
	fsthathere(msg2);
	fsthahere(msg2);
        sprintf(msg2,"lh%dapp%dremtesttimeo 0",lhnum,appnum);
        fsthathere(msg2);
        fsthahere(msg2);
	sprintf(msg2,"lh%dapp%dstat %d",lhnum,appnum,GUP);
	fsthathere(msg2);
	fsthahere(msg2);

} else if (mode == OFF) {	
	sprintf(msg2,"lh%dapp%dtesttimeo 0",lhnum,appnum);
	fsthathere(msg2);
	fsthahere(msg2);
        sprintf(msg2,"lh%dapp%dremtesttimeo 0",lhnum,appnum);
        fsthathere(msg2);
        fsthahere(msg2);
	sprintf(msg2,"lh%dapp%dstat %d",lhnum,appnum,GDOWN);
	fsthathere(msg2);
	fsthahere(msg2);
} else if (mode == NOCHECK) {	
	sprintf(msg2,"lh%dapp%dtesttimeo 0",lhnum,appnum);
	fsthathere(msg2);
	fsthahere(msg2);
        sprintf(msg2,"lh%dapp%dremtesttimeo 0",lhnum,appnum);
        fsthathere(msg2);
        fsthahere(msg2);
	sprintf(msg2,"lh%dapp%dstat %d",lhnum,appnum,NOCHECK);
	fsthathere(msg2);
	fsthahere(msg2);
}
}


/*
 */

fstlocate(int lhnum, char *lhname, int tonode, 
	int *lhnode,int *lhstat, int *lhtimeo,
	char *app0name,
	char *app1name,
	char *app2name,
	char *app3name,
	int *app0testres,
	int *app1testres,
	int *app2testres,
	int *app3testres
	) {
int goingdown=NO;

if (strlen(lhname) == 0 ) {
	sprintf(msg,"VirtualHost not Defined.");
	logit(msg);
	exit(1);
}
if ( (tonode == *lhnode) && (*lhstat == UP || *lhstat == GUP)) {
	sprintf(msg,"VirtualHost Currently Located on Node.");
	logit(msg);
	exit(1);
}

if (shm->verbose == YES) {
	sprintf(msg,"Moving %s to %d",lhname,tonode);
	logit(msg);
}

if (*lhstat == UP) {
	sprintf(msg,"lh%dstat %d",lhnum,GDOWN);
	fsthathere(msg);
	fsthahere(msg);
	goingdown = YES;
	while (*lhstat == UP 
		|| *lhstat == GDOWN 
		|| *lhtimeo != 0) {
		sleep(1);
	}
}
if (tonode != 9 && ! (goingdown == YES && *lhstat == FAILED)) {
	sprintf(msg,"lh%dapp0testres 0",lhnum); 
	fsthathere(msg);
	fsthahere(msg);

	sprintf(msg,"lh%dapp1testres 0",lhnum); 
	fsthathere(msg);
	fsthahere(msg);

	sprintf(msg,"lh%dapp2testres 0",lhnum); 
	fsthathere(msg);
	fsthahere(msg);

	sprintf(msg,"lh%dapp3testres 0",lhnum); 
	fsthathere(msg);
	fsthahere(msg);

	sprintf(msg,"lh%dnode %d",lhnum,tonode);
	fsthathere(msg);
	fsthahere(msg);

	sprintf(msg,"lh%dstat %d",lhnum,GUP);
	fsthathere(msg);
	fsthahere(msg);
}
}


fstlhprint(int lhnum,char *lhname,int lhnode,int lhstat,int lhfailing,char *lhpath,int lhfallback,int lhfailto,
char *app0name,int app0stat,int app0testres,int app0testtimeo,int app0crit,
char *app1name,int app1stat,int app1testres,int app1testtimeo,int app1crit,
char *app2name,int app2stat,int app2testres,int app2testtimeo,int app2crit,
char *app3name,int app3stat,int app3testres,int app3testtimeo,int app3crit) {

char s0[64]="";
char s1[64]="";
char s2[64]="";
char s3[64]="";
int x;

strlcpy(s0,lhname,15);
sprintf(s1,"%s/%s",fsthastat(lhstat),(lhfailing == FAILED) ? "YES": "NO");
printf("------------------------------------------------------");
for (x=0; x<strlen(lhpath); x++)
	printf("-");
printf("\n");
printf("Num %12.12s     %8.8s %10.10s   %8.8s   %s\n","VirtualHost","CurrNode","Stat Fail","Back/To","Path");
printf("%1d %15.15s    %8d %10.10s   %4d %3d   %s\n",lhnum,s0,lhnode,s1,lhfallback,lhfailto,lhpath);
printf("------------------------------------------------------");
for (x=0; x<strlen(lhpath); x++)
	printf("-");
printf("\n");
printf("%16.16s : %10d %10d %10d %10d\n","Application",0,1,2,3);
printf("%16.16s : %10s %10s %10s %10s\n","Name",app0name,app1name,app2name,app3name);
printf("%16.16s : %10s %10s %10s %10s\n","Status",
	(app0name[0]==0) ? "-" : fstappstat(app0stat),
	(app1name[0]==0) ? "-" : fstappstat(app1stat),
	(app2name[0]==0) ? "-" : fstappstat(app2stat),
	(app3name[0]==0) ? "-" : fstappstat(app3stat));
sprintf(s0,"%d/%d",app0testres,app0crit);
sprintf(s1,"%d/%d",app1testres,app1crit);
sprintf(s2,"%d/%d",app2testres,app2crit);
sprintf(s3,"%d/%d",app3testres,app3crit);
printf("%16.16s : %10s %10s %10s %10s\n","Test Failures",s0,s1,s2,s3);

}








main(int argc, char *argv[]) {
int	x,tonode,rcode,reserve_hdl,syncfromnode=-1;
char	command[128]="",confname[128]="";
char	s[BUFSIZ]="";
time_t	now;
char nodename[MAXHOSTNAMELEN]="";

strcpy(fsthaprogname,argv[0]);
mypid=getpid();
sprintf(msg, "Usage:  %s  [command]\n", fsthaprogname);
  strcat(msg,"\n");
  strcat(msg,"where [command] is bootup\n");
  strcat(msg,"                   autostart  [ /dev/rdsk/cXtXdXs2 ]\n");
  strcat(msg,"                   autostart+ [ /dev/rdsk/cXtXdXs2 ]\n");
  strcat(msg,"                   startup  <0|1|2|3> n=node to start from\n");
  strcat(msg,"                   shutdown\n");
  strcat(msg,"                   status\n");
  strcat(msg,"                   export  [tag]\n");
  strcat(msg,"\n");
  strcat(msg,"                   locate <VH#> <Node# or 'N'>\n");
  strcat(msg,"\n");
  strcat(msg,"                   nocheck <App#> <VH#>\n");
  strcat(msg,"                   on <App#> <VH#>\n");
  strcat(msg,"                   off  <App#> <VH#>\n");
  strcat(msg,"\n");


if ( argc > 1 ) {
	if ( strncmp(argv[1],"bootup",6) != 0 
		&& strncmp(argv[1],"autostart",9) != 0 
		&& strncmp(argv[1],"autostart+",10) != 0 
		&& strncmp(argv[1],"startup",7) != 0 
		&& strncmp(argv[1],"shutdown",8) != 0 
		&& strncmp(argv[1],"status",6) != 0 
		&& strncmp(argv[1],"export",6) != 0 
		&& strncmp(argv[1],"backup",6) != 0 
		&& strncmp(argv[1],"nocheck",5) != 0 
		&& strncmp(argv[1],"on",2) != 0 
		&& strncmp(argv[1],"off",3) != 0 
		&& strncmp(argv[1],"sethalf",7) != 0 
		&& strncmp(argv[1],"setfull",7) != 0 
		&& strncmp(argv[1],"setfullq",8) != 0 
		&& strncmp(argv[1],"locate",6) != 0 ) {
		fprintf(stderr,msg);
		exit(1);
	}
	else {
		strcpy(command,argv[1]);
		if (strcmp(command,"locate") == 0
			|| strcmp(command,"sethalf") == 0
			|| strcmp(command,"setfull") == 0
			|| strcmp(command,"setfullq") == 0
			|| strcmp(command,"on") == 0
			|| strcmp(command,"off") == 0
			|| strcmp(command,"nocheck") == 0) { 
			if (argc != 4) {
				fprintf(stderr,msg);
				exit(1);
			}
		}
		if (strcmp(command,"startup") == 0 ) {
			if (argc != 3) {
				fprintf(stderr,msg);
				exit(1);
			}
			else {
				syncfromnode=atoi(argv[2]);
			}
		}
	}
}

gethostname(nodename,MAXHOSTNAMELEN-2);
nodename[MAXHOSTNAMELEN-1]=0;
sprintf(confname,"%s.conf",nodename);

if (strcmp(command,"autostart") == 0 
	|| strcmp(command,"autostart+") == 0
	|| strcmp(command,"startup") == 0
	|| strcmp(command,"bootup") == 0) {

	if ( create_shm() != 0 ) {
		printf("\n");
		printf("Problem:\n");
		printf("\n");
		system("/usr/bin/ipcs");
		printf("\n");
		system("/usr/bin/ps -e|grep fst|grep -v grep");
		printf("\n");
		printf("HA appears to be initialized in memory.\n");
		printf("Only one instance can exist per node.\n");
		printf(" To clean up previous instance, please: \n");
		printf("   (1) kill all fst* processes\n");
		printf("    (2) remove shared memory (man ipcrm)\n");
		printf("and try again\n");
		printf("\n");
		printf("HA Aborted.\n");
		system("logger -p daemon.notice HA appears to be initialized in memory. HA Aborted.");
		exit(1);
	}
	/* default config stuff */
	shm->hbtimeout = 3;
	shm->hbdelay = 1;
	shm->qnode = -1;
	shm->fstastlh0app0_pid=0;
	shm->fstastlh0app1_pid=0;
	shm->fstastlh0app2_pid=0;
	shm->fstastlh0app3_pid=0;
	shm->fstastlh1app0_pid=0;
	shm->fstastlh1app1_pid=0;
	shm->fstastlh1app2_pid=0;
	shm->fstastlh1app3_pid=0;
	shm->fstastlh2app0_pid=0;
	shm->fstastlh2app1_pid=0;
	shm->fstastlh2app2_pid=0;
	shm->fstastlh2app3_pid=0;
	shm->fstastlh3app0_pid=0;
	shm->fstastlh3app1_pid=0;
	shm->fstastlh3app2_pid=0;
	shm->fstastlh3app3_pid=0;
	shm->fsthbb_pid=0;
	shm->fsthbm_pid=0;
	shm->fsthbserv_pid=0;
	shm->fstsvr_pid=0;
	shm->fstasm_pid=0;
	printf("Shm ID %d = %d bytes. \n",shmid,sizeof(*shm));
	fflush(stdout);
	sprintf(shm->cf,"%s/%s",ETCDIR,confname);
	sprintf(msg,"Configuration = %s",shm->cf);
	logit(msg);
	fflush(stdout);
	get_conf();
	if (strcmp(command,"autostart") == 0 || strcmp(command,"autostart+") == 0 ) {
		if (strcmp(command,"autostart+") == 0 ) {
			shm->reserve_strict = PLUS;
        		printf("Reserve Disk Policy In Use. If failure result = NODE DOWN!\n");
        		system("logger -p daemon.notice Reserve Disk Policy In Use. If failure result = NODE DOWN!\n");
		}
		else {
			shm->reserve_strict = HA;
        		printf("Reserve Disk Policy In Use. If failure result = Down HA.\n");
        		system("logger -p daemon.notice Reserve Disk Policy In Use. If failure result = Down HA.\n");
		}
		printf("Checking Reservation Disk\n");
		if (argc > 2) {
			strncpy(shm->reserve_disk,argv[2],127);
		}
		reserve_hdl=open(shm->reserve_disk,O_NDELAY);
		if (reserve_hdl == -1) {
			fprintf(stderr,"Unable to Open Reservation Disk (%s). Autostart Aborted.\n",shm->reserve_disk);
			system("logger -p daemon.notice Unable to open Reservation Disk. Autostart Aborted.");
			remove_shm();
			exit(1);
		}
		close(reserve_hdl);
			printf("Reserving Disk...\n");
			if (reserve_disk() == 0) {
        			printf("Reserve Disk Succeeded. Booting Cluster.\n");
        			system("logger -p daemon.notice Reserve Disk Succeeded. Cluster BOOTUP.\n");
				strcpy(command,"bootup");
			} else {
        			printf("Reserve Disk not succeeded. Joining Cluster.\n");
        			system("logger -p daemon.notice Reserve Disk already in use. Cluster JOIN.");
				strcpy(command,"startup");
			}
	} else {
		if (shm->reserve_disk[0] == 0)
			strcpy(shm->reserve_disk,"none");
		shm->reserve_strict = MANUAL;
		printf("WARNING: No Disk Reservation.\n");
	}
} else {
	get_shm(0);
}


if (strcmp(command,"bootup") == 0 ) {
printf("Booting Cluster\n");
fflush(stdout);


	sprintf(shm->cf,"%s/%s",ETCDIR,confname);
	printf("Configuration = %s\n",shm->cf);
	sprintf(msg,"Configuration = %s",shm->cf);
	logit(msg);


	sprintf(msg,"/usr/bin/netstat -a|grep \\.%s\\ ",shm->port);
	if (system(msg) == 0) {
		printf("\n");
		printf("Problem:\n");
		printf("\n");
		printf("There appears to be sockets using Port %s.\n",shm->port);
		printf("This may indicate that HA is already initailized. Otherwise,\n");
		printf("Please wait up to 4 mins (tcp_time_wait_interval) and try again.\n");
		printf("\n");
		printf("Bootup Aborted.\n");
		sprintf(msg,"logger -p daemon.notice There appears to be sockets using Port %s. Bootup Aborted.",shm->port);
		system(msg);
		remove_shm();
		exit(1);
	}

	/* setup stuff */
	shm->cstat = GUP;
	shm->dirty = YES;
	shm->unfreeze = YES;
	shm->node0stat = DOWN;
	shm->node1stat = DOWN;
	shm->node2stat = DOWN;
	shm->node3stat = DOWN;
	shm->hb0stat0 = DOWN;
	shm->hb0stat1 = DOWN;
	shm->hb0stat2 = DOWN;
	shm->hb0stat3 = DOWN;
	shm->hb1stat0 = DOWN;
	shm->hb1stat1 = DOWN;
	shm->hb1stat2 = DOWN;
	shm->hb1stat3 = DOWN;
	shm->insync = NO;
	shm->lh0stat = DOWN;
	shm->lh1stat = DOWN;
	shm->lh2stat = DOWN;
	shm->lh3stat = DOWN;

	sprintf(msg,"nohup fsthbm >/dev/null 2>&1 &");
	logit("Starting fsthbm.");
	printf("Starting fsthbm.\n");
	system(msg);
}

else if (strcmp(command,"startup") == 0 ) {
printf("Starting Node\n");
fflush(stdout);
	/* default config stuff */
	shm->insync = NO;
	shm->qnode = -1;

	sprintf(msg,"/usr/bin/netstat -a|grep \\.%s\\ ",shm->port);
	if (system(msg) == 0) {
		printf("\n");
		printf("Problem:\n");
		printf("\n");
		printf("There appears to be sockets using Port %s\n",shm->port);
		printf("This may indicate that HA is already initailized. Otherwise,\n");
		printf("Please wait up to 4 mins (tcp_time_wait_interval) and try again.\n");
		printf("\n");
		printf("Startup Aborted.\n");
		sprintf(msg,"logger -p daemon.notice There appears to be sockets using Port %s. Startup Aborted.",shm->port);
		system(msg);
		remove_shm();
		exit(1);
	}

	/* setup stuff */
	shm->cstat = JOIN;
	shm->dirty = YES;
	shm->unfreeze = YES;
	shm->node0stat = DOWN;
	shm->node1stat = DOWN;
	shm->node2stat = DOWN;
	shm->node3stat = DOWN;
	shm->hb0stat0 = DOWN;
	shm->hb0stat1 = DOWN;
	shm->hb0stat2 = DOWN;
	shm->hb0stat3 = DOWN;
	shm->hb1stat0 = DOWN;
	shm->hb1stat1 = DOWN;
	shm->hb1stat2 = DOWN;
	shm->hb1stat3 = DOWN;
/***
	sprintf(msg,"/usr/sbin/ping %s 3",shm->node0);
	if (system(msg) == 0) shm->node0stat = UP;
	sprintf(msg,"/usr/sbin/ping %s 3",shm->node1);
	if (system(msg) == 0) shm->node1stat = UP;
	sprintf(msg,"/usr/sbin/ping %s 3",shm->node2);
	if (system(msg) == 0) shm->node2stat = UP;
	sprintf(msg,"/usr/sbin/ping %s 3",shm->node3);
	if (system(msg) == 0) shm->node3stat = UP;
	sprintf(msg,"/usr/sbin/ping %s 3",shm->hb0pg0);
	if (system(msg) == 0) shm->hb0stat0 = UP;
	sprintf(msg,"/usr/sbin/ping %s 3",shm->hb0pg1);
	if (system(msg) == 0) shm->hb0stat1 = UP;
	sprintf(msg,"/usr/sbin/ping %s 3",shm->hb0pg2);
	if (system(msg) == 0) shm->hb0stat2 = UP;
	sprintf(msg,"/usr/sbin/ping %s 3",shm->hb0pg3);
	if (system(msg) == 0) shm->hb0stat3 = UP;
	sprintf(msg,"/usr/sbin/ping %s 3",shm->hb1pg0);
	if (system(msg) == 0) shm->hb1stat0 = UP;
	sprintf(msg,"/usr/sbin/ping %s 3",shm->hb1pg1);
	if (system(msg) == 0) shm->hb1stat1 = UP;
	sprintf(msg,"/usr/sbin/ping %s 3",shm->hb1pg2);
	if (system(msg) == 0) shm->hb1stat2 = UP;
	sprintf(msg,"/usr/sbin/ping %s 3",shm->hb1pg3);
	if (system(msg) == 0) shm->hb1stat3 = UP;
***/
	shm->lh0stat = DOWN;
	shm->lh1stat = DOWN;
	shm->lh2stat = DOWN;
	shm->lh3stat = DOWN;
	if (syncfromnode == -1) {
		sprintf(msg,"nohup fsthbm >/dev/null 2>&1 &");
	} else {
		sprintf(msg,"nohup fsthbm %d >/dev/null 2>&1 &",syncfromnode);
	}
	logit("Starting fsthbm.");
	system(msg);
	while (shm->cstat == JOIN)
		sleep(1);
}

else if (strncmp(command,"sethalf",7) == 0 ) {
	printf("WARNING: WARNING: WARNING: WARNING: WARNING: WARNING: WARNING: WARNING\n");
	printf("\n");
	printf("Use of the sethalf command can corrupt your cluster database.\n");
	printf("Please don not use unless you know what you are doing!!\n");
	printf("\n");
	printf("Press ^C to abort or [Return] to continue...");
	fgets(msg,BUFSIZ,stdin);

	sprintf(msg,"Setting %s = %s\n",argv[2],argv[3]);
	logit(msg);
	sprintf(msg,"%s %s",argv[2],argv[3]);
	fsthahere(msg);
	sprintf(msg,"dirty %d",YES);
	fsthahere(msg);
}
else if (strncmp(command,"setfullq",8) == 0 ) {

	sprintf(msg,"Setting %s = %s\n",argv[2],argv[3]);
	logit(msg);
	sprintf(msg,"%s %s",argv[2],argv[3]);
	fsthasend(msg);
	sprintf(msg,"dirty %d",YES);
	fsthahere(msg);
}
else if (strncmp(command,"setfull",7) == 0 ) {
	printf("WARNING: WARNING: WARNING: WARNING: WARNING: WARNING: WARNING: WARNING\n");
	printf("\n");
	printf("Use of the setfull command can corrupt your cluster database.\n");
	printf("Please don not use unless you know what you are doing!!\n");
	printf("\n");
	printf("Press ^C to abort or [Return] to continue...");
	fgets(msg,BUFSIZ,stdin);

	sprintf(msg,"Setting %s = %s\n",argv[2],argv[3]);
	logit(msg);
	sprintf(msg,"%s %s",argv[2],argv[3]);
	fsthasend(msg);
	sprintf(msg,"dirty %d",YES);
	fsthahere(msg);
}

else if (strncasecmp(command,"nocheck",7) == 0 
		|| strncasecmp(command,"on",2) == 0
		|| strncasecmp(command,"off",3) == 0) {
	sprintf(msg,"Turn App%s of LH%s to %s\n",argv[2],argv[3],argv[1]);
	logit(msg);
	if (strncasecmp(command,"nocheck",5) == 0) 
		x=NOCHECK;
	else if (strncasecmp(command,"on",5) == 0) {
		x=ON;
	}
	else if (strncasecmp(command,"off",5) == 0) {
		x=OFF;
	}

	if (strcmp(argv[3],"0") == 0) {
		if (shm->lh0stat != UP && x==ON) {
			sprintf(msg,"Virtualhost %s is not Up. Request Denied.",shm->lh0name);
			logit(msg);
			exit(0);
		}
		if (strcmp(argv[2],"0") == 0) {
			fstappswitch(0,0, &(shm->lh0app0stat),shm->lh0app0name, 
				x, shm->lh0app0start,shm->lh0app0stop,
				&(shm->lh0app0testres), &(shm->lh0app0testtimeo),
				&(shm->lh0app0timeo));
		} else
		if (strcmp(argv[2],"1") == 0) {
			fstappswitch(0,1, &(shm->lh0app1stat),shm->lh0app1name, 
				x, shm->lh0app1start,shm->lh0app1stop,
				&(shm->lh0app0testres), &(shm->lh0app0testtimeo),
				&(shm->lh0app0timeo));
		} else
		if (strcmp(argv[2],"2") == 0) {
			fstappswitch(0,2, &(shm->lh0app2stat),shm->lh0app2name, 
				x, shm->lh0app2start,shm->lh0app2stop,
				&(shm->lh0app0testres), &(shm->lh0app0testtimeo),
				&(shm->lh0app0timeo));
		} else
		if (strcmp(argv[2],"3") == 0) {
			fstappswitch(0,3, &(shm->lh0app3stat),shm->lh0app3name, 
				x, shm->lh0app3start,shm->lh0app3stop,
				&(shm->lh0app0testres), &(shm->lh0app0testtimeo),
				&(shm->lh0app0timeo));
		}

	} else


	if (strcmp(argv[3],"1") == 0) {
		if (shm->lh1stat != UP && x==ON) {
			sprintf(msg,"Virtualhost %s is not Up. Request Denied.",shm->lh1name);
			logit(msg);
			exit(0);
		}
		if (strcmp(argv[2],"0") == 0) {
			fstappswitch(1,0, &(shm->lh1app0stat),shm->lh1app0name, 
				x, shm->lh1app0start,shm->lh1app0stop,
				&(shm->lh1app0testres), &(shm->lh1app0testtimeo),
				&(shm->lh1app0timeo));
		} else
		if (strcmp(argv[2],"1") == 0) {
			fstappswitch(1,1, &(shm->lh1app1stat),shm->lh1app1name, 
				x, shm->lh1app1start,shm->lh1app1stop,
				&(shm->lh1app0testres), &(shm->lh1app0testtimeo),
				&(shm->lh1app0timeo));
		} else
		if (strcmp(argv[2],"2") == 0) {
			fstappswitch(1,2, &(shm->lh1app2stat),shm->lh1app2name, 
				x, shm->lh1app2start,shm->lh1app2stop,
				&(shm->lh1app0testres), &(shm->lh1app0testtimeo),
				&(shm->lh1app0timeo));
		} else
		if (strcmp(argv[2],"3") == 0) {
			fstappswitch(1,3, &(shm->lh1app3stat),shm->lh1app3name, 
				x, shm->lh1app3start,shm->lh1app3stop,
				&(shm->lh1app0testres), &(shm->lh1app0testtimeo),
				&(shm->lh1app0timeo));
		}

	} else


	if (strcmp(argv[3],"2") == 0) {
		if (shm->lh2stat != UP && x==ON) {
			sprintf(msg,"Virtualhost %s is not Up. Request Denied.",shm->lh2name);
			logit(msg);
			exit(0);
		}
		if (strcmp(argv[2],"0") == 0) {
			fstappswitch(2,0, &(shm->lh2app0stat),shm->lh2app0name, 
				x, shm->lh2app0start,shm->lh2app0stop,
				&(shm->lh2app0testres), &(shm->lh2app0testtimeo),
				&(shm->lh2app0timeo));
		} else
		if (strcmp(argv[2],"1") == 0) {
			fstappswitch(2,1, &(shm->lh2app1stat),shm->lh2app1name, 
				x, shm->lh2app1start,shm->lh2app1stop,
				&(shm->lh2app0testres), &(shm->lh2app0testtimeo),
				&(shm->lh2app0timeo));
		} else
		if (strcmp(argv[2],"2") == 0) {
			fstappswitch(2,2, &(shm->lh2app2stat),shm->lh2app2name, 
				x, shm->lh2app2start,shm->lh2app2stop,
				&(shm->lh2app0testres), &(shm->lh2app0testtimeo),
				&(shm->lh2app0timeo));
		} else
		if (strcmp(argv[2],"3") == 0) {
			fstappswitch(2,3, &(shm->lh2app3stat),shm->lh2app3name, 
				x, shm->lh2app3start,shm->lh2app3stop,
				&(shm->lh2app0testres), &(shm->lh2app0testtimeo),
				&(shm->lh2app0timeo));
		}

	} else


	if (strcmp(argv[3],"3") == 0) {
		if (shm->lh3stat != UP && x==ON) {
			sprintf(msg,"Virtualhost %s is not Up. Request Denied.",shm->lh3name);
			logit(msg);
			exit(0);
		}
		if (strcmp(argv[2],"0") == 0) {
			fstappswitch(3,0, &(shm->lh3app0stat),shm->lh3app0name, 
				x, shm->lh3app0start,shm->lh3app0stop,
				&(shm->lh3app0testres), &(shm->lh3app0testtimeo),
				&(shm->lh3app0timeo));
		} else
		if (strcmp(argv[2],"1") == 0) {
			fstappswitch(3,1, &(shm->lh3app1stat),shm->lh3app1name, 
				x, shm->lh3app1start,shm->lh3app1stop,
				&(shm->lh3app0testres), &(shm->lh3app0testtimeo),
				&(shm->lh3app0timeo));
		} else
		if (strcmp(argv[2],"2") == 0) {
			fstappswitch(3,2, &(shm->lh3app2stat),shm->lh3app2name, 
				x, shm->lh3app2start,shm->lh3app2stop,
				&(shm->lh3app0testres), &(shm->lh3app0testtimeo),
				&(shm->lh3app0timeo));
		} else
		if (strcmp(argv[2],"3") == 0) {
			fstappswitch(3,3, &(shm->lh3app3stat),shm->lh3app3name, 
				x, shm->lh3app3start,shm->lh3app3stop,
				&(shm->lh3app0testres), &(shm->lh3app0testtimeo),
				&(shm->lh3app0timeo));
		}

	} 

	sprintf(msg,"dirty %d",YES);
	fsthahere(msg);

}

else if (strncmp(command,"export",6) == 0 ) {
	if (argc == 3) {
		strcpy(msg,argv[2]);
		export_conf(msg);
	} else {
		export_conf("");
	}
	exit(0);
}
else if (strncmp(command,"backup",6) == 0 ) {
	sprintf(msg,"dirty %d",YES);
	fsthahere(msg);
	if (shm->verbose == YES)
		logit("Backing Up Configuration");
	fsthahere("backup 0");
	exit(0);
}

else if (strncmp(command,"locate",6) == 0 ) {
	sprintf(msg,"Locating VHost %s to Node %s\n",argv[2],argv[3]);
	logit(msg);

	if (strcmp(argv[3],"0") == 0)
		tonode=0;
	else if (strcmp(argv[3],"1") == 0)
		tonode=1;
	else if (strcmp(argv[3],"2") == 0)
		tonode=2;
	else if (strcmp(argv[3],"3") == 0)
		tonode=3;
	else
		tonode=9;

	if (tonode == 0 && shm->node0stat == DOWN ||
		tonode ==1 && shm->node1stat == DOWN ||
		tonode ==2 && shm->node2stat == DOWN ||
		tonode ==3 && shm->node3stat == DOWN) {
			logit("Node Down. Locate Request Failed");
			exit(1);
	}
	
	if (strcmp(argv[2],"0") == 0) {
		fstlocate(0, shm->lh0name, tonode, 
			&(shm->lh0node),&(shm->lh0stat),&(shm->lh0timeo),
			shm->lh0app0name,shm->lh0app1name,
			shm->lh0app2name,shm->lh0app3name,
			&(shm->lh0app0testres),&(shm->lh0app1testres),
			&(shm->lh0app2testres),&(shm->lh0app3testres)
			);
	}
	else if (strcmp(argv[2],"1") == 0) {
		fstlocate(1, shm->lh1name, tonode, 
			&(shm->lh1node),&(shm->lh1stat),&(shm->lh1timeo),
			shm->lh1app0name,shm->lh1app1name,
			shm->lh1app2name,shm->lh1app3name,
			&(shm->lh1app0testres),&(shm->lh1app1testres),
			&(shm->lh1app2testres),&(shm->lh1app3testres)
			);
	}
	else if (strcmp(argv[2],"2") == 0) {
		fstlocate(2, shm->lh2name, tonode, 
			&(shm->lh2node),&(shm->lh2stat),&(shm->lh2timeo),
			shm->lh2app0name,shm->lh2app1name,
			shm->lh2app2name,shm->lh2app3name,
			&(shm->lh2app0testres),&(shm->lh2app1testres),
			&(shm->lh2app2testres),&(shm->lh2app3testres)
			);
	}
	else if (strcmp(argv[2],"3") == 0) {
		fstlocate(3, shm->lh3name, tonode, 
			&(shm->lh3node),&(shm->lh3stat),&(shm->lh3timeo),
			shm->lh3app0name,shm->lh3app1name,
			shm->lh3app2name,shm->lh3app3name,
			&(shm->lh3app0testres),&(shm->lh3app1testres),
			&(shm->lh3app2testres),&(shm->lh3app3testres)
			);
	}
	sprintf(msg,"dirty %d",YES);
	fsthahere(msg);
}

else if (strncmp(command,"shutdown",8) == 0 ) {
	release_disk();
	
	shm->cstat = GDOWN;

	if (shm->lh0stat == UP || shm->lh0stat == GUP) {
		if (shm->lh0node == shm->nodeN ) {
			shm->lh0stat = GDOWN;
			logit("Waiting for VH0 to Go Down");
			while (shm->lh0stat == GDOWN) { 
				sleep(1); 
				}
			logit("VH0 is Down");
		}
	}
	if (shm->lh1stat == UP || shm->lh1stat == GUP) {
		if (shm->lh1node == shm->nodeN ) {
			shm->lh1stat = GDOWN;
			logit("Waiting for VH1 to Go Down");
			while (shm->lh1stat == GDOWN) { 
				sleep(1); 
				}
			logit("VH1 is Down");
		}
	}
	if (shm->lh2stat == UP || shm->lh2stat == GUP) {
		if (shm->lh2node == shm->nodeN ) {
			shm->lh2stat = GDOWN;
			logit("Waiting for VH2 to Go Down");
			while (shm->lh2stat == GDOWN) { 
				sleep(1); 
				}
			logit("VH2 is Down");
		}
	}
	if (shm->lh3stat == UP || shm->lh3stat == GUP) {
		if (shm->lh3node == shm->nodeN ) {
			shm->lh3stat = GDOWN;
			logit("Waiting for VH3 to Go Down");
			while (shm->lh3stat == GDOWN) { 
				sleep(1); 
				}
			logit("VH3 is Down");
		}
	}
	logit("Cluster Node is Shutting Down");
	fsthahere("cstat 1");
	sprintf(msg,"dirty %d",YES);
	fsthahere(msg);
	sprintf(msg,"cstat %d",DOWN);

	logit("Waiting for processes to die...");
	sleep (20);
	logit("Killing remaining processes...");
	fstkill("fstast");
	fstkill("fsthbb");
	fstkill("fsthbm");
	fstkill("fstasm");
	fstkill("fstsvr");
	fstkill("fsthbserv");

	remove_shm();
	logit("Cluster is Down");
	sprintf(msg,"/usr/bin/ipcrm -m %d > /dev/null 2>&1",shmid);
	system(msg);
}

else  {
	while (1) {
		time(&now);
		sprintf(s,"%s (%s)",nodename,fsthastat(shm->cstat));
		printf("%16.16s : %s\n",s,ctime(&now));
		fflush(stdout);
		printf("%16.16s : %s : %s\n","Reservation Disk",shm->reserve_disk,
			shm->reserve_strict == MANUAL ? "NOT USED." :
			(shm->reserve_strict == HA ? "IN USE: Failure result = Down HA." :
			"IN USE: Failure result = NODE DOWN!"));
		fflush(stdout);
		if (shm->cstat == DOWN) {
			break;
		}
		printf("\n");
		fflush(stdout);
	
		printf("%9.9s %6.6s < %8.8s %4.4-s %d\n",shm->hb0ip,shm->hb0dv,shm->hb0pg0,(shm->nodeN == 0 )? " ":fsthastat(shm->hb0stat0),(shm->nodeN == 0 )? now:atoi(shm->hb0tm0));
		printf("%11.11s %4.4s < %8.8s %4.4-s %d\n","","",shm->hb0pg1,(shm->nodeN == 1 )? " ":fsthastat(shm->hb0stat1),(shm->nodeN == 1 )? now:atoi(shm->hb0tm1));
		printf("%11.11s %4.4s < %8.8s %4.4-s %d\n","","",shm->hb0pg2,(shm->nodeN == 2 )? " ":fsthastat(shm->hb0stat2),(shm->nodeN == 2 )? now:atoi(shm->hb0tm2));
		printf("%11.11s %4.4s < %8.8s %4.4-s %d\n","","",shm->hb0pg3,(shm->nodeN == 3 )? " ":fsthastat(shm->hb0stat3),(shm->nodeN == 3 )? now:atoi(shm->hb0tm3));
		printf("%9.9s %6.6s < %8.8s %4.4-s %d\n",shm->hb1ip,shm->hb1dv,shm->hb1pg0,(shm->nodeN == 0 )? " ":fsthastat(shm->hb1stat0),(shm->nodeN == 0 )? now:atoi(shm->hb1tm0));
		printf("%11.11s %4.4s < %8.8s %4.4-s %d\n","","",shm->hb1pg1,(shm->nodeN == 1 )? " ":fsthastat(shm->hb1stat1),(shm->nodeN == 1 )? now:atoi(shm->hb1tm1));
		printf("%11.11s %4.4s < %8.8s %4.4-s %d\n","","",shm->hb1pg2,(shm->nodeN == 2 )? " ":fsthastat(shm->hb1stat2),(shm->nodeN == 2 )? now:atoi(shm->hb1tm2));
		printf("%11.11s %4.4s < %8.8s %4.4-s %d\n","","",shm->hb1pg3,(shm->nodeN == 3 )? " ":fsthastat(shm->hb1stat3),(shm->nodeN == 3 )? now:atoi(shm->hb1tm3));
		printf("\n");
		printf("%16.16s : %5.5-s %.2f%c\n",shm->node0,fsthastat(shm->node0stat),shm->node0load,shm->qnode == 0 ? 'R' : ' ');
		printf("%16.16s : %5.5-s %.2f%c\n",shm->node1,fsthastat(shm->node1stat),shm->node1load,shm->qnode == 1 ? 'R' : ' ');
		printf("%16.16s : %5.5-s %.2f%c\n",shm->node2,fsthastat(shm->node2stat),shm->node2load,shm->qnode == 2 ? 'R' : ' ');
		printf("%16.16s : %5.5-s %.2f%c\n",shm->node3,fsthastat(shm->node3stat),shm->node3load,shm->qnode == 3 ? 'R' : ' ');
		printf("\n");
	
	
			fstlhprint(0,shm->lh0name,shm->lh0node,shm->lh0stat,
				shm->lh0failing,shm->lh0path,shm->lh0fallback,shm->lh0failto,
				shm->lh0app0name,shm->lh0app0stat,shm->lh0app0testres,
				shm->lh0app0testtimeo,shm->lh0app0crit,shm->lh0app1name,
				shm->lh0app1stat,shm->lh0app1testres,shm->lh0app1testtimeo,
				shm->lh0app1crit,shm->lh0app2name,shm->lh0app2stat,
				shm->lh0app2testres,shm->lh0app2testtimeo,shm->lh0app2crit,
				shm->lh0app3name,shm->lh0app3stat,shm->lh0app3testres,
				shm->lh0app3testtimeo,shm->lh0app3crit);
	
		fflush(stdout);
	
		if (shm->lh1name[0] != 0) {
	
			fstlhprint(1,shm->lh1name,shm->lh1node,shm->lh1stat,
				shm->lh1failing,shm->lh1path,shm->lh1fallback,shm->lh1failto,
				shm->lh1app0name,shm->lh1app0stat,shm->lh1app0testres,
				shm->lh1app0testtimeo,shm->lh1app0crit,shm->lh1app1name,
				shm->lh1app1stat,shm->lh1app1testres,shm->lh1app1testtimeo,
				shm->lh1app1crit,shm->lh1app2name,shm->lh1app2stat,
				shm->lh1app2testres,shm->lh1app2testtimeo,shm->lh1app2crit,
				shm->lh1app3name,shm->lh1app3stat,shm->lh1app3testres,
				shm->lh1app3testtimeo,shm->lh1app3crit);
	
			}
			fflush(stdout);
	
		if (shm->lh2name[0] != 0) {
	
			fstlhprint(2,shm->lh2name,shm->lh2node,shm->lh2stat,
				shm->lh2failing,shm->lh2path,shm->lh2fallback,shm->lh2failto,
				shm->lh2app0name,shm->lh2app0stat,shm->lh2app0testres,
				shm->lh2app0testtimeo,shm->lh2app0crit,shm->lh2app1name,
				shm->lh2app1stat,shm->lh2app1testres,shm->lh2app1testtimeo,
				shm->lh2app1crit,shm->lh2app2name,shm->lh2app2stat,
				shm->lh2app2testres,shm->lh2app2testtimeo,shm->lh2app2crit,
				shm->lh2app3name,shm->lh2app3stat,shm->lh2app3testres,
				shm->lh2app3testtimeo,shm->lh2app3crit);
	
		}
		fflush(stdout);
	
		if (shm->lh3name[0] != 0) {
	
			fstlhprint(3,shm->lh3name,shm->lh3node,shm->lh3stat,
				shm->lh3failing,shm->lh3path,shm->lh3fallback,shm->lh3failto,
				shm->lh3app0name,shm->lh3app0stat,shm->lh3app0testres,
				shm->lh3app0testtimeo,shm->lh3app0crit,shm->lh3app1name,
				shm->lh3app1stat,shm->lh3app1testres,shm->lh3app1testtimeo,
				shm->lh3app1crit,shm->lh3app2name,shm->lh3app2stat,
				shm->lh3app2testres,shm->lh3app2testtimeo,shm->lh3app2crit,
				shm->lh3app3name,shm->lh3app3stat,shm->lh3app3testres,
				shm->lh3app3testtimeo,shm->lh3app3crit);
	
		}
		fflush(stdout);
	
		if (strncmp(command,"status",6) == 0 ) {
			sleep(2);
			system("/usr/bin/clear");
		}
		else
			exit(0);
	}
	
}
}
