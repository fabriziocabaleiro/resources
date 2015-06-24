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
 * \file   main.c
 * \author Fabrizio Cabaleiro
 * \brief  
 * \date July 19, 2013, 11:21 AM
 */

#include <stdlib.h>
#include "resinfo.h"
#include "conffile.h"
#include "args.h"
#include "mainfunc.h"

int main(int argc, char** argv) 
{
    ri *head;
    args arg;
    
    args_get(argc, argv, &arg);
    head = read_conf_file("config.conf");
    assign_func(head);
    
    if(arg.daemon && mf_daemonize())
        return EXIT_FAILURE;
    if(arg.test)
    {
        args_print(&arg);
        ri_print(head);
        if(head)
            gconf_print(head->gc);
    }
    if(arg.version)
        mf_print_version();
    if(arg.graph)
        mf_graph(head, &arg);
    if(arg.collect)
        mf_collector(head);

    if(head)
        gconf_free(head->gc);
    ri_free(head);
    printf("Thanks for using resources\n");
    return (EXIT_SUCCESS);
}

