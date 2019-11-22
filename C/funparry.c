#include <stdio.h>

int add(int a,int b)
{
    return a+b;
}

int dec(int a,int b)
{
    return a-b;
}

int seq(int a)
{
    return a*a;
}

int main()
{
   /*因为函数的参数个数既有一个的也有俩个的所以定义成（）这种形式*/
    int (*funparry[3])()={add,dec,seq};
    int i=0,data1=0,data2=0,ret; 
    while(1)
    {
      printf("0. add\n");
      printf("1. dec\n");
      printf("2. seq\n");
      printf("select number\n");
      scanf("%d",&i);
      if(i==2)
      {
          printf("input data1\n");
          scanf("%d",&data1);
          ret = funparry[i](data1);
      }
      else
      {
          printf("input data1 and data2\n");
          scanf("%d %d",&data1,&data2);
          ret = funparry[i](data1,data2);
      }
      printf("result is %d\n",ret);
    }

   return 0;
}