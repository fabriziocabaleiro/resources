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
 * \file   rrdtool.c
 * \author Fabrizio Cabaleiro
 * \brief  
 * \date July 21, 2013, 10:13 PM
 *
 */

#include "rrdtool.h"

int rrd_exits(ri *node)
{
    char fname[100];
    snprintf(fname, 100, "%s/%s.rrd", node->gc->rpath, node->rname);
    struct stat buf;
    if(stat(fname, &buf))
        return 0;
    else
        return 1;
}

void rrd_create(ri *node)
{
    char cmd[400];
    char cmd1[100];
    char cmd2[200];
    char cmd3[100];
    
    snprintf(cmd1, 100, "rrdtool create %s/%s.rrd --step 30 ", node->gc->rpath, node->rname);
    if(!strcmp(node->type, "cpu"))
    {
        snprintf(cmd2, 200,
                "DS:user:GAUGE:60:0:100 "
                "DS:nice:GAUGE:60:0:100 "
                "DS:system:GAUGE:60:0:100 "
                "DS:iowait:GAUGE:60:0:100 "
                "DS:steal:GAUGE:60:0:100 "
                "DS:idle:GAUGE:60:0:100 ");
    }
    else if(!strcmp(node->type, "net"))
    {
        snprintf(cmd2, 200,
                "DS:in:COUNTER:60:0:U "
                "DS:out:COUNTER:60:0:U ");
    }
    else if(!strcmp(node->type, "disk"))
    {
        snprintf(cmd2, 200,
                "DS:total:GAUGE:60:0:U "
                "DS:used:GAUGE:60:0:U ");
    }
    else if(!strcmp(node->type, "uptime"))
    {
        snprintf(cmd2, 200,
                "DS:users:GAUGE:60:0:U "
                "DS:l1:GAUGE:60:0:U "
                "DS:l5:GAUGE:60:0:U "
                "DS:l15:GAUGE:60:0:U ");
    }
    else if(!strcmp(node->type, "mem"))
    {
        snprintf(cmd2, 200,
                "DS:total:GAUGE:60:0:U "
                "DS:free:GAUGE:60:0:U "
                "DS:buffer:GAUGE:60:0:U "
                "DS:cached:GAUGE:60:0:U ");
    }
    else if(!strcmp(node->type, "swap"))
    {
        snprintf(cmd2, 200,
                "DS:total:GAUGE:60:0:U "
                "DS:free:GAUGE:60:0:U ");
    }
    else if(!strcmp(node->type, "ps"))
    {
        snprintf(cmd2, 200,
                "DS:cpu:GAUGE:60:0:2000 "
                "DS:mem:GAUGE:60:0:2000 ");
    }
    else if(!strcmp(node->type, "all_users"))
    {
        snprintf(cmd2, 200,
                "DS:users:GAUGE:60:0:U ");
    }
    else if(!strcmp(node->type, "current_users"))
    {
        snprintf(cmd2, 200,
                "DS:users:GAUGE:60:0:U ");
    }
    else
    {
        log_write_msg(node->gc->log, "Error rrd_create %s: %s", node->label, strerror(errno));
        return;
    }
    snprintf(cmd3, 100, 
            "RRA:AVERAGE:0.5:1:20160 "      /* Single sample for one week */
            "RRA:AVERAGE:0.5:20:52560 "     /* Average of ten minutes, data for one year */
            "RRA:AVERAGE:0.5:120:43800 ");  /* Average of one hour, for 5 years */     
    
    snprintf(cmd, 400, "%s %s %s ", cmd1, cmd2,cmd3);
    system(cmd);
}

void rrd_graph(ri *node, const args *arg)
{
    char cmd[900];
    char cmd2[700];
    if(!strcmp(node->type, "cpu"))
    {
        snprintf(cmd2, 700, 
                "DEF:user=%1$s/%2$s.rrd:user:AVERAGE "
                "DEF:nice=%1$s/%2$s.rrd:nice:AVERAGE "
                "DEF:system=%1$s/%2$s.rrd:system:AVERAGE "
                "DEF:iowait=%1$s/%2$s.rrd:iowait:AVERAGE "
                "DEF:steal=%1$s/%2$s.rrd:steal:AVERAGE "
                "DEF:idle=%1$s/%2$s.rrd:idle:AVERAGE "
                "AREA:user#ff0000:'User' "
                "STACK:nice#00ff00:'Nice' "
                "STACK:system#0000ff:'System' "
                "STACK:iowait#00ffff:'IOwait' "
                "STACK:steal#ff00ff:'Steal' "
                "STACK:idle#ffff00:'Idle' ", node->gc->rpath, node->rname);
    }
    else if(!strcmp(node->type, "disk"))
    {
        snprintf(cmd2, 700,
                "-b 1024 "
                "DEF:total=%1$s/%2$s.rrd:total:AVERAGE "
                "DEF:used=%1$s/%2$s.rrd:used:AVERAGE "
                "CDEF:used2=used,1024,* "
                "CDEF:available=total,used,-,1024,* "
                "AREA:used2#00ff00:'Used' "
                "STACK:available#0000ff:'Free' ", node->gc->rpath, node->rname);
    }
    else if(!strcmp(node->type, "net"))
    {
        snprintf(cmd2, 700,
                "DEF:in=%1$s/%2$s.rrd:in:AVERAGE "
                "DEF:out=%1$s/%2$s.rrd:out:AVERAGE "
                "LINE2:in#00ff00:'Incomming Bytes/s' "
                "LINE2:out#0000ff:'Outgoing Bytes/s' ", node->gc->rpath, node->rname);
    }
    else if(!strcmp(node->type, "uptime"))
    {
        snprintf(cmd2, 700,
                "DEF:users=%1$s/%2$s.rrd:users:AVERAGE "
                "DEF:l1=%1$s/%2$s.rrd:l1:AVERAGE "
                "DEF:l5=%1$s/%2$s.rrd:l5:AVERAGE "
                "DEF:l15=%1$s/%2$s.rrd:l15:AVERAGE "
                /*"LINE2:users#000000:'Users' "*/
                "LINE2:l1#ff0000:'1 minute' "
                "LINE2:l5#00ff00:'5 minutes' "
                "LINE2:l15#0000ff:'15 minutes' ", node->gc->rpath, node->rname);
    }
    else if(!strcmp(node->type, "mem"))
    {
        snprintf(cmd2, 700,
                "DEF:total=%1$s/%2$s.rrd:total:AVERAGE "
                "DEF:free=%1$s/%2$s.rrd:free:AVERAGE "
                "DEF:buffer=%1$s/%2$s.rrd:buffer:AVERAGE "
                "DEF:cached=%1$s/%2$s.rrd:cached:AVERAGE "
                "CDEF:free2=free,1024,* "
                "CDEF:buffer2=buffer,1024,* "
                "CDEF:cached2=cached,1024,* "
                "CDEF:other=total,free,-,buffer,-,cached,-,1024,* "
                "AREA:other#00ff00:'Inactive' "
                "STACK:buffer2#ff0000:'Buffer' "
                "STACK:cached2#ffff00:'Cached' "
                "STACK:free2#0000ff:'Free' ", node->gc->rpath, node->rname);
    }
    else if(!strcmp(node->type, "swap"))
    {
        snprintf(cmd2, 700,
                "DEF:total=%1$s/%2$s.rrd:total:AVERAGE "
                "DEF:free=%1$s/%2$s.rrd:free:AVERAGE "
                "CDEF:used=total,free,-,1024,* "
                "CDEF:free2=free,1024,* "
                "AREA:used#00ff00:'Used' "
                "STACK:free2#0000ff:'Free' ", node->gc->rpath, node->rname);
    }
    else if(!strcmp(node->type, "ps"))
    {
        snprintf(cmd2, 700,
                "DEF:cpu=%1$s/%2$s.rrd:cpu:AVERAGE "
                "DEF:mem=%1$s/%2$s.rrd:mem:AVERAGE "
                "LINE2:cpu#ff0000:'CPU %%' "
                "LINE2:mem#00ffff:'RAM %%' ", node->gc->rpath, node->rname);
    }
    else if(!strcmp(node->type, "all_users"))
    {
        snprintf(cmd2, 700,
                "DEF:users=%1$s/%2$s.rrd:users:AVERAGE "
                "LINE2:users#00ff00:'All Users' ", node->gc->rpath, node->rname);
    }
    else if(!strcmp(node->type, "current_users"))
    {
        snprintf(cmd2, 700,
                "DEF:users=%1$s/%2$s.rrd:users:AVERAGE "
                "LINE2:users#00ff00:'Current Users' ", node->gc->rpath, node->rname);
    }
    else
    {
        log_write_msg(node->gc->log, "Error trying to graph type %s that didn't match any", node->type);
        return;
    }
    snprintf(cmd, 900, "rrdtool graph %s/%s.png -l 0 -t '%s' -s -3600 -e -30 "
            "-w 550 -h 300 %s %s", node->gc->gpath, node->gname, node->label, 
            arg->rrdopt == NULL? "": arg->rrdopt, cmd2);
    system(cmd);
}

void rrd_update(ri *node, char *data)
{
    char cmd[300];
    if(!rrd_exits(node))
        rrd_create(node);
    snprintf(cmd, 300, "rrdtool update %s/%s.rrd N%s", node->gc->rpath, node->rname, data);
    log_write_msg(node->gc->log, "%s", cmd);
    system(cmd);
}
