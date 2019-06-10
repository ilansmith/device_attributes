APP=serial_number
OBJS=main.o serial_number.o sysexec.o

%.o:%.c
	gcc -c $^ -o $@

$(APP): $(OBJS)
	gcc -o $@ $^

clean:
	rm -f *.o $(APP)
