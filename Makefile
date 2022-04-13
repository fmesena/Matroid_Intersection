CC = g++
CFLAGS  = -g -Wall -pedantic -std=c++17

all: exec

exec: main.o SAP.o Cun86.o Indep.o Rank.o gen.o utils.o Bipartite_Matching.o ds/LinkCutTree.o oracles/RightMatching.o oracles/LeftMatching.o oracles/Uniform.o oracles/Graphic.o
	$(CC) $(CFLAGS) main.o SAP.o Cun86.o Indep.o Rank.o gen.o utils.o Bipartite_Matching.o LinkCutTree.o RightMatching.o LeftMatching.o Uniform.o Graphic.o -o exec

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

SAP.o:  SAP.cpp SAP.h
	$(CC) $(CFLAGS) -c SAP.cpp
	
Cun86.o:  Cun86.cpp Cun86.h utils.h oracles/Base.h
	$(CC) $(CFLAGS) -c Cun86.cpp

Indep.o:  Indep.cpp Indep.h utils.h oracles/Base.h
	$(CC) $(CFLAGS) -c Indep.cpp

Rank.o:  Rank.cpp Rank.h utils.h oracles/Base.h
	$(CC) $(CFLAGS) -c Rank.cpp

gen.o:  gen.cpp gen.h ds/Edge.h
	$(CC) $(CFLAGS) -c gen.cpp

ds/LinkCutTree.o:  ds/LinkCutTree.c ds/LinkCutTree.h ds/Edge.h
	gcc -g -c ds/LinkCutTree.c
	
utils.o:  utils.cpp utils.h 
	$(CC) $(CFLAGS) -c utils.cpp

BipartiteMatching.o:  BipartiteMatching.cpp BipartiteMatching.h 
	$(CC) $(CFLAGS) -c BipartiteMatching.cpp

oracles/RightMatching.o:  oracles/RightMatching.cpp oracles/RightMatching.h oracles/Base.h ds/Edge.h
	$(CC) $(CFLAGS) -c oracles/RightMatching.cpp

oracles/LeftMatching.o:  oracles/LeftMatching.cpp oracles/LeftMatching.h oracles/Base.h ds/Edge.h
	$(CC) $(CFLAGS) -c oracles/LeftMatching.cpp

oracles/Graphic.o:  oracles/Graphic.cpp oracles/Graphic.h oracles/Base.h ds/Edge.h
	$(CC) $(CFLAGS) -c oracles/Graphic.cpp

oracles/Uniform.o:  oracles/Uniform.cpp oracles/Uniform.h oracles/Base.h
	$(CC) $(CFLAGS) -c oracles/Uniform.cpp

run:
	./exec
	
clean:
	rm -f *.o exec