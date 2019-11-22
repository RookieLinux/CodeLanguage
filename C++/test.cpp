#include <stdio.h>
#include <string.h>
/*回调函数*/
extern int fun(int (*funp)(int n));
int fun1(int n)
{
   printf("%d\n",n);
   return 0;
}
int main()
{
   fun(fun1);
   return 0;
}

/*数组指针专题*/
/* 注释 CTRL+K+C   取消注释 CTRL+K+U */
// int main()
// {
//    int i=0; 
//    char *a[3]={"123","456","789"};
//    /* 定义一个数组指针  指针的类型是类型是char*a[3] */
//    /* eg：定义一个类型为int a[6]的数组指针   int(*p)[6]; */
//    char*(*p)[3];
//    p = &a;
//    for(i=0;i<3;i++)
//    printf("%s;",(*p)[i]);  
//    return 0;
// }

// /*数组传参*/
// void fun_buf(int n,int buf[])
// {
//   int i=0;
//   for(i=0;i<n;i++)
//   {
//      printf("buf[%d]=%d\n",i,buf[i]);
//   }
// }
// int main()
// {
//    int buf[5]={0,1,2,3,4}; 
//    fun_buf(5,buf);
    
//    return 0;
// }
