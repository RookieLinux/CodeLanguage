//
// Created by rookie on 2025/4/24.
//

#ifndef COMMON_H
#define COMMON_H


/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:        the pointer to the member.
 * @type:       the type of the container struct this is embedded in.
 * @member:     the name of the member within the struct.
 *
 */
#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})


#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)


#endif //COMMON_H
