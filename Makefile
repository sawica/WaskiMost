OBJ = zparametrami.o
all: WaskiMost
WaskiMost: $(OBJ)
    gcc $(OBJ) -o WaskiMost
$(OBJ): clean
clean: rm -f *.o WaskiMost