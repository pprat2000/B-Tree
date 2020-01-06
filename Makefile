OBJS = b_tree_delete.o b_tree_display.o \
		b_tree_insert.o b_tree_search.o \
		b_tree_utils.o

SLIB = libStaticBTree.a

DLIB = libDynamicBTree.so

all: lib driver

%.o: %.c $(wildcard *.h)
	@echo 'Compiling '$*
	@gcc -c -fPIC -Wall -o $@ $<

%.a: $(OBJS)
	@echo 'Building Static Library '$*
	@ar rcs $@ $(OBJS)
	@sudo mv $@ /usr/lib
	@echo 'Done'

%.so: $(OBJS)
	@echo 'Building Dynamic Library '$*
	@gcc -shared -o $@ $(OBJS)
	@sudo mv $@ /usr/lib/
	@echo 'Done'

lib: $(SLIB) $(DLIB)

driver:
	@echo 'Building Driver Programs'
	@gcc driver.c -lStaticBTree -o static_driver.o
	@gcc driver.c -lDynamicBTree -o dynamic_driver.o
	@echo 'Done'

clean:
	rm -f *.o *.a *.so

go: clean all