BIN := bin
SRC := src
INCLUDE := include

CC := gcc
PDF  := $(wildcard *.pdf)
TEXT := $(patsubst %.pdf,%.txt,$(PDF))
OBJECTS := $(SRC)/assignment.o
EXECUTABLE := assignment.o
ARGUMENTS := 

all: $(EXECUTABLE)

# $^ is $(OBJECTS)
$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ -o $(BIN)/$(EXECUTABLE)

# students: src/main.o src/student.o
# 	gcc -Wall -Werror -Iinclude src/main.o src/student.o -o bin/students

run: $(EXECUTABLE)
	./$(BIN)/$(EXECUTABLE) $(ARGUMENTS)

clean:
	$(RM) $(OBJECTS) $(BIN)/$(EXECUTABLE) $(BIN)/data.txt

.PHONY: all, run, clean