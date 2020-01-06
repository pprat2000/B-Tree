all: lib driver

lib:
	gcc -c -Wall b_tree.c
	ar rcs libBTree.a b_tree.o

driver:
	gcc driver.c -L. -lBTree -o driver.o

clean:
	rm -f *.o *.a