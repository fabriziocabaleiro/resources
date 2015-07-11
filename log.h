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
 * \file   log.h
 * \author Fabrizio Cabaleiro
 * \brief  Functionality to log information to a file
 * \date July 25, 2013, 11:15 AM
 */

#ifndef LOG_H
#define	LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include <stdarg.h>

/**
 * @brief Write a message into the log file.
 * @param file File name.
 * @param fmt  Format with text and variables.
 * @param ...  Variable amount of arguments.
 */
void log_write_msg(char *fmt, ...);

/**
 * @brief Set a log file to write
 * @param file File name.
 */
int log_set_file(char *file);

/**
 * @brief Get a log file name
 * @param void
 * @return char*
 */
char* log_get_file(void);

#endif	/* LOG_H */

