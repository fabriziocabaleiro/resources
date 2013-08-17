/* 
 * File:   resweb.c
 * Author: fabrizio
 *
 * Created on July 25, 2013, 9:50 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <time.h>
#include "resinfo.h"
#include "log.h"
#include "conffile.h"

#define _GNU_SOURCE
#define __USE_XOPEN
#define POST_SIZE 50

typedef struct postdata
{
    int  start;
    char str_start[POST_SIZE];
    int  end;
    char str_end[POST_SIZE];
    int  lower;
    char str_lower[POST_SIZE];
    int  upper;
    char str_upper[POST_SIZE];
    char str_res[POST_SIZE];
}pd;

void css()
{
    printf("\t\t<style type='text/css'>\n");
    printf(
            "\t\t\thtml body { width: 100%%; margin: auto;}\n"
            "\t\t\t.imgtable { width: 100%%; text-align: center}\n"
            "\t\t\t.imgtable td {width: 50%%;}\n"
            "\t\t\t.imgtable img { margin: auto; }\n"
            "\t\t\t.menutable { margin: auto; }\n");
    printf("\t\t</style>\n");
}

void head()
{
    printf("\t<head>\n");
    css();
    printf("\t\t<title>PlaceTribe's resources</title>\n");
    printf("\t</head>\n");
}

void images(ri *res)
{
    int tr = 0;
    printf("\t<table class='imgtable'>\n");
    for(; res != NULL; res = res->next)
    {
        if(tr % 2 == 0)
            printf("\t\t<tr>\n");
        printf("\t\t\t<td><img src='/png/%s.png' /></td>\n", res->gname);
        if((tr + 1) % 2 == 0)
            printf("\t\t</tr>\n");
        tr++;
    }
    if(tr % 2 != 0)
        printf("\t\t</tr>\n");
    printf("\t</table>\n");
}

void menu(ri *res, pd *post)
{
    ri *it;
    printf("\t\t<form method='POST' action=''>\n"
           "\t\t\t<table class='menutable'>\n"
           "\t\t\t\t<tr>\n");
    printf("\t\t\t\t\t<td>Start [dd/mm/yy [hh:mm]]<input type='text' "
            "name='start' value='%s' maxlength='16' size='16' /></td>\n", 
            *post->str_start == '\0'? "": post->str_start);
    printf("\t\t\t\t\t<td>End [dd/mm/yy [hh:mm]]<input type='text' name='end'"
            " value='%s' maxlength='16' size='16' /></td>\n",
            *post->str_end == '\0'? "": post->str_end);
    printf("\t\t\t\t</tr>\n\t\t\t\t<tr>\n");
    printf("\t\t\t\t\t<td>Resource<select name='res'>\n"
            "\t\t\t\t\t\t<option value='all'>All</option>\n");
    for(it = res; it != NULL; it = it->next)
        printf("\t\t\t\t\t\t<option value='%s' %s>%s</option>\n",
                it->gname, !strcmp(it->gname, post->str_res)? "SELECTED": "", it->label);
    printf("\t\t\t\t\t</select></td>\n");
    printf("\t\t\t\t\t<td>Lower<input type='text' name='lower'"
            " value='%s' maxlength='16' size='10' /></td>\n",
            *post->str_lower == '\0'? "": post->str_lower);
    printf("\t\t\t\t\t<td>Upper<input type='text' name='upper'"
            " value='%s' maxlength='16' size='10' /></td>\n",
            *post->str_upper == '\0'? "": post->str_upper);
    printf("\t\t\t\t\t<td><input type='submit' value='Go' /></td>\n");
    printf("\t\t\t\t</tr>\n"
           "\t\t\t</table>\n"
           "\t\t</form>\n");
}

void image(ri *res, pd *post)
{
    for(; res != NULL; res = res->next)
        if(!strcmp(res->gname, post->str_res))
            break;
    if(res == NULL)
        return;
    printf("\t\t<div style='width: 100%%; text-align:center;'>\n");
    printf("\t\t\t<img src='/png/%s.png' style='margin: auto' />\n", res->gname);
    printf("\t\t</div>\n");
}

void body(ri *res, pd *post)
{
    printf("\t<body>\n");
    menu(res, post);
    if(!strcmp(post->str_res, "all"))
        images(res);
    else
        image(res, post);
    printf("\t</body>\n");
}

void html(ri *res, pd *post)
{
    printf("<html>\n");
    head();
    body(res, post);
    printf("</html>\n");
}

int get_epoch(char *s)
{
    struct tm t;
    t.tm_hour = t.tm_min = 0;
    t.tm_sec = 0;
    t.tm_isdst = -1;
    if(sscanf(s, "%d/%d/%d %d:%d", &t.tm_mday, &t.tm_mon, &t.tm_year, &t.tm_hour, &t.tm_min) < 3)
        return -1;
    t.tm_mon -= 1;
    t.tm_year -= 1900;
    return (int)(mktime(&t) - time(NULL));
}

char get_hexa(char *s)
{
    unsigned int x;
    sscanf(s, "%2x", &x);
    return (char)x;
}

void decode(char *s)
{
    char buf[POST_SIZE];
    int i, j;
    for(i = 0, j = 0; *(s + i) != '\0'; i++)
    {
        if(*(s + i) == '+')
            *(buf + j++) = ' ';
        else if(*(s + i) != '%')
            *(buf + j++) = *(s + i);
        else
        {
            *(buf + j++) = get_hexa(s + i + 1);
            i += 2;
        }
    }
    *(buf + j) = '\0';
    strncpy(s, buf, POST_SIZE);
}

void get_post(pd *post)
{
    char var[POST_SIZE], val[POST_SIZE];
    char buf[2 * POST_SIZE + 1];
    for(;;)
    {
        *var = *val = '\0';
        if(scanf("%101[^&\n]&", buf) != 1)
            break;
        if(sscanf(buf, "%50[^=\n]=%50[^&\n]&", var, val) != 2)
            continue;
        decode(val);
        if(!strcmp(var, "start") && get_epoch(val) != -1)
        {
            post->start = get_epoch(val);
            strcpy(post->str_start, val);
        }
        else if(!strcmp(var, "end") && get_epoch(val) != -1)
        {
            post->end = get_epoch(val);
            strcpy(post->str_end, val);
        }
        else if(!strcmp(var, "lower"))
        {
            sscanf(val, "%d", &post->lower);
            sprintf(post->str_lower, "%d", post->lower);
        }
        else if(!strcmp(var, "upper"))
        {
            sscanf(val, "%d", &post->upper);
            sprintf(post->str_upper, "%d", post->upper);
        }
        else if(!strcmp(var, "res"))
            strcpy(post->str_res, val);
    }
}

int main(int argc, char** argv) 
{
    ri *res;
    pd post = {.start = -3600, .end = -30};
    *post.str_res   = '\0';
    *post.str_start = '\0';
    *post.str_end   = '\0';
    *post.str_lower = '\0';
    *post.str_upper = '\0';
    char *cl;
    char cmd[300];
    char lim[100];
    
    *lim = '\0';
    res = read_conf_file("config.conf");
    printf("Content-type: text/html\n\n");  
    
    cl = getenv("CONTENT_LENGTH");
    if(cl != NULL && *cl != '0')
        get_post(&post);
    if(*post.str_lower != '\0' || *post.str_upper != '\0')
    {
        strcpy(lim, "-r");
        if(*post.str_lower != '\0')
        {
            strcat(lim, " -l ");
            strcat(lim, post.str_lower);
        }
        if(*post.str_upper != '\0')
        {
            strcat(lim, " -u ");
            strcat(lim, post.str_upper);
        }
    }
    snprintf(cmd, 200, "./resources -g %s -r '-s %d -e %d %s' > /dev/null", 
            post.str_res, post.start, post.end, lim);
    system(cmd);
    html(res, &post);
    return (EXIT_SUCCESS);
}

