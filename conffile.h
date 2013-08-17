/* 
 * File:   conffile.h
 * Author: fabrizio
 *
 * Created on July 19, 2013, 6:19 PM
 */

#ifndef CONFFILE_H
#define	CONFFILE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <error.h>
#include "resinfo.h"

#define RESOURCE        1   /**< A resource type    */
#define CONFIGURATION   2   /**< Configuration type */

/**
 * @brief Read the configuration file and return a list of resources information
 * @param cfn Configuration File Name
 * @return The head of the resources information list
 */
ri* read_conf_file(char *cfn);

#endif	/* CONFFILE_H */

