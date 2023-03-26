CC = gcc
SDIR = srcs
IDIR = headers
ODIR = objs

OBJS += $(ODIR)/cd.o
OBJS += $(ODIR)/execute_external.o
OBJS += $(ODIR)/kill.o
OBJS += $(ODIR)/linkedlist.o
OBJS += $(ODIR)/list.o
OBJS += $(ODIR)/myshell.o
OBJS += $(ODIR)/path.o
OBJS += $(ODIR)/printenv.o
OBJS += $(ODIR)/prompt.o
OBJS += $(ODIR)/pwd.o
OBJS += $(ODIR)/setenv.o
OBJS += $(ODIR)/which.o

CFLAGS = -g -I $(IDIR)
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
	rm -f $(ODIR)/*.o myshell