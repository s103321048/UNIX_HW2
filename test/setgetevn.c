#include <stdlib.h>
#include <stdio.h>
int main(){

	setenv("THIS", ".", 1);
	printf("after setenv\n");

	
	char *a = getenv("THIS");
	if (a != NULL)
		printf("%s\n", a);
}
