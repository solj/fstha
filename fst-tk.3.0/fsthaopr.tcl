#!/bin/sh
#
# HA/FST - High Availability Cluster
# Copyright (C) 2003-2006 Reggie Beavers
# 
# Licensed under the terms of the GNU General Public License:
# 		http://www.opensource.org/licenses/gpl-license.php
# 
# For further information visit:
# 		http://www.fstha.com/
# 
# 
# File Name: fsthaopr.tcl
# 		fsthatool - TCL/TK GUI interface
# 
# File Authors:
# 		Reggie Beavers (reggiebeavers@fstha.com)
# \
exec wish "$0" "$*"

package require Tk

set node [exec uname -n]
set basedir "/opt/FSTha/"
set cf "/opt/FSTha/etc/$node.conf"
if { [file exists $cf] } {
} else {
 exec touch $cf 
}
set prv_cftime [ file mtime $cf ]
set cur_cftime 0

#set wUP [ image create photo -file /opt/FSTha/lib/up.gif ]
#set wDOWN [ image create photo -palette 8 -file /opt/FSTha/lib/up.gif]
#set wCHANGING [ image create photo -file /opt/FSTha/lib/changing.gif ]
#set wFAILED [ image create photo -file /opt/FSTha/lib/failed.gif ]
#set wQUESTION [ image create photo -file /opt/FSTha/lib/question.gif ]
#
#global cf node prv_cftime cur_cftime wUP wDOWN wCHANGING wFAILED wQUESTION 
set green_dot [ image create photo -file /opt/FSTha/lib/images/green-dot.gif ]
set blue_dot [ image create photo -file /opt/FSTha/lib/images/blue-dot.gif ]
set yellow_dot [ image create photo -file /opt/FSTha/lib/images/yellow-dot.gif ]
set grey_dot [ image create photo -file /opt/FSTha/lib/images/grey-dot.gif ]
set lgrey_dot [ image create photo -file /opt/FSTha/lib/images/lgrey-dot.gif ]
set red_dot [ image create photo -file /opt/FSTha/lib/images/red-dot.gif ]
set brown_dot [ image create photo -file /opt/FSTha/lib/images/brown-dot.gif ]
set green_cone [ image create photo -file /opt/FSTha/lib/images/green-cone.gif ]
set lgrey_cone [ image create photo -file /opt/FSTha/lib/images/lgrey-cone.gif ]
set header_box [ image create photo -file /opt/FSTha/lib/images/header.gif ]
global cf node prv_cftime cur_cftime green_dot blue_dot yello_dot grey_dot lgrey_dot brown_dot red_dot green_cone lgrey_cone header_box

##
##
proc whatstatapp {stat} {
if { $stat == "0" } {
	puts "ON"
} elseif { $stat == "1" } {
	puts "OFF"
} elseif { $stat == "3" } {
	puts "FAIL"
} elseif { $stat == "5" } {
	puts "@ON"
} elseif { $stat == "4" } {
	puts "@OFF"
} else {
	puts "?"
}

}

##
##
##
proc whatstat {stat} {
global green_dot blue_dot yello_dot grey_dot lgrey_dot brown_dot red_dot 
if { $stat == "0" } {
#UP
	return green_dot
} elseif { $stat == "1" } {
#DOWN
	return brown_dot
} elseif { $stat == "3" } {
#FAILED
	return red_dot
} elseif { $stat == "4" } {
#CHANGING
	return blue_dot
} elseif { $stat == "5" } {
#CHANGING
	return blue_dot
} else {
#QUESTION
	return lgrey_dot
}

}

##
##
proc do_clautoplus {} {
	global e_rdisk
	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if { $cluster_up == "0" } {
		tk_messageBox -title Sorry -message "The Cluster Is Running.\nPlease Shutdown first." -type ok
		catch {raise .cl }
		return	
	} 
	catch { exec fstha autostart+ $e_rdisk }
}

##
##
proc do_clauto {} {
	global e_rdisk
	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if { $cluster_up == "0" } {
		tk_messageBox -title Sorry -message "The Cluster Is Running.\nPlease Shutdown first." -type ok
		catch {raise .cl }
		return	
	} 
	catch { exec fstha autostart $e_rdisk }
}

##
##
proc do_clboot {} {
	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if { $cluster_up == "0" } {
		tk_messageBox -title Sorry -message "The Cluster Is Running.\nPlease Shutdown first." -type ok
		catch {raise .cl }
		return	
	} 
	catch { exec fstha bootup }
}

##
##
proc do_clstart {} {
	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if { $cluster_up == "0" } {
		tk_messageBox -title Sorry -message "The Cluster Is Running.\nPlease Shutdown first." -type ok
		catch {raise .cl }
		return	
	} 
	catch { exec fstha startup }
}

##
##
proc do_clresync {} {
	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if { $cluster_up != 0} {
		tk_messageBox -title Sorry -message "The Cluster Is Not Running." -type ok
		catch {raise .cl }
		return	
	} 
	catch { exec echo "y" | fstha sethalf cstat 6 }
}

##
##
proc do_clstop {} {
	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if { $cluster_up != 0} {
		tk_messageBox -title Sorry -message "The Cluster Is Not Running." -type ok
		catch {raise .cl }
		return	
	} 
	catch { exec fstha shutdown }
	show_down
	destroy .cl
}
##
##
proc do_clset {} {
	global node
	global e_appfailtimeo e_hb0dv e_hb1dv e_htmlrefreshsecs e_lhfailtimeo e_node0 e_node1 e_node2 e_node3 e_hbdelay e_hbtimeout e_rdisk e_hbport e_port e_verbose e_nodeN e_appfailtesttimeo
	set good2go "yes"
	foreach {eee} { $e_appfailtimeo $e_hb0dv $e_hb1dv $e_htmlrefreshsecs $e_lhfailtimeo $e_node0 $e_node1 $e_node2 $e_node3 $e_hbdelay $e_hbtimeout $e_rdisk $e_hbport $e_port $e_verbose $e_nodeN $e_appfailtesttimeo} {
	eval set eeee $eee
		if {$eeee == "" } {
			set good2go "no"
		}
	}
	if {$good2go == "no" } {
		tk_messageBox -title Sorry -message "All fields must contain data to continue." -type ok
		catch {raise .clcf }
		return	
	}
	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if {$cluster_up == "0"} {
		tk_messageBox -title Sorry -message "The Cluster Is Now Running. You must perform a Shutdown before Configuring." -type ok
		catch {raise .cl }
		return	
	} 
	set f [ open "/opt/FSTha/etc/${node}.conf" w 644 ]
	puts $f "cstat 1"
	puts $f "verbose $e_verbose"
	puts $f "htmlrefreshsecs $e_htmlrefreshsecs"
	puts $f "htmlallowexec 1"
	puts $f "port $e_port"
	puts $f "hbport $e_hbport"
	puts $f "reserve_disk $e_rdisk"
	if { "$e_rdisk" != "none" } {
		puts $f "reserve_strict 0"
	}
	puts $f "logfile /var/adm/messages"
	puts $f "syslogfac daemon.notice"
	puts $f "lh0failto 8"
	puts $f "lh1failto 8"
	puts $f "lh2failto 8"
	puts $f "lh3failto 8"
	puts $f "lh0fallback 9"
	puts $f "lh1fallback 9"
	puts $f "lh2fallback 9"
	puts $f "lh3fallback 9"
	puts $f "node0 $e_node0"
	puts $f "node1 $e_node1"
	puts $f "node2 $e_node2"
	puts $f "node3 $e_node3"
	puts $f "nodeN $e_nodeN"
	if {$e_nodeN == "0"} {
		puts $f "hb0 $e_hb0dv 10.1.1.1 255.255.255.0 10.1.1.255"
	} elseif {$e_nodeN == "1"} {
		puts $f "hb0 $e_hb0dv 10.1.1.2 255.255.255.0 10.1.1.255"
	} elseif {$e_nodeN == "2"} {
		puts $f "hb0 $e_hb0dv 10.1.1.3 255.255.255.0 10.1.1.255"
        } elseif {$e_nodeN == "3"} {
		puts $f "hb0 $e_hb0dv 10.1.1.4 255.255.255.0 10.1.1.255"
        }
	if {$e_nodeN == "0"} {
		puts $f "hb1 $e_hb1dv 10.1.2.1 255.255.255.0 10.1.2.255"
	} elseif {$e_nodeN == "1"} {
		puts $f "hb1 $e_hb1dv 10.1.2.2  255.255.255.0 10.1.2.255"
	} elseif {$e_nodeN == "2"} {
                puts $f "hb1 $e_hb1dv 10.1.2.3 255.255.255.0 10.1.2.255"
        } elseif {$e_nodeN == "3"} {
                puts $f "hb1 $e_hb1dv 10.1.2.4 255.255.255.0 10.1.2.255"
        }

	puts $f "hb0pg0 10.1.1.1"
	puts $f "hb0pg1 10.1.1.2"
	puts $f "hb0pg2 10.1.1.3"
	puts $f "hb0pg3 10.1.1.4"
	puts $f "hb1pg0 10.1.2.1"
	puts $f "hb1pg1 10.1.2.2"
	puts $f "hb1pg2 10.1.2.3"
	puts $f "hb1pg3 10.1.2.4"

	puts $f "hbdelay $e_hbdelay"
	puts $f "hbtimeout $e_hbtimeout"
	puts $f "lhfailtimeo $e_lhfailtimeo"
	puts $f "appfailtimeo $e_appfailtimeo"
	puts $f "appfailtesttimeo $e_appfailtesttimeo"
	close $f
	catch {raise .cl }
	destroy .clcf
}

##
##
proc do_clcf2 {} {
	global e_nodeN e_node0 e_node1 e_node2 e_node3
	set good2go "yes"
	foreach {eee} { "$e_nodeN" } {
	eval set eeee $eee
		if {$eeee == "" } {
			set good2go "no"
		}
	}
	if {$good2go == "no"} {
		tk_messageBox -title Sorry -message "All fields must contain data to continue." -type ok
		catch {raise .clcf }
		return	
	}
	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if {$cluster_up == "0"} {
		tk_messageBox -title Sorry -message "The Cluster Is Now Running. You must perform a Shutdown before Configuring." -type ok
		catch {raise .cl }
		return	
	} 
	destroy .clcf
	toplevel .clcf -borderwidth 10 
	wm title .clcf "Cluster Configure"
	update

	label .clcf.l_port    -text "Cluster Communication Port"
	label .clcf.l_hbport    -text "Heartbeat Port"
	label .clcf.l_rdisk    -text "Reservation Disk"
	label .clcf.l_hb0dv   -text "This  Node's Heartbeat-0 NIC (eg qfe0)" -relief raised
	label .clcf.l_hb1dv   -text "This  Node's Heartbeat-1 NIC (eg qfe1)" -relief raised
	label .clcf.l_node0 -text  "Node0's Public IP" -relief raised
	label .clcf.l_node1 -text  "Node1's Public IP" -relief raised
	label .clcf.l_node2 -text  "Node2's Public IP" -relief raised
	label .clcf.l_node3 -text  "Node3's Public IP" -relief raised
	label .clcf.l_hbtimeout -wraplength 300 -text "Number of seconds before other nodes heartbeats timeout"
	label .clcf.l_hbdelay -wraplength 300 -text "Number of seconds between sending heartbeats"

	label .clcf.l_verbose -text "Verbose"

	label .clcf.l_lhfailtimeo -wraplength 300 -text "Number of seconds for virutal host transitions to complete before time-out"
	label .clcf.l_appfailtimeo -wraplength 300 -text "Number of seconds for application transitions to complete before time-out"
	label .clcf.l_appfailtesttimeo -wraplength 300 -text "Number of seconds for application test to complete before time-out"
	label .clcf.l_htmlrefreshsecs -wraplength 300 -text "Number of seconds between refreshes of HTML interface page"

	entry .clcf.e_port -textvariable e_port -width 10
	.clcf.e_port delete 0 10
	.clcf.e_port insert 0 "1500"
	entry .clcf.e_hbport -textvariable e_hbport -width 10
	.clcf.e_hbport delete 0 10
	.clcf.e_hbport insert 0 "1501"
	entry .clcf.e_rdisk -textvariable e_rdisk -width 30
	.clcf.e_rdisk delete 0 30
	.clcf.e_rdisk insert 0 "none"
	entry .clcf.e_hb0dv -textvariable e_hb0dv -width 10
	entry .clcf.e_hb1dv -textvariable e_hb1dv -width 10
	entry .clcf.e_node0 -textvariable  e_node0 -width 30
	entry .clcf.e_node1 -textvariable  e_node1 -width 30
	entry .clcf.e_node2 -textvariable  e_node2 -width 30
	entry .clcf.e_node3 -textvariable  e_node3 -width 30
	entry .clcf.e_hbtimeout -textvariable e_hbtimeout -width 5
	.clcf.e_hbtimeout delete 0 5
	.clcf.e_hbtimeout insert 0 3
	entry .clcf.e_hbdelay -textvariable e_hbdelay -width 5
	.clcf.e_hbdelay delete 0 5
	.clcf.e_hbdelay insert 0 1

frame .clcf.verbosef
radiobutton .clcf.verbosef0 -text yes -variable e_verbose  -value 0
radiobutton .clcf.verbosef1 -text no -variable e_verbose  -value 1
.clcf.verbosef1 select
pack .clcf.verbosef0 .clcf.verbosef1 -side left -in .clcf.verbosef

	entry .clcf.e_lhfailtimeo -textvariable e_lhfailtimeo -width 5
	.clcf.e_lhfailtimeo delete 0 5
	.clcf.e_lhfailtimeo insert 0 60
	entry .clcf.e_appfailtimeo -textvariable e_appfailtimeo -width 5
	.clcf.e_appfailtimeo delete 0 5
	.clcf.e_appfailtimeo insert 0 60
	entry .clcf.e_appfailtesttimeo -textvariable e_appfailtesttimeo -width 5
	.clcf.e_appfailtesttimeo delete 0 5
	.clcf.e_appfailtesttimeo insert 0 60
	entry .clcf.e_htmlrefreshsecs -textvariable e_htmlrefreshsecs -width 5
	.clcf.e_htmlrefreshsecs delete 0 5
	.clcf.e_htmlrefreshsecs insert 0 30

	label .clcf.lab -text ""

	grid config .clcf.l_port -column 0 -row 4 -sticky "e"
	grid config .clcf.l_hbport -column 0 -row 5 -sticky "e"
	grid config .clcf.l_rdisk -column 0 -row 6 -sticky "e"
	grid config .clcf.l_hb0dv -column 0 -row 7 -sticky "e"
	grid config .clcf.l_hb1dv -column 0 -row 8 -sticky "e"
grid config .clcf.lab -column 0 -row 9 -sticky "e"
	grid config .clcf.l_node0 -column 0 -row 10 -sticky "e"
	grid config .clcf.l_node1 -column 0 -row 11 -sticky "e"
	grid config .clcf.l_node2 -column 0 -row 12 -sticky "e"
	grid config .clcf.l_node3 -column 0 -row 13 -sticky "e"
	grid config .clcf.l_hbtimeout -column 0 -row 14 -sticky "e"
	grid config .clcf.l_hbdelay -column 0 -row 15 -sticky "e"
grid config .clcf.lab -column 0 -row 16 -sticky "e"
	grid config .clcf.l_verbose -column 0 -row 17 -sticky "e"
grid config .clcf.lab -column 0 -row 18 -sticky "e"
	grid config .clcf.l_lhfailtimeo -column 0 -row 19 -sticky "e"
	grid config .clcf.l_appfailtimeo -column 0 -row 20 -sticky "e"
	grid config .clcf.l_appfailtesttimeo -column 0 -row 21 -sticky "e"
	grid config .clcf.l_htmlrefreshsecs -column 0 -row 22 -sticky "e"

	grid config .clcf.e_port -column 1 -row 4 -sticky "w"
	grid config .clcf.e_hbport -column 1 -row 5 -sticky "w"
	grid config .clcf.e_rdisk -column 1 -row 6 -sticky "w"
	grid config .clcf.e_hb0dv -column 1 -row 7 -sticky "w"
	grid config .clcf.e_hb1dv -column 1 -row 8 -sticky "w"
grid config .clcf.lab -column 1 -row 9 -sticky "w"
	grid config .clcf.e_node0 -column 1 -row 10 -sticky "w"
	grid config .clcf.e_node1 -column 1 -row 11 -sticky "w"
	grid config .clcf.e_node2 -column 1 -row 12 -sticky "w"
	grid config .clcf.e_node3 -column 1 -row 13 -sticky "w"
	grid config .clcf.e_hbtimeout -column 1 -row 14 -sticky "w"
	grid config .clcf.e_hbdelay -column 1 -row 15 -sticky "w"
grid config .clcf.lab -column 0 -row 16 -sticky "e"
	grid config .clcf.verbosef -column 1 -row 17 -sticky "w"
grid config .clcf.lab -column 0 -row 18 -sticky "e"
	grid config .clcf.e_lhfailtimeo -column 1 -row 19 -sticky "w"
	grid config .clcf.e_appfailtimeo -column 1 -row 20 -sticky "w"
	grid config .clcf.e_appfailtesttimeo -column 1 -row 21 -sticky "w"
	grid config .clcf.e_htmlrefreshsecs -column 1 -row 22 -sticky "w"


#	button .clcf.set -text "Set" -command "catch {do_clset}"
	button .clcf.set -text "Set" -command do_clset
	button .clcf.exit -text "Close" -command "catch {raise .cl}; destroy .clcf"
	
	grid config .clcf.set -column 0 -row 30 -sticky "e"
	grid config .clcf.exit -column 1 -row 30 -sticky "w"

	grid columnconfigure . 1 -weight 1

}
##
##
proc do_clcf {} {
	global node
	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if {$cluster_up == "0"} {
		tk_messageBox -title Sorry -message "The Cluster Is Running. You must perform a Shutdown before Configuring." -type ok
		catch {raise .cl }
		return	
	} 
	if { [file exists "/opt/FSTha/etc/${node}.conf"] } {
		set answer [ tk_messageBox -title Warning -message "A previous configuration exist. Continuing will overwrite\n/opt/FSTha/etc/${node}.conf.\nDo you wish to continue?" -type yesno]
		if { $answer == "no" } {
			catch {raise .cl }
			return
		}
	}
	toplevel .clcf -borderwidth 10 
	wm title .clcf "Cluster Configure"
	update

	label .clcf.l_nodeN -text "This is which node of cluster\n"

frame .clcf.nodeNf
radiobutton .clcf.e_nodeN0 -text 0 -variable e_nodeN -value 0
radiobutton .clcf.e_nodeN1 -text 1 -variable e_nodeN -value 1
radiobutton .clcf.e_nodeN2 -text 2 -variable e_nodeN -value 2
radiobutton .clcf.e_nodeN3 -text 3 -variable e_nodeN -value 3
pack .clcf.e_nodeN0 .clcf.e_nodeN1 .clcf.e_nodeN2 .clcf.e_nodeN3 -side left -in .clcf.nodeNf

	grid config .clcf.l_nodeN -column 0 -row 1 -sticky "e"

	grid config .clcf.nodeNf -column 1 -row 1 -sticky "w"
	button .clcf.set -text "Next" -command "catch {do_clcf2}"
	button .clcf.exit -text "Close" -command "catch {raise .cl}; destroy .clcf"
	
	grid config .clcf.set -column 0 -row 25 -sticky "e"
	grid config .clcf.exit -column 1 -row 25 -sticky "w"

	grid columnconfigure . 1 -weight 1

}
##
##
proc do_cluster {} {

	if { [winfo exists .vh] == "1" || [winfo exists .app] == "1" } {
                tk_messageBox -title Sorry -message "Please close Virtual Hosts or Applications before continuing." -type ok
		destroy .cl
                return
	}
	toplevel .cl -borderwidth 10 
	wm title .cl "Cluster"

	frame .cl.auto 
	pack .cl.auto 
	label .cl.auto.l0 -text "USE RESERVATION DISK POLICY (QUORUM DISK USAGE)\n==============================================="
	label .cl.auto.l1 -text "Use Reservation Disk to start nodes in\ncluster and to protect against split-brain.\n"
	label .cl.auto.l2 -text "\nAutostart"
	label .cl.auto.l3 -text "After starting, if other node's becomes non ping-able\nand if reservation fails then STOP HA."
	label .cl.auto.l4 -text "\nAutostart-Plus"
	label .cl.auto.l5 -text "After starting, if other node's becomes non ping-able\nand if reservation fails then bring node DOWN TO OBP (OK) PROMPT.\n(BEST PROTECTION)"
	label .cl.auto.l6 -text " "
	button .cl.auto.bauto -text "  AUTOSTART   " -command do_clauto
	button .cl.auto.bautoplus -text "AUTOSTART-PLUS" -command do_clautoplus
	label .cl.auto.l7 -text "Reservation Disk"
	entry .cl.auto.rdisk -textvariable e_rdisk -width 30
	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if {$cluster_up == "0"} {
		set erdisk [exec fstha export reserve_disk]
		set erdisk [lindex $erdisk 1]
	}  else {
		set erdisk ""
	}
	.cl.auto.rdisk delete 0 30
	.cl.auto.rdisk insert 0 $erdisk

	label .cl.auto.l8  -text "(e.g. /dev/rdsk/c7t2d0s2)"
	label .cl.auto.l9 -text "\n \n NON-RESERVATION DISK POLICY (MANUAL CONTROL)\n============================================="
	pack .cl.auto.l0 .cl.auto.l1 .cl.auto.bautoplus .cl.auto.bauto .cl.auto.l7 .cl.auto.rdisk .cl.auto.l8 .cl.auto.l2 .cl.auto.l3 .cl.auto.l4 .cl.auto.l5 .cl.auto.l6 .cl.auto.l9 -side top

	frame .cl.top 
	pack .cl.top  
	button .cl.top.boot -text " Bootup  " -command do_clboot
	button .cl.top.start -text " Startup " -command do_clstart
	label .cl.top.l1 -text "     "
#	button .cl.top.resync -text "Force Resync\nFrom Peer" -command do_clresync
	pack .cl.top.boot .cl.top.l1 .cl.top.start -side left -padx 0p -pady 10 

	frame .cl.mid1 
	pack .cl.mid1 
	label .cl.mid1.l1  -text "Bootup"
	label .cl.mid1.la  -width 40 -text "Manually start the only instance of HA\nrunning on this cluster. The other node's\ncannot be running HA."
	label .cl.mid1.l2 -text "Startup"
	label .cl.mid1.lb -width 40 -text "Manually start HA, Join cluster."
	label .cl.mid1.l3 -text "Shutdown"
	label .cl.mid1.lc -width 40 -text "Manually stop HA, Leave cluster."
	label .cl.mid1.ll1 -width 40 -text " "
	label .cl.mid1.ll2 -width 40 -text " "
	label .cl.mid1.ll3 -width 40 -text " "
	pack .cl.mid1.l1 .cl.mid1.la .cl.mid1.ll1 .cl.mid1.l2 .cl.mid1.lb .cl.mid1.ll2 .cl.mid1.l3 .cl.mid1.lc .cl.mid1.ll3 -side top 

	frame .cl.bot -borderwidth 10
	pack .cl.bot  
	button .cl.bot.cf -text "Configure" -command "catch {do_clcf}"
	label  .cl.bot.l1 -text "     "
	button .cl.bot.stop -text "Shutdown " -command do_clstop
	label  .cl.bot.l2 -text "     "
	button .cl.bot.exit -text "  Close  " -command "destroy .cl"
	pack .cl.bot.cf .cl.bot.l1 .cl.bot.stop .cl.bot.l2 .cl.bot.exit -side left

	update

}

##
##
proc do_vhset {} {
	global node node0 node1 node2 node3 .vh.middle.vh0 .vh.middle.vh1 .vh.middle.vh2 .vh.middle.vh3

	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if {$cluster_up != 0} {
		tk_messageBox -title Sorry -message "The Cluster Is No Longer Running." -type ok
		destroy .vh
		return	
	} 
	global vhnum vhname vhpath .vhcf doip dofs pdev adev failto fallback addr mask mount_device raw_device fstype mount_point bcast ether ip_args fs_args cmdline
	set good2go "yes"
	foreach {eee} { $vhname $vhpath } {
	eval set eeee $eee
		if {$eeee == "" } {
			set good2go "no"
		}
	}
	if {$good2go == "no"} {
		tk_messageBox -title Sorry -message "All fields must contain data to continue." -type ok
		catch {raise .vhcf }
		return	
	}
	file mkdir $vhpath
	set mypid [ pid ]

	set ip_args  ""
	if { $doip == "2" } {
		if { [file exist ${vhpath}/nettab] } {
			if { [file exist ${vhpath}/nettab.$mypid] == "0" } {
				file rename ${vhpath}/nettab ${vhpath}/nettab.$mypid
			}
		}
		set ip_args  " -1 $pdev -2 $adev -I $addr -m $mask -b $bcast -e $ether"
	} elseif { $doip == "1" } {
		if { [file exist ${vhpath}/nettab] } {
			if { [file exist ${vhpath}/nettab.$mypid] == "0" } {
				file rename ${vhpath}/nettab ${vhpath}/nettab.$mypid
			}
		}
		set ip_args  " -1 $pdev -2 $adev -i $addr -m $mask -b $bcast -e $ether"
	}

	set fs_args  ""
	if { $dofs != 0 } {
		if { [file exist ${vhpath}/fstab] } {
			if { [file exist ${vhpath}/fstab.$mypid] == "0" } {
				file rename ${vhpath}/fstab ${vhpath}/fstab.$mypid
			}
		}
		set fs_args  " -D $mount_device -R $raw_device -M $mount_point -F $fstype"
	}

	set cmdline "fstmodvh -t $failto -f $fallback -x $vhnum -n $vhname -d $vhpath $ip_args  $fs_args" 
	catch {eval exec echo "\n\n\n" | $cmdline }
#	catch {exec logger -p daemon.notice $cmdline }

	eval .vh.middle.vh${vhnum} configure -text "$vhname-DOWN-$node"
#	destroy .vhcf .vh
	destroy .vhcf
	update idletask
}
##
##
proc do_vhremove {} {
	global .vh.middle.vh0 .vh.middle.vh1 .vh.middle.vh2 .vh.middle.vh3
	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if {$cluster_up != 0} {
		tk_messageBox -title Sorry -message "The Cluster Is No Longer Running." -type ok
		destroy .vh
		return	
	} 
	global node vhnum .vh
	update
	global cf node prv_cftime cur_cftime

##	set grepstr "lh${vhnum}stat"
##	set vhstat [exec fstha export $grepstr]
##	set vhstat [lindex $vhstat 1]
##	set grepstr "lh${vhnum}name"
##	set vhname [exec fstha export $grepstr]
##	set vhname [lindex $vhname 1]
	foreach { i j} [exec fstbolt vh${vhnum}stat ]  {
		eval set $i "-"
		eval set $i "$j"
	}

	if { $vhstat == "0" } {
		tk_messageBox -title Sorry -message "$vhname is currently up. Please bring it down before removing it." -type ok
		catch {raise .vh }

	} elseif { $vhstat == "9" } {
		tk_messageBox -title Sorry -message "Virtual Host is undefined." -type ok
		catch {raise .vh }

	} else {
		catch {exec fstdelvh -v $vhnum}
#		destroy .vh
		exec sleep 2
		eval .vh.middle.vh${vhnum} configure -text "--"
		update idletask
	}
		
}
##
##
proc set_ipmode {} {
	global doip vhname vhpath .vhcf

	if { $doip == "0" } {
		.vhcf.ip.pdev configure -state disabled
		.vhcf.ip.adev configure -state disabled
		.vhcf.ip.addr configure -state disabled
		.vhcf.ip.mask configure -state disabled
		.vhcf.ip.bcast configure -state disabled
		.vhcf.ip.ether configure -state disabled
	} else {
		.vhcf.ip.pdev configure -state normal
		.vhcf.ip.adev configure -state normal
		.vhcf.ip.addr configure -state normal
		.vhcf.ip.mask configure -state normal
		.vhcf.ip.bcast configure -state normal
		.vhcf.ip.ether configure -state normal
		tk_messageBox -title Notice -message "Any of ${vhname}'s previously defined ${vhpath}/nettab file will be overwritten. You can edit this file for additional IP address definitions." -type ok
		catch {raise .vhcf }
	}
}
##
##
proc set_fsmode {} {
	global dofs mount_device raw_device mount_point fstype vhname vhpath

	if { $dofs == "0" } {
		.vhcf.fs.mount_device configure -state disabled
		.vhcf.fs.raw_device configure -state disabled
		.vhcf.fs.mount_point configure -state disabled
		.vhcf.fs.fstype configure -state disabled
	} else {
		.vhcf.fs.mount_device configure -state normal
		.vhcf.fs.raw_device configure -state normal
		.vhcf.fs.mount_point configure -state normal
		.vhcf.fs.fstype configure -state normal
		tk_messageBox -title Notice -message "Any of ${vhname}'s previously defined ${vhpath}/fstab file will be overwritten. You can edit this file for additional IP address definitions." -type ok
		catch {raise .vhcf }
	}
}
##
##
proc do_vhconfigure {} {
	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if {$cluster_up != 0} {
		tk_messageBox -title Sorry -message "The Cluster Is No Longer Running." -type ok
		destroy .vh
		return	
	} 
	global node vhnum
	update
	global cf node prv_cftime cur_cftime
	toplevel .vhcf -borderwidth 10 
	wm title .vhcf "Configure Virtual Host"
	update

##	set grepstr "lh${vhnum}name"
##	set vhname [exec fstha export $grepstr]
##	set vhname [lindex $vhname 1]

##	set grepstr "lh${vhnum}path"
##	set vhpath [exec fstha export $grepstr]
##	set vhpath [lindex $vhpath 1]

##	set grepstr "lh${vhnum}stat"
##	set vhstat [exec fstha export $grepstr]
##	set vhstat [lindex $vhstat 1]
	foreach { i j} [exec fstbolt vh${vhnum}stat ]  {
		eval set $i "-"
		eval set $i "$j"
	}

	if { $vhstat == "0" } {
		tk_messageBox -title Sorry -message "$vhname is currently up. Please bring it down before changing it." -type ok
		catch {raise .vh }
		destroy .vhcf

	} else {
		frame .vhcf.top -borderwidth 10
		pack .vhcf.top -fill x
		label .vhcf.top.pname -text "Virtual Host's Name "  -anchor e
		entry .vhcf.top.ename -textvariable vhname -width 40
		.vhcf.top.ename delete 0 40
		.vhcf.top.ename insert 0 $vhname
		pack .vhcf.top.pname .vhcf.top.ename -side left 
	
		frame .vhcf.mid -borderwidth 10
		pack .vhcf.mid -fill x
		label .vhcf.mid.ppath -text "Virtual Host's Path "  -anchor e
		entry .vhcf.mid.epath -textvariable vhpath -width 40
		.vhcf.mid.epath delete 0 40
		.vhcf.mid.epath insert 0 $vhpath
		pack .vhcf.mid.ppath .vhcf.mid.epath -side left 

		frame .vhcf.mid2 -borderwidth 10
		pack .vhcf.mid2 -fill x
		radiobutton .vhcf.mid2.f0 -text "Fail to Node 0 if Available" -variable failto -value "0" 
		radiobutton .vhcf.mid2.f1 -text "Fail to Node 1 if Available" -variable failto -value "1" 
		radiobutton .vhcf.mid2.f2 -text "Fail to Node 2 if Available" -variable failto -value "2" 
		radiobutton .vhcf.mid2.f3 -text "Fail to Node 3 if Available" -variable failto -value "3" 
		radiobutton .vhcf.mid2.f8 -text "Fail to Any Node    " -variable failto -value "8" 
		radiobutton .vhcf.mid2.f9 -text "Don't Fail to Any Node    " -variable failto -value "9" 
		.vhcf.mid2.f8 select 
		pack .vhcf.mid2.f0 .vhcf.mid2.f1 .vhcf.mid2.f8 .vhcf.mid2.f9 -side top
		frame .vhcf.mid3 -borderwidth 10
		pack .vhcf.mid3 -fill x
		radiobutton .vhcf.mid3.f0 -text "Fallback to Node 0 When Available" -variable fallback -value "0" 
		radiobutton .vhcf.mid3.f1 -text "Fallback to Node 1 When Available" -variable fallback -value "1" 
		radiobutton .vhcf.mid3.f2 -text "Fallback to Node 2 When Available" -variable fallback -value "2" 
		radiobutton .vhcf.mid3.f3 -text "Fallback to Node 3 When Available" -variable fallback -value "3" 
		radiobutton .vhcf.mid3.f8 -text "Fallback to Any Node    " -variable fallback -value "8" 
		radiobutton .vhcf.mid3.f9 -text "Don't Fallback to Any Node    " -variable fallback -value "9" 
		.vhcf.mid3.f9 select 
		pack .vhcf.mid3.f0 .vhcf.mid3.f1 .vhcf.mid3.f8 .vhcf.mid3.f9 -side top
	
		frame .vhcf.bot -borderwidth 10
		pack .vhcf.bot -fill x
		label .vhcf.bot.lab -text "Hint: The Virtual Host's Path is a\ndirectory where all filenames that\nmatch S*.up will be executed when the\nvirtual host is brought up. Conversly,\nall S*.down scripts are executed when\nthe virtual host is brought down." 
		pack .vhcf.bot.lab -side top
			
		frame .vhcf.opts -borderwidth 10
		pack .vhcf.opts -fill x
		frame .vhcf.ip -borderwidth 10
		frame .vhcf.fs -borderwidth 10
		pack .vhcf.ip  .vhcf.fs -side left -in .vhcf.opts

		label .vhcf.ip.pdevlab -text "Primary NIC Device (eg qfe0)"
		entry .vhcf.ip.pdev -textvariable pdev -width 10
		label .vhcf.ip.adevlab -text "Alternate NIC Device (eg qfe4)"
		entry .vhcf.ip.adev -textvariable adev -width 10
		label .vhcf.ip.addrlab -text "Virtual IP Address"
		entry .vhcf.ip.addr -textvariable addr -width 16
		label .vhcf.ip.masklab -text "Netmask"
		entry .vhcf.ip.mask -textvariable mask -width 16
		label .vhcf.ip.bcastlab -text "Broadcast Address"
		entry .vhcf.ip.bcast -textvariable bcast -width 16
		label .vhcf.ip.etherlab -text "Ethernet Address"
		entry .vhcf.ip.ether -textvariable ether -width 18
		radiobutton .vhcf.ip.do1 -text "Setup Standard     IP scripts (For $node ONLY)" -variable doip -value "1" -command set_ipmode
		radiobutton .vhcf.ip.do2 -text "Setup Multipathing IP scripts (For $node ONLY)" -variable doip -value "2" -command set_ipmode
		label .vhcf.ip.mlab -text       "Note1: Multipathing IP requires 2 dummy IPs which\nwill be assigned +1 and +2 of your virtual host IP.\n\nNote2: If using Standard IP with only 1 interface\nthen use a logical for the second, e.g. eri2, eri2:1" 
		pack .vhcf.ip.do1 .vhcf.ip.do2 .vhcf.ip.pdevlab .vhcf.ip.pdev .vhcf.ip.adevlab .vhcf.ip.adev .vhcf.ip.addrlab .vhcf.ip.addr .vhcf.ip.masklab .vhcf.ip.mask .vhcf.ip.bcastlab .vhcf.ip.bcast .vhcf.ip.etherlab .vhcf.ip.ether .vhcf.ip.mlab -side top -in .vhcf.ip


		label .vhcf.fs.mountlab -text "Device to Mount (/dev/dsk/cXtXdXsX)"
		entry .vhcf.fs.mount_device -textvariable mount_device -width 30
		label .vhcf.fs.rawlab -text "Device to fsck (/dev/rdsk/cXtXdXsX)"
		entry .vhcf.fs.raw_device -textvariable raw_device -width 30
		label .vhcf.fs.mplab -text "Mount Point Path"
		entry .vhcf.fs.mount_point -textvariable mount_point -width 20
		label .vhcf.fs.fslab -text "File System Type"
		entry .vhcf.fs.fstype -textvariable fstype -width 10
		label .vhcf.fs.mtptlab1 -text " "
		label .vhcf.fs.mtptlab2 -text "Example devices:\n\n/dev/dsk/c1t2d0s0\n/dev/vx/dsk/datadg/vol01\n/dev/md/dsk/d10\n/dev/md/myset/dsk/d11"
		checkbutton .vhcf.fs.do -text "Setup File System (For $node ONLY)" -variable dofs -command set_fsmode
#		.vhcf.fs.do select
		pack  .vhcf.fs.do .vhcf.fs.mountlab .vhcf.fs.mount_device .vhcf.fs.rawlab .vhcf.fs.raw_device .vhcf.fs.mplab .vhcf.fs.mount_point .vhcf.fs.fslab .vhcf.fs.fstype .vhcf.fs.mtptlab1 .vhcf.fs.mtptlab2 -side top -in .vhcf.fs


		frame .vhcf.bottom -borderwidth 10
		pack .vhcf.bottom -fill x
		button .vhcf.bottom.set -text "Set" -command do_vhset
		button .vhcf.bottom.close -text "Close" -command "catch {raise .vh}; destroy .vhcf"
		pack .vhcf.bottom.set .vhcf.bottom.close -side top
	}
		
}
##
##
proc do_move2three {} {
	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if {$cluster_up != 0} {
		tk_messageBox -title Sorry -message "The Cluster Is No Longer Running." -type ok
		destroy .vh
		return	
	} 
	global vhnum 
	switch $vhnum {
		0 { catch {exec fstha locate 0 3 }
			}
		1 { catch {exec fstha locate 1 3 }
			}
		2 { catch {exec fstha locate 2 3 }
			}
		3 { catch {exec fstha locate 3 3 }
			}
	}	
	global .vh.middle.vh0 .vh.middle.vh1 .vh.middle.vh2 .vh.middle.vh3 vh0name vh1name vh2name vh3name
	eval .vh.middle.vh${vhnum} configure -text "\$vh${vhnum}name"
	update idletask

}
##
##
proc do_move2two {} {
	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if {$cluster_up != 0} {
		tk_messageBox -title Sorry -message "The Cluster Is No Longer Running." -type ok
		destroy .vh
		return	
	} 
	global vhnum 
	switch $vhnum {
		0 { catch {exec fstha locate 0 2 }
			}
		1 { catch {exec fstha locate 1 2 }
			}
		2 { catch {exec fstha locate 2 2 }
			}
		3 { catch {exec fstha locate 3 2 }
			}
	}	
	global .vh.middle.vh0 .vh.middle.vh1 .vh.middle.vh2 .vh.middle.vh3 vh0name vh1name vh2name vh3name
	eval .vh.middle.vh${vhnum} configure -text "\$vh${vhnum}name"
	update idletask

}
##
##
proc do_move2one {} {
	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if {$cluster_up != 0} {
		tk_messageBox -title Sorry -message "The Cluster Is No Longer Running." -type ok
		destroy .vh
		return	
	} 
	global vhnum 
	switch $vhnum {
		0 { catch {exec fstha locate 0 1 }
			}
		1 { catch {exec fstha locate 1 1 }
			}
		2 { catch {exec fstha locate 2 1 }
			}
		3 { catch {exec fstha locate 3 1 }
			}
	}	
	global .vh.middle.vh0 .vh.middle.vh1 .vh.middle.vh2 .vh.middle.vh3 vh0name vh1name vh2name vh3name
	eval .vh.middle.vh${vhnum} configure -text "\$vh${vhnum}name"
	update idletask

}
##
##
proc do_move2zero {} {
	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if {$cluster_up != 0} {
		tk_messageBox -title Sorry -message "The Cluster Is No Longer Running." -type ok
		destroy .vh
		return	
	} 
	global vhnum 
	switch $vhnum {
		0 { catch {exec fstha locate 0 0 }
			}
		1 { catch {exec fstha locate 1 0 }
			}
		2 { catch {exec fstha locate 2 0 }
			}
		3 { catch {exec fstha locate 3 0 }
			}
	}	
	global .vh.middle.vh0 .vh.middle.vh1 .vh.middle.vh2 .vh.middle.vh3 vh0name vh1name vh2name vh3name
	eval .vh.middle.vh${vhnum} configure -text "\$vh${vhnum}name"
	update idletask
}

##
##
proc do_bringdown {} {
	global vhnum 
	global node0 node1 node2 node3 vh0stat vh0node vh0name vh0nodename 
	switch $vhnum {
		0 { catch {exec fstha locate 0 N }
			}
		1 { catch {exec fstha locate 1 N }
			}
		2 { catch {exec fstha locate 2 N }
			}
		3 { catch {exec fstha locate 3 N }
			}
	}	
	update idletask
	global .vh.middle.vh0 .vh.middle.vh1 .vh.middle.vh2 .vh.middle.vh3 vh0name vh1name vh2name vh3name
	eval .vh.middle.vh${vhnum} configure -text "\$vh${vhnum}name"
	update idletask
}
##
##
proc do_vh {} {
	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if {$cluster_up != 0} {
		tk_messageBox -title Sorry -message "The Cluster Is Not Running." -type ok
		return	
	} 
	if { [winfo exists .cl] == "1" || [winfo exists .app] == "1" } {
                tk_messageBox -title Sorry -message "Please close Cluster or Applications before continuing." -type ok
                return
	}
	global node
	global cf node prv_cftime cur_cftime
	global node0 node1 node2 node3
	global vh0stat vh0node vh0name vh0nodename 
	global vh1stat vh1node vh1name vh1nodename 
	global vh2stat vh2node vh2name vh2nodename 
	global vh3stat vh3node vh3name vh3nodename 
	toplevel .vh -borderwidth 10 
	wm title .vh "Virtual Hosts"
	label .vh.slidedot -text "Please wait..."
	pack .vh.slidedot
	update

	foreach { i j} [exec fstbolt vhstat ]  {
		eval set $i "-"
		eval set $i "$j"
	}

	frame .vh.top -borderwidth 10
	pack .vh.top  -fill x
	button .vh.top.move2zero -text "Move->$node0" -command do_move2zero
	button .vh.top.move2one -text "Move->$node1" -command do_move2one
	button .vh.top.move2two -text "Move->$node2" -command do_move2two
	button .vh.top.move2three -text "Move->$node3" -command do_move2three
	button .vh.top.bringdown -text "Bring Down" -command do_bringdown
	if { $node0 != "127.0.0.1" } {
	pack .vh.top.move2zero .vh.top.move2one -side left -padx 0p -pady 10
	}
	if { $node1 != "127.0.0.1" } {
	pack .vh.top.move2one -side left -padx 0p -pady 10
	}
	if { $node2 != "127.0.0.1" } {
	pack .vh.top.move2two -side left -padx 0p -pady 10
	}
	if { $node3 != "127.0.0.1" } {
	pack .vh.top.move2three -side left -padx 0p -pady 10
	}
	pack .vh.top.bringdown -side left -padx 0p -pady 10

	frame .vh.middle -borderwidth 10
	pack .vh.middle -fill x
	radiobutton .vh.middle.vh0 -text "$vh0name-$vh0stat-$vh0nodename" -variable vhnum -value 0
	.vh.middle.vh0 select
	radiobutton .vh.middle.vh1 -text "$vh1name-$vh1stat-$vh1nodename" -variable vhnum -value 1
	radiobutton .vh.middle.vh2 -text "$vh2name-$vh2stat-$vh2nodename" -variable vhnum -value 2
	radiobutton .vh.middle.vh3 -text "$vh3name-$vh3stat-$vh3nodename" -variable vhnum -value 3
	pack .vh.middle.vh0 .vh.middle.vh1 .vh.middle.vh2 .vh.middle.vh3 -side top
		
	frame .vh.bot -borderwidth 10
	pack .vh.bot  -fill x
	button .vh.bot.configure -text "Configure" -command "catch {do_vhconfigure}"
	button .vh.bot.remove -text "Remove" -command do_vhremove
	pack .vh.bot.configure .vh.bot.remove -side left -padx 0p -pady 10
		
	frame .vh.bottom -borderwidth 10
	pack .vh.bottom  -fill x
	button .vh.bottom.exit -text "Close" -command "destroy .vh"
	pack .vh.bottom.exit -side right -padx 0p -pady 10

	destroy .vh.slidedot
}

##
##
proc do_appset {} {
	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if {$cluster_up != 0} {
		tk_messageBox -title Sorry -message "The Cluster Is No Longer Running." -type ok
		destroy .app
		return	
	} 
	global .app .app.vh0 .app.vh1 .app.vh2 app.vh3
	global .app.vh0.app0 .app.vh0.app1 .app.vh0.app2 .app.vh0.app3 
	global .app.vh1.app0 .app.vh1.app1 .app.vh1.app2 .app.vh1.app3 
	global .app.vh2.app0 .app.vh2.app1 .app.vh2.app2 .app.vh2.app3 
	global .app.vh3.app0 .app.vh3.app1 .app.vh3.app2 .app.vh3.app3 
	global appnum e_name e_start e_stop e_test e_rtest e_c e_p apcftype addition vnpath
	set good2go "yes"
	foreach {eee} { $e_name $e_start $e_stop $e_test $e_rtest $e_c $e_p} {
	eval set eeee $eee
		if {$eeee == "" } {
			set good2go "no"
		}
	}
	if { $apcftype == "nfs" || $apcftype == "oracle" } {
		set good2go "yes"
		foreach {eee} { $addition } {
		eval set eeee $eee
			if {$eeee == "" } {
				set good2go "no"
			}
		}
	}
	if {$good2go == "no"} {
		tk_messageBox -title Sorry -message "All fields must contain data to continue." -type ok
		catch {raise .apcf }
		return	
	}
	if {[string length $appnum] == "1"} {
		set vn 0
		set ap $appnum
	} else {
		set vn [string index $appnum 0 ]
		set ap [string range $appnum 1 1]
	}
	if { $apcftype == "nic" || $apcftype == "samba" || $apcftype == "apache"} {
		catch {exec echo "\n" | fstmodapp -v $vn -x $ap -F $apcftype -c $e_c -p $e_p }
	} elseif { $apcftype == "nfs" || $apcftype == "oracle" } {
		catch {exec echo "\n" | fstmodapp -v $vn -x $ap -F $apcftype -a $addition -c $e_c -p $e_p }
	} else {
		catch {exec echo "\n" | fstmodapp -v $vn -x $ap -n $e_name -s $e_start -o $e_stop -t $e_test -r $e_rtest -c $e_c -p $e_p }
	}
	
	exec sleep 2
	eval global .app.vh$vn.app$ap vh${vn}app${ap}name
	eval set apname \"[exec fstha export  lh${vn}app${ap}name]\"
	set apname [lindex $apname 1]
	eval set apstat \"[exec fstha export  lh${vn}app${ap}stat]\"
	set apstat [lindex $apstat 1]
#	foreach { i j} [exec fstbolt vh${vn}app${ap}stat ]  {
#		eval set $i "-"
#		eval set $i "$j"
#	}
	if { $apname == "" || $apname == "-" } {
		set apstat ""
	} else {
		if { $apstat == "0" } {
			set apstat ON
		} else {
			if { $apstat == "8" } {
				set apstat NOCHK
			} else {
				if { $apstat == "5" } {
					set apstat @UP
				} else {
					set apstat OFF
				}
			}
		}
	}
	eval set vh${vn}app${ap}name $apname
#	eval .app.vh$vn.app$ap configure -text "$apname-$apstat"
if { $vn == 0 } {
	if { $ap == 0 } {
		.app.vh0.app0 configure -text "$apname-$apstat"
	}
	if { $ap == 1 } {
		.app.vh0.app1 configure -text "$apname-$apstat"
	}
	if { $ap == 2 } {
		.app.vh0.app2 configure -text "$apname-$apstat"
	}
	if { $ap == 3 } {
		.app.vh0.app3 configure -text "$apname-$apstat"
	}
}
if { $vn == 1 } {
	if { $ap == 0 } {
		.app.vh1.app0 configure -text "$apname-$apstat"
	}
	if { $ap == 1 } {
		.app.vh1.app1 configure -text "$apname-$apstat"
	}
	if { $ap == 2 } {
		.app.vh1.app2 configure -text "$apname-$apstat"
	}
	if { $ap == 3 } {
		.app.vh1.app3 configure -text "$apname-$apstat"
	}
}
if { $vn == 2 } {
	if { $ap == 0 } {
		.app.vh2.app0 configure -text "$apname-$apstat"
	}
	if { $ap == 1 } {
		.app.vh2.app1 configure -text "$apname-$apstat"
	}
	if { $ap == 2 } {
		.app.vh2.app2 configure -text "$apname-$apstat"
	}
	if { $ap == 3 } {
		.app.vh2.app3 configure -text "$apname-$apstat"
	}
}
if { $vn == 3 } {
	if { $ap == 0 } {
		.app.vh3.app0 configure -text "$apname-$apstat"
	}
	if { $ap == 1 } {
		.app.vh3.app1 configure -text "$apname-$apstat"
	}
	if { $ap == 2 } {
		.app.vh3.app2 configure -text "$apname-$apstat"
	}
	if { $ap == 3 } {
		.app.vh3.app3 configure -text "$apname-$apstat"
	}
}
	update idletasks
	destroy .apcf 
}
##
##
proc do_appon {} {
	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if {$cluster_up != 0} {
		tk_messageBox -title Sorry -message "The Cluster Is No Longer Running." -type ok
		destroy .app
		return	
	} 
	global appnum
	if {[string length $appnum] == "1"} {
		set vn 0
		set ap $appnum
	} else {
		set vn [string index $appnum 0 ]
		set ap [string range $appnum 1 1]
	}
##	set grepstr "lh${vn}app${ap}stat"
##	set apstat [exec fstha export $grepstr]
##	set apstat [lindex $apstat 1]
##	set grepstr "lh${vn}app${ap}name"
##	set apname [exec fstha export $grepstr]
##	set apname [lindex $apname 1]
	foreach { i j} [exec fstbolt vh${vn}app${ap}stat ]  {
		eval set $i "-"
		eval set $i "$j"
	}
	
	if {$apname == ""  || $apname == "-" } {
		tk_messageBox -title Sorry -message "Application has not been defined." -type ok
		catch {raise .app }

	} else {
		catch {exec fstha on $ap $vn}
		exec sleep 2
		eval global .app.vh$vn.app$ap vh${vn}app${ap}name
		eval set apstat \"[exec fstha export  lh${vn}app${ap}stat]\"
		set apstat [lindex $apstat 1]
		if { $apstat == "0" } {
			set apstat ON
		} else {
			if { $apstat == "8" } {
				set apstat NOCHK
			} else {
				if { $apstat == "5" } {
					set apstat @UP
				} else {
					set apstat OFF
				}
			}
		}
		eval .app.vh$vn.app$ap configure -text "\$vh${vn}app${ap}name-$apstat"
		update idletasks
	}
}
##
##
proc do_appoff {} {
	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if {$cluster_up != 0} {
		tk_messageBox -title Sorry -message "The Cluster Is No Longer Running." -type ok
		destroy .app
		return	
	} 
	global appnum
	if {[string length $appnum] == "1"} {
		set vn 0
		set ap $appnum
	} else {
		set vn [string index $appnum 0 ]
		set ap [string range $appnum 1 1]
	}
##	set grepstr "lh${vn}app${ap}stat"
##	set apstat [exec fstha export $grepstr]
##	set apstat [lindex $apstat 1]
##	set grepstr "lh${vn}app${ap}name"
##	set apname [exec fstha export $grepstr]
##	set apname [lindex $apname 1]
	foreach { i j} [exec fstbolt vh${vn}app${ap}stat ]  {
		eval set $i "-"
		eval set $i "$j"
	}
	
	if {$apname == ""  || $apname == "-" } {
		tk_messageBox -title Sorry -message "Application has not been defined." -type ok
		catch {raise .app }

	} else {
		catch {exec fstha off $ap $vn}
		exec sleep 2
		eval global .app.vh$vn.app$ap vh${vn}app${ap}name
		eval set apstat \"[exec fstha export  lh${vn}app${ap}stat]\"
		set apstat [lindex $apstat 1]
		if { $apstat == "0" } {
			set apstat ON
		} else {
			if { $apstat == "8" } {
				set apstat NOCHK
			} else {
				if { $apstat == "5" } {
					set apstat @UP
				} else {
					set apstat OFF
				}
			}
		}
		eval .app.vh$vn.app$ap configure -text "\$vh${vn}app${ap}name-$apstat"
		update idletasks
	}
}
##
##
proc do_appnochk {} {
	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if {$cluster_up != 0} {
		tk_messageBox -title Sorry -message "The Cluster Is No Longer Running." -type ok
		destroy .app
		return	
	} 
	global appnum
	if {[string length $appnum] == "1"} {
		set vn 0
		set ap $appnum
	} else {
		set vn [string index $appnum 0 ]
		set ap [string range $appnum 1 1]
	}
##	set grepstr "lh${vn}app${ap}stat"
##	set apstat [exec fstha export $grepstr]
##	set apstat [lindex $apstat 1]
##	set grepstr "lh${vn}app${ap}name"
##	set apname [exec fstha export $grepstr]
##	set apname [lindex $apname 1]
	foreach { i j} [exec fstbolt vh${vn}app${ap}stat ]  {
		eval set $i "-"
		eval set $i "$j"
	}
	
	if {$apname == ""  || $apname == "-" } {
		tk_messageBox -title Sorry -message "Application has not been defined." -type ok
		catch {raise .app }

	} else {
		catch {exec fstha nocheck $ap $vn}
		exec sleep 2
		eval global .app.vh$vn.app$ap vh${vn}app${ap}name
		eval set apstat \"[exec fstha export  lh${vn}app${ap}stat]\"
		set apstat [lindex $apstat 1]
		if { $apstat == "0" } {
			set apstat ON
		} else {
			if { $apstat == "8" } {
				set apstat NOCHK
			} else {
				if { $apstat == "5" } {
					set apstat @UP
				} else {
					set apstat OFF
				}
			}
		}
		eval .app.vh$vn.app$ap configure -text "\$vh${vn}app${ap}name-$apstat"
		update idletasks
	}
}
##
##
proc do_appremove {} {
	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if {$cluster_up != 0} {
		tk_messageBox -title Sorry -message "The Cluster Is No Longer Running." -type ok
		destroy .app
		return	
	} 
	global appnum 
	if {[string length $appnum] == "1"} {
		set vn 0
		set ap $appnum
	} else {
		set vn [string index $appnum 0 ]
		set ap [string range $appnum 1 1]
	}
##	set grepstr "lh${vn}app${ap}name"
##	set apname [exec fstha export $grepstr]
##	set apname [lindex $apname 1]
	foreach { i j} [exec fstbolt vh${vn}app${ap}stat ]  {
		eval set $i "-"
		eval set $i "$j"
	}
	if {$apname == ""  || $apname == "-" } {
		tk_messageBox -title Sorry -message "Application has not been defined." -type ok
		catch {raise .app }
		return
	}
##	set grepstr "lh${vn}app${ap}stat"
##	set apstat [exec fstha export $grepstr]
##	set apstat [lindex $apstat 1]
##	set grepstr "lh${vn}app${ap}name"
##	set apname [exec fstha export $grepstr]
##	set apname [lindex $apname 1]

	if { $apstat == "0" && $apname != "" && $apname != "-" } {
		tk_messageBox -title Sorry -message "$apname is currently On. Please turn it Off before removing it." -type ok
		catch {raise .app }

	} else {
		catch {exec fstdelapp -v $vn -x $ap}
		exec sleep 2
		eval global .app.vh$vn.app$ap vh${vn}app${ap}name
##		eval set apname \"[exec fstha export  lh${vn}app${ap}name]\"
##		set apname [lindex $apname 1]
##		eval set apstat \"[exec fstha export  lh${vn}app${ap}stat]\"
##		set apstat [lindex $apstat 1]
	foreach { i j} [exec fstbolt vh${vn}app${ap}stat ]  {
		eval set $i "-"
		eval set $i "$j"
	}
		if { $apname == ""  || $apname == "-" } {
			set apname ""
			set apstat ""
		} else {
			if { $apstat == "0" } {
				set apstat ON
			} else {
				if { $apstat == "8" } {
					set apstat NOCHK
				} else {
					if { $apstat == "5" } {
						set apstat @UP
					} else {
						set apstat OFF
					}
				}
			}
		}
		eval set vh${vn}app${ap}name $apname
		eval .app.vh$vn.app$ap configure -text "$apname-$apstat"
		update idletasks
#		destroy .app
	}
}
##
##
proc do_appconfigure {} {
	set cluster_up [catch {exec ps -e | grep fsthbm } ]
	if {$cluster_up != 0} {
		tk_messageBox -title Sorry -message "The Cluster Is No Longer Running." -type ok
		destroy .app
		return	
	} 
	toplevel .apcf -borderwidth 10 
	wm title .apcf "Configure Application"
	update
	
	global appnum apcftype addition vnpath

	if {[string length $appnum] == "1"} {
		set vn 0
		set ap $appnum
	} else {
		set vn [string index $appnum 0 ]
		set ap [string range $appnum 1 1]
	}
	set grepstr "lh${vn}path"
	set vnpath [exec fstha export $grepstr]
	set vnpath [lindex $vnpath 1]
	
	if { $apcftype != "custom" } {
		set apstat 1
		set apname ${apcftype}$vn
		set apstart ${vnpath}/${apname}/start
		set apstop ${vnpath}/${apname}/stop
		set aptest ${vnpath}/${apname}/test
		set aprtest ${vnpath}/${apname}/remtest
		set ap_c 10
		set ap_m 30
		set ap_p 60
	}  else {
##		set grepstr "lh${vn}app${ap}stat"
##		set apstat [exec fstha export $grepstr]
##		set apstat [lindex $apstat 1]
##		set grepstr "lh${vn}app${ap}name"
##		set apname [exec fstha export $grepstr]
##		set apname [lindex $apname 1]
##		set grepstr "lh${vn}app${ap}start"
##		set apstart [exec fstha export $grepstr]
##		set apstart [lindex $apstart 1]
##		set grepstr "lh${vn}app${ap}stop"
##		set apstop [exec fstha export $grepstr]
##		set apstop [lindex $apstop 1]
##		set grepstr "lh${vn}app${ap}test"
##		set aptest [exec fstha export $grepstr]
##		set aptest [lindex $aptest 1]
##		set grepstr "lh${vn}app${ap}crit"
##		set ap_c [exec fstha export $grepstr]
##		set ap_c [lindex $ap_c 1]
##		set grepstr "lh${vn}app${ap}timeo"
##		set ap_m [exec fstha export $grepstr]
##		set ap_m [lindex $ap_m 1]
##		set grepstr "lh${vn}app${ap}pause"
##		set ap_p [exec fstha export $grepstr]
##		set ap_p [lindex $ap_p 1]
	foreach { i j} [exec fstbolt vh${vn}app${ap}stat ]  {
		eval set $i "-"
		eval set $i "$j"
	}
		if {$ap_m == "0"} {
			set ap_m 30
		}
		if {$ap_p == "0"} {
			set ap_p 60
		}
	}

##	set grepstr "lh${vn}app${ap}stat"
##	set apstat [exec fstha export $grepstr]
##	set apstat [lindex $apstat 1]

	if { $apstat == "0" } {
		tk_messageBox -title Sorry -message "$apname is currently On. Please turn it Off before configuring it." -type ok
		destroy .apcf

	} else {
		if { $apcftype == "nfs"} {
			label .apcf.additionlab -text "Mount Point"
			entry .apcf.addition -textvariable addition -width 60
			grid config .apcf.additionlab -column 0 -row 0 -sticky "e"
			grid config .apcf.addition -column 1 -row 0 -sticky "w"
		} 
		if { $apcftype == "oracle"} {
			label .apcf.additionlab -text "Oracle SID"
			entry .apcf.addition -textvariable addition -width 60
			grid config .apcf.additionlab -column 0 -row 0 -sticky "e"
			grid config .apcf.addition -column 1 -row 0 -sticky "w"
		} 
		label .apcf.l_name -text "Application Name\n"
		label .apcf.l_start -text "Start Script\n"
		label .apcf.l_stop -text "Stop Script\n"
		label .apcf.l_test -text "Test Script\n"
		label .apcf.l_rtest -text "Remote Test Script\n"
		label .apcf.l_c -text "Number of Test Failures that\nTigger Failover 0=Never"
		label .apcf.l_p -text "Number of SECS To Pause\nBefore Testing Begins"
		label .apcf.lab -text " \n"

		entry .apcf.e_name -textvariable e_name -width 60
		.apcf.e_name delete 0 60
		.apcf.e_name insert 0 $apname
		if { $apcftype != "custom" } {
			.apcf.e_name configure -state disabled
		}
		entry .apcf.e_start -textvariable e_start -width 60
		.apcf.e_start delete 0 60
		.apcf.e_start insert 0 $apstart
		if { $apcftype != "custom" } {
			.apcf.e_start configure -state disabled
		}
		entry .apcf.e_stop -textvariable e_stop -width 60
		.apcf.e_stop delete 0 60
		.apcf.e_stop insert 0 $apstop
		if { $apcftype != "custom" } {
			.apcf.e_stop configure -state disabled
		}
		entry .apcf.e_test -textvariable e_test -width 60
		.apcf.e_test delete 0 60
		.apcf.e_test insert 0 $aptest
		if { $apcftype != "custom" } {
			.apcf.e_test configure -state disabled
		}
		entry .apcf.e_rtest -textvariable e_rtest -width 60
		.apcf.e_rtest delete 0 60
		.apcf.e_rtest insert 0 $aprtest
		if { $apcftype != "custom" } {
			.apcf.e_rtest configure -state disabled
		}
		entry .apcf.e_c -textvariable e_c -width 5
		.apcf.e_c delete 0 5
		.apcf.e_c insert 0 $ap_c
		entry .apcf.e_p -textvariable e_p -width 5
		.apcf.e_p delete 0 5
		.apcf.e_p insert 0 $ap_p

		grid config .apcf.l_name -column 0 -row 1 -sticky "e"
		grid config .apcf.l_start -column 0 -row 2 -sticky "e"
		grid config .apcf.l_stop -column 0 -row 3 -sticky "e"
		grid config .apcf.l_test -column 0 -row 4 -sticky "e"
		grid config .apcf.l_rtest -column 0 -row 5 -sticky "e"
		grid config .apcf.l_c -column 0 -row 6 -sticky "e"
		grid config .apcf.l_p -column 0 -row 7 -sticky "e"

		grid config .apcf.e_name -column 1 -row 1 -sticky "w"
		grid config .apcf.e_start -column 1 -row 2 -sticky "w"
		grid config .apcf.e_stop -column 1 -row 3 -sticky "w"
		grid config .apcf.e_test -column 1 -row 4 -sticky "w"
		grid config .apcf.e_rtest -column 1 -row 5 -sticky "w"
		grid config .apcf.e_c -column 1 -row 6 -sticky "w"
		grid config .apcf.e_p -column 1 -row 7 -sticky "w"

		button .apcf.set -text "Set" -command do_appset
		button .apcf.exit -text "Close" -command "catch {raise .app}; destroy .apcf"
		
		grid config .apcf.lab -column 0 -row 8 
		grid config .apcf.lab -column 1 -row 8 
		grid config .apcf.set -column 0 -row 9 -sticky "e"
		grid config .apcf.exit -column 1 -row 9 -sticky "w"

		grid columnconfigure . 1 -weight 1

	}
}

##
##
proc do_app {} {
        set cluster_up [catch {exec ps -e | grep fsthbm } ]
        if {$cluster_up != 0} {
                tk_messageBox -title Sorry -message "The Cluster Is Not Running." -type ok
                return
        }
	if { [winfo exists .vh] == "1" || [winfo exists .cl] == "1" } {
                tk_messageBox -title Sorry -message "Please close Cluster or VirutalHosts before continuing." -type ok
                return
	}
        global node
        global cf node prv_cftime cur_cftime
	global vh0app0name vh0app1name vh0app2name vh0app3name 
	global vh1app0name vh1app1name vh1app2name vh1app3name 
	global vh2app0name vh2app1name vh2app2name vh2app3name 
	global vh3app0name vh3app1name vh3app2name vh3app3name 
	global .app.vh0.app0 .app.vh0.app1 .app.vh0.app2 .app.vh0.app3 
	global .app.vh1.app0 .app.vh1.app1 .app.vh1.app2 .app.vh1.app3 
	global .app.vh2.app0 .app.vh2.app1 .app.vh2.app2 .app.vh2.app3 
	global .app.vh3.app0 .app.vh3.app1 .app.vh3.app2 .app.vh3.app3 
        toplevel .app -borderwidth 10
        wm title .app "Applications"
        label .app.slidedot -text "Please wait..."
        pack .app.slidedot
        update


        frame .app.top -borderwidth 10
        pack .app.top  -fill x
        button .app.top.do_appon -text "Turn On" -command do_appon
        button .app.top.do_appoff -text "Turn Off" -command do_appoff
        button .app.top.do_appnochk -text "Set\nNoCheck\nMode" -command do_appnochk
        pack .app.top.do_appon .app.top.do_appoff .app.top.do_appnochk -side left -padx 0p -pady 10

        frame .app.mid -borderwidth 10 
        pack .app.mid -fill x
        frame .app.vh0 -borderwidth 10
        pack .app.vh0
	foreach { i j} [exec fstbolt vhapp ]  {
		eval set $i "-"
		eval set $i "$j"
	}
        if {$vh0node != "9" && $vh0name != "" && $vh0name != "-"}  {
		if { $vh0app0name != "" && $vh0app0name != "-"} {
			if { $vh0app0stat == "0"  } {
				set vh0app0stat ON
			} else {
				if { $vh0app0stat == "8" } {
					set vh0app0stat NOCHK
				} else {
					if { $vh0app0stat == "5" } {
						set vh0app0stat @UP
					} else {
						set vh0app0stat OFF
					}
				}
			}
		} else {
			set vh0app0stat ""
		}
		if { $vh0app1name != "" && $vh0app1name != "-"} {
			if { $vh0app1stat == "0"  } {
				set vh0app1stat ON
			} else {
				if { $vh0app1stat == "8" } {
					set vh0app1stat NOCHK
				} else {
					if { $vh0app0stat == "5" } {
						set vh0app0stat @UP
					} else {
						set vh0app1stat OFF
					}
				}
			}
		} else {
			set vh0app1stat ""
		}
		if { $vh0app2name != "" && $vh0app2name != "-"} {
			if { $vh0app2stat == "0"  } {
				set vh0app2stat ON
			} else {
				if { $vh0app2stat == "8" } {
					set vh0app2stat NOCHK
				} else {
					if { $vh0app0stat == "5" } {
						set vh0app0stat @UP
					} else {
						set vh0app2stat OFF
					}
				}
			}
		} else {
			set vh0app2stat ""
		}
		if { $vh0app3name != "" && $vh0app3name != "-"} {
			if { $vh0app3stat == "0"  } {
				set vh0app3stat ON
			} else {
				if { $vh0app3stat == "8" } {
					set vh0app3stat NOCHK
				} else {
					if { $vh0app0stat == "5" } {
						set vh0app0stat @UP
					} else {
						set vh0app3stat OFF
					}
				}
			}
		} else {
			set vh0app3stat ""
		}

                label .app.vh0.lab -text "$vh0name"
                radiobutton .app.vh0.app0 -text "$vh0app0name-$vh0app0stat" -variable appnum -value 0
                .app.vh0.app0 select
                radiobutton .app.vh0.app1 -text "$vh0app1name-$vh0app1stat" -variable appnum -value 1
                radiobutton .app.vh0.app2 -text "$vh0app2name-$vh0app2stat" -variable appnum -value 2
                radiobutton .app.vh0.app3 -text "$vh0app3name-$vh0app3stat" -variable appnum -value 3
                pack .app.vh0.lab .app.vh0.app0 .app.vh0.app1 .app.vh0.app2 .app.vh0.app3 -side top -in .app.vh0
                pack .app.vh0 -in .app.mid  -side left
        }


        frame .app.vh1 -borderwidth 10
        pack .app.vh1 -fill x -in .app.mid
##        set vh1node [exec fstha export lh1node]
##        set vh1node [lindex $vh1node 1]
##        set vh1name [exec fstha export lh1name]
##        set vh1name [lindex $vh1name 1]
        if {$vh1node != "9" && $vh1name != "" && $vh1name != "-"}  {
##                set vh1app0name [exec fstha export lh1app0name]
##                set vh1app0name [lindex $vh1app0name 1]
##                set vh1app1name [exec fstha export lh1app1name]
##                set vh1app1name [lindex $vh1app1name 1]
##                set vh1app2name [exec fstha export lh1app2name]
##                set vh1app2name [lindex $vh1app2name 1]
##                set vh1app3name [exec fstha export lh1app3name]
##               set vh1app3name [lindex $vh1app3name 1]
##                set vh1app0stat [exec fstha export lh1app0stat]
##                set vh1app0stat [lindex $vh1app0stat 1]
##                set vh1app1stat [exec fstha export lh1app1stat]
##                set vh1app1stat [lindex $vh1app1stat 1]
##                set vh1app2stat [exec fstha export lh1app2stat]
##                set vh1app2stat [lindex $vh1app2stat 1]
##                set vh1app3stat [exec fstha export lh1app3stat]
##                set vh1app3stat [lindex $vh1app3stat 1]
		if { $vh1app0name != "" && $vh1app0name != "-"} {
			if { $vh1app0stat == "0"  } {
				set vh1app0stat ON
			} else {
				if { $vh1app0stat == "8" } {
					set vh1app0stat NOCHK
				} else {
					if { $vh0app0stat == "5" } {
						set vh0app0stat @UP
					} else {
						set vh1app0stat OFF
					}
				}
			}
		} else {
			set vh1app0stat ""
		}
		if { $vh1app1name != "" && $vh1app1name != "-"} {
			if { $vh1app1stat == "0"  } {
				set vh1app1stat ON
			} else {
				if { $vh1app1stat == "8" } {
					set vh1app1stat NOCHK
				} else {
					if { $vh0app0stat == "5" } {
						set vh0app0stat @UP
					} else {
						set vh1app1stat OFF
					}
				}
			}
		} else {
			set vh1app1stat ""
		}
		if { $vh1app2name != "" && $vh1app2name != "-"} {
			if { $vh1app2stat == "0"  } {
				set vh1app2stat ON
			} else {
				if { $vh1app2stat == "8" } {
					set vh1app2stat NOCHK
				} else {
					if { $vh0app0stat == "5" } {
						set vh0app0stat @UP
					} else {
						set vh1app2stat OFF
					}
				}
			}
		} else {
			set vh1app2stat ""
		}
		if { $vh1app3name != "" && $vh1app3name != "-"} {
			if { $vh1app3stat == "0"  } {
				set vh1app3stat ON
			} else {
				if { $vh1app3stat == "8" } {
					set vh1app3stat NOCHK
				} else {
					if { $vh0app0stat == "5" } {
						set vh0app0stat @UP
					} else {
						set vh1app3stat OFF
					}
				}
			}
		} else {
			set vh1app3stat ""
		}

                label .app.vh1.lab -text "$vh1name"
                radiobutton .app.vh1.app0 -text "$vh1app0name-$vh1app0stat" -variable appnum -value 10
                .app.vh1.app0 select
                radiobutton .app.vh1.app1 -text "$vh1app1name-$vh1app1stat" -variable appnum -value 11
                radiobutton .app.vh1.app2 -text "$vh1app2name-$vh1app2stat" -variable appnum -value 12
                radiobutton .app.vh1.app3 -text "$vh1app3name-$vh1app3stat" -variable appnum -value 13
                pack .app.vh1.lab .app.vh1.app0 .app.vh1.app1 .app.vh1.app2 .app.vh1.app3 -side top -in .app.vh1
                pack .app.vh1 -in .app.mid -after .app.vh0 -side left
        }

        frame .app.vh2 -borderwidth 10
        pack .app.vh2 -fill x -in .app.mid
##        set vh2node [exec fstha export lh2node]
##        set vh2node [lindex $vh2node 1]
##        set vh2name [exec fstha export lh2name]
##        set vh2name [lindex $vh2name 1]
        if {$vh2node != "9" && $vh2name != "" && $vh2name != "-"}  {
##                set vh2app0name [exec fstha export lh2app0name]
##                set vh2app0name [lindex $vh2app0name 1]
##                set vh2app1name [exec fstha export lh2app1name]
##               set vh2app1name [lindex $vh2app1name 1]
##               set vh2app2name [exec fstha export lh2app2name]
##               set vh2app2name [lindex $vh2app2name 1]
##               set vh2app3name [exec fstha export lh2app3name]
##               set vh2app3name [lindex $vh2app3name 1]
##                set vh2app0stat [exec fstha export lh2app0stat]
##                set vh2app0stat [lindex $vh2app0stat 1]
##                set vh2app1stat [exec fstha export lh2app1stat]
##                set vh2app1stat [lindex $vh2app1stat 1]
##                set vh2app2stat [exec fstha export lh2app2stat]
##                set vh2app2stat [lindex $vh2app2stat 1]
##                set vh2app3stat [exec fstha export lh2app3stat]
##                set vh2app3stat [lindex $vh2app3stat 1]
		if { $vh2app0name != "" && $vh2app0name != "-"} {
			if { $vh2app0stat == "0"  } {
				set vh2app0stat ON
			} else {
				if { $vh2app0stat == "8" } {
					set vh2app0stat NOCHK
				} else {
					if { $vh0app0stat == "5" } {
						set vh0app0stat @UP
					} else {
						set vh2app0stat OFF
					}
				}
			}
		} else {
			set vh2app0stat ""
		}
		if { $vh2app1name != "" && $vh2app1name != "-"} {
			if { $vh2app1stat == "0"  } {
				set vh2app1stat ON
			} else {
				if { $vh2app1stat == "8" } {
					set vh2app1stat NOCHK
				} else {
					if { $vh0app0stat == "5" } {
						set vh0app0stat @UP
					} else {
						set vh2app1stat OFF
					}
				}
			}
		} else {
			set vh2app1stat ""
		}
		if { $vh2app2name != "" && $vh2app2name != "-"} {
			if { $vh2app2stat == "0"  } {
				set vh2app2stat ON
			} else {
				if { $vh2app2stat == "8" } {
					set vh2app2stat NOCHK
				} else {
					if { $vh0app0stat == "5" } {
						set vh0app0stat @UP
					} else {
						set vh2app2stat OFF
					}
				}
			}
		} else {
			set vh2app2stat ""
		}
		if { $vh2app3name != "" && $vh2app3name != "-"} {
			if { $vh2app3stat == "0"  } {
				set vh2app3stat ON
			} else {
				if { $vh2app3stat == "8" } {
					set vh2app3stat NOCHK
				} else {
					if { $vh0app0stat == "5" } {
						set vh0app0stat @UP
					} else {
						set vh2app3stat OFF
					}
				}
			}
		} else {
			set vh2app3stat ""
		}

                label .app.vh2.lab -text "$vh2name"
                radiobutton .app.vh2.app0 -text "$vh2app0name-$vh2app0stat" -variable appnum -value 20
                .app.vh2.app0 select
                radiobutton .app.vh2.app1 -text "$vh2app1name-$vh2app1stat" -variable appnum -value 21
                radiobutton .app.vh2.app2 -text "$vh2app2name-$vh2app2stat" -variable appnum -value 22
                radiobutton .app.vh2.app3 -text "$vh2app3name-$vh2app3stat" -variable appnum -value 23
                pack .app.vh2.lab .app.vh2.app0 .app.vh2.app1 .app.vh2.app2 .app.vh2.app3 -side top -in .app.vh2
                pack .app.vh2 -in .app.mid -after .app.vh1 -side left
        }


        frame .app.vh3 -borderwidth 10
        pack .app.vh3 -fill x -in .app.mid
##        set vh3node [exec fstha export lh3node]
##        set vh3node [lindex $vh3node 1]
##        set vh3name [exec fstha export lh3name]
##        set vh3name [lindex $vh3name 1]
        if {$vh3node != "9" && $vh3name != "" && $vh3name != "-"}  {
##                set vh3app0name [exec fstha export lh3app0name]
##                set vh3app0name [lindex $vh3app0name 1]
##                set vh3app1name [exec fstha export lh3app1name]
##                set vh3app1name [lindex $vh3app1name 1]
##                set vh3app2name [exec fstha export lh3app2name]
##                set vh3app2name [lindex $vh3app2name 1]
##                set vh3app3name [exec fstha export lh3app3name]
##                set vh3app3name [lindex $vh3app3name 1]
##                set vh3app0stat [exec fstha export lh3app0stat]
##                set vh3app0stat [lindex $vh3app0stat 1]
##                set vh3app1stat [exec fstha export lh3app1stat]
##                set vh3app1stat [lindex $vh3app1stat 1]
##                set vh3app2stat [exec fstha export lh3app2stat]
##                set vh3app2stat [lindex $vh3app2stat 1]
##                set vh3app3stat [exec fstha export lh3app3stat]
##                set vh3app3stat [lindex $vh3app3stat 1]
		if { $vh3app0name != "" && $vh3app0name != "-"} {
			if { $vh3app0stat == "0"  } {
				set vh3app0stat ON
			} else {
				if { $vh3app0stat == "8" } {
					set vh3app0stat NOCHK
				} else {
					if { $vh0app0stat == "5" } {
						set vh0app0stat @UP
					} else {
						set vh3app0stat OFF
					}
				}
			}
		} else {
			set vh3app0stat ""
		}
		if { $vh3app1name != "" && $vh3app1name != "-"} {
			if { $vh3app1stat == "0"  } {
				set vh3app1stat ON
			} else {
				if { $vh3app1stat == "8" } {
					set vh3app1stat NOCHK
				} else {
					if { $vh0app0stat == "5" } {
						set vh0app0stat @UP
					} else {
						set vh3app1stat OFF
					}
				}
			}
		} else {
			set vh3app1stat ""
		}
		if { $vh3app2name != "" && $vh3app2name != "-"} {
			if { $vh3app2stat == "0"  } {
				set vh3app2stat ON
			} else {
				if { $vh3app2stat == "8" } {
					set vh3app2stat NOCHK
				} else {
					if { $vh0app0stat == "5" } {
						set vh0app0stat @UP
					} else {
						set vh3app2stat OFF
					}
				}
			}
		} else {
			set vh3app2stat ""
		}
		if { $vh3app3name != "" && $vh3app3name != "-"} {
			if { $vh3app3stat == "0"  } {
				set vh3app3stat ON
			} else {
				if { $vh3app3stat == "8" } {
					set vh3app3stat NOCHK
				} else {
					if { $vh0app0stat == "5" } {
						set vh0app0stat @UP
					} else {
						set vh3app3stat OFF
					}
				}
			}
		} else {
			set vh3app3stat ""
		}

                label .app.vh3.lab -text "$vh3name"
                radiobutton .app.vh3.app0 -text "$vh3app0name-$vh3app0stat" -variable appnum -value 30
                .app.vh3.app0 select
                radiobutton .app.vh3.app1 -text "$vh3app1name-$vh3app1stat" -variable appnum -value 31
                radiobutton .app.vh3.app2 -text "$vh3app2name-$vh3app2stat" -variable appnum -value 32
                radiobutton .app.vh3.app3 -text "$vh3app3name-$vh3app3stat" -variable appnum -value 33
                pack .app.vh3.lab .app.vh3.app0 .app.vh3.app1 .app.vh3.app2 .app.vh3.app3 -side top -in .app.vh3
                pack .app.vh3 -in .app.mid -after .app.vh2 -side left
        }


        frame .app.bot -borderwidth 10
        pack .app.bot  -fill x -padx 0p -pady 10
        radiobutton .app.bot.custom -text "Custom   " -variable apcftype -value custom
        .app.bot.custom select 
        radiobutton .app.bot.samba -text "Samba    " -variable apcftype -value samba -width 15
        radiobutton .app.bot.nfs -text "NFS      " -variable apcftype -value nfs -width 15
        radiobutton .app.bot.nic -text "NIC      " -variable apcftype -value nic -width 15
        radiobutton .app.bot.apache -text "Apache   " -variable apcftype -value apache -width 15
#        radiobutton .app.bot.mysql -text "MySql    " -variable apcftype -value mysql -width 15
        radiobutton .app.bot.oracle -text "Oracle   " -variable apcftype -value oracle -width 15
        button .app.bot.configure -text "Configure" -command "catch {do_appconfigure}"
        button .app.bot.remove -text "Remove" -command do_appremove
	label .app.bot.lab -text " "
        pack \
		.app.bot.configure \
                .app.bot.samba \
                .app.bot.nfs \
                .app.bot.nic \
                .app.bot.apache \
		.app.bot.oracle  \
		.app.bot.custom \
		.app.bot.lab .app.bot.remove -side top -in .app.bot
#               .app.bot.mysql \

        frame .app.bottom -borderwidth 10
        pack .app.bottom  -fill x
        button .app.bottom.exit -text "Close" -command "destroy .app"
        pack .app.bottom.exit -side right -padx 0p -pady 10

        destroy .app.slidedot
}




##
##
proc log {msg} {
	global .lf.logw
	if {[string length $msg]} {
		.lf.logw configure -state normal
		.lf.logw insert end "$msg\n"
		.lf.logw see end
		.lf.logw configure -state disabled
		update
	}
}
##
##
proc clog {chan} {
	global .lf.logw
	
	if { ![eof $chan]} {
		log [gets $chan]
	}
}

##
proc show_down {} {
	global .nodelist lgrey_dot lgrey_cone 
		.nodelist.0_cstat configure -image $lgrey_cone
		.nodelist.1_cstat configure -image $lgrey_cone
		.nodelist.2_cstat configure -image $lgrey_cone
		.nodelist.3_cstat configure -image $lgrey_cone
		.nodelist.0_vh0 configure -image $lgrey_dot
		.nodelist.0_vh1 configure -image $lgrey_dot
		.nodelist.0_vh2 configure -image $lgrey_dot
		.nodelist.0_vh3 configure -image $lgrey_dot
		.nodelist.1_vh0 configure -image $lgrey_dot
		.nodelist.1_vh1 configure -image $lgrey_dot
		.nodelist.1_vh2 configure -image $lgrey_dot
		.nodelist.1_vh3 configure -image $lgrey_dot
		.nodelist.2_vh0 configure -image $lgrey_dot
		.nodelist.2_vh1 configure -image $lgrey_dot
		.nodelist.2_vh2 configure -image $lgrey_dot
		.nodelist.2_vh3 configure -image $lgrey_dot
		.nodelist.3_vh0 configure -image $lgrey_dot
		.nodelist.3_vh1 configure -image $lgrey_dot
		.nodelist.3_vh2 configure -image $lgrey_dot
		.nodelist.3_vh3 configure -image $lgrey_dot
		.nodelist.0_hb0 configure -image $lgrey_dot
		.nodelist.1_hb0 configure -image $lgrey_dot
		.nodelist.2_hb0 configure -image $lgrey_dot
		.nodelist.3_hb0 configure -image $lgrey_dot
		.nodelist.0_hb1 configure -image $lgrey_dot
		.nodelist.1_hb1 configure -image $lgrey_dot
		.nodelist.2_hb1 configure -image $lgrey_dot
		.nodelist.3_hb1 configure -image $lgrey_dot
}

##
##
proc showstat {chan} {
	global .nodelist mypid2 dorefresh  forceupdate .msgline.line stillupdating green_dot blue_dot yellow_dot grey_dot lgrey_dot brown_dot red_dot green_cone lgrey_cone header_box

	if { ![eof $chan]} {
	set cluster_up [catch {exec ps -e | grep fsthbm } retval ]
	if {$cluster_up != 0} {
		while { [gets $chan foo] >= 0 } {
			set stillupdating yes
		}
		if { $forceupdate == "off" } {
			show_down
		}
		return	
	} 
	set stillupdating no
	while { [gets $chan foo] >= 0 } {
		set stillupdating yes
	}

	if { $forceupdate == "off" && $stillupdating == "yes"} {
		if { $dorefresh == "on" } {
			.msgline.line configure -text "Cluster update in progress."
			update
			set dorefresh off
			return
		}
	}
#	if { $forceupdate == "off" && $stillupdating == "yes"} {
#		.msgline.line configure -text "Cluster update occurred. Display is Stale."
#		update
#		set dorefresh off
#		return
#	}
	set forceupdate off
	.msgline.line configure -text  "Please wait, working..."
	update
	
	foreach { i j} [exec fstbolt showstat ]  {
		eval set $i "-"
		eval set $i "$j"
	}

	.nodelist.0_node configure -text $node0
	.nodelist.1_node configure -text $node1
	.nodelist.2_node configure -text $node2
	.nodelist.3_node configure -text $node3
	
	if { $node0stat == "0" } {
		.nodelist.0_cstat configure -image $green_cone
	} else {
		.nodelist.0_cstat configure -image $lgrey_cone
	}
	if { $node1stat == "0" } {
		.nodelist.1_cstat configure -image $green_cone
	} else {
		.nodelist.1_cstat configure -image $lgrey_cone
	}
	if { $node2stat == "0" } {
		.nodelist.2_cstat configure -image $green_cone
	} else {
		.nodelist.2_cstat configure -image $lgrey_cone
	}
	if { $node3stat == "0" } {
		.nodelist.3_cstat configure -image $green_cone
	} else {
		.nodelist.3_cstat configure -image $lgrey_cone
	}

	eval .nodelist.0_vh0 configure -image $lgrey_dot
	eval .nodelist.0_vh1 configure -image $lgrey_dot
	eval .nodelist.0_vh2 configure -image $lgrey_dot
	eval .nodelist.0_vh3 configure -image $lgrey_dot
	eval .nodelist.1_vh0 configure -image $lgrey_dot
	eval .nodelist.1_vh1 configure -image $lgrey_dot
	eval .nodelist.1_vh2 configure -image $lgrey_dot
	eval .nodelist.1_vh3 configure -image $lgrey_dot
	eval .nodelist.2_vh0 configure -image $lgrey_dot
	eval .nodelist.2_vh1 configure -image $lgrey_dot
	eval .nodelist.2_vh2 configure -image $lgrey_dot
	eval .nodelist.2_vh3 configure -image $lgrey_dot
	eval .nodelist.3_vh0 configure -image $lgrey_dot
	eval .nodelist.3_vh1 configure -image $lgrey_dot
	eval .nodelist.3_vh2 configure -image $lgrey_dot
	eval .nodelist.3_vh3 configure -image $lgrey_dot
	
	if { $vh0node == "0" } {
		if { $vh0name != "" && $vh0name != "-" } {
			set getimage [ whatstat $vh0stat]
			eval set useimage \$$getimage
			eval .nodelist.0_vh0 configure -image $useimage
			if { $vh0stat == "0" } {
				if { $vh0app0stat == "1" || $vh0app0stat == "2" || $vh0app0stat == "3" || $vh0app1stat == "1" || $vh0app1stat == "2" || $vh0app1stat == "3" || $vh0app2stat == "1" || $vh0app2stat == "2" || $vh0app2stat == "3" || $vh0app3stat == "1" || $vh0app3stat == "2" || $vh0app3stat == "3" }  {
					.nodelist.0_vh0 configure -image $yellow_dot
				}
set x 0
			}
		} else {
			.nodelist.0_vh0 configure -image $lgrey_dot
		}
	}
	if { $vh1node == "0" } {
		if { $vh1name != "" && $vh1name != "-" } {
		set getimage [ whatstat $vh1stat]
		eval set useimage \$$getimage
		eval .nodelist.0_vh1 configure -image $useimage
		if { $vh1stat == "0" } {
		if { $vh1app0stat == "1" || $vh1app0stat == "2" || $vh1app0stat == "3" || $vh1app1stat == "1" || $vh1app1stat == "2" || $vh1app1stat == "3" || $vh1app2stat == "1" || $vh1app2stat == "2" || $vh1app2stat == "3" || $vh1app3stat == "1" || $vh1app3stat == "2" || $vh1app3stat == "3" }  {
			.nodelist.0_vh1 configure -image $yellow_dot
			}
		}
		} else {
		.nodelist.0_vh1 configure -image $lgrey_dot
		}
	}
	if { $vh2node == "0" } {
		if { $vh2name != "" && $vh2name != "-" } {
		set getimage [ whatstat $vh2stat]
		eval set useimage \$$getimage
		eval .nodelist.0_vh2 configure -image $useimage
		if { $vh2stat == "0" } {
		if { $vh2app0stat == "1" || $vh2app0stat == "2" || $vh2app0stat == "3" || $vh2app1stat == "1" || $vh2app1stat == "2" || $vh2app1stat == "3" || $vh2app2stat == "1" || $vh2app2stat == "2" || $vh2app2stat == "3" || $vh2app3stat == "1" || $vh2app3stat == "2" || $vh2app3stat == "3" }  {
			eval .nodelist.0_vh2 configure -image $yellow_dot
			}
		}
		} else {
		eval .nodelist.0_vh2 configure -image $lgrey_dot
		}
	}
	if { $vh3node == "0" } {
		if { $vh3name != "" && $vh3name != "-" } {
		set getimage [ whatstat $vh3stat]
		eval set useimage \$$getimage
		eval .nodelist.0_vh3 configure -image $useimage
		if { $vh3stat == "0" } {
		if { $vh3app0stat == "1" || $vh3app0stat == "2" || $vh3app0stat == "3" || $vh3app1stat == "1" || $vh3app1stat == "2" || $vh3app1stat == "3" || $vh3app2stat == "1" || $vh3app2stat == "2" || $vh3app2stat == "3" || $vh3app3stat == "1" || $vh3app3stat == "2" || $vh3app3stat == "3" }  {
			eval .nodelist.0_vh3 configure -image $yellow_dot
			}
		}
		} else {
		eval .nodelist.0_vh3 configure -image $lgrey_dot
		}
	}

	if { $vh0node == "1" } {
		if { $vh0name != "" && $vh0name != "-" } {
		set getimage [ whatstat $vh0stat]
		eval set useimage \$$getimage
		eval .nodelist.1_vh0 configure -image $useimage
		if { $vh0stat == "0" } {
		if { $vh0app0stat == "1" || $vh0app0stat == "2" || $vh0app0stat == "3" || $vh0app1stat == "1" || $vh0app1stat == "2" || $vh0app1stat == "3" || $vh0app2stat == "1" || $vh0app2stat == "2" || $vh0app2stat == "3" || $vh0app3stat == "1" || $vh0app3stat == "2" || $vh0app3stat == "3" }  {
			eval .nodelist.1_vh0 configure -image $yellow_dot
			}
		}
		} else {
		eval .nodelist.1_vh0 configure -image $lgrey_dot
		}
	}
	if { $vh1node == "1" } {
		if { $vh1name != "" && $vh1name != "-" } {
		set getimage [ whatstat $vh1stat]
		eval set useimage \$$getimage
		eval .nodelist.1_vh1 configure -image $useimage
		if { $vh1stat == "0" } {
		if { $vh1app0stat == "1" || $vh1app0stat == "2" || $vh1app0stat == "3" || $vh1app1stat == "1" || $vh1app1stat == "2" || $vh1app1stat == "3" || $vh1app2stat == "1" || $vh1app2stat == "2" || $vh1app2stat == "3" || $vh1app3stat == "1" || $vh1app3stat == "2" || $vh1app3stat == "3" }  {
			eval .nodelist.1_vh1 configure -image $yellow_dot
			}
		}
		} else {
		eval .nodelist.1_vh1 configure -image $lgrey_dot
		}
	}
	if { $vh2node == "1" } {
		if { $vh2name != "" && $vh2name != "-" } {
		set getimage [ whatstat $vh2stat]
		eval set useimage \$$getimage
		eval .nodelist.1_vh2 configure -image $useimage
		if { $vh2stat == "0" } {
		if { $vh2app0stat == "1" || $vh2app0stat == "2" || $vh2app0stat == "3" || $vh2app1stat == "1" || $vh2app1stat == "2" || $vh2app1stat == "3" || $vh2app2stat == "1" || $vh2app2stat == "2" || $vh2app2stat == "3" || $vh2app3stat == "1" || $vh2app3stat == "2" || $vh2app3stat == "3" }  {
			eval .nodelist.1_vh2 configure -image $yellow_dot
			}
		}
		} else {
		eval .nodelist.1_vh2 configure -image $lgrey_dot
		}
	}
	if { $vh3node == "1" } {
		if { $vh3name != "" && $vh3name != "-" } {
		set getimage [ whatstat $vh3stat]
		eval set useimage \$$getimage
		eval .nodelist.1_vh3 configure -image $useimage
		if { $vh3stat == "0" } {
		if { $vh3app0stat == "1" || $vh3app0stat == "2" || $vh3app0stat == "3" || $vh3app1stat == "1" || $vh3app1stat == "2" || $vh3app1stat == "3" || $vh3app2stat == "1" || $vh3app2stat == "2" || $vh3app2stat == "3" || $vh3app3stat == "1" || $vh3app3stat == "2" || $vh3app3stat == "3" }  {
			eval .nodelist.1_vh3 configure -image $yellow_dot
			}
		}
		} else {
		eval .nodelist.1_vh3 configure -image $lgrey_dot
		}
	}

	if { $vh0node == "2" } {
		if { $vh0name != "" && $vh0name != "-" } {
		set getimage [ whatstat $vh0stat]
		eval set useimage \$$getimage
		eval .nodelist.2_vh0 configure -image $useimage
		if { $vh0stat == "0" } {
		if { $vh0app0stat == "1" || $vh0app0stat == "2" || $vh0app0stat == "3" || $vh0app1stat == "1" || $vh0app1stat == "2" || $vh0app1stat == "3" || $vh0app2stat == "1" || $vh0app2stat == "2" || $vh0app2stat == "3" || $vh0app3stat == "1" || $vh0app3stat == "2" || $vh0app3stat == "3" }  {
			eval .nodelist.2_vh0 configure -image $yellow_dot
			}
		}
		} else {
		eval .nodelist.2_vh0 configure -image $lgrey_dot
		}
	}
	if { $vh1node == "2" } {
		if { $vh1name != "" && $vh1name != "-" } {
		set getimage [ whatstat $vh1stat]
		eval set useimage \$$getimage
		eval .nodelist.2_vh1 configure -image $useimage
		if { $vh1stat == "0" } {
		if { $vh1app0stat == "1" || $vh1app0stat == "2" || $vh1app0stat == "3" || $vh1app1stat == "1" || $vh1app1stat == "2" || $vh1app1stat == "3" || $vh1app2stat == "1" || $vh1app2stat == "2" || $vh1app2stat == "3" || $vh1app3stat == "1" || $vh1app3stat == "2" || $vh1app3stat == "3" }  {
			eval .nodelist.2_vh1 configure -image $yellow_dot
			}
		}
		} else {
		eval .nodelist.2_vh1 configure -image $lgrey_dot
		}
	}
	if { $vh2node == "2" } {
		if { $vh2name != "" && $vh2name != "-" } {
		set getimage [ whatstat $vh2stat]
		eval set useimage \$$getimage
		eval .nodelist.2_vh2 configure -image $useimage
		if { $vh2stat == "0" } {
		if { $vh2app0stat == "1" || $vh2app0stat == "2" || $vh2app0stat == "3" || $vh2app1stat == "1" || $vh2app1stat == "2" || $vh2app1stat == "3" || $vh2app2stat == "1" || $vh2app2stat == "2" || $vh2app2stat == "3" || $vh2app3stat == "1" || $vh2app3stat == "2" || $vh2app3stat == "3" }  {
			eval .nodelist.2_vh2 configure -image $yellow_dot
			}
		}
		} else {
		eval .nodelist.2_vh2 configure -image $lgrey_dot
		}
	}
	if { $vh3node == "2" } {
		if { $vh3name != "" && $vh3name != "-" } {
		set getimage [ whatstat $vh3stat]
		eval set useimage \$$getimage
		eval .nodelist.2_vh3 configure -image $useimage
		if { $vh3stat == "0" } {
		if { $vh3app0stat == "1" || $vh3app0stat == "2" || $vh3app0stat == "3" || $vh3app1stat == "1" || $vh3app1stat == "2" || $vh3app1stat == "3" || $vh3app2stat == "1" || $vh3app2stat == "2" || $vh3app2stat == "3" || $vh3app3stat == "1" || $vh3app3stat == "2" || $vh3app3stat == "3" }  {
			eval .nodelist.2_vh3 configure -image $yellow_dot
			}
		}
		} else {
		eval .nodelist.2_vh3 configure -image $lgrey_dot
		}
	}

	if { $vh0node == "3" } {
		if { $vh0name != "" && $vh0name != "-" } {
		set getimage [ whatstat $vh0stat]
		eval set useimage \$$getimage
		eval .nodelist.3_vh0 configure -image $useimage
		if { $vh0stat == "0" } {
		if { $vh0app0stat == "1" || $vh0app0stat == "2" || $vh0app0stat == "3" || $vh0app1stat == "1" || $vh0app1stat == "2" || $vh0app1stat == "3" || $vh0app2stat == "1" || $vh0app2stat == "2" || $vh0app2stat == "3" || $vh0app3stat == "1" || $vh0app3stat == "2" || $vh0app3stat == "3" }  {
			eval .nodelist.3_vh0 configure -image $yellow_dot
			}
		}
		} else {
		eval .nodelist.3_vh0 configure -image $lgrey_dot
		}
	}
	if { $vh1node == "3" } {
		if { $vh1name != "" && $vh1name != "-" } {
		set getimage [ whatstat $vh1stat]
		eval set useimage \$$getimage
		eval .nodelist.3_vh1 configure -image $useimage
		if { $vh1stat == "0" } {
		if { $vh1app0stat == "1" || $vh1app0stat == "2" || $vh1app0stat == "3" || $vh1app1stat == "1" || $vh1app1stat == "2" || $vh1app1stat == "3" || $vh1app2stat == "1" || $vh1app2stat == "2" || $vh1app2stat == "3" || $vh1app3stat == "1" || $vh1app3stat == "2" || $vh1app3stat == "3" }  {
			eval .nodelist.3_vh1 configure -image $yellow_dot
			}
		}
		} else {
		eval .nodelist.3_vh1 configure -image $lgrey_dot
		}
	}
	if { $vh2node == "3" } {
		if { $vh2name != "" && $vh2name != "-" } {
		set getimage [ whatstat $vh2stat]
		eval set useimage \$$getimage
		eval .nodelist.3_vh2 configure -image $useimage
		if { $vh2stat == "0" } {
		if { $vh2app0stat == "1" || $vh2app0stat == "2" || $vh2app0stat == "3" || $vh2app1stat == "1" || $vh2app1stat == "2" || $vh2app1stat == "3" || $vh2app2stat == "1" || $vh2app2stat == "2" || $vh2app2stat == "3" || $vh2app3stat == "1" || $vh2app3stat == "2" || $vh2app3stat == "3" }  {
			eval .nodelist.3_vh2 configure -image $yellow_dot
			}
		}
		} else {
		eval .nodelist.3_vh2 configure -image $lgrey_dot
		}
	}
	if { $vh3node == "3" } {
		if { $vh3name != "" && $vh3name != "-" } {
		set getimage [ whatstat $vh3stat]
		eval set useimage \$$getimage
		eval .nodelist.3_vh3 configure -image $useimage
		if { $vh3stat == "0" } {
		if { $vh3app0stat == "1" || $vh3app0stat == "2" || $vh3app0stat == "3" || $vh3app1stat == "1" || $vh3app1stat == "2" || $vh3app1stat == "3" || $vh3app2stat == "1" || $vh3app2stat == "2" || $vh3app2stat == "3" || $vh3app3stat == "1" || $vh3app3stat == "2" || $vh3app3stat == "3" }  {
			eval .nodelist.3_vh3 configure -image $yellow_dot
			}
		}
		} else {
		eval .nodelist.3_vh3 configure -image $lgrey_dot
		}
	}


	if { $hb0stat0 == "0" } {
		.nodelist.0_hb0 configure -image $green_dot
	} else {
		.nodelist.0_hb0 configure -image $lgrey_dot
	}
	if { $hb0stat1 == "0" } {
		.nodelist.1_hb0 configure -image $green_dot
	} else {
		.nodelist.1_hb0 configure -image $lgrey_dot
	}
	if { $hb0stat2 == "0" } {
		.nodelist.2_hb0 configure -image $green_dot
	} else {
		.nodelist.2_hb0 configure -image $lgrey_dot
	}
	if { $hb0stat3 == "0" } {
		.nodelist.3_hb0 configure -image $green_dot
	} else {
		.nodelist.3_hb0 configure -image $lgrey_dot
	}
	if { $hb1stat0 == "0" } {
		.nodelist.0_hb1 configure -image $green_dot
	} else {
		.nodelist.0_hb1 configure -image $lgrey_dot
	}
	if { $hb1stat1 == "0" } {
		.nodelist.1_hb1 configure -image $green_dot
	} else {
		.nodelist.1_hb1 configure -image $lgrey_dot
	}
	if { $hb1stat2 == "0" } {
		.nodelist.2_hb1 configure -image $green_dot
	} else {
		.nodelist.2_hb1 configure -image $lgrey_dot
	}
	if { $hb1stat3 == "0" } {
		.nodelist.3_hb1 configure -image $green_dot
	} else {
		.nodelist.3_hb1 configure -image $lgrey_dot
	}
	
	set stillupdating no
	set dorefresh off
	.msgline.line configure -text "Refresh Complete."
	update
	}
}

##
##
proc myexit { } {
	global mypid1 mypid2
	catch { exec kill  $mypid1 $mypid2 }
	exit
}
	
##
##
proc screen {} {
	global cf node prv_cftime cur_cftime mypid1 mypid2 stillupdating forceupdate dorefresh basedir .lf.logw green_dot blue_dot yellow_dot grey_dot lgrey_dot brown_dot red_dot green_cone lgrey_cone header_box

	label .ban -image $header_box 
	pack .ban -padx 0 -pady 0


	set stillupdating no

	frame .top -borderwidth 10
	button .top.cluster -text "Cluster\n${node}" \
		-command "catch {do_cluster; raise .cl}"
	button .top.vh -text "Virtual Hosts" \
		-command "catch {do_vh; raise .vh}"
	button .top.app -text "Applications" \
		-command "catch { do_app; raise .app}"
	button .top.exit -text "Exit" -command "myexit"
	pack .top.cluster .top.vh .top.app .top.exit -side left -in .top
	pack .top -side top -padx 0p -pady 10

	frame .nodelist -borderwidth 10
	pack .nodelist

	label .nodelist.l_cstat -text "" 
	label .nodelist.l_node -text "Node" -relief ridge
	label .nodelist.l_hb0 -text "10.1.1.x" -relief ridge
	label .nodelist.l_hb1 -text "10.1.2.x" -relief ridge
	label .nodelist.l_vh0 -text "vhost0" -relief ridge
	label .nodelist.l_vh1 -text "vhost1" -relief ridge
	label .nodelist.l_vh2 -text "vhost2" -relief ridge
	label .nodelist.l_vh3 -text "vhost3" -relief ridge

	label .nodelist.0_cstat -image $lgrey_cone -relief ridge
	label .nodelist.0_node -text "" -relief ridge
	label .nodelist.0_hb0 -image $lgrey_dot -relief ridge
	label .nodelist.0_hb1 -image $lgrey_dot -relief ridge
	label .nodelist.0_vh0 -image $lgrey_dot -relief ridge
	label .nodelist.0_vh1 -image $lgrey_dot -relief ridge
	label .nodelist.0_vh2 -image $lgrey_dot -relief ridge
	label .nodelist.0_vh3 -image $lgrey_dot -relief ridge

	label .nodelist.1_cstat -image $lgrey_cone -relief ridge
	label .nodelist.1_node -text "" -relief ridge
	label .nodelist.1_hb0 -image $lgrey_dot -relief ridge
	label .nodelist.1_hb1 -image $lgrey_dot -relief ridge
	label .nodelist.1_vh0 -image $lgrey_dot -relief ridge
	label .nodelist.1_vh1 -image $lgrey_dot -relief ridge
	label .nodelist.1_vh2 -image $lgrey_dot -relief ridge
	label .nodelist.1_vh3 -image $lgrey_dot -relief ridge

	label .nodelist.2_cstat -image $lgrey_cone -relief ridge
	label .nodelist.2_node -text "" -relief ridge
	label .nodelist.2_hb0 -image $lgrey_dot -relief ridge
	label .nodelist.2_hb1 -image $lgrey_dot -relief ridge
	label .nodelist.2_vh0 -image $lgrey_dot -relief ridge
	label .nodelist.2_vh1 -image $lgrey_dot -relief ridge
	label .nodelist.2_vh2 -image $lgrey_dot -relief ridge
	label .nodelist.2_vh3 -image $lgrey_dot -relief ridge

	label .nodelist.3_cstat -image $lgrey_cone -relief ridge
	label .nodelist.3_node -text "" -relief ridge
	label .nodelist.3_hb0 -image $lgrey_dot -relief ridge
	label .nodelist.3_hb1 -image $lgrey_dot -relief ridge
	label .nodelist.3_vh0 -image $lgrey_dot -relief ridge
	label .nodelist.3_vh1 -image $lgrey_dot -relief ridge
	label .nodelist.3_vh2 -image $lgrey_dot -relief ridge
	label .nodelist.3_vh3 -image $lgrey_dot -relief ridge

	grid config .nodelist.l_cstat -column 0 -row 0 -sticky "e"
	grid config .nodelist.l_node -column 1 -row 0 -sticky "w"
	grid config .nodelist.l_hb0 -column 2 -row 0 -sticky "w"
	grid config .nodelist.l_hb1 -column 3 -row 0 -sticky "w"
	grid config .nodelist.l_vh0 -column 4 -row 0 -sticky "w"
	grid config .nodelist.l_vh1 -column 5 -row 0 -sticky "w"
	grid config .nodelist.l_vh2 -column 6 -row 0 -sticky "w"
	grid config .nodelist.l_vh3 -column 7 -row 0 -sticky "w"

	grid config .nodelist.0_cstat -column 0 -row 1 -sticky "e"
	grid config .nodelist.0_node -column 1 -row 1 -sticky "w"
	grid config .nodelist.0_hb0 -column 2 -row 1 -sticky "w"
	grid config .nodelist.0_hb1 -column 3 -row 1 -sticky "w"
	grid config .nodelist.0_vh0 -column 4 -row 1 -sticky "w"
	grid config .nodelist.0_vh1 -column 5 -row 1 -sticky "w"
	grid config .nodelist.0_vh2 -column 6 -row 1 -sticky "w"
	grid config .nodelist.0_vh3 -column 7 -row 1 -sticky "w"

	grid config .nodelist.1_cstat -column 0 -row 2 -sticky "e"
	grid config .nodelist.1_node -column 1 -row 2 -sticky "w"
	grid config .nodelist.1_hb0 -column 2 -row 2 -sticky "w"
	grid config .nodelist.1_hb1 -column 3 -row 2 -sticky "w"
	grid config .nodelist.1_vh0 -column 4 -row 2 -sticky "w"
	grid config .nodelist.1_vh1 -column 5 -row 2 -sticky "w"
	grid config .nodelist.1_vh2 -column 6 -row 2 -sticky "w"
	grid config .nodelist.1_vh3 -column 7 -row 2 -sticky "w"

	grid config .nodelist.2_cstat -column 0 -row 3 -sticky "e"
	grid config .nodelist.2_node -column 1 -row 3 -sticky "w"
	grid config .nodelist.2_hb0 -column 2 -row 3 -sticky "w"
	grid config .nodelist.2_hb1 -column 3 -row 3 -sticky "w"
	grid config .nodelist.2_vh0 -column 4 -row 3 -sticky "w"
	grid config .nodelist.2_vh1 -column 5 -row 3 -sticky "w"
	grid config .nodelist.2_vh2 -column 6 -row 3 -sticky "w"
	grid config .nodelist.2_vh3 -column 7 -row 3 -sticky "w"

	grid config .nodelist.3_cstat -column 0 -row 4 -sticky "e"
	grid config .nodelist.3_node -column 1 -row 4 -sticky "w"
	grid config .nodelist.3_hb0 -column 2 -row 4 -sticky "w"
	grid config .nodelist.3_hb1 -column 3 -row 4 -sticky "w"
	grid config .nodelist.3_vh0 -column 4 -row 4 -sticky "w"
	grid config .nodelist.3_vh1 -column 5 -row 4 -sticky "w"
	grid config .nodelist.3_vh2 -column 6 -row 4 -sticky "w"
	grid config .nodelist.3_vh3 -column 7 -row 4 -sticky "w"

	set forceupdate on
	set dorefresh on

	if [ catch {set statchan [ open "| fstcstat" "r" ] } retval ] {
		set msg "Could not open fstcstat pipe. $retval"
		log { [puts $msg] }
	} else {
		fconfigure $statchan -blocking false
		fileevent $statchan readable [list showstat $statchan ]
		set mypid2 [ pid $statchan ] 
	}

#	frame .last -borderwidth 20
#	pack .last -side top
#	button .last.refresh -text "Refresh" -command "set dorefresh on;showstat $statchan"
#	pack .last.refresh  -side left -in .last


	frame .lf 
#	pack .lf -fill both -expand true
	pack .lf  -expand true
	text .lf.logw -width 82 -wrap word -height 14 -borderwidth 2 -relief ridge -setgrid true -yscrollcommand { .lf.scroll set} 
	scrollbar .lf.scroll -command { .lf.logw yview }
	pack .lf.scroll .lf.logw -side right -fill both -in .lf
		

	set logfile /var/adm/messages
	set c 0
	if [ catch {set fid [open $cf "r"] } ] {
	} else {
		while { ![eof $fid] } {
			gets $fid str
			if { [lindex $str 0] == "logfile" } {
				set logfile [lindex $str  1]
				set c [expr ${c} + 1]
				if { $c == "2" } {
					break
				}
			}
			if { [lindex $str 0] == "syslogfac" } {
				set syslogfac [lindex $str  1]
				set c [expr ${c} + 1]
				if { $c == "2" } {
					break
				}
			}
		}
	}
	if [ catch {set chan [ open "| tail -50f $logfile" "r" ] } retval] {
		set msg "Could not open $logfile. $retval"
		log { [puts $msg] }
	} else {
		fconfigure $chan -blocking false
		fileevent $chan readable [list clog $chan ]
		set mypid1 [ pid $chan ] 
	}




	frame .legend -borderwidth 10
	pack .legend -side top 
#-fill x
	label .legend.blue -image $blue_dot
	label .legend.green -image $green_dot
	label .legend.yellow -image $yellow_dot
	label .legend.brown -image $brown_dot
	label .legend.red -image $red_dot
	label .legend.bluex -text "Transition"
	label .legend.greenx -text "All is OK"
	label .legend.yellowx -text "App(s) Down"
	label .legend.brownx -text "Down"
	label .legend.redx -text "Failure"
	pack .legend.green .legend.greenx .legend.yellow .legend.yellowx .legend.blue .legend.bluex .legend.red .legend.redx .legend.brown .legend.brownx  -side left -in .legend

	frame .msgline -borderwidth 10
	pack .msgline -fill x
	label .msgline.line  -text "Ready." 
	pack .msgline.line -side left -in .msgline

}
set f [open "/opt/FSTha/etc/fstha.version" "r"]
gets $f fstversion
wm title . $fstversion
close $f
screen
