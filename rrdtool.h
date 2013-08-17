/* 
 * File:   rrdtool.h
 * Author: fabrizio
 *
 * Created on July 21, 2013, 11:27 AM
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

int rrd_exits(ri *node);

void rrd_create(ri *node);

void rrd_update(ri *node, char *data);

void rrd_graph(ri *node, const argts *arg);

#endif	/* RRDTOOL_H */

