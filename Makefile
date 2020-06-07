# Macros para compilacao
CC = gcc
UNAME_S := $(shell uname -s)
CFLAGS_LINUX = -Wextra -lfl
CFLAGS_OSX = -Wextra -ll
DIR = src
FILENAME = $(DIR)/main.c
YYTABH = $(DIR)/y.tab.h
YYTABC = $(DIR)/y.tab.c
LEXOUT = $(DIR)/lex.yy.c
YACCFILE = $(DIR)/main.y
LEXFILE = $(DIR)/main.l
OPFILEH = $(DIR)/operation.h
OPFILEC = $(DIR)/operation.c
TARGET = ./main
BJS = $(SRCS:.c=.o)
YACC = bison
LEX = flex


# Macros para teste
BASH = sh
TEST_SCRIPT = test.sh
VERBOSE ?= 1

# Macros para construcao do zip
ZIP = zip
USERNAME ?= $(USER)
ZIPFILE = $(USERNAME).zip
EXTENSIONS = *.c *.h *.in *.out *.sh

.PHONY: depend clean

all:$(TARGET)

$(TARGET):$(LEXOUT) $(YYTABC)
ifeq ($(UNAME_S),Linux)
	$(CC) -o$(TARGET) $(LEXOUT) $(YYTABC) $(OPFILEC) $(CFLAGS_LINUX)
endif
ifeq ($(UNAME_S),Darwin)
	$(CC) -o$(TARGET) $(LEXOUT) $(YYTABC) $(OPFILEC) $(CFLAGS_OSX)
endif


$(LEXOUT):$(LEXFILE) $(YYTABC)
	$(LEX) -o$(LEXOUT) $(LEXFILE)

$(YYTABC):$(YACCFILE)
	$(YACC) -o$(YYTABC) -dy $(YACCFILE) -v

test:all
	$(BASH) $(TEST_SCRIPT) $(TARGET) $(VERBOSE)

zip:clean
	$(ZIP) -R $(ZIPFILE)  Makefile $(EXTENSIONS)

clean:
	$(RM) $(YYTABC)
	$(RM) $(YYTABH)
	$(RM) $(LEXOUT)
	$(RM) ./$(TARGET)
	$(RM) $(DIR)/*.o
	$(RM) ./$(ZIPFILE)
