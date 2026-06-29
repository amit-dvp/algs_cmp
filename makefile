# Target to build the executable
merge_sort: maman_12.c
# gcc -g -Wall -pedantic -ansi maman_12.c -o maman_12 -lm
	gcc -g -Wall -pedantic -std=gnu99 maman_12.c -o maman_12 -lm
# Clean rule to remove previous files
clean:
	rm -f maman_12 maman_12.o 

# Add a default rule to clean before building
.PHONY: all clean
all: clean maman_12

# - To clean manually, run:  
#   ```bash
#   make clean
#   ```
# - To clean and build in one step, run:  
#   ```bash
#   make all