#include "mainfunc.h"

int mf_collector(ri *head)
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
        nfds = mf_exec_cmd(&readfds, head);
        
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
        mf_sleep_next_time(st);
    }
    return 0;
}

int mf_graph(ri *head, args *arg)
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

void mf_sleep_next_time(int st)
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

int mf_exec_cmd(fd_set *readfds, ri *head)
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

void mf_print_version()
{
    printf("Version 1.0\n"
           "Compiled date: %s\n",
           __DATE__);
}
