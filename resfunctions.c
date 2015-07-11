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
 * \file   resfunctions.c
 * \author Fabrizio Cabaleiro
 * \brief  
 * \date 
 *
 */


#include "resfunctions.h"

/* Common execution of command with popen */
static int rf_cmd_common(ri *node, char *cmd);

/* CPU command */
static int rf_cmd_cpu(ri *node);

/* CPU data collector */
static int rf_get_cpu(ri *node, char *data);

/* Net command */
static int rf_cmd_net(ri *node);

/* Net data collector */
static int rf_get_net(ri *node, char *data);

/* Disk command */
static int rf_cmd_disk(ri *node);

/* Disk data collector */
static int rf_get_disk(ri *node, char *data);

/* Uptime command */
static int rf_cmd_uptime(ri *node);

/* Uptime data collector */
static int rf_get_uptime(ri *node, char *data);

/* Memory command */
static int rf_cmd_mem(ri *node);

/* Memory data collector */
static int rf_get_mem(ri *node, char *data);

/* Swap command */
static int rf_cmd_swap(ri *node);

/* Swap data collector */
static int rf_get_swap(ri *node, char *data);

/* All users command */
static int rf_cmd_all_users(ri *node);

/* All users data collector */
static int rf_get_all_users(ri *node, char *data);

/* Current users command */
static int rf_cmd_current_users(ri *node);

/* Current users data collector */
static int rf_get_current_users(ri *node, char *data);

/* PS command */
static int rf_cmd_ps(ri *node);

/* PS data collector */
static int rf_get_ps(ri *node, char *data);

static void rf_log_generic(ri *node, const char *func, int line)
{
    log_write_msg("In function %s, line %d, resources type %s with label %s had"
                  " an error: %m", func, line, node->type, node->label);
}

int ri_done(ri *head)
{
    for(; head != NULL; head = head->next)
        if(head->status == RI_RUNNING)
            return 0;
    return 1;
}

void assign_func(ri *head)
{
    for(; head != NULL; head = head->next)
    {
        if(!strcmp(head->type, "cpu"))
        {
            head->exec_cmd = rf_cmd_cpu;
            head->get_data = rf_get_cpu;
        }
        else if(!strcmp(head->type, "net"))
        {
            head->exec_cmd = rf_cmd_net;
            head->get_data = rf_get_net;
        }
        else if(!strcmp(head->type, "disk"))
        {
            head->exec_cmd = rf_cmd_disk;
            head->get_data = rf_get_disk;
        }
        else if(!strcmp(head->type, "uptime"))
        {
            head->exec_cmd = rf_cmd_uptime;
            head->get_data = rf_get_uptime;
        }
        else if(!strcmp(head->type, "mem"))
        {
            head->exec_cmd = rf_cmd_mem;
            head->get_data = rf_get_mem;
        }
        else if(!strcmp(head->type, "swap"))
        {
            head->exec_cmd = rf_cmd_swap;
            head->get_data = rf_get_swap;
        }
        else if(!strcmp(head->type, "all_users"))
        {
            head->exec_cmd = rf_cmd_all_users;
            head->get_data = rf_get_all_users;
        }
        else if(!strcmp(head->type, "current_users"))
        {
            head->exec_cmd = rf_cmd_current_users;
            head->get_data = rf_get_current_users;
        }
        else if(!strcmp(head->type, "ps"))
        {
            head->exec_cmd = rf_cmd_ps;
            head->get_data = rf_get_ps;
        }
        else
            printf("No function assigned to %s", head->label);
    }
}

static int rf_cmd_common(ri *node, char *cmd)
{
    int ret = 0;
    if((node->pf = popen(cmd, "r")) == NULL)
    {
        log_write_msg("Error rf_cmd_common %s: %s", node->label,
                      strerror(errno));
        ret = -1;
    }
    node->fd = fileno(node->pf);
    return ret;
}

static int rf_get_common_scan(ri *node, int values, char *fmt, ...)
{
    va_list va;  /* variable argument */
    int ret = 0; /* return value */
    int sr;      /* scan return */
    va_start(va, fmt);
    sr = vfscanf(node->pf, fmt, va);
    if(feof(node->pf))
    {
        log_write_msg("In function %s, the EOF was found for type %s with label"
                      " %s", __func__, node->type, node->label);
        ret = -1;
    }
    else if(sr < 0) /* Error */
    {
        rf_log_generic(node, __func__, __LINE__);
        ret = -1;
    }
    else if(sr != values) /* Error */
    {
        log_write_msg("In function %s, I was expecting to read "
                "%d values and I read %d values for type %s with label %s",
                __func__, values, sr, node->type, node->label);
        ret = -1;
    }
    va_end(va);
    return ret;
}

static int rf_get_common_print(char *data, ri *node, char *fmt, ...)
{
    va_list va;
    int ret = 0;
    va_start(va, fmt);
    if(vsnprintf(data, 40, fmt, va) < 0)
    {
        rf_log_generic(node, __func__, __LINE__);
        ret = -1;
    }
    if(pclose(node->pf))
        rf_log_generic(node, __func__, __LINE__);
    va_end(va);
    return ret;
}

static int rf_cmd_cpu(ri *node)
{
    return rf_cmd_common(node, "iostat -c 1 2|grep -v ^$|tail -n1");
}

static int rf_get_cpu(ri *node, char *data)
{
    float user, nice, system, iowait, steal, idle;
    if(rf_get_common_scan(node, 6, "%f %f %f %f %f %f", 
                       &user, &nice, &system, &iowait, &steal, &idle) == 0 &&
       rf_get_common_print(data, node, ":%2.3f:%2.3f:%2.3f:%2.3f:%2.3f:%2.3f",
                           user, nice, system, iowait, steal, idle) == 0)
        return 0;
    return -1;
}

static int rf_cmd_net(ri *node)
{
    int fd[2];
    FILE *pf;
    long long int in, out;
    char fname[45];
    char data[30];
    snprintf(fname, 45, "/sys/class/net/%s/statistics/rx_bytes", node->dev);
    if((pf = fopen(fname, "r")) == NULL)
    {
        printf("rf_cmd_net fopen in error: %s\n", strerror(errno));
        return -1;
    }
    if(fscanf(pf, "%Ld", &in) != 1)
    {
        printf("rf_cmd_net fscanf in error: %s\n", strerror(errno));
        return -1;
    }
    fclose(pf);
    snprintf(fname, 45, "/sys/class/net/%s/statistics/tx_bytes", node->dev);
    if((pf = fopen(fname, "r")) == NULL)
    {
        printf("rf_cmd_net fopen out error: %s\n", strerror(errno));
        return -1;
    }
    if(fscanf(pf, "%Ld", &out) != 1)
    {
        printf("rf_cmd_net fscanf out error: %s\n", strerror(errno));
        return -1;
    }
    fclose(pf);
    if(pipe(fd) != 0)
    {
        printf("rf_cmd_net pipe error: %s\n", strerror(errno));
        return -1;
    }
    snprintf(data, 30, "%Ld %Ld", in, out);
    write(fd[1], data, 30);
    close(fd[1]);
    node->fd = fd[0];
    node->pf = fdopen(node->fd, "r");
    return 0;
}

static int rf_get_net(ri *node, char *data)
{
    int ret = 0;
    long long int in, out;
    if(fscanf(node->pf, "%Ld %Ld", &in, &out) != 2)
    {
        log_write_msg("Error rf_get_cpu %s: %s", node->label, strerror(errno));
        ret = -1;
    }
    else
        snprintf(data, 40, ":%Ld:%Ld", in, out);
    pclose(node->pf);
    return ret;
}

static int rf_cmd_disk(ri *node)
{
    char cmd[40];
    snprintf(cmd, 40, "df -P %s|tail -n1", node->fs);
    return rf_cmd_common(node, cmd);
}

static int rf_get_disk(ri *node, char *data)
{
    int ret = 0;
    long long int total, used;
    if(fscanf(node->pf, "%*s %Ld %Ld", &total, &used) != 2)
    {
        log_write_msg("Error rf_get_cpu %s: %s", node->label, strerror(errno));
        ret = -1;
    }
    else
        snprintf(data, 40, ":%Ld:%Ld", total, used);
    pclose(node->pf);
    return ret;
}

static int rf_cmd_uptime(ri *node)
{
    char cmd[] = "uptime|tr ',' ' '|awk '{for(i=1;i<=NF;i++)if($i ~ /user/)printf \"%d \",$(i-1);printf \"%f %f %f\", $(NF-2), $(NF-1), $NF;}'";
    return rf_cmd_common(node, cmd);
}

static int rf_get_uptime(ri *node, char *data)
{
    int ret = 0;
    float l1, l5, l15;
    int users;
    if(fscanf(node->pf, "%d %f %f %f", &users, &l1, &l5, &l15) != 4)
    {
        log_write_msg("Error rf_get_cpu %s: %s", node->label, strerror(errno));
        ret = -1;
    }
    else
        snprintf(data, 40, ":%d:%.3f:%.3f:%.3f", users, l1, l5, l15);
    pclose(node->pf);
    return ret;
}

static int rf_cmd_mem(ri *node)
{
    char cmd[] = "grep /proc/meminfo -e MemTotal -e MemFree -e Buffers -e Cached|awk '{print $2}'";
    return rf_cmd_common(node, cmd);
}

static int rf_get_mem(ri *node, char *data)
{
    int ret = 0;
    int mtotal, mfree, buffer, cached;
    if(fscanf(node->pf, "%d %d %d %d", &mtotal, &mfree, &buffer, &cached) != 4)
    {
        log_write_msg("Error rf_get_cpu %s: %s", node->label, strerror(errno));
        ret = -1;
    }
    else
        snprintf(data, 40, ":%d:%d:%d:%d", mtotal, mfree, buffer, cached);
    pclose(node->pf);
    return ret;
}

static int rf_cmd_swap(ri *node)
{
    char cmd[] = "grep /proc/meminfo -e SwapTotal -e SwapFree|awk '{print $2}'";
    return rf_cmd_common(node, cmd);
}

static int rf_get_swap(ri *node, char *data)
{
    int ret = 0;
    int mtotal, mfree;
    if(fscanf(node->pf, "%d %d", &mtotal, &mfree) != 2)
    {
        log_write_msg("Error rf_get_cpu %s: %s", node->label, strerror(errno));
        ret = -1;
    }
    else
        snprintf(data, 40, ":%d:%d", mtotal, mfree);
    pclose(node->pf);
    return ret;
}

static int rf_cmd_all_users(ri *node)
{
    char cmd[] = "mysql -uroot -proot -s <<< 'SELECT COUNT(*) FROM placetribe.user_t'";
    return rf_cmd_common(node, cmd);
}

static int rf_get_all_users(ri *node, char *data)
{
    int users;
    int ret = 0;
    if(fscanf(node->pf, "%d", &users) != 1)
    {
        log_write_msg("Error rf_get_all_users: %s", 
                      strerror(errno));
        ret = -1;
    }
    else
        snprintf(data, 40, ":%d", users);
    pclose(node->pf);
    return ret;
}

static int rf_cmd_current_users(ri *node)
{
    char cmd[] = "mysql -uroot -proot -s <<< 'SELECT COUNT(DISTINCT session_id)"
                 " FROM placetribe.user_logged_t'";
    return rf_cmd_common(node, cmd);
}

static int rf_get_current_users(ri *node, char *data)
{
    int users;
    int ret = 0;
    if(fscanf(node->pf, "%d", &users) != 1)
    {
        log_write_msg("Error rf_get_current_users: %s", strerror(errno));
        ret = -1;
    }
    else
        snprintf(data, 40, ":%d", users);
    pclose(node->pf);
    return ret;
}

static int rf_cmd_ps(ri *node)
{
    char cmd[200];
    if(node->user)
        snprintf(cmd, 200, "ps h -o pcpu,pmem -u %s", node->user);
    else
        snprintf(cmd, 200, "ps h -o pcpu,pmem -C %s", node->command);
    return rf_cmd_common(node, cmd);
}

static int rf_get_ps(ri *node, char *data)
{
    float cpu = 0, mem = 0;
    float tcpu = 0, tmem = 0; /* total cpu and mem */
    int frv; /* fscanf return value */
    for(;;)
    {
        frv = fscanf(node->pf, "%f %f", &cpu, &mem);
        if(frv != 2)
        {
            if(feof(node->pf))
                break;
            log_write_msg("In function %s, I was expecting to read "
                    "%d values and I read %d values for type %s with label %s",
                    __func__, 2, frv, node->type, node->label);
            return -1;
        }
        tcpu += cpu;
        tmem += mem;
    }
    return rf_get_common_print(data, node, ":%3.3f:%3.3f", tcpu, tmem);
}
