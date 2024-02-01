# Makefile pour compiler le programme AVL

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -std=c99

# Fichiers sources et objets
SRCS = avl.c main.c
OBJS = $(SRCS:.c=.o)

# Exécutable cible
TARGET = avl

.PHONY: all clean run

# Règle pour construire l'exécutable
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Règle pour compiler chaque fichier source en un fichier objet
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Cible pour exécuter le programme
run: $(TARGET)
	./$(TARGET)

# Cible pour nettoyer les fichiers objets et l'exécutable
clean:
	rm -f $(TARGET) $(OBJS)

