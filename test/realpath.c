#include <stdio.h>  
#include <unistd.h>  
#include <stdlib.h>

int main(int argc, char **argv){  
   if (argc != 2) {  
       printf("Usage: %s <file>\n", argv[0]);  
       return 1;  
   }  
  
   char path[1024];  
   char *exist;  
  
   exist=realpath(argv[1], path);  
  
   /*經過化簡後的路徑*/  
   puts(path);  
  
   /*如果檔案不存在將會是 NULL*/  
   if(exist==NULL){  
       puts("NULL");  
   }else{  
       puts(exist);  
   }  
  
   return 0;  
}  
