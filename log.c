#include "log.h"

void logtime(FILE *pf)
{
    char buf[30];
    time_t t = time(NULL);
    strftime(buf, 30, "%F %T", localtime(&t));
    fprintf(pf, "%s: ", buf);
}

void logmsg(char *file, char *fmt, ...)
{
    FILE *pf;
    va_list arg;
    va_start(arg, fmt);
    if((pf = fopen(file, "a")) == NULL)
        return;
    logtime(pf);
    vfprintf(pf, fmt, arg);
    fprintf(pf, "\n");
    fclose(pf);
    va_end(arg);
}
