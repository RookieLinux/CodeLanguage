#include <stdio.h>
#include <string.h>
char* fun(char p[],int n)
{   
   char *buf;
   buf = p; 
   return buf;
}

int main()
{
   char str[]="pointer";
   printf("%s",fun(str,strlen(str)));
   return 0;
}
