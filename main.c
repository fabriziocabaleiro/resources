/* 
 * File:   main.c
 * Author: fabrizio
 *
 * Created on July 19, 2013, 11:21 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>
#include <signal.h>
#include "resinfo.h"
#include "conffile.h"
#include "resfunctions.h"
#include "rrdtool.h"
#include "args.h"

int collector(ri *head);
int graph(ri *head, argts *arg);
int exec_cmd(fd_set *readfds, ri *head);
void sleep_next_time(int st);

int main(int argc, char** argv) 
{
    ri *head;
    argts arg;
    
    get_args(argc, argv, &arg);
    head = read_conf_file("config.conf");
    assign_func(head);
    
    if(arg.test)
    {
        print_args(arg);
        print_ri(head);
        print_gconf(head->gc);
    }
    if(arg.collect)
        collector(head);
    if(arg.graph)
        graph(head, &arg);
    free_ri(head);
    return (EXIT_SUCCESS);
}

int collector(ri *head)
{
    ri *it;
    fd_set readfds;
    fd_set rfdscpy;
    struct timespec timeout;
    sigset_t sset;
    char data[40];
    int nfds;
    int psr;
    int st;
    
    timeout.tv_nsec = 0;
    timeout.tv_sec  = 2;
    sigemptyset(&sset);
    
    for(;;)
    {
        st = (int)time(NULL);
        nfds = exec_cmd(&readfds, head);
        
        while(!ri_done(head))
        {
            memcpy(&rfdscpy, &readfds, sizeof(fd_set));
            psr = pselect(nfds, &rfdscpy, NULL, NULL, &timeout, &sset);
            if(psr == -1)
            {
                printf("pselect error: %s\n", strerror(errno));
                return -1;
            }
            else if(psr == 0)
            {
                printf("No bits sets\n");
                continue;
            }
            else
            {
                for(it = head; it != NULL; it = it->next)
                {
                    if(FD_ISSET(it->fd, &rfdscpy))
                    {
                        if(it->get_data(it, data) == 0)
                            rrd_update(it, data);
                        FD_CLR(it->fd, &readfds);
                        it->done = 1;
                    }
                }
            }
        }
        sleep_next_time(st);
    }
    return 0;
}

int graph(ri *head, argts *arg)
{
    if(arg->res == NULL || !strcmp(arg->res, "all"))
    {
        for(;head != NULL; head = head->next)
            rrd_graph(head, arg);
    }
    else
    {
        for(;head != NULL; head = head->next)
        {
            if(!strcmp(head->gname, arg->res))
            {
                rrd_graph(head, arg);
                break;
            }
        }
    }
    return 0;
}

void sleep_next_time(int st)
{
    int nt;
    int ct;
    int sl;
    nt = st + 30;
    ct = (int)time(NULL);
    if(nt < ct)
        return;
    sl = nt - ct;
    if(sl < 0 || sl > 30)
        sl = 0;
    sleep(sl);
}

int exec_cmd(fd_set *readfds, ri *head)
{
    int nfds = 0;
    ri *it;
    FD_ZERO(readfds);
    for(it = head; it != NULL; it = it->next)
    {
        if(it->exec_cmd == NULL)
            continue;
        /* Add to the select read file descriptor only the descriptors that
         * were set to something different than 1 */
        if(it->exec_cmd(it) == 0) 
        {
            FD_SET(it->fd, readfds);
            nfds = nfds < it->fd ? it->fd : nfds;
            it->done = 0;
        }
    }
    return nfds + 1;
}
