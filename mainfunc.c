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
    int psr;                  /* PSelect Return */
    time_t st;                /* Starting time  */
    time_t ct;                /* Current time   */
    
    timeout.tv_nsec = 0;
    sigemptyset(&sset);
    
    for(;;)
    {
        st = time(NULL);
        nfds = mf_exec_cmd(&readfds, head);
        
        while(!ri_done(head))
        {
            ct = time(NULL);
            /* There are process that are taking more than 30 seconds to
             * terminate, close those process and start the loop again */
            if(ct >= (st + 30))
            {
                for(it = head; it != NULL; it = it->next)
                {
                    if(it->status == RI_RUNNING)
                    {
                        log_write_msg(head->gc->log, "Closing process type %s "
                                      "with label %s for timeout",
                                      head->type, head->label);
                        /* TODO: close file pointer ? */
                        it->status = RI_DONE;
                    }
                }
                break;
            }
            memcpy(&rfdscpy, &readfds, sizeof(fd_set));
            timeout.tv_sec = 30 - (ct - st);
            if(timeout.tv_sec < 0 || timeout.tv_sec > 30)
                timeout.tv_sec = 0;
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
                        FD_CLR(it->fd, &readfds);
                        if(it->get_data(it, data) == 0)
                        {
                            rrd_update(it, data);
                            it->status = RI_DONE;
                        }
                        else
                            it->status = RI_DONE_WITH_ERROR;
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

void mf_sleep_next_time(time_t st)
{
    time_t nt;
    time_t ct;
    time_t sl;
    nt = st + 30;
    ct = time(NULL);
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
         * were correctly opened (returned different from -1 */
        if(it->exec_cmd(it) == 0) 
        {
            FD_SET(it->fd, readfds);
            nfds = nfds < it->fd ? it->fd : nfds;
            it->status = RI_RUNNING;
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

int mf_daemonize()
{
    pid_t pid;
    pid = fork();
    if(pid < 0)
    {
        printf("Error couldn't fork\n");
    }
    else if(pid == 0) /* child */
    {
        if(fclose(stdin))
            printf("Closing stdin: %s\n", strerror(errno));
        if(fclose(stdout))
            printf("Closing stdout: %s\n", strerror(errno));
        if(fclose(stderr))
            printf("Closing stderr: %s\n", strerror(errno));
        if(chdir("/"))
            printf("Changing dir: %s\n", strerror(errno));
    }
    else /* parent */
    {
        printf("Process has fork with pid %d\n", pid);
        _exit(EXIT_SUCCESS);
    }
    return pid;
}

