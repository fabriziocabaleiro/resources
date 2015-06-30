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
 * \file   conffile.c
 * \author Fabrizio Cabaleiro
 * \brief  
 * \date 
 *
 */

#include "conffile.h"

int get_type(char *line)
{
    char type[30];
    sscanf(line, "[%30[^]]]", type);
    if(!strcmp(type, "conf"))
        return CONFIGURATION;
    return RESOURCE;
}

ri* read_conf_file(char *cfn)
{
    FILE *fin;
    ri *head;
    ri *buf;
    gconf *gc;
    int tp;
    char line[200];
    char type[30];
    
    head = NULL;
    buf  = NULL;
    gc   = gconf_new();
    
    if(cfn == NULL)
    {
        printf("Configuration file name is a NULL pointer\n");
        return NULL;
    }
    if((fin = fopen(cfn, "r")) == NULL)
    {
        printf("Error opening configuration file '%s'\n", cfn);
        return NULL;
    }
    while(!feof(fin))
    {
        fscanf(fin, "%200[^\n]\n", line);
        if(*line == '\n' || *line == '#')
            continue;
        if(*line == '[')
        {
            tp = get_type(line);
            if(tp == RESOURCE)
            {
                if(buf != NULL)
                    ri_add(&head, buf);
                buf = ri_new();
                sscanf(line, "[%30[^]]]", type);
                buf->type = (char*)malloc((1 + strlen(type)) * sizeof(char));
                strcpy(buf->type, type);
            }
        }
        else if(tp == RESOURCE)
            ri_fill(buf, line);
        else if(tp == CONFIGURATION)
            gconf_fill(gc, line);
    }
    ri_add(&head, buf);
    for(buf = head; buf != NULL; buf = buf->next)
        buf->gc = gc;
    if(fclose(fin))
        printf("Error closing configuration file: %s\n", strerror(errno));
    if(!head)
        gconf_free(gc);
    return head;
}
