#ifndef _MAINFUNC_H_
#define _MAINFUNC_H_

/**
 * \file mainfunc.h
 * \author Fabrizio Cabaleiro
 * \date   19 Jan 2014
 * \brief  Collection of function used in the main
 * In this file are the function that are called from main
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>
#include <signal.h>
#include "resinfo.h"
#include "conffile.h"
#include "resfunctions.h"
#include "rrdtool.h"
#include "args.h"

/**
 * \brief Collect resources data and write to rrd database
 * \param head The head of the resources information list
 * \return Zero on success, other value if error
 */
int mf_collector(ri *head);

/**
 * \brief Graph the data from the rrd database
 */
int mf_graph(ri *head, args *arg);

/**
 * \brief Execute the commands to collect data from each resource
 */
int mf_exec_cmd(fd_set *readfds, ri *head);

/**
 * \brief Sleep for the rest of the time, until the next period to collect data
 */
void mf_sleep_next_time(int st);

/**
 * \brief Print the current version of the program
 */
void mf_print_version();

#endif

