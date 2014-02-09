/**
 * \file   conffile.h
 * \author fabrizio
 * \brief  All necessary function to read a configuration file
 * \date   July 19, 2013, 6:19 PM
 */

#ifndef CONFFILE_H
#define	CONFFILE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <error.h>
#include "resinfo.h"

//#define RESOURCE        1   /**< A resource type    */
//#define CONFIGURATION   2   /**< Configuration type */
/** 
 * \enum label_types
 * \brief Type of labels that can be found in the configuration file
 */
enum label_types
{
   RESOURCE,     /**< Configuration label of type resource */
   CONFIGURATION /**< Configuration label of type global configuration */
};

/**
 * @brief Read the configuration file and return a list of resources information
 * @param cfn Configuration File Name
 * @return The head of the resources information list
 */
ri* read_conf_file(char *cfn);

#endif	/* CONFFILE_H */

