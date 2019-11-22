#include <stddef.h>

extern int fun1(int n);
int(*g_funp)(int)=NULL;
int fun(int(*funp)(int n))
{
  fun1(100);
  g_funp = funp;
  return 0;
}