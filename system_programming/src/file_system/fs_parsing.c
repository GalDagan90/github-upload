#include <stdio.h>		/* printf(), puts()	*/
#include <fcntl.h>		/* read_flags	*/
#include <string.h>		/* strlen(), memcpy(), strtok()	*/
#include <unistd.h>		/* lseek(), read()	*/
#include <stdlib.h>		/* malloc(), free()	*/

#include "fs_parsing.h"

#define BASE_OFFSET 1024

typedef enum exit_status
{
	FAIL = -1,
	SUCCESS
} status_t;

unsigned int g_block_size = 0;

/*******************************************************************************
*						Static Function Declerations						   *
*******************************************************************************/

static int PrintSuperBlock(struct ext2_super_block *super_block, int fd);
static int ReadGroupDescBlock(struct ext2_group_desc *group_desc, int fd,
						unsigned int block_group);
static int ReadInode(int fd, struct ext2_super_block *super_block,
				struct ext2_group_desc *group_descriptor,
				struct ext2_inode *inode, unsigned int local_inode);
static int FindFile(char *file, int fd, struct ext2_inode *inode, 
			struct ext2_group_desc *group_desc, struct ext2_super_block *sb);
static void PrintFile(int fd, struct ext2_inode *inode);
static int FindFilePath(char *file_path, int fd, struct ext2_inode *inode, 
			struct ext2_group_desc *group_desc, struct ext2_super_block *sb,
			struct ext2_inode *ret_inode);
static int ReadFromFileDesc(int fd, size_t offset, size_t n_bytes, void *dest);
static void PrintBlock(int fd, size_t block_offset);
static void PrintINDBlock(int fd, size_t block_offset);
static void PrintDINDBlock(int fd, size_t block_offset);
static void PrintTINDBlock(int fd, size_t block_offset);

/******************************************************************************/

int PrintFileData(const char *file_path, const char *driver_path)
{
	struct ext2_super_block super_block;
	struct ext2_group_desc group_desc;
	struct ext2_inode inode;
	struct ext2_inode ret_inode;
	int fd = 0;
	char path_copy[1024] = {0};
	status_t status = FAIL;

	if ((fd = open(driver_path, O_RDONLY)) == FAIL)
	{
		perror("Failed to open device\n");
		return FAIL;
	}

	status = PrintSuperBlock(&super_block, fd);
	status = ReadGroupDescBlock(&group_desc, fd, 0);
	status = ReadInode(fd, &super_block, &group_desc, &inode, 1);

	memcpy(path_copy, file_path, strlen(file_path) + 1);
	status = FindFilePath(path_copy, fd, &inode, &group_desc, &super_block, &ret_inode);

	if (FAIL != status)
	{
		PrintFile(fd, &ret_inode);
	}

	close(fd);

	return status;
}

/******************************************************************************/

static int PrintSuperBlock(struct ext2_super_block *super_block, int fd)
{
	if (FAIL == ReadFromFileDesc(fd, BASE_OFFSET, 
						sizeof(struct ext2_super_block), super_block))
	{
		close(fd);

		return FAIL;
	}

	if (EXT2_SUPER_MAGIC != super_block->s_magic)
	{
		printf("Could not print the super_block block\n");

		return (FAIL);
	}

	g_block_size = BASE_OFFSET << super_block->s_log_block_size;
	
	/* printf("\nReading super_block-block from device /dev/ram0 :\n"
	       "Inodes count            : %u\n"
	       "Blocks count            : %u\n"
	       "Reserved blocks count   : %u\n"
	       "Free blocks count       : %u\n"
	       "Free inodes count       : %u\n"
	       "First data block        : %u\n"
	       "Block size              : %u\n"
	       "Blocks per group        : %u\n"
	       "Inodes per group        : %u\n"
	       "Creator OS              : %u\n"
	       "First non-reserved inode: %u\n"
	       "Size of inode structure : %hu\n"
	       ,
	       super_block->s_inodes_count,  
	       super_block->s_blocks_count,
	       super_block->s_r_blocks_count,
	       super_block->s_free_blocks_count,
	       super_block->s_free_inodes_count,
	       super_block->s_first_data_block,
	       g_block_size,
	       super_block->s_blocks_per_group,
	       super_block->s_inodes_per_group,
	       super_block->s_creator_os,
	       super_block->s_first_ino,
	       super_block->s_inode_size); */

	return (SUCCESS);
}

static int ReadGroupDescBlock(struct ext2_group_desc *group_desc, int fd,
						unsigned int block_group)
{
	size_t offset = g_block_size + (sizeof(struct ext2_group_desc)) * block_group;

	if (FAIL == ReadFromFileDesc(fd, offset, 
						sizeof(struct ext2_group_desc), group_desc))
	{
		close(fd);

		return FAIL;
	}

	/* printf("\nReading first group-descriptor from device /dev/ram0\n"
	       "Blocks bitmap block: %u\n"
	       "Inodes bitmap block: %u\n"
	       "Inodes table block : %u\n"
	       "Free blocks count  : %u\n"
	       "Free inodes count  : %u\n"
	       "Directories count  : %u\n",
	       group_desc->bg_block_bitmap,
	       group_desc->bg_inode_bitmap,
	       group_desc->bg_inode_table,
	       group_desc->bg_free_blocks_count,
	       group_desc->bg_free_inodes_count,
	       group_desc->bg_used_dirs_count); */

	return (SUCCESS); 
}

static int ReadInode(int fd, struct ext2_super_block *super_block,
				struct ext2_group_desc *group_descriptor,
				struct ext2_inode *inode, unsigned int local_inode)
{
	size_t offset = (g_block_size * group_descriptor->bg_inode_table) +
			((local_inode) * super_block->s_inode_size);

	if (FAIL == ReadFromFileDesc(fd, offset, sizeof(struct ext2_inode), inode))
	{
		close(fd);

		return FAIL;
	}

	return SUCCESS;
}

static int FindFilePath(char *file_path, int fd, struct ext2_inode *inode, 
			struct ext2_group_desc *group_desc, struct ext2_super_block *sb,
			struct ext2_inode *ret_inode)
{
	char *token = NULL;
	int inode_number = 0;

	for (token = strtok(file_path, "/"); token != NULL && inode_number != FAIL;
		token = strtok(NULL, "/"))
	{
		inode_number = FindFile(token, fd, inode, group_desc, sb);
		ReadInode(fd, sb, group_desc, ret_inode, inode_number);
		inode = ret_inode;
	}

	return ((-1 == inode_number) ? FAIL : SUCCESS);
}

static int FindFile(char *file, int fd, struct ext2_inode *inode, 
			struct ext2_group_desc *group_desc, struct ext2_super_block *sb)
{
	char block[BLOCK_SIZE];
	struct ext2_dir_entry_2 *entry = (struct ext2_dir_entry_2 *)block;
	unsigned int size = 0;
	unsigned int block_group = 0;
	unsigned int local_inode = 0;

	if (FAIL == ReadFromFileDesc(fd, inode->i_block[0] * g_block_size,
				sizeof(struct ext2_dir_entry_2), entry))
	{
		close(fd);

		return FAIL;
	}

	while ((size < inode->i_size) && entry->inode)
	{
		if (0 == strncmp(file, entry->name, entry->name_len))
		{
			block_group = (entry->inode - 1) / sb->s_inodes_per_group;
    		local_inode = (entry->inode - 1) % sb->s_inodes_per_group;
			ReadGroupDescBlock(group_desc, fd, block_group);

			return local_inode;
		}

		entry = (struct ext2_dir_entry_2 *)((char *)entry + entry->rec_len);
		size += entry->rec_len;
	}

	return FAIL;
}

static void PrintFile(int fd, struct ext2_inode *inode)
{
	int i = 0;

	for(i = 0; inode->i_block[i] != 0 && i < EXT2_N_BLOCKS; ++i)
	{
		size_t block_offset = inode->i_block[i] * g_block_size;
		if (i < EXT2_NDIR_BLOCKS)
		{
			PrintBlock(fd, block_offset);
		}
		else if (i == EXT2_IND_BLOCK)
		{
			PrintINDBlock(fd, block_offset);
		}
		else if (i == EXT2_DIND_BLOCK)
		{
			PrintDINDBlock(fd, block_offset);
		}
		else if (i == EXT2_TIND_BLOCK)
		{
			PrintTINDBlock(fd, block_offset);
		}
	}
}

static int ReadFromFileDesc(int fd, size_t offset, size_t n_bytes, void *dest)
{
	lseek(fd, offset, SEEK_SET);

	return read(fd, dest, n_bytes);
}

static void PrintBlock(int fd, size_t block_offset)
{
	char buffer[4096 + 1] = {'\0'};

	if (FAIL != ReadFromFileDesc(fd, block_offset, g_block_size, buffer))
	{
		printf("%s", buffer);
	}
}

static void PrintINDBlock(int fd, size_t block_offset)
{
	unsigned int *block_runner = NULL;
	void *indirect_block = malloc(g_block_size);
	if (NULL == indirect_block)
	{
		close(fd);

		return;
	}

	if (FAIL == ReadFromFileDesc(fd, block_offset, g_block_size, indirect_block))
	{
		close(fd);

		return;
	}

	block_runner = (unsigned int *)indirect_block;
	for (; 0 != *block_runner; ++block_runner)
	{
		PrintBlock(fd, *block_runner * g_block_size);
	}

	free(indirect_block);
}

static void PrintDINDBlock(int fd, size_t block_offset)
{
	unsigned int *block_runner = NULL;
	void *db_indirect_block = malloc(g_block_size);
	if (NULL == db_indirect_block)
	{
		close(fd);

		return;
	}

	if (FAIL == ReadFromFileDesc(fd, block_offset, g_block_size, db_indirect_block))
	{
		close(fd);
		free(db_indirect_block);

		return;
	}

	block_runner = (unsigned int *)db_indirect_block;
	for (; 0 != *block_runner; ++block_runner)
	{
		PrintINDBlock(fd, *block_runner * g_block_size);
	}

	free(db_indirect_block);
}

static void PrintTINDBlock(int fd, size_t block_offset)
{
	unsigned int *block_runner = NULL;
	void *tp_indirect_block = malloc(g_block_size);
	if (NULL == tp_indirect_block)
	{
		close(fd);

		return;
	}

	if (FAIL == ReadFromFileDesc(fd, block_offset, g_block_size, tp_indirect_block))
	{
		close(fd);
		free(tp_indirect_block);

		return;
	}

	block_runner = (unsigned int *)tp_indirect_block;
	for (; 0 != *block_runner; ++block_runner)
	{
		PrintDINDBlock(fd, *block_runner * g_block_size);
	}

	free(tp_indirect_block);
}