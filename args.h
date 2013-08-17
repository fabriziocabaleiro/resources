/* 
 * File:   args.h
 * Author: fabrizio
 *
 * Created on July 21, 2013, 10:13 PM
 */

#ifndef ARGS_H
#define	ARGS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

typedef struct arguments /** @struct arguments  */
{
    int collect;         /**< Collect data      */
    int graph;           /**< Graph data        */
    char *res;           /**< Resource to graph */
    int test;            /**< Run test mode     */
    char *rrdopt;        /**< Additional options for the rrdtool command */
}argts;

void init_args(argts *arg);
int get_args(int argc, char **argv, argts *arg);
void print_args(argts arg);

#endif	/* ARGS_H */

