#ifndef EXT2_H
#define EXT2_H

#include "disk.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>



#define MAGICNUM 0xabcd

#define BLOCKNUM 126*32
#define INODENUM 32*32
#define FILE     0
#define DIR      1
#define BOTH_FILE_DIR    2

#define DATABLOCKOFFSET  66
#define ROOTDIRBLOCK     2
#define TRUE      1
#define FALSE     0
#define INVALIDBLOCKNUM   0xffffffff
#define TYPE_CP     0
#define TYPE_LS     1
#define BOTH_INODE_DATA     0
#define ONLY_INODE          1
#define ONLY_DATA           2
#define SIZE_DIR_ITEM       128
    
#define BLOCK_SIZE          1024
#define INODE_BLOCK_NUM     6

#define LS_PATH_SIZE        20
#define LS_MAX_SIZE         50

#define O_RDONLY  0x000
#define O_WRONLY  0x001
#define O_RDWR    0x002
#define O_CREATE  0x200

//结构体 128*4 +32*4 +4*4 = 164*4 B = 656 B

typedef struct super_block {
    int32_t magic_num;        // 幻数，用于识别文件系统
    int32_t free_block_count; // 空闲数据块数
    int32_t free_inode_count; // 空闲inode数
    int32_t dir_inode_count;  //  目录inode数
    uint32_t block_map[128];  // 数据块占用位图
    uint32_t inode_map[32];   // inode占用位图
}super_block;

// 24+4+4 = 32B
struct inode {
    uint32_t size;            // 文件大小 
    uint16_t file_type;       // 文件类型（文件/文件夹）
    uint16_t link;            // 文件链接数
    uint32_t block_point[6];  // 数据块指针
};


struct dir_item {
    uint32_t inode_id;        // 当前目录项表示的文件/目录的对应inode
    uint16_t valid;           // 当前目录项是否有效
    uint8_t type;             // 当前目录项类型（文件/目录）
    char name[121];           // 目录项表示的文件/目录的文件名/目录名
}; 



/**
 * @brief 初始化超级块
 */
int init_superblock();

/**
 * @brief 初始化根目录
 */
void init_rootDirectory();
/**
 * @brief 初始化所有文件
 */
void del_all_file();

/**
 * @brief 初始化文件系统
 */ 
int init_naiveExt2();


/**
 * @brief 删除指定blockNum的dir_item
 */
void del_inode_block(struct inode *delInode);

/**
 * @brief 写超级块
 * @param type 判断inode是不是目录
 * @param option 控制同时写inode、datablock还是分别写
 * 
 */
int write_super_block(int inode_num,int data_block_num,int type,int option);

void free_super_block(int inode_num,int data_block_num,int type,int option);


// 读入超级块
void read_super_block();

// 申请空闲inode
int apply_free_inode();

// 申请空闲数据block
int apply_free_block();

// 将inode 读到inodeIn
void read_inode(int inodeNum,char *inodeIn);

// 写inode所在的block
void write_inode(int inodeNum,struct inode inodeIn);

// 写inode对应某个文件的数据block
void write_inode_block(int inodeNum,struct dir_item dirItem);

// 读取目录inode对应的数据block，数据将读到全局变量temp_buf;
void read_inode_block(int inodeNum,char *filePath,int option);

// 查找对应目录Inode下的某个文件所在的inode
int find_directory_inode_Num(int inodeNum,char *fileName,int type);

// 创建目录 
int mkdir(char *filePath);

// 创建文件,同时通过参数返回dir_item
char* touch(char *filePath);

// 查看目录
int ls(char *filePath,int option);

// 处理路径
void handle_path(char *filePath,int *dirInode,char *temp,int *temp_index,int *isRoot);

// 复制数据块
void copy_data_block(int beforeBlockNum,int nextBlockNum);

// 复制inode与数据块
void copy_inode_data(int beforeInode,int nextInode);

// 复制文件
int copy(char *filePath,char *nextFilePath);

//cd操作 改变当前的路径
void change_current_path(char *filePath);

//删除目录项
void remove_diritem(int inodeNum,char *name);

//删除文件
void remove_file(char *charName);

//在inodeNum中是否存在filePath;
int is_exsit_dir_item(int inodeNum,char *dir_name);





#endif