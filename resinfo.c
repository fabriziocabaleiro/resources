/* 
 * File:   resinfo.c
 * Author: fabrizio
 *
 * Created on July 19, 2013, 11:27 AM
 */

#include "resinfo.h"

ri* new_ri()
{
    ri *new;
    new = (ri*)malloc(sizeof(ri));
    new->type = NULL;
    new->fs   = NULL;
    new->dev  = NULL;
    new->gname= NULL;
    new->rname= NULL;
    new->done = '\0';
    new->next = NULL;
    new->exec_cmd = NULL;
    new->get_data = NULL;
    return new;
}

void add_ri(ri **head, ri *new)
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

void fill_ri(ri *node, char *line)
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
    else
    {
        printf("free %s %s\n", var, buf);
        free(buf);
    }
}

void free_ri(ri *node)
{
    if(node == NULL)
        return;
    free_ri(node->next);
    if(node->type != NULL)
        free(node->type);
    free(node);
}

void print_ri(ri *node)
{
    char sfmt[] = "%-12s%s\n";
    char xfmt[] = "%-12s%x\n";
    if(node == NULL)
        return;
    print_ri(node->next);
    printf("\n");
    printf(sfmt, "type", node->type);
    printf(sfmt, "label", node->label);
    printf(sfmt, "rname", node->rname);
    printf(sfmt, "gname", node->gname);
    printf(xfmt, "exec_cmd", node->exec_cmd);
    printf(xfmt, "get_data", node->get_data);
}

gconf* new_gconf()
{
    gconf *new;
    new = (gconf*)malloc(sizeof(gconf));
    new->gpath = NULL;
    new->rpath = NULL;
    return new;
}

void fill_gconf(gconf *gc, char *line)
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
    else if(!strcmp(var, "log"))
        gc->log   = buf;
    else
        free(buf);
}

void print_gconf(gconf *gc)
{
    char sfmt[] = "%-6s%s\n";
    printf(sfmt, "rpath", gc->rpath);
    printf(sfmt, "gpath", gc->gpath);
}