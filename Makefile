APP=serial_number
OBJS=main.o serial_number.o sysexec.o
CFLAGS:=-Werror -Wall

ifdef CONFIG_DEBUG
CFLAGS+= -O0 -g
else
CFLAGS+= -O3
endif

%.o:%.c
	gcc -c $^ $(CFLAGS) -o $@

$(APP): $(OBJS)
	gcc -o $@ $^

clean:
	rm -f *.o $(APP)
