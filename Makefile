
NAME=egts
proc=main
elib=egtsFunc

CC=gcc
STRIP=strip
RM=rm

CFLAG =-std=gnu99 -O0 -Wall -g -pipe -D_GNU_SOURCE -D_REENTERANT

INC=/usr/include

MACHINE := $(shell uname -m)
ifeq ($(MACHINE), x86_64)
	LIB_DIR = /usr/lib
else
	LIB_DIR = /usr/lib32
endif

$(NAME): $(elib).o $(proc).o
	$(CC) -o $(NAME) $(elib).o $(proc).o -L$(LIB_DIR) -lpthread -ldl -lncurses
	$(STRIP) $(NAME)
$(proc).o: $(proc).c
	$(CC) -c $(proc).c $(CFLAG) -I$(INC)
$(elib).o: $(elib).c
	$(CC) -c $(elib).c $(CFLAG) -I$(INC)

clean:
	$(RM) -f *.o $(NAME)

