# makefile
all: sandboxso main
main:main.c
	gcc main.c -o sandbox
sandboxso:sandboxso.c
	gcc -shared -fPIC sandboxso.c -o sandboxso.so -ldl
clean:
	rm -f sandboxso.so sandbox
