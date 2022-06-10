#ifndef __ILRD_RD5678_FS_PARSING_H__
#define __ILRD_RD5678_FS_PARSING_H__

#include <linux/fs.h>
#include </usr/include/ext2fs/ext2_fs.h>
#include <linux/types.h>

int PrintFileData(const char *file_path, const char *driver_path);

#endif /* __ILRD_RD5678_FS_PARSING_H__ */