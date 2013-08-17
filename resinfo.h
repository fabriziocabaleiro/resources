/* 
 * File:   resinfo.h
 * Author: fabrizio
 *
 * Created on July 19, 2013, 11:27 AM
 */

#ifndef RESINFO_H
#define	RESINFO_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <error.h>

#define RES_DONE 1

typedef struct globalconf
{
    char *rpath;
    char *gpath;
    char *log;
}gconf;

/**
 * @struct resinfo
 * @brief  Contains all the information of a resource
 */
typedef struct resinfo
{
    char *type;                        /**< Type of resource                        */
    char *rname;                       /**< RRDTOOL file name                       */
    char *gname;                       /**< PNG file name                           */
    char *label;
    gconf *gc;
    int done;
    
    int (*exec_cmd)(struct resinfo *);  /**< Function pointer to get file descriptor */
    int (*get_data)(struct resinfo *, char *data); /**< get data from file                      */
    FILE* pf;
    int   fd;
    
    char *fs;                          /**< File system */
    char *dev;
    
    struct resinfo *next;              /**< Next instance of resource info          */
}ri;

ri* new_ri();
void add_ri(ri **head, ri *new);
void fill_ri(ri *node, char *line);
void free_ri(ri *head);
void print_ri(ri *head);

gconf* new_gconf();
void fill_gconf(gconf *gc, char *line);
void print_gconf(gconf *gc);

#endif	/* RESINFO_H */

