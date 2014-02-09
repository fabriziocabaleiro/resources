#include "log.h"

static void log_write_time(FILE *pf)
{
    char buf[30];
    time_t t = time(NULL);
    strftime(buf, 30, "%F %T", localtime(&t));
    fprintf(pf, "%s: ", buf);
}

void log_write_msg(char *file, char *fmt, ...)
{
    FILE *pf;
    va_list arg;
    va_start(arg, fmt);
    if((pf = fopen(file, "a")) == NULL)
        return;
    log_write_time(pf);
    vfprintf(pf, fmt, arg);
    fprintf(pf, "\n");
    fclose(pf);
    va_end(arg);
}
