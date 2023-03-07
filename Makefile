CC = gcc
SDIR = srcs
IDIR = headers
ODIR = objs

OBJS += $(ODIR)/myshell.o
OBJS += $(ODIR)/path.o
OBJS += $(ODIR)/which.o

CFLAGS = -I $(IDIR)
LIBS =

make: myshell

myshell: $(OBJS)
	$(CC) -o myshell $(OBJS) $(LIBS)

$(ODIR)/%.o: $(SDIR)/%.c | $(ODIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(ODIR):
	mkdir -p $@

.PHONY: clean

clean:
	rm $(ODIR)/*.o myshell