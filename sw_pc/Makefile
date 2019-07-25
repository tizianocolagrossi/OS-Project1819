programma : ghost.o cont_sett_struct.o set_finger.o
	gcc ghost.o cont_sett_struct.o set_finger.o -lxdo -lpthread -o ghost

ghost.o : ghost.c
	gcc -c ghost.c -lxdo 

cont_sett_struct.o :cont_sett_struct.c 
	gcc -lxdo -c cont_sett_struct.c 


