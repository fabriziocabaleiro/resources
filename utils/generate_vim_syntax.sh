#!/bin/bash

files=$*
i=0
function get_ctags
{
    ctags -x --c-kinds=$1 $files | awk -v i=$i -v c=$2 '{
        printf("syn keyword kw%d %s\n", i, $1);
    }
    END {
        printf("hi kw%d ctermfg=%s\n", i, c);
    }'
    i=$(($i + 1))
}

get_ctags fp  blue > func.vim
get_ctags est lightgreen > types.vim
