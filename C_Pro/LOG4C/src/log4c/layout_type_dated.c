static const char version[] = "$Id: layout_type_dated.c,v 1.10 2013/09/29 17:38:18 valtri Exp $";

/*
 * layout.c
 *
 * Copyright 2001-2003, Meiosys (www.meiosys.com). All rights reserved.
 *
 * See the COPYING file for the terms of usage and distribution.
 */
#ifndef HAVE_CONFIG_H
#include "config.h"
#endif

//#include <log4c/layout.h>
//#include <log4c/priority.h>
//#include <sd/sprintf.h>
//#include <sd/sd_xplatform.h>
#include "layout.h"
#include "priority.h"
#include "../sd/sprintf.h"
#include "../sd/sd_xplatform.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*******************************************************************************/
static const char* dated_format(
    const log4c_layout_t*  	a_layout,
    const log4c_logging_event_t*a_event)
{
    static char buffer[1024];

#ifdef LOG4C_POSIX_TIMESTAMP
    struct tm tm;
    time_t t;

    t = a_event->evt_timestamp.tv_sec;
    gmtime_r(&t, &tm);
    snprintf(buffer, sizeof(buffer), "%04d%02d%02d %02d:%02d:%02d.%03ld %-8s %s- %s\n",
             tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
             tm.tm_hour, tm.tm_min, tm.tm_sec,
             a_event->evt_timestamp.tv_usec / 1000,
             log4c_priority_to_string(a_event->evt_priority),
             a_event->evt_category, a_event->evt_msg);
#else
    SYSTEMTIME stime;

    if (FileTimeToSystemTime(&a_event->evt_timestamp, &stime)) {
        snprintf(buffer, sizeof(buffer), "%04d%02d%02d %02d:%02d:%02d.%03ld %-8s %s- %s\n",
                 stime.wYear, stime.wMonth , stime.wDay,
                 stime.wHour, stime.wMinute, stime.wSecond,
                 stime.wMilliseconds,
                 log4c_priority_to_string(a_event->evt_priority),
                 a_event->evt_category, a_event->evt_msg);
    }
#endif
    return buffer;
}

/*******************************************************************************/
const log4c_layout_type_t log4c_layout_type_dated = {
    "dated",
    dated_format,
};
