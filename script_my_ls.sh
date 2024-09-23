#! /bin/sh
cd /home/edwin/epitech/my_ls
make re >> make_compilation
make re 2>> make_compilation
valgrind ./my_ls -alRdrt Makefile  README.md  make_compilation   result_compilation  test.c > result_compilation
