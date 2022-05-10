CC = g++
CFLAGS  = -g -pg -Wall -pedantic -std=c++17 -O3

all: exec

exec: main.o SAP.o Cun86.o CLSSW_Indep.o CLSSW_Approx.o CLSSW_Rank.o gen.o utils.o EdmondsKarp.o HopcroftKarp.o ds/LinkCutTree.o oracles/RightMatching.o oracles/LeftMatching.o oracles/Uniform.o oracles/Graphic.o
	$(CC) $(CFLAGS) -o exec main.o SAP.o Cun86.o CLSSW_Indep.o CLSSW_Approx.o CLSSW_Rank.o gen.o utils.o EdmondsKarp.o HopcroftKarp.o LinkCutTree.o RightMatching.o LeftMatching.o Uniform.o Graphic.o

main.o: main.cpp SAP.h Cun86.h CLSSW_Indep.h CLSSW_Approx.h CLSSW_Rank.h gen.h EdmondsKarp.h HopcroftKarp.h
	$(CC) $(CFLAGS) -c main.cpp

SAP.o:  SAP.cpp SAP.h utils.h oracles/Base.h
	$(CC) $(CFLAGS) -c SAP.cpp
	
Cun86.o:  Cun86.cpp Cun86.h utils.h oracles/Base.h
	$(CC) $(CFLAGS) -c Cun86.cpp

CLSSW_Indep.o:  CLSSW_Indep.cpp CLSSW_Indep.h utils.h oracles/Base.h
	$(CC) $(CFLAGS) -c CLSSW_Indep.cpp

CLSSW_Approx.o:  CLSSW_Approx.cpp CLSSW_Approx.h utils.h oracles/Base.h
	$(CC) $(CFLAGS) -c CLSSW_Approx.cpp

CLSSW_Rank.o:  CLSSW_Rank.cpp CLSSW_Rank.h utils.h oracles/Base.h
	$(CC) $(CFLAGS) -c CLSSW_Rank.cpp

gen.o:  gen.cpp gen.h ds/Edge.h
	$(CC) $(CFLAGS) -c gen.cpp
	
utils.o:  utils.cpp utils.h 
	$(CC) $(CFLAGS) -c utils.cpp

EdmondsKarp.o:  EdmondsKarp.cpp EdmondsKarp.h 
	$(CC) $(CFLAGS) -c EdmondsKarp.cpp
	
HopcroftKarp.o:  HopcroftKarp.cpp HopcroftKarp.h 
	$(CC) $(CFLAGS) -c HopcroftKarp.cpp

ds/LinkCutTree.o:  ds/LinkCutTree.c ds/LinkCutTree.h
	gcc -g -c -pg 				 ds/LinkCutTree.c

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