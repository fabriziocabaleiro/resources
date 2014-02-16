#include "args.h"

void args_init(args *arg)
{
    arg->collect = 0;
    arg->graph   = 0;
    arg->test    = 0;
    arg->version = 0;
    arg->daemon  = 0;
    arg->rrdopt  = NULL;
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
            arg->version = 1;
    }
    return ret;
}

void args_print(args *arg)
{
    char dfmt[] = "%-10s%d\n";
    printf(dfmt, "collect", arg->collect);
    printf(dfmt, "graph", arg->graph);
    printf(dfmt, "test", arg->test);
    printf(dfmt, "version", arg->version);
    printf(dfmt, "daemon", arg->daemon);
}
