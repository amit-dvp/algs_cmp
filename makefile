# Target to build the executable
algs_cmp: algs_cmp.c
# gcc -g -Wall -pedantic -ansi algs_cmp.c -o algs_cmp -lm
	gcc -g -Wall -pedantic -std=gnu99 algs_cmp.c -o algs_cmp -lm
# Clean rule to remove previous files
clean:
	rm -f algs_cmp algs_cmp.o 

# Add a default rule to clean before building
.PHONY: all clean
all: clean algs_cmp

# - To clean manually, run:  
#   ```bash
#   make clean
#   ```
# - To clean and build in one step, run:  
#   ```bash
#   make all
