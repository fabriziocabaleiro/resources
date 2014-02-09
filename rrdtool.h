/**
 * \file   rrdtool.h
 * \author fabrizio
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

