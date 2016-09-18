OBJ = gram.tab.o lex.yy.o Chunk.o action.o main.o

VPATH = ../src
CPLUS_FLAGS = -g
CPLUS = g++ -std=c++11
ZAPIF = zapif

test: $(ZAPIF)
	(../test/run_tests.sh)

$(ZAPIF): $(OBJ)
	$(CPLUS) $(OBJ) -o $@
 
gram.tab.cpp gram.tab.h: gram.ypp action.h
	bison -r all -d $<

lex.yy.c: token.l gram.tab.h 
	flex $<

%.o: %.cpp
	$(CPLUS) -c $(CPLUS_FLAGS) -I.. $<

clean:
	rm -f gram.tab.cpp gram.tab.hpp lex.yy.c *.o $(ZAPIF) gram.output tmp.out

# Count source lines -- requires cloc (https://github.com/AlDanial/cloc)
count:
	cloc ../src/*

*.o: Makefile action.h Chunk.h
