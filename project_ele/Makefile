OBJ = ser
SRC = ser.c ser_tcp_init.c send_info.c user_ctl.c url2.c myepoll.c
CC = gcc

$(OBJ):$(SRC)
	$(CC) $(SRC) -o $(OBJ) -lsqlite3

clean:
	rm -f $(OBJ)