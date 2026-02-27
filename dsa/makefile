cc = clang
flags = -Wall -Wextra -g
src = hashmap
target = out

all: $(target)

$(target): $(src)
	$(cc) $(flags) $(src).c -o $(target) && ./$(target)

clr:
	find . -type f -executable -delete
