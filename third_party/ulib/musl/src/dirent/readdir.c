#include "__dirent.h"
#include "syscall.h"
#include <dirent.h>
#include <errno.h>

int __getdents(int, struct dirent*, size_t);

struct dirent* readdir(DIR* dir) {
    struct dirent* de;

    if (dir->buf_pos >= dir->buf_end) {
        int len = __syscall(SYS_getdents, dir->fd, dir->buf, sizeof dir->buf);
        if (len <= 0) {
            if (len < 0 && len != -ENOENT)
                errno = -len;
            return 0;
        }
        dir->buf_end = len;
        dir->buf_pos = 0;
    }
    de = (void*)(dir->buf + dir->buf_pos);
    dir->buf_pos += de->d_reclen;
    dir->tell = de->d_off;
    return de;
}