ext=.exe
LF=-lvJoyInterface
LIB=-Llib
OPATH=obj
RM=del /f
RMDIR=rd /s /q
MD=md

.PHONY: all clean
.SUFFIXES:

CC=g++
name=ACon

HEAD=
OBJ=acon.o
INCLUDE=inc

CF=-Wall -g -std=c++11

################             Recipes                ###############################

all: bin $(OPATH) $(patsubst %.o, $(OPATH)/%.o, $(OBJ))
	$(CC) -o bin/$(name)$(ext) $(patsubst %.o, $(OPATH)/%.o, $(OBJ)) $(LIB) $(CF) $(LF)

bin:
	$(MD) bin
	
$(OPATH):
	$(MD) $(OPATH)
	
$(OPATH)/%.o:%.cpp $(HEAD)
	$(CC) -c -o $@ $< $(CF) -I$(INCLUDE)
	
clean:
	$(RMDIR) bin
	$(RMDIR) $(OPATH)

	