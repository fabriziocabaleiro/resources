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
 * \file   rrdtool.h
 * \author Fabrizio Cabaleiro
 * \brief  Functions to wrap RRDTool functions
 * \data July 21, 2013, 11:27 AM
 */

#ifndef RRDTOOL_H
#define	RRDTOOL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include "resinfo.h"
#include "args.h"
#include "log.h"

/**
 * \brief Check if the rrdtool file exists
 * \param node A resources information structure pointer
 * \return Integer, 1 if it exists and zero if it doesn't
 */
int rrd_exits(ri *node);

/**
 * \brief Create a rrd file for the giving resources information
 * \param node A resources information structure pointer
 * \return void
 */
void rrd_create(ri *node);

/**
 * \brief Update the content of the rrd database with new data
 * \param node A resources information structure pointer that owns the data that
 * is passed in the argument
 * \param data A string with new data to store
 * \return void
 */
void rrd_update(ri *node, char *data);

/**
 * \brief Generate a graph of the giving resources information structure pointer
 * \param node A resource information structure pointer
 * \param arg A pointer to the main arguments
 * \return void
 */
void rrd_graph(ri *node, const args *arg);

#endif	/* RRDTOOL_H */

