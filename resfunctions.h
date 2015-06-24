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
 * \file   resfunctions.h
 * \author Fabrizio Cabaleiro
 * \brief  Define all the functions used by a resources to get the data that is
 * going to be store and graph
 * \date July 19, 2013, 7:09 PM
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

/**
 * \brief Assign a corresponding function to every single element of the
 * resources list, this function it set the pointer to function attribute in
 * the structure to the corresponding function
 * \param head The head of the resources list
 * \return VOID
 */
void assign_func(ri *head);

/**
 * \brief Check if every resource is done fetching it's data 
 * \param head The head of the resources info list 
 * \return ZERO if any resource in the resources list is not done, ONE if all
 * resources are done fetching it's data
 */
int  ri_done(ri *head);

#endif	/* RESFUNCTIONS_H */

