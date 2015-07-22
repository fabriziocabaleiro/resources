/* vim: set shiftwidth=4 : set tabstop=4 : set expandtab : */
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
 * \file   resinfo.c
 * \author Fabrizio Cabaleiro
 * \brief  
 * \date July 19, 2013, 11:27 AM
 *
 */

#include "resinfo.h"

ri* ri_new()
{
    ri *new;
    new = (ri*)malloc(sizeof(ri));
    new->type     = NULL;
    new->rname    = NULL;
    new->gname    = NULL;
    new->label    = NULL;
    new->fs       = NULL;
    new->dev      = NULL;
    new->command  = NULL;
    new->user     = NULL;
    new->next     = NULL;
    new->exec_cmd = NULL;
    new->get_data = NULL;
    new->pf       = NULL;
    new->fd       = RI_UNUSED_FD;
    new->status   = RI_BEGIN;
    return new;
}

void ri_add(ri **head, ri *new)
{
    ri *it;
    if(*head == NULL)
        *head = new;
    else
    {
        it = *head;
        while(it->next != NULL)
            it = it->next;
        it->next = new;
    }
}

void ri_fill(ri *node, char *line)
{
    char var[40], val[160];
    char *buf;
    sscanf(line, "%40s %160[^\n]", var, val);
    buf = (char*)malloc((1 + strlen(val)) * sizeof(char));
    strcpy(buf, val);
    if(!strcmp(var, "fs"))
        node->fs = buf;
    else if(!strcmp(var, "rname"))
        node->rname = buf;
    else if(!strcmp(var, "gname"))
        node->gname = buf;
    else if(!strcmp(var, "label"))
        node->label = buf;
    else if(!strcmp(var, "dev"))
        node->dev = buf;
    else if(!strcmp(var, "cmd"))
        node->command = buf;
    else if(!strcmp(var, "user"))
        node->user = buf;
    else
    {
        printf("free %s %s\n", var, buf);
        free(buf);
    }
}

void ri_free(ri *node)
{
    if(node == NULL)
        return;
    ri_free(node->next);
    if(node->type != NULL)
        free(node->type);
    if(node->rname != NULL)
        free(node->rname);
    if(node->gname != NULL)
        free(node->gname);
    if(node->label != NULL)
        free(node->label);
    if(node->fs != NULL)
        free(node->fs);
    if(node->dev != NULL)
        free(node->dev);
    if(node->command != NULL)
        free(node->command);
    if(node->user != NULL)
        free(node->user);
    free(node);
}

void ri_print(ri *node)
{
    char sfmt[] = "%-12s%s\n";
    char xfmt[] = "%-12s%x\n";
    if(node == NULL)
        return;
    ri_print(node->next);
    printf("\n");
    printf(sfmt, "type", node->type);
    printf(sfmt, "label", node->label);
    printf(sfmt, "rname", node->rname);
    printf(sfmt, "gname", node->gname);
    printf(xfmt, "exec_cmd", node->exec_cmd);
    printf(xfmt, "get_data", node->get_data);
}

gconf* gconf_new()
{
    gconf *new;
    new = (gconf*)malloc(sizeof(gconf));
    new->gpath = NULL;
    new->wgpath = NULL;
    new->rpath = NULL;
    new->log   = NULL;
    return new;
}

void gconf_free(gconf *gc)
{
    if(gc->gpath)
        free(gc->gpath);
    if(gc->wgpath)
        free(gc->wgpath);
    if(gc->rpath)
        free(gc->rpath);
    if(gc->log)
        free(gc->log);
    free(gc);
}

void gconf_fill(gconf *gc, char *line)
{
    char var[40], val[160];
    char *buf;
    sscanf(line, "%40s %160[^\n]", var, val);
    buf = (char*)malloc((1 + strlen(val)) * sizeof(char));
    strcpy(buf, val);
    if(!strcmp(var, "rpath"))
        gc->rpath = buf;
    else if(!strcmp(var, "gpath"))
        gc->gpath = buf;
    else if(!strcmp(var, "wgpath"))
        gc->wgpath = buf;
    else if(!strcmp(var, "log"))
        gc->log   = buf;
    else
        free(buf);
}

void gconf_print(gconf *gc)
{
    char sfmt[] = "%-6s%s\n";
    printf(sfmt, "rpath",  gc->rpath);
    printf(sfmt, "gpath",  gc->gpath);
    printf(sfmt, "wgpath", gc->wgpath);
    printf(sfmt, "log",    gc->log);
}
