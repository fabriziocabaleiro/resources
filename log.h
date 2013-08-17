/* 
 * File:   log.h
 * Author: fabrizio
 *
 * Created on July 25, 2013, 11:15 AM
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
void logmsg(char *file, char *fmt, ...);

#endif	/* LOG_H */

