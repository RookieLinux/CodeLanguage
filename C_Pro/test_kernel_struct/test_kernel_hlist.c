//
// Created by rookie on 2025/4/24.
//

#include "test_kernel_hlist.h"
#include <stdio.h>

int main() {
    printf("test kernel hlist!\n");

    //---------------------------------初始化---------------------------------
    struct hlist_head my_hash_table[10];
    int i;
    for (i = 0; i < 10; i++) {
        INIT_HLIST_HEAD(&my_hash_table[i]);
    }

    //--------------------------------- 插入 ---------------------------------
    struct my_struct {
        int data;
        struct hlist_node node;
    };

    struct my_struct entry1;
    entry1.data = 42;
    hlist_add_head(&entry1.node, &my_hash_table[0]);

    struct my_struct entry2;
    entry2.data = 43;
    hlist_add_head(&entry2.node, &my_hash_table[0]);

    struct my_struct entry3;
    entry3.data = 44;
    hlist_add_head(&entry3.node, &my_hash_table[0]);

    //--------------------------------- 遍历 ---------------------------------
    printf("遍历\n");
    struct my_struct *entry;
    hlist_for_each_entry(entry, &my_hash_table[0], node) {
        printf("Entry data: %d\n", entry->data);
    }
    //--------------------------------- 删除 ---------------------------------
    printf("删除\n");
    hlist_del(&entry1.node);
    hlist_for_each_entry(entry, &my_hash_table[0], node) {
        printf("Entry data: %d\n", entry->data);
    }

    return 0;
}
