#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>                                                                      
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <dirent.h>
typedef int (*orig_open_f_type)(const char *pathname, int flags);
typedef int (*t_chdir)(const char *path);
typedef int (*t_chmod)(const char *path, mode_t mode);
typedef int (*t_chown)(const char *path, uid_t owner, gid_t group);
typedef int (*t_creat)(const char *path, mode_t mode);
typedef FILE *(*t_fopen)(const char * restrict path, const char * restrict mode);
typedef int (*t_link)(const char *path1, const char *path2);
typedef int (*t_mkdir)(const char *path, mode_t mode);
typedef int (*t_open)(const char *path, int oflag, ...);
typedef int (*t_open_add)(const char *path, int oflag, mode_t mode);
typedef int (*t_openat)(int fd, const char *path, int oflag, ...);
typedef int (*t_openat_add)(int fd, const char *path, int oflag, mode_t mode);
typedef DIR *(*t_opendir)(const char *filename);
typedef ssize_t (*t_readlink)(const char *restrict path, char *restrict buf, size_t bufsize);
typedef int (*t_remove)(const char *path);
typedef int (*t_rename)(const char *old, const char *new);
typedef int (*t_rmdir)(const char *path);
typedef int (*t_stat)(int ver, const char * path, struct stat * stat_buf);
typedef int (*t_stat64)(int ver, const char * path, struct stat64 * stat_buf);
typedef int (*t_statx)(int dirfd, const char *path, int flags, 
		  unsigned int mask, struct statx *statxbuf);
typedef int (*t_symlink)(const char *path1, const char *path2);
typedef int (*t_unlink)(const char *path);

char sandbox[1024];
char *now;

int path_check(const char *cmd, const char *path){
	now = getenv("path_now");
	//printf("in sandbox==> %s\n", now);
	realpath(now, sandbox);
    char trg_path[1024];
    realpath(path, trg_path);
    if (strncmp(trg_path, sandbox, strlen(sandbox)) != 0){
        fprintf( stderr,"[sandbox] %s: access to %s is not allowed\n", cmd, path);
        return 0;
    }
    return 1;
}

int chdir(const char *path){
    if ( path_check("chdir", path) ){
        t_chdir orig_chdir;
        orig_chdir = (t_chdir)dlsym(RTLD_NEXT,"chdir");
        return orig_chdir( path );
    }
    return -1;
}

int chmod(const char *path, mode_t mode){
    if ( path_check("chmod", path) ){
        t_chmod orig_chmod;
        orig_chmod = (t_chmod)dlsym(RTLD_NEXT,"chmod");
        return orig_chmod( path, mode );
    }
    return -1;
}
int chown(const char *path, uid_t owner, gid_t group){
    if ( path_check("chown", path) ){
        t_chown orig_chown;
        orig_chown = (t_chown)dlsym(RTLD_NEXT, "chown");
        return orig_chown( path, owner, group);
    }
    return -1;
}
int creat(const char *path, mode_t mode){
    if ( path_check("creat", path) ){
        t_creat orig_creat;
        orig_creat = (t_creat)dlsym(RTLD_NEXT, "creat");
        return orig_creat( path, mode);
    }
    return -1;
}

int creat64(const char *path, mode_t mode){
    if ( path_check("creat64", path) ){
        t_creat orig_creat64;
        orig_creat64 = (t_creat)dlsym(RTLD_NEXT, "creat64");
        return orig_creat64( path, mode);
    }
    return -1;
}

FILE *fopen(const char * restrict path, const char * restrict mode){
    if ( path_check("fopen", path) ){
        t_fopen orig_fopen;
        orig_fopen = (t_fopen)dlsym(RTLD_NEXT, "fopen");
        return orig_fopen( path, mode);
    }
    return 0;
}

FILE *fopen64(const char * restrict path, const char * restrict mode){
    if ( path_check("fopen64", path) ){
        t_fopen orig_fopen64;
        orig_fopen64 = (t_fopen)dlsym(RTLD_NEXT, "fopen64");
        return orig_fopen64( path, mode);
    }
    return 0;
}

int link(const char *path1, const char *path2){
    if ( path_check("link", path1) && path_check("link", path2)){
        t_link orig_link;
        orig_link = (t_link)dlsym(RTLD_NEXT, "link");
        return orig_link( path1, path2);
    }
    return  -1; 
}
int mkdir(const char *path, mode_t mode){
    if ( path_check("mkdir", path) ){
        t_mkdir orig_mkdir;
        orig_mkdir = (t_mkdir)dlsym(RTLD_NEXT, "mkdir");
        return orig_mkdir( path, mode);
    }   
    return  -1;
}

int open(const char *path, int oflag, ...){
	va_list ap;
    int *args;
    int argno = 0;

    va_start(ap, oflag);
	mode_t mode;
    mode = va_arg(ap, mode_t);
	va_end(ap);
    if ( path_check("open", path) ){
		t_open orig_open;
        t_open_add orig_open_add;
		orig_open = (t_open)dlsym(RTLD_NEXT,"open");
		orig_open_add = (t_open_add)dlsym(RTLD_NEXT, "open");
		if (mode>=0 && mode <=777) 
			return orig_open_add(path, oflag, mode);
		else
			return orig_open(path, oflag);
    }
	return  -1;
}

int open64(const char *path, int oflag, ...){
	va_list ap;
    int *args;
    int argno = 0;

    va_start(ap, oflag);
	mode_t mode;
    mode = va_arg(ap, mode_t);
	va_end(ap);
    if ( path_check("open64", path) ){
		t_open orig_open64;
        t_open_add orig_open_add64;
		orig_open64 = (t_open)dlsym(RTLD_NEXT,"open64");
		orig_open_add64 = (t_open_add)dlsym(RTLD_NEXT, "open64");
		if (mode>=0 && mode <=777) 
			return orig_open_add64(path, oflag, mode);
		else
			return orig_open64(path, oflag);
    }
	return  -1;
}

int openat(int fd, const char *path, int oflag, ...){
	va_list ap;
    int *args;
    int argno = 0;

    va_start(ap, oflag);
	mode_t mode;
    mode = va_arg(ap, mode_t);
	va_end(ap);
    if ( path_check("openat", path) ){
        t_openat orig_openat;
		t_openat_add orig_openat_add;
        orig_openat = (t_openat)dlsym(RTLD_NEXT,"openat");
        orig_openat_add = (t_openat_add)dlsym(RTLD_NEXT,"openat");
        if (mode >=0 && mode <= 777)
			return orig_openat_add(fd, path, oflag, mode);
		else
			return orig_openat(fd, path, oflag);
    }
    return  -1;
}

int openat64(int fd, const char *path, int oflag, ...){
	va_list ap;
    int *args;
    int argno = 0;

    va_start(ap, oflag);
	mode_t mode;
    mode = va_arg(ap, mode_t);
	va_end(ap);
    if ( path_check("openat64", path) ){
        t_openat orig_openat;
		t_openat_add orig_openat_add;
        orig_openat = (t_openat)dlsym(RTLD_NEXT,"openat64");
        orig_openat_add = (t_openat_add)dlsym(RTLD_NEXT,"openat64");
        if (mode >=0 && mode <= 777)
			return orig_openat_add(fd, path, oflag, mode);
		else
			return orig_openat(fd, path, oflag);
    }
    return  -1;
}
DIR *opendir(const char *filename){
     if ( path_check("opendir", filename) ){
        t_opendir orig_opendir;
        orig_opendir = (t_opendir)dlsym(RTLD_NEXT,"opendir");
        return orig_opendir( filename);
    }
    return  0;
}

ssize_t readlink(const char *restrict path, char *restrict buf, size_t bufsize){
     if ( path_check("readlink", path) ){
        t_readlink orig_readlink;
        orig_readlink = (t_readlink)dlsym(RTLD_NEXT,"readlink");
        return orig_readlink( path, buf, bufsize);
    }   
    return  -1; 
}

int remove(const char *path){
     if ( path_check("remove", path) ){
        t_remove orig_remove;
        orig_remove = (t_remove)dlsym(RTLD_NEXT,"remove");
        return orig_remove( path);
    }   
    return  -1; 
}


int rename(const char *old, const char *new){
     if ( path_check("rename", old) && path_check("rename", new) ){
        t_rename orig_rename;
        orig_rename = (t_rename)dlsym(RTLD_NEXT,"rename");
        return orig_rename( old, new);
    }   
    return  -1; 
}

int rmdir(const char *path){
     if ( path_check("rmdir", path) ){
        t_rmdir orig_rmdir;
        orig_rmdir = (t_rmdir)dlsym(RTLD_NEXT,"rmdir");
        return orig_rmdir( path);
    }   
    return  -1; 
}

int __xstat(int ver, const char * path, struct stat * stat_buf){
     if ( path_check("__xstat", path) ){
        t_stat orig_stat;
        orig_stat = (t_stat)dlsym(RTLD_NEXT,"__xstat");
        return orig_stat( ver, path, stat_buf);
    }   
    return -1; 
}

int statx(int dirfd, const char *path, int flags,
                 unsigned int mask, struct statx *statxbuf){
     if ( path_check("statx", path) ){
        t_statx orig_stat;
        orig_stat = (t_statx)dlsym(RTLD_NEXT, "statx");
        return orig_stat( dirfd, path, flags, mask, statxbuf);
    }   
    return -1; 
}

int __lxstat(int ver, const char * path, struct stat * stat_buf){
     if ( path_check("__lxstat", path) ){
        t_stat orig_stat;
        orig_stat = (t_stat)dlsym(RTLD_NEXT,"__lxstat");
        return orig_stat( ver, path, stat_buf);
    }
    return -1;
}

int __xstat64(int ver, const char * path, struct stat64 * stat_buf){
     if ( path_check("__xstat64", path) ){
        t_stat64 orig_stat64;
        orig_stat64 = (t_stat64)dlsym(RTLD_NEXT,"__xstat64");
        return orig_stat64( ver, path, stat_buf);
    }   
    return -1; 
}

int __lxstat64(int ver, const char * path, struct stat64 * stat_buf){
     if ( path_check("__lxstat64", path) ){
        t_stat64 orig_stat64;
        orig_stat64 = (t_stat64)dlsym(RTLD_NEXT,"__lxstat64");
        return orig_stat64( ver, path, stat_buf);
    }
    return -1;
}
int symlink(const char *path1, const char *path2){
     if ( path_check("symlink", path2) ){
        t_symlink orig_symlink;
        orig_symlink = (t_symlink)dlsym(RTLD_NEXT,"symlink");
        return orig_symlink( path1, path2);
    }   
    return  -1; 
}

int unlink(const char *path){
     if ( path_check("unlink", path) ){
        t_unlink orig_unlink;
        orig_unlink = (t_unlink)dlsym(RTLD_NEXT,"unlink");
        return orig_unlink( path);
    }   
    return  -1; 
}

int fstat(int fd, struct stat *buf){
	fprintf(stderr, "[sandbox] fstat: not allowed\n");
}  
int execl(const char *path, const char *arg0, ...){
    fprintf(stderr,"[sandbox] execl(%s): not allowed\n", path);    
}
int execle(const char *path, const char *arg0, ...){
    fprintf(stderr,"[sandbox] execle(%s): not allowed\n", path);  
}
int execlp(const char *file, const char *arg0, ...){
    fprintf(stderr,"[sandbox] execlp(%s): not allowed\n", file);  
}
int execv(const char *path, char *const argv[]){
    fprintf(stderr,"[sandbox] execv(%s): not allowed\n", path);  
}
int execve(const char *path, char *const argv[], char *const envp[]){
    fprintf(stderr,"[sandbox] execve(%s): not allowed\n", path);  
}
int execvp(const char *file, char *const argv[]){
    fprintf(stderr,"[sandbox] execvp(%s): not allowed\n", file);  
}
int system(const char *command){
    fprintf(stderr,"[sandbox] system(%s): not allowed\n", command);
}
