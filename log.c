/*

Copyright (C) 2013-2015 Fabrizio Cabaleiro T.

This file is part of Resources.

Resources is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

Resources is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with Resources; see the file COPYING.  If not, see
<http://www.gnu.org/licenses/>.

*/

/** 
 * \file   log.c
 * \author Fabrizio Cabaleiro
 * \brief  
 * \date 
 *
 */

#include "log.h"

static void log_write_time(FILE *pf)
{
    char buf[30];
    time_t t = time(NULL);
    strftime(buf, 30, "%F %T", localtime(&t));
    fprintf(pf, "%s: ", buf);
}

void log_write_msg(char *file, char *fmt, ...)
{
    FILE *pf;
    va_list arg;
    va_start(arg, fmt);
    if((pf = fopen(file, "a")) == NULL)
        return;
    log_write_time(pf);
    vfprintf(pf, fmt, arg);
    fprintf(pf, "\n");
    fclose(pf);
    va_end(arg);
}
