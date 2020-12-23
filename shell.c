#include "shell.h"
#include "ext2.h"
#include "ext2.c"
#include "common.h"
#include <string.h>
#include <stdio.h>


int sh_list(){
    printf(GREEN"----------------------------------------------\n");
    printf(CYAN"|   Simplified EXT2 File system                |\n");
    printf(CYAN"|   支持的命令：                               |\n");
    printf(CYAN"|   查看目录     ： ls [dir]                   |\n");
    printf(CYAN"|   创建目录     ： mkdir [dir]                |\n");
    printf(CYAN"|   创建文件     ： touch [file]               |\n");
    printf(CYAN"|   删除文件/目录： rm [file/dir]              |\n");
    printf(CYAN"|   复制文件     ： cp [fileA] [fileB]         |\n");
    printf(CYAN"|   退出         ： exit                       |\n");
    printf(GREEN"----------------------------------------------\n");
}

int getcmd(char *buf, int nbuf) //读取指令
{
//   fprintf(2, "@ ");
  fprintf(stderr,LIGHT_PURPLE"EXT2$: ~%s >"NONE,currentPath);
  memset(buf, 0, nbuf); //初始话buf全为0
  fgets(buf, nbuf, stdin);      //从缓冲区读
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}

void getargs(char *buf, char* argv[],int *argc){
    int argv_num=0;  //参数个数
    int buf_index=0; //缓冲区索引
    for(;buf[buf_index]!='\n'&&buf[buf_index]!='\0';buf_index++){
        while(strchr(whitespace,buf[buf_index])){ //跳过空格
            buf_index++;
        }
        // echo  "hello world"
        argv[argv_num] = buf + buf_index; //参数列表存储第一个参数
        argv_num ++;
        while(strchr(whitespace,buf[buf_index]) == 0){ //寻找下一个空格
            buf_index++;
        }
        buf[buf_index]='\0';//修改成字符串终结符'\0'
    }
    argv[argv_num] = 0;
    *argc = argv_num;
}

void runcmd(char* argv[],int argc){
    read_super_block();
    int soutIndex =-1;
    for(int i = 0;i<argc;i++){
        if(!strcmp(argv[i],"|")){ //管道通信
            if(argc != i){ //后面至少还有一个参数
            }
            else{
                fprintf(stderr,"%s\n","no argv after |");
            }
        }
        if(!strcmp(argv[i],">")){ //输出重定向到下一个文件！
            if(i<argc-1){ //后面至少还有一个参数
                soutIndex = i;
            }
            else{
                fprintf(stderr,"%s\n","no argv after >");
            }
        }
        if(!strcmp(argv[i],"<")){ //输入重定向
            if(argc != i){ //后面至少还有一个参数
                argv[i] = '\0'; //停止
            }
            else{
                fprintf(stderr,"%s\n","no argv after <");
            }
        }
    }

    if(!strcmp(argv[0],"ls")){//ls
        if(strcmp(argv[argc-1],"-a") == 0){
            for(int i = 1;i<argc-1;i++){
                ls(argv[i],LS_RECURISON);
            }
            if(argc == 2) ls("",LS_RECURISON);
        }else
        {
            for(int i = 1;i<argc;i++){
                ls(argv[i],LS_ONLY_CURRENT_DIR);
            }
        }
        if(argc == 1) ls(currentPath,LS_RECURISON);

    }else if(!strcmp(argv[0],"mkdir")){
        for(int i = 1;i<argc;i++){
            mkdir(argv[i]);
        }
    }
    else if(!strcmp(argv[0],"touch")){
        for(int i = 1;i<argc;i++){
            touch(argv[i]);
        }
    }
    else if(!strcmp(argv[0],"cp")){
        if(argc == 3){
            copy(argv[1],argv[2]);
        }        
    }else if(!strcmp(argv[0],"rm")){
        if(!strcmp(argv[1],"-rf")){
            del_all_file();
        }else{
            remove_file(argv[1]);
        }
    }else if(!strcmp(argv[0],"cd")){
        if(argc == 2)
            change_current_path(argv[1]);
    }else if(!strcmp(argv[0],"exit")){
        fprintf(stderr,">>>>Ext2: exit \n");
        exit(0);
    }else
    {
        fprintf(stderr,RED"erro: no order called:\"%s\" in this directory\n",argv[0]);
    }
    
}

int sh_main(void){
    static char buf[128];
    while(getcmd(buf, sizeof(buf)) >= 0){
      char *argv[MAXARGS]; 
      int argc = -1;       
      getargs(buf,argv,&argc); 
      runcmd(argv,argc);
    }
  exit(0);
}

