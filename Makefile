CC = gcc
SDIR = srcs
IDIR = headers
ODIR = objs

OBJS += $(ODIR)/cd.o
OBJS += $(ODIR)/kill.o
OBJS += $(ODIR)/list.o
OBJS += $(ODIR)/myshell.o
OBJS += $(ODIR)/path.o
OBJS += $(ODIR)/printenv.o
OBJS += $(ODIR)/pwd.o
OBJS += $(ODIR)/where.o
OBJS += $(ODIR)/which.o

CFLAGS = -I $(IDIR) -ggdb3
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