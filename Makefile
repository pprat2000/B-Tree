OBJS = b_tree_delete.o b_tree_display.o \
		b_tree_insert.o b_tree_search.o \
		b_tree_utils.o

SLIB = libBTree.a

DLIB = libBTree.so

all: lib driver

%.o: %.c $(wildcard *.h)
	@echo 'Compiling '$*
	@gcc -c -Wall -o $@ $<

%.a: $(OBJS)
	@echo 'Building Static Library '$*
	@ar rcs $@ $(OBJS)
	@echo 'Done'

lib: $(SLIB)

driver:
	@echo 'Building Driver Program'
	@gcc driver.c -L. -lBTree -o driver.o
	@echo 'Done'

clean:
	rm -f *.o *.a