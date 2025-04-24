#include <stdio.h>
#include "test_kernel_list.h"

typedef struct worker {
    struct list_head list;
    int id;
}worker;

int main() {
    printf("test kernel list!\n");

    //---------------------------------初始化---------------------------------
    worker per1;
    INIT_LIST_HEAD(&per1.list);
    per1.id = 1;

    //--------------------------------- 插入 ---------------------------------
    worker per2;
    per2.id = 2;
    list_add(&per2.list, &per1.list);

    worker per3;
    per3.id = 3;
    list_add(&per3.list, &per1.list);

    worker per4;
    per4.id = 4;
    list_add(&per4.list, &per1.list);

    //--------------------------------- 遍历 ---------------------------------
    printf("遍历\n");
    worker * pos;
    list_for_each_entry(pos, &(per1.list), list)
    {
        printf("pos->id is:%d\n",pos->id);
    }

    printf("**************************\n");

    list_for_each_entry_reverse(pos, &(per1.list), list)
    {
        printf("pos->id is:%d\n",pos->id);
    }

    //--------------------------------- 删除 ---------------------------------
    printf("删除\n");
    list_del_init(&per4.list);
    list_for_each_entry(pos, &(per1.list), list)
    {
        printf("pos->id is:%d\n",pos->id);
    }

    return 0;
}
