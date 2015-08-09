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
 * \file   mainfunc.c
 * \author Fabrizio Cabaleiro
 * \brief  
 * \date 
 *
 */

#include "mainfunc.h"

/* In collect mode, run until signal changes the value of stop_running */
static int stop_running = 0;


int mf_collector(ri *head)
{
    ri *it;
    fd_set readfds;
    fd_set rfdscpy;
    struct timespec timeout;
    sigset_t sset;
    char data[40];
    int nfds;                 /* highest file descriptor plus 1 */
    int psr;                  /* PSelect Return */
    time_t st;                /* Starting time  */
    time_t ct;                /* Current time   */

    timeout.tv_nsec = 0;
    sigemptyset(&sset);

    while(stop_running == 0)
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
                        log_write_msg("Closing process type %s with label %s "
                                      "for timeout", it->type, it->label);
                        if(it->pf)
                        {
                            pclose(it->pf);
                            it->pf = NULL;
                        }
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
                    if(it->fd == RI_UNUSED_FD)
                        continue;
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
    int hfd = 0; /* highest file descriptor */
    ri *it; /* TODO: not needed ? rm */
    FD_ZERO(readfds);
    for(it = head; it != NULL; it = it->next)
    {
        if(it->exec_cmd == NULL)
            continue;
        /* Add to the select read file descriptor only the descriptors that
         * were correctly opened (returned different from -1 */
        if(it->exec_cmd(it) == 0 && it->fd != RI_UNUSED_FD) 
        {
            FD_SET(it->fd, readfds);
            hfd = hfd < it->fd ? it->fd : hfd;
            it->status = RI_RUNNING;
        }
    }
    return hfd + 1;
}

#ifndef VERSION
#define VERSION "UNKNOWN"
#endif
void mf_print_version()
{
    printf("Version %s\n"
           "Compiled date: %s\n",
           VERSION, __DATE__);
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
        exit(EXIT_SUCCESS);
    }
    return pid;
}

int mf_set_signal_handle()
{
    if(signal(SIGINT,  mf_signal_handle) == SIG_ERR ||
       signal(SIGQUIT, mf_signal_handle) == SIG_ERR ||
       signal(SIGSEGV, mf_signal_handle) == SIG_ERR ||
       signal(SIGTERM, mf_signal_handle) == SIG_ERR ||
       signal(SIGHUP,  SIG_IGN)          == SIG_ERR ) {
        printf("Error setting signal handle\n");
        return -1;
    }
    return 0;
}

void mf_signal_handle(int sig)
{
    char *mail_cmd;
    log_write_msg("Signal %d received, starting to close everything", sig);
    /* using alloca to avoid reserving static memory for this case */
    mail_cmd = (char*)alloca(200 * sizeof(int));
    snprintf(mail_cmd, 200, "mailx %s <<< \"resources was killed by signal %d\""
             , getenv("USER"), sig);
    system(mail_cmd);
    stop_running = 1;
}

