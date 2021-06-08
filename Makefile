OBJ = main.c
all: WaskiMost
WaskiMost: $(OBJ)
		gcc $(OBJ) -o WaskiMost -pthread