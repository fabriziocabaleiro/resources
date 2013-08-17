#include "conffile.h"

int get_type(char *line)
{
    char type[30];
    sscanf(line, "[%30[^]]]", type);
    if(!strcmp(type, "conf"))
        return CONFIGURATION;
    return RESOURCE;
}

ri* read_conf_file(char *cfn)
{
    FILE *fin;
    ri *head;
    ri *buf;
    gconf *gc;
    int tp;
    char line[200];
    char type[30];
    
    head = NULL;
    buf  = NULL;
    gc   = new_gconf();
    
    if(cfn == NULL)
    {
        printf("Configuration file name is a NULL pointer\n");
        return NULL;
    }
    if((fin = fopen(cfn, "r")) == NULL)
    {
        printf("Error opening configuration file '%s'\n", cfn);
        return NULL;
    }
    for(;;)
    {
        fscanf(fin, "%200[^\n]\n", line);
        if(feof(fin))
            break;
        if(*line == '\n' || *line == '#')
            continue;
        if(*line == '[')
        {
            tp = get_type(line);
            if(tp == RESOURCE)
            {
                if(buf != NULL)
                    add_ri(&head, buf);
                buf = new_ri();
                sscanf(line, "[%30[^]]]", type);
                buf->type = (char*)malloc((1 + strlen(type)) * sizeof(char));
                strcpy(buf->type, type);
            }
        }
        else if(tp == RESOURCE)
            fill_ri(buf, line);
        else if(tp == CONFIGURATION)
            fill_gconf(gc, line);
    }
    add_ri(&head, buf);
    for(buf = head; buf != NULL; buf = buf->next)
        buf->gc = gc;
    return head;
}
