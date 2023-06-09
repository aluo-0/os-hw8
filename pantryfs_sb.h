#ifndef __PANTRYFS_SB_H__
#define __PANTRYFS_SB_H__
#include "pantryfs_inode.h"
#include "pantryfs_file.h"

/* Macros to set, test, and clear a bit array of integers. */
#define SETBIT(A, k)     (A[((k) / 32)] |=  (1 << ((k) % 32)))
#define CLEARBIT(A, k)   (A[((k) / 32)] &= ~(1 << ((k) % 32)))
#define IS_SET(A, k)     (A[((k) / 32)] &   (1 << ((k) % 32)))

/* This macro will declare a bit vector. I use it to declare an array of the
 * right size inside the pantryfs_sb.
 */
#define DECLARE_BIT_VECTOR(name, size) uint32_t name[(size / 32) + 1];

#define PANTRYFS_MAGIC_NUMBER  0x00004118
#define PFS_BLOCK_SIZE 4096


/* Inode numbers start from 1. It's because if a function is supposed to
 * return an inode number and there's an error, the function returns 0!
 */
#define PANTRYFS_ROOT_INODE_NUMBER 1

/*  Data block #  |  Contents
 * -------------------------------
 *	0	  |  Superblock
 *	1         |  Inode Store
 *	2         |  Root Data Block
 */
#define PANTRYFS_SUPERBLOCK_DATABLOCK_NUMBER 0
#define PANTRYFS_INODE_STORE_DATABLOCK_NUMBER 1
#define PANTRYFS_ROOT_DATABLOCK_NUMBER 2

/* The inode store is one 4096 byte-block. The following macro calculates
 * how many pantryfs_inodes we can shove in the inode store.
 */
#define PFS_MAX_INODES (PFS_BLOCK_SIZE / sizeof(struct pantryfs_inode))
#define PFS_MAX_CHILDREN ((loff_t) (PFS_BLOCK_SIZE / sizeof(struct pantryfs_dir_entry)))

#define PFS_SB_MEMBERS uint64_t version;\
	uint64_t magic;\
	DECLARE_BIT_VECTOR(free_inodes, PFS_MAX_INODES);\
	DECLARE_BIT_VECTOR(free_data_blocks, PFS_MAX_INODES);

/* This is the superblock, as it will be serialized onto the disk. */
struct pantryfs_super_block {
	PFS_SB_MEMBERS

	/* Padding, so that this structure takes up the entire block. */
	char __padding__[PFS_BLOCK_SIZE - sizeof(struct {PFS_SB_MEMBERS})];
};

/* In the VFS superblock, we need to have a pointer to the buffer_heads for the
 * inode store and superblock so that we can mark them as dirty when they're
 * modified inode.
 */
struct pantryfs_sb_buffer_heads {
	struct buffer_head *sb_bh;
	struct buffer_head *i_store_bh;
};
#endif /* ifndef __PANTRYFS_SB_H__ */
