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
 * \file   resinfo.h
 * \author Fabrizio Cabaleiro
 * \brief  Information of the resources structure
 * \date July 19, 2013, 11:27 AM
 */

#ifndef RESINFO_H
#define	RESINFO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <error.h>
#include "args.h"

#define RES_DONE 1

/* Some data is collected without the need of popen, then the file descriptor is
 * not used. */
#define RI_UNUSED_FD -2

/** 
 * \struct globalconf 
 * \brief  Global configuration struct 
 */
typedef struct globalconf
{
    char *rpath;  /**< Path to the rrd directory */
    char *gpath;  /**< Path to the graph (png) directory */
    char *wgpath; /**< Web path to the graph (png) directory */
    char *log;    /**< File name with whole path for logging */ 
    args *arg;    /**< Given arguments */
}gconf;

/**
 * \enum resource_status_enum
 * \brief Enumerate resources possible status
 * All the possible status of the resource is list here, from the begin, to the
 * running process and any possible error or done */
typedef enum resouce_status_enum
{
    RI_BEGIN,     /**< Begin value for the resource status */
    RI_DONE,      /**< The resource is done, it ran and fetched it's data */
    RI_DONE_WITH_ERROR, /**< The file descriptor was ready for reading, but, in 
                          the process there was an error */
    RI_RUNNING,   /**< The resource is executing a command to generate data */
}ri_status;

/**
 * @struct resinfo
 * @brief  Contains all the information of a resource
 */
typedef struct resinfo
{
    char *type;  /**< Type of resource              */
    char *rname; /**< RRDTOOL file name             */
    char *gname; /**< PNG file name                 */
    char *label; /**< Label to display in the graph */
    gconf *gc;   /**< Pointer to global configuration structure */
    int done;    /**< Shows if the resource is done fetching the data that it 
                      needs */
    ri_status status;  /**< Specify the current status of the resource */
    
    /**
     * \brief Execute a command to generate the expecting data
     * \param node A resource information node
     * \return Upon successful completion, 0 shall be returned; otherwise, −1
     * shall be returned and errno set to indicate the error.
     * This function shall execute a command to generate data to be read from
     * the file pointer, it shall fill the pf attribute with a valid file
     * pointer and fd with the corresponding file descriptor of pf
     */
    int (*exec_cmd)(struct resinfo *);  /**< Function pointer to get file
                                             descriptor */
    /**
     * \brief Get the data from the file pointer generated by the exec_cmd
     * command
     * \param node A resource information node
     * \param data A string to store the data in the form used by the rrdtool
     * \return TODO
     * TODO:
     */
    int (*get_data)(struct resinfo *, char *data); /**< Get data from file */
    FILE* pf; /**< Pointer to file, this file is normally generated with popen*/
    int   fd; /**< File descriptor, used with pselect to see if the asynchronous
                   data is ready to be read */

    char *fs;   /**< File system, used by df when getting the used disk space */
    char *dev;  /**< Device, used to get the transmitted bytes from the internet
                     devices */
    char *command; /**< Name of the command to be monitored with PS */
    char *user;    /**< Name of user to be monitored with PS */

    struct resinfo *next;  /**< Next instance of resource info          */
}ri;

/** 
 * \brief Allocate and define memory for a new instance of a resinfo structure
 * \param void
 * \return A pointer to a new resinfo structure
 */
ri* ri_new();

/**
 * \brief Add a new resinfo element to the list of resources information
 * \param head The head of the list
 * \param new The new instance to add into the list
 * \return void
 */
void ri_add(ri **head, ri *new);

/**
 * \brief Fill the content of a resource info structure
 * \param node A pointer to the resinfo structure to fill
 * \param line A line of the file with data to use to fill the structure
 * \return void
 */
void ri_fill(ri *node, char *line);

/**
 * \brief Free all the memory allocated for the resinfo structure
 * \param head The head of the list of resinfo structure
 * \return void
 */
void ri_free(ri *head);

/**
 * \brief Print the entire list of resinfo structures with all their content
 * \param head The head of the resinfo structure
 * \return void
 */
void ri_print(ri *head);

/**
 * \brief Allocate and define memory for a new instance of a gconf structure
 * \param void
 * \return A new instance of a gconf structure
 */
gconf* gconf_new();

void gconf_free(gconf*);

/**
 * \brief Fill a gconf instance with data from the line giving by argument
 * \param gc An instance of gconf structure to be fill
 * \param line A line from the configuration file used to fetch data from
 * \return void
 */
void gconf_fill(gconf *gc, char *line);

/**
 * \brief Print all the attributes of the gconf structure
 * \param gc A gconf structure pointer
 * \return void
 */
void gconf_print(gconf *gc);

#endif	/* RESINFO_H */

