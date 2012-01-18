#
# Makefile for HA/FST
#
CC = /usr/local/bin/gcc
DEFINES = -D_REENTRANT -DSUNOS5
INC =
LDFLAGS = -lnsl -lsocket
OBJS= 
CFLAGS = $(COPTS) $(DEFINES) $(INC)

TARGETS = fstrdchk fsthbb fstsvr fstif fsthbm fstasm fstha fstast fsthamon fstcgi fstbolt fstupper fstrdchk fstupper  fsthbserv 
all: $(TARGETS)
 
fstupper: fstupper.c
	$(CC) $(CFLAGS) -o fstupper fstupper.c 

fsthbserv: fsthbserv.c
	$(CC) $(CFLAGS) -o fsthbserv fsthbserv.c  $(LDFLAGS) $(OBJS)

fstrdchk: fstrdchk.c
	$(CC) $(CFLAGS) -o fstrdchk fstrdchk.c 

fsthamon: fsthamon.c
	$(CC) $(CFLAGS) -o fsthamon fsthamon.c -lcurses -lintl -ldl 

fstcgi: fstcgi.c
	$(CC) $(CFLAGS) -o fstcgi fstcgi.c $(LDFLAGS) $(OBJS) 

fstha: fstha.c
	$(CC) $(CFLAGS) -o fstha fstha.c $(LDFLAGS) $(OBJS)

fstbolt: fstbolt.c
	$(CC) $(CFLAGS) -o fstbolt fstbolt.c 

fstasm: fstasm.c
	$(CC) $(CFLAGS) -o fstasm fstasm.c $(LDFLAGS) $(OBJS)

fsthbb: fsthbb.c
	$(CC) $(CFLAGS) -o fsthbb fsthbb.c $(LDFLAGS) $(OBJS) 

fstast: fstast.c
	$(CC) $(CFLAGS) -o fstast fstast.c 

fstif: fstif.c
	$(CC) $(CFLAGS) -o fstif fstif.c 

fsthbm: fsthbm.c
	$(CC) $(CFLAGS) -o fsthbm fsthbm.c $(LDFLAGS) $(OBJS)

fstsvr: fstsvr.c
	$(CC) $(CFLAGS) -o fstsvr fstsvr.c $(LDFLAGS)  $(OBJS)

clean:
	rm -f core fsthbb fstsvr fstif fsthbm fstasm fstha fstast fsthamon fstcgi fstbolt fstupper fstrdchk fstupper  fsthbserv
 
rmtarget:
	rm -f $(TARGETS)
 
clobber: clean rmtarget

install:
	mkdir -p /opt/FSTha/bin/SunOS /opt/FSTha/etc /opt
	cp $(TARGETS) notify /opt/FSTha/bin
	cp fstif /opt/FSTha/bin/SunOS
	cp -r lib man  /opt/FSTha/
