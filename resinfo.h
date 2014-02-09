/** 
 * \file   resinfo.h
 * \author fabrizio
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

#define RES_DONE 1

/** 
 * \struct globalconf 
 * \brief  Global configuration struct 
 */
typedef struct globalconf
{
    char *rpath;  /**< Path to the rrd directory */
    char *gpath;  /**< Path to the graph (png) directory */
    char *log;    /**< File name with whole path for logging */ 
}gconf;

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
    
    int (*exec_cmd)(struct resinfo *);  /**< Function pointer to get file
                                             descriptor */
    int (*get_data)(struct resinfo *, char *data); /**< Get data from file */
    FILE* pf; /**< Pointer to file, this file is normally generated with popen*/
    int   fd; /**< File descriptor, used with pselect to see if the asynchronous
                   data is ready to be read */
    
    char *fs;   /**< File system, used by df when getting the used disk space */
    char *dev;  /**< Device, used to get the transmitted bytes from the internet
                     devices */
    
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

