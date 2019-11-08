#include <stdio.h>  
#include <unistd.h>  
#include <stdlib.h>
#include <getopt.h> 
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char* const short_options = "pd";

int path_check(const char *path){
    char *now = getenv("path_now");
    char sandbox[1024];
    realpath(now, sandbox);
    char trg_path[1024];
    realpath(path, trg_path);
    if (strncmp(trg_path, sandbox, strlen(sandbox)) != 0){ 
        fprintf( stderr,"ls: cannot open directory '%s'\n", path);
        return 0;
    }   
    return 1;
}

int command_getopt(int argc, char *argv[])
{  
    if (argc == 1){
		fprintf(stderr ,"no command given.\n");
        return 0;
    }
    int ls_loc, pl, dl, ddl = 0; // dash p location, dash d location, dash dash location
	int c;
	char so_path[1024];
	char *exist_so;
	char newpath[1024];
	char *exist_new;
    while((c = getopt(argc, argv, short_options)) != -1)      
    { 
        switch (c)
        {   
        case 'p':
			exist_so = realpath( argv[optind], so_path );
			if (exist_so == NULL){
				fprintf(stderr, "No such path exist\n");
				exit(0);
			}
			strcat( exist_so , "/sandboxso.so");
			setenv( "LD_PRELOAD", exist_so,1);  
            break;  
        case 'd':
			exist_new = realpath(argv[optind], newpath);
			if (exist_new == NULL){
				fprintf(stderr, "No such file exist\n");
				exit(0);
			}
			setenv( "path_now", exist_new,1);
            break;  
        default:
            fprintf(stderr,"usage: ./sandbox [-p sopath] [-d basedir] [--] cmd [cmd args ...]\n");
            fprintf(stderr,"\t-p: set the path to sandbox.so, default = ./sandbox.so\n");
            fprintf(stderr,"\t-d: restrict directory, default = .\n");
            fprintf(stderr,"\t--: seperate the arguments for sandbox and for the executed command\n"); 
        }
    }
	for (int i = 1; i < argc; i++){
		if ( strcmp(argv[i], "-p")==0 ) pl = 1;
		if ( strcmp(argv[i], "-d")==0 ) dl = 1;
		if ( strcmp(argv[i], "--")==0 ) ddl = i+1; // get the position after --
		if ( strcmp(argv[i], "ls")==0 ) ls_loc = i;
	}
	ddl = ddl + dl + pl; // because "-p" & "-d" are not count as argv
	if ( strcmp(argv[1], "ls")==0 ){
		char* const onlyls[3] = {"ls", ".", NULL};
 		execvp( onlyls[0], onlyls );
	}
	if (dl != 0 && ls_loc != 0){
		char *const onlylss[3] = {argv[ls_loc], argv[ls_loc+1], NULL};
		if (onlylss[1] == NULL)
			execvp(onlylss[0], onlylss);		
		else if (path_check(onlylss[1])){
//			printf("get ls_loc =%d\n", ls_loc);
			execvp(onlylss[0], onlylss);
		}
	}

	char* arg[1024];
	int count = 0;
	if (ddl != 0){ // get, run after "--"
		for (int i = ddl; i < argc; i++){
			arg[count++] = argv[i];
//			printf("cmd after --:%d%s ", i,argv[i]);
		}
//		printf("\n");
//		printf("\n%s \n", arg);

		execvp(arg[0], arg);
	}
//	printf("pl=%d \ndl=%d \n--=%d\n", pl, dl, ddl);
	return 0;
}

int main(int argc, char**argv){
	// /dev/tty => control terminal
	int fd = open("/dev/tty", O_WRONLY, 0666);
	dup2(fd, STDERR_FILENO);

	setenv("LD_PRELOAD","./sandboxso.so",1);
	setenv("path_now", ".", 1);
	command_getopt(argc, argv);
	
	return 0;
}
