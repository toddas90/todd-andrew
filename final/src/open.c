#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ext2fs/ext2fs.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../include/open.h"
#include "../include/util.h"
#include "../include/terminal.h"
#include "../include/type.h"

extern int dev, ninodes, nblocks, imap, bmap;
extern SUPER *sp;
extern GD *gp;
extern PROC *running;

int my_open(char *filename, int flags) {
    int ino = getino(filename);
    if (ino == 0) {
        my_creat(filename);
        ino = getino(filename);
    }
    MINODE *mip = iget(dev, ino);

    OFT *newoft;
    newoft->mode = flags;
    newoft->minodePtr = mip;
    newoft->refCount = 1;
    newoft->offset = 0;

    for (int i = 0; i<NFD; i++) {
        if (running->fd[i].refCount == 0) {
            running->fd[i] = &newoft;
            return i;
        }
    }

    return -1;
}