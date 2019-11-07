#include <stdio.h>  
#include <unistd.h>  
#include <stdlib.h>
#include <getopt.h> 
#include <string.h>


char* const short_options = "pd";

int command_getopt(int argc, char *argv[])
{  
     if (argc == 1){
         printf("no command given.\n");
         return 0;
     }
     if (argc == 2 && strcmp(argv[1], "ls")==0){
        system("ls");
     }
     if (argc == 2 && strcmp(argv[1],"--") == 0){
        printf("I get %s\n", argv[1]);
     }
     int c;
     while((c = getopt(argc, argv, short_options)) != -1)      
     {   
         switch (c)
         {   
         case 'p':
             printf(" this is P.\n");
             break;  
         case 'd':  
             printf("。this is DDDDD\n");
             break;  
         default:
             printf("usage: ./sandbox [-p sopath] [-d basedir] [--] cmd [cmd args ...]\n");
             printf("\t-p: set the path to sandbox.so, default = ./sandbox.so\n");
             printf("\t-d: restrict directory, default = .\n");
             printf("\t--: seperate the arguments for sandbox and for the executed command\n"); 
         }       
     }       
     return 0;
}  

int find_path(char *mylocation, char *filename){  
   char path[1024];  
   char *exist;  
   exist=realpath(filename, path);  
   /*如果檔案不存在將會是 NULL*/  
   if(exist==NULL){  
       puts("No such file found");
   }else{
       strcpy(mylocation, exist);
   }  
   return 0;  
}

int split(char **arr, char *str, const char *del) {
    int count = 0;
    char *s = strtok(str, del);
    while(s != NULL) {
        *arr++ = s;
        s = strtok(NULL, del);
        count++;
    }   
    return count;
}

int same_path(char *mylocation, char *target_location){
    char *arr_m[1024];
    char *arr_t[1024];
    char const *del = "/";
    int my_len, target_len;
    my_len = split(arr_m, mylocation, del) - 1; // minus 1 is becasue don'y want file itself
    target_len = split(arr_t, target_location, del) - 1;
//    printf("my = %d, target = %d\n", my_len, target_len);  
    int outside_sandbox = 0;
    for (int i = 0; i<my_len; i++){
        if ( (my_len > target_len) && strcmp( *(arr_m+i), *(arr_t+i))==0){
            outside_sandbox = 1;
            break;
        }
    }
    if (!outside_sandbox){
        printf("inside sandbox\n");
    }
    else{
        puts("outside.... reject\n");
    }
    return 0;
}

int main(int argc, char**argv){
    //command_getopt(argc, argv);
    char mylocation[1024], target_location[1024];
    find_path( mylocation, argv[0]);
    find_path(target_location, argv[1]);
//    printf("my file: %s\n", mylocation);
//    printf("target : %s\n", target_location);

    same_path(mylocation, target_location);
    return 0;
}
