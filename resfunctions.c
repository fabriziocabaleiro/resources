#include "resfunctions.h"

void assign_func(ri *head)
{
    for(; head != NULL; head = head->next)
    {
        if(!strcmp(head->type, "cpu"))
        {
            head->exec_cmd = cmd_cpu;
            head->get_data = get_cpu;
        }
        else if(!strcmp(head->type, "net"))
        {
            head->exec_cmd = cmd_net;
            head->get_data = get_net;
        }
        else if(!strcmp(head->type, "disk"))
        {
            head->exec_cmd = cmd_disk;
            head->get_data = get_disk;
        }
        else if(!strcmp(head->type, "uptime"))
        {
            head->exec_cmd = cmd_uptime;
            head->get_data = get_uptime;
        }
        else if(!strcmp(head->type, "mem"))
        {
            head->exec_cmd = cmd_mem;
            head->get_data = get_mem;
        }
        else if(!strcmp(head->type, "swap"))
        {
            head->exec_cmd = cmd_swap;
            head->get_data = get_swap;
        }
        else if(!strcmp(head->type, "all_users"))
        {
            head->exec_cmd = cmd_all_users;
            head->get_data = get_all_users;
        }
        else if(!strcmp(head->type, "current_users"))
        {
            head->exec_cmd = cmd_current_users;
            head->get_data = get_current_users;
        }
        else
            printf("No function assigned to %s", head->label);
    }
}

int ri_done(ri *head)
{
    for(; head != NULL; head = head->next)
        if(!head->done)
            return 0;
    return 1;
}

int cmd_common(ri *node, char *cmd)
{
    int ret = 0;
    if((node->pf = popen(cmd, "r")) == NULL)
    {
        log_write_msg(node->gc->log, "Error cmd_common %s: %s", node->label, strerror(errno));
        ret = -1;
    }
    node->fd = fileno(node->pf);
    return ret;
}

int cmd_cpu(ri *node)
{
    return cmd_common(node, "iostat -c 1 2|grep -v ^$|tail -n1");
}

int get_cpu(ri *node, char *data)
{
    int ret = 0;
    float user, nice, system, iowait, steal, idle;
    if(fscanf(node->pf, "%f %f %f %f %f %f", &user, &nice, &system, &iowait, &steal, &idle) != 6)
    {
        log_write_msg(node->gc->log, "Error get_cpu %s: %s", node->label, strerror(errno));
        ret = -1;
    }
    else
        snprintf(data, 40, ":%2.3f:%2.3f:%2.3f:%2.3f:%2.3f:%2.3f", user, nice, system, iowait, steal, idle);
    pclose(node->pf);
    return ret;
}

int cmd_net(ri *node)
{
    int fd[2];
    FILE *pf;
    long long int in, out;
    char fname[45];
    char data[30];
    snprintf(fname, 45, "/sys/class/net/%s/statistics/rx_bytes", node->dev);
    if((pf = fopen(fname, "r")) == NULL)
    {
        printf("cmd_net fopen in error: %s\n", strerror(errno));
        return -1;
    }
    if(fscanf(pf, "%Ld", &in) != 1)
    {
        printf("cmd_net fscanf in error: %s\n", strerror(errno));
        return -1;
    }
    fclose(pf);
    snprintf(fname, 45, "/sys/class/net/%s/statistics/tx_bytes", node->dev);
    if((pf = fopen(fname, "r")) == NULL)
    {
        printf("cmd_net fopen out error: %s\n", strerror(errno));
        return -1;
    }
    if(fscanf(pf, "%Ld", &out) != 1)
    {
        printf("cmd_net fscanf out error: %s\n", strerror(errno));
        return -1;
    }
    fclose(pf);
    if(pipe(fd) != 0)
    {
        printf("cmd_net pipe error: %s\n", strerror(errno));
        return -1;
    }
    snprintf(data, 30, "%Ld %Ld", in, out);
    write(fd[1], data, 30);
    close(fd[1]);
    node->fd = fd[0];
    node->pf = fdopen(node->fd, "r");
    return 0;
}

int get_net(ri *node, char *data)
{
    int ret = 0;
    long long int in, out;
    if(fscanf(node->pf, "%Ld %Ld", &in, &out) != 2)
    {
        log_write_msg(node->gc->log, "Error get_cpu %s: %s", node->label, strerror(errno));
        ret = -1;
    }
    else
        snprintf(data, 40, ":%Ld:%Ld", in, out);
    pclose(node->pf);
    return ret;
}

int cmd_disk(ri *node)
{
    char cmd[40];
    snprintf(cmd, 40, "df -P %s|tail -n1", node->fs);
    return cmd_common(node, cmd);
}

int get_disk(ri *node, char *data)
{
    int ret = 0;
    long long int total, used;
    if(fscanf(node->pf, "%*s %Ld %Ld", &total, &used) != 2)
    {
        log_write_msg(node->gc->log, "Error get_cpu %s: %s", node->label, strerror(errno));
        ret = -1;
    }
    else
        snprintf(data, 40, ":%Ld:%Ld", total, used);
    pclose(node->pf);
    return ret;
}

int cmd_uptime(ri *node)
{
    char cmd[] = "uptime|tr ',' ' '|awk '{for(i=1;i<=NF;i++)if($i ~ /user/)printf \"%d \",$(i-1);print $(NF-2)" "$(NF-1)" "$NF;}'";
    return cmd_common(node, cmd);
}

int get_uptime(ri *node, char *data)
{
    int ret = 0;
    float l1, l5, l15;
    int users;
    if(fscanf(node->pf, "%d %f %f %f", &users, &l1, &l5, &l15) != 4)
    {
        log_write_msg(node->gc->log, "Error get_cpu %s: %s", node->label, strerror(errno));
        ret = -1;
    }
    else
        snprintf(data, 40, ":%d:%.3f:%.3f:%.3f", users, l1, l5, l15);
    pclose(node->pf);
    return ret;
}

int cmd_mem(ri *node)
{
    char cmd[] = "grep /proc/meminfo -e MemTotal -e MemFree -e Buffers -e Cached|awk '{print $2}'";
    return cmd_common(node, cmd);
}

int get_mem(ri *node, char *data)
{
    int ret = 0;
    int mtotal, mfree, buffer, cached;
    if(fscanf(node->pf, "%d %d %d %d", &mtotal, &mfree, &buffer, &cached) != 4)
    {
        log_write_msg(node->gc->log, "Error get_cpu %s: %s", node->label, strerror(errno));
        ret = -1;
    }
    else
        snprintf(data, 40, ":%d:%d:%d:%d", mtotal, mfree, buffer, cached);
    pclose(node->pf);
    return ret;
}

int cmd_swap(ri *node)
{
    char cmd[] = "grep /proc/meminfo -e SwapTotal -e SwapFree|awk '{print $2}'";
    return cmd_common(node, cmd);
}

int get_swap(ri *node, char *data)
{
    int ret = 0;
    int mtotal, mfree;
    if(fscanf(node->pf, "%d %d", &mtotal, &mfree) != 2)
    {
        log_write_msg(node->gc->log, "Error get_cpu %s: %s", node->label, strerror(errno));
        ret = -1;
    }
    else
        snprintf(data, 40, ":%d:%d", mtotal, mfree);
    pclose(node->pf);
    return ret;
}

int cmd_all_users(ri *node)
{
    char cmd[] = "mysql -uroot -proot -s <<< 'SELECT COUNT(*) FROM placetribe.user_t'";
    return cmd_common(node, cmd);
}

int get_all_users(ri *node, char *data)
{
    int users;
    int ret = 0;
    if(fscanf(node->pf, "%d", &users) != 1)
    {
        log_write_msg(node->gc->log, "Error get_all_users: %s", strerror(errno));
        ret = -1;
    }
    else
        snprintf(data, 40, ":%d", users);
    pclose(node->pf);
    return ret;
}

int cmd_current_users(ri *node)
{
    char cmd[] = "mysql -uroot -proot -s <<< 'SELECT COUNT(DISTINCT session_id) FROM placetribe.user_logged_t'";
    return cmd_common(node, cmd);
}

int get_current_users(ri *node, char *data)
{
    int users;
    int ret = 0;
    if(fscanf(node->pf, "%d", &users) != 1)
    {
        log_write_msg(node->gc->log, "Error get_current_users: %s", strerror(errno));
        ret = -1;
    }
    else
        snprintf(data, 40, ":%d", users);
    pclose(node->pf);
    return ret;
}
