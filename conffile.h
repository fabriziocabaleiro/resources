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
 * \file   conffile.h
 * \author Fabrizio Cabaleiro
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
#include <errno.h>
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

