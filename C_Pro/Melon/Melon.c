#include "./include/mln_log.h"

void fun1(int p[2][5], int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            printf("%d ", *(*(p+i)+j));
        }
        printf("\n");
    }
}

void fun2(int p[][5], int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            printf("%d ", *(*(p+i)+j));
        }
        printf("\n");
    }
}

void fun3(int (*p)[5], int row, int col)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            printf("%d ", *(*(p+i)+j));
        }
        printf("\n");
    }
}
int main() {

    int p[2][5] = {
                    1,2,3,4,5,
                    6,7,8,9,10
                  };
    fun1(p,2,5);
    fun2(p,2,5);
    fun3(p,2,5);
    mln_log(debug, "This will be outputted to stderr\n");
    //mln_log_init(NULL);
    mln_log(debug, "This will be outputted to stderr and log file\n");

    return 0;
}