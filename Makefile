usecl: usecl.c
	mkdir -p bin
	gcc -o bin/usecl usecl.c -lsystemd
clean:
	rm bin/usecl
