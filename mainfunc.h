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
 * \file mainfunc.h
 * \author Fabrizio Cabaleiro
 * \date   19 Jan 2014
 * \brief  Collection of function used in the main
 * In this file are the function that are called from main
 */

#ifndef _MAINFUNC_H_
#define _MAINFUNC_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include "resinfo.h"
#include "conffile.h"
#include "resfunctions.h"
#include "rrdtool.h"
#include "args.h"
#include "log.h"

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
 * \return Highest file descriptor plus 1, in order to be used with pselect.
 */
int mf_exec_cmd(fd_set *readfds, ri *head);

/**
 * \brief Sleep for the rest of the time, until the next period to collect data
 */
void mf_sleep_next_time(time_t st);

/**
 * \brief Print the current version of the program
 */
void mf_print_version();

/**
 * \brief Daemonize the process
 * \return Zero on success, -1 upon error.
 */
int mf_daemonize();

/**
 * \brief Set signal handle for SIGINT, SIGQUIT, SIGSEGV and SIGTERM
 * \return Zero on success.
 */
int mf_set_signal_handle();

/**
 * \brief Signal handle for SIGINT, SIGQUIT, SIGSEGV and SIGTERM.
 * Handle signal and send an email to the person that stated the process
 * letting him know that it was terminated with which signal.
 */
void mf_signal_handle(int sig);

#endif

