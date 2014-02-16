/** 
 * \file   args.h
 * \author Fabrizio
 * \brief  argument processing
 * \date July 21, 2013, 10:13 PM
 *
 * This file hold all the functions and structure definitions to handle
 * correctly the arguments given in the command line.
 */

#ifndef _ARGS_H_
#define	_ARGS_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

/** 
 * \struct arguments  
 * \brief  This structure hold all the information giving by the user in the
 * command line 
 *
 * This structure contains all the information that can be giving from the user
 * to the process, some information will be store just as a flag and other will
 * be store as a char*
 */
typedef struct argument_struct 
{
    unsigned int collect:1; /**< Run the program in collecting mode */
    unsigned int test:1;    /**< Run the program in test mode */
    unsigned int graph:1;   /**< Run the program in graphing mode, the program
                                 just graph the rrd data and finish */
    unsigned int version:1; /**< Tell the program to print the current version
                                 and then exit */
    unsigned int daemon:1;  /**< Run the program in daemon mode */
    char *res;     /**< The type of resources to graph in GRAPH mode  */
    char *rrdopt;  /**< Additional options for the rrdtool command */
}args;

/** 
 * @brief Set the initial values for the arguments struct
 * @param arg The arguments struct to be initialized
 * @return VOID
 */
void args_init(args *arg);

/**
 * @brief Get the arguments from argc and argv into the arguments structure
 * @param argc The amount of arguments giving to the process
 * @param argv A pointer to char*, each char* has a word giving by argument
 * @return The amount of arguments that were correctly read
 */
int args_get(int argc, char **argv, args *arg);

/**
 * @brief Prints the content of the arguments structure
 * @param arg The arguments structure to be printed
 * @return VOID
 */
void args_print(args *arg);

#endif	/* ARGS_H */

