#ifndef FS_H
#define FS_H

#define MAX_FILES 10
#define MAX_FILENAME_LEN 32
#define MAX_FILE_SIZE 1024

#include "vstring.h"

typedef struct {
    char name[MAX_FILENAME_LEN];
    char data[MAX_FILE_SIZE];
    int size;
    int is_open;
    int type;
} File;

void fs_init(void);
int fs_dir_size(const char *dirname, int new_size);
int fs_mkdir(const char *dirname);
int fs_create(const char *filename);
int fs_write(const char *filename, const char *data, int size);
int fs_read(const char *filename, char *buffer, int size);
extern void fs_ls(void);
extern void fs_cat(const char *filename);

#endif // FS_H
