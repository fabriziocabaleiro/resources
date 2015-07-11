/* vim: set shiftwidth=4 : set tabstop=4 : set expandtab : */
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
 * \file   args.c
 * \author Fabrizio Cabaleiro
 * \brief  
 * \date 
 *
 */

#include "args.h"

void args_init(args *arg)
{
    arg->collect  = 0;
    arg->graph    = 0;
    arg->test     = 0;
    arg->version  = 0;
    arg->daemon   = 0;
    arg->res      = NULL;
    arg->rrdopt   = NULL;
    arg->rpath    = NULL;
    arg->gpath    = NULL;
    arg->logfile  = NULL;
    arg->conffile = NULL;
}

/** 
 * @brief  Check if the given configuration file is ok.
 * @param  file, string with the configuration file name
 * @return Zero on success, else any other value.
 * @date   Sat Jul 11 00:43:17 CLT 2015
 * Check if the configuration file exists and it is a regular file
 */
static int check_configuration_file(char *file)
{
    struct stat file_info;
    if(stat(file, &file_info))
    {
        printf("Error: configuration file: %s\n", strerror(errno));
        return -1;
    }
    if((file_info.st_mode & S_IFMT) != S_IFREG)
    {
        printf("Error: Given configuration file is not a regular file\n");
        return -1;
    }
    return 0;
}

/** 
 * @brief  Check if the given path is ok
 * @param  directory, string with the directory name.
 * @return Zero on success, else any other value.
 * @date   Sat Jul 11 00:43:17 CLT 2015
 * Check if the given directory exists and it is a directory.
 */
static int check_valid_path(char *file)
{
    struct stat file_info;
    if(stat(file, &file_info))
    {
        printf("Error: path: %s\n", strerror(errno));
        return -1;
    }
    if((file_info.st_mode & S_IFMT) != S_IFDIR)
    {
        printf("Error: given path is not a directory\n");
        return -1;
    }
    return 0;
}

int args_get(int argc, char **argv, args *arg)
{
    int i;
    int ret = 0;
    args_init(arg);
    for(i = 1; i < argc; i++)
    {
        if(!strcmp(*(argv + i), "-c") || !strcmp(*(argv + i), "--collect"))
        {
            arg->collect = 1;
            ret++;
        }
        else if(!strcmp(*(argv + i), "-g") || !strcmp(*(argv + i), "--graph"))
        {
            arg->graph = 1;
            arg->res = *(argv + i + 1);
            ret++;
        }
        else if(!strcmp(*(argv + i), "-t") || !strcmp(*(argv + i), "--test"))
        {
            arg->test = 1;
            ret++;
        }
        else if(!strcmp(*(argv + i), "-d") || !strcmp(*(argv + i), "--daemon"))
        {
            arg->daemon = 1;
            ret++;
        }
        else if(!strcmp(*(argv + i), "-r") || !strcmp(*(argv + i), "--rrdopt"))
        {
            arg->rrdopt = *(argv + i + 1);
            ret++;
        }
        else if(!strcmp(*(argv + i), "-v") || !strcmp(*(argv + i), "--version"))
        {
            arg->version = 1;
            ret++;
        }
        else if(!strcmp(*(argv + i), "-gp") || !strcmp(*(argv + i), "--gpath"))
        {
            if(!check_valid_path(*(argv + i + 1)))
            {
                arg->gpath = *(argv + i + 1);
                ret++;
            }
        }
        else if(!strcmp(*(argv + i), "-rp") || !strcmp(*(argv + i), "--rpath"))
        {
            if(!check_valid_path(*(argv + i + 1)))
            {
                arg->rpath = *(argv + i + 1);
                ret++;
            }
        }
        else if(!strcmp(*(argv + i), "-f") || !strcmp(*(argv + i), "--conffile"))
        {
            if(!check_configuration_file(*(argv + i + 1)))
            {
                arg->conffile = *(argv + i + 1);
                ret++;
            }
        }
        else if(!strcmp(*(argv + i), "-l") || !strcmp(*(argv + i), "--log"))
        {
            arg->logfile = *(argv + i + 1);
            log_set_file(*(argv + i + 1));
            ret++;
        }
    }
    return ret;
}

void args_print(args *arg)
{
    char dfmt[] = "%-10s%d\n";
    char sfmt[] = "%-10s%s\n";
    printf(dfmt, "collect", arg->collect);
    printf(dfmt, "test", arg->test);
    printf(dfmt, "graph", arg->graph);
    printf(dfmt, "version", arg->version);
    printf(dfmt, "daemon", arg->daemon);
    printf(sfmt, "resources", arg->res);
    printf(sfmt, "rrdopt", arg->rrdopt);
    printf(sfmt, "gpath", arg->gpath);
    printf(sfmt, "rpath", arg->rpath);
    printf(sfmt, "conffile", arg->conffile);
    printf(sfmt, "logfile", arg->logfile);
}
