usecl: usecl.c
	mkdir -p bin
	gcc -o bin/usecl usecl.c 
clean:
	rm bin/usecl
