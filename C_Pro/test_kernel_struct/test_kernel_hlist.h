//
// Created by rookie on 2025/4/24.
//

#ifndef TEST_KERNEL_HLIST_H
#define TEST_KERNEL_HLIST_H

#include <stdio.h>
#include "common.h"

struct hlist_head {
    struct hlist_node *first;
};

struct hlist_node {
    struct hlist_node *next, **pprev;//pprev指向当前节点的前驱节点的next指针
};

//自己定义  内核中另有实现
#define READ_ONCE(a) (a)
#define WRITE_ONCE(a, b) (a = b)
//初始化
#define HLIST_HEAD_INIT { .first = NULL }
#define HLIST_HEAD(name) struct hlist_head name = {  .first = NULL }
#define INIT_HLIST_HEAD(ptr) ((ptr)->first = NULL)
//非空判断
static inline int hlist_empty(const struct hlist_head *h)
{
    return !READ_ONCE(h->first);//宏定义用于原子读
}
//在hlist的开头插入一个新项,其中第一个struct hlist_node的pprev
//指向struct hlist_head的first
static inline void hlist_add_head(struct hlist_node *n, struct hlist_head *h)
{
    struct hlist_node *first = h->first;
    n->next = first;
    if (first)
        first->pprev = &n->next;
    WRITE_ONCE(h->first, n);//宏定义用于原子写
    n->pprev = &h->first;
}

//在指定节点之前添加一个新节点
/* next must be != NULL */
static inline void hlist_add_before(struct hlist_node *n,
                                    struct hlist_node *next)
{
    n->pprev = next->pprev;
    n->next = next;
    next->pprev = &n->next;
    WRITE_ONCE(*(n->pprev), n);
}
//在指定节点之后添加一个新节点
static inline void hlist_add_behind(struct hlist_node *n,
                                    struct hlist_node *prev)
{
    n->next = prev->next;
    WRITE_ONCE(prev->next, n);
    n->pprev = &prev->next;

    if (n->next)
        n->next->pprev  = &n->next;
}

static inline void __hlist_del(struct hlist_node *n)
{
    struct hlist_node *next = n->next;
    struct hlist_node **pprev = n->pprev;

    WRITE_ONCE(*pprev, next);//n的前一个节点的next指针指向n的下一个节点
    if (next)
        next->pprev = pprev;
}
//删除一个节点
static inline void hlist_del(struct hlist_node *n)
{
    __hlist_del(n);
    n->next = NULL;
    n->pprev = NULL;
}

/*
 * Move a list from one list head to another. Fixup the pprev
 * reference of the first entry if it exists.
 */
static inline void hlist_move_list(struct hlist_head *old,
                                   struct hlist_head *new)
{
    new->first = old->first;
    if (new->first)
        new->first->pprev = &new->first;
    old->first = NULL;
}

#define hlist_entry(ptr, type, member) container_of(ptr,type,member)

#define hlist_for_each(pos, head) \
	for (pos = (head)->first; pos ; pos = pos->next)

#define hlist_for_each_safe(pos, n, head) \
	for (pos = (head)->first; pos && ({ n = pos->next; 1; }); \
	     pos = n)

#define hlist_entry_safe(ptr, type, member) \
	({ typeof(ptr) ____ptr = (ptr); \
	   ____ptr ? hlist_entry(____ptr, type, member) : NULL; \
	})

/**
 * hlist_for_each_entry	- iterate over list of given type
 * @pos:	the type * to use as a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the hlist_node within the struct.
 */
#define hlist_for_each_entry(pos, head, member)				\
	for (pos = hlist_entry_safe((head)->first, typeof(*(pos)), member);\
	     pos;							\
	     pos = hlist_entry_safe((pos)->member.next, typeof(*(pos)), member))

#endif //TEST_KERNEL_HLIST_H
