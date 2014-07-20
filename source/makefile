shell : shell.o jobproc.o sigj.o myecho.o mypwd.o
	cc -o shell shell.o jobproc.o sigj.o myecho.o mypwd.o
	rm *.o
	echo Compilation complete. Type ./shell to run program
shell.o : shell.c shell.h
	cc -c shell.c
jobproc.o: jobproc.c jobproc.h
	cc -c jobproc.c
sigj.o:sigj.c sigj.h
	cc -c sigj.c
myecho.o:myecho.c myecho.h
	cc -c myecho.c
mypwd.o: mypwd.c mypwd.h
	cc -c mypwd.c
clean :
	rm shell
	rm *.o
