#include <stdio.h>
main () {
char s[BUFSIZ],*p;

while (fgets(s,BUFSIZ,stdin) != NULL) {
for (p=s; p[0]!=0; p++) 
	p[0]=toupper(p[0]);
}
printf(s);
}
