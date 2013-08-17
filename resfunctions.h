/* 
 * File:   resfunctions.h
 * Author: fabrizio
 *
 * Created on July 19, 2013, 7:09 PM
 */

#ifndef RESFUNCTIONS_H
#define	RESFUNCTIONS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "resinfo.h"
#include "rrdtool.h"
#include "log.h"

void assign_func(ri *head);

int  ri_done(ri *head);

int cmd_cpu(ri *node);
int get_cpu(ri *node, char *data);

int cmd_net(ri *node);
int get_net(ri *node, char *data);

int cmd_disk(ri *node);
int get_disk(ri *node, char *data);

int cmd_uptime(ri *node);
int get_uptime(ri *node, char *data);

int cmd_mem(ri *node);
int get_mem(ri *node, char *data);

int cmd_swap(ri *node);
int get_swap(ri *node, char *data);

int cmd_all_users(ri *node);
int get_all_users(ri *node, char *data);

int cmd_current_users(ri *node);
int get_current_users(ri *node, char *data);

#endif	/* RESFUNCTIONS_H */

