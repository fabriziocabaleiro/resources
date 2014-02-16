/** 
 * \file   main.c
 * \author fabrizio
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
        gconf_print(head->gc);
    }
    if(arg.version)
        mf_print_version();
    if(arg.graph)
        mf_graph(head, &arg);
    if(arg.collect)
        mf_collector(head);
    ri_free(head);
    printf("Thanks for using resources\n");
    return (EXIT_SUCCESS);
}

