#include "args.h"

void init_args(argts *arg)
{
    arg->collect = 0;
    arg->graph   = 0;
    arg->test    = 0;
    arg->rrdopt  = NULL;
}

int get_args(int argc, char **argv, argts *arg)
{
    int i;
    int ret = 0;
    init_args(arg);
    for(i = 1; i < argc; i++)
    {
        if(!strcmp(*(argv + i), "-c"))
        {
            arg->collect = 1;
            ret++;
        }
        else if(!strcmp(*(argv + i), "-g"))
        {
            arg->graph = 1;
            ret++;
        }
        else if(!strcmp(*(argv + i), "-t"))
        {
            arg->test = 1;
            ret++;
        }
        else if(!strcmp(*(argv + i), "-r"))
        {
            arg->rrdopt = *(argv + i + 1);
            ret++;
        }
    }
    return ret;
}

void print_args(argts arg)
{
    char dfmt[] = "%-10s%d\n";
    printf(dfmt, "collect", arg.collect);
    printf(dfmt, "graph", arg.graph);
    printf(dfmt, "test", arg.test);
}
