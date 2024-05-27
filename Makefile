SURSE = main.c player.c team.c match.c bst.c avl.c
OBIECTE = $(SURSE:.c=.o)

all: lanParty

lanParty: $(OBIECTE)
	gcc -Wall -Wpedantic $(OBIECTE) -o lanParty

%.o: %.c
	gcc -Wall -Wpedantic -std=c99 -g -c $< -o $@

clean:
	rm -f $(OBIECTE) lanParty
	rm -r out/

.PHONY: all clean

