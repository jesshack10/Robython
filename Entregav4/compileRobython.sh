#! /bin/bash

bison -d robython.y
flex robython.l
g++ robython.tab.c lex.yy.c -lfl -o robython
./robython
