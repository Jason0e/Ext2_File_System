#ifndef SHELL_H
#define SHELL_H


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>

//LS只展示当前路径
#define LS_ONLY_CURRENT_DIR    0
//LS递归展示文件
#define LS_RECURISON           1

//输入参数最大个数
#define MAXARGS                4

int sh_list();

int getcmd(char *buf, int nbuf) ;//读取指令

void getargs(char *buf, char* argv[],int *argc);

void runcmd(char* argv[],int argc);

#endif