OBJ = zparametrami.c
all: WaskiMost
WaskiMost: $(OBJ)
		gcc $(OBJ) -o WaskiMost -pthread