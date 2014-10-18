objs = bin/Map.o bin/MiscUtil.o bin/Voronoi.o bin/Alphabeta.o bin/Timer.o bin/IterativeDeepening.o
CFLAGS = -O2 
#CCFLAGS = -g -D DEBUG
#CCFLAGS = -O2 -D TIMER

all: bin/IterAlphabetaBot

bin/IterAlphabetaBot: bin/IterAlphabetaBot.o $(objs)
	g++ $(CCFLAGS) -o bin/IterAlphabetaBot bin/IterAlphabetaBot.o $(objs)

bin/IterAlphabetaBot.o: src/IterAlphabetaBot.cc
	g++ $(CCFLAGS) -c -o bin/IterAlphabetaBot.o src/IterAlphabetaBot.cc

bin/Map.o: src/Map.cc
	g++ $(CCFLAGS) -c -o bin/Map.o src/Map.cc

bin/MiscUtil.o: src/MiscUtil.cc
	g++ $(CCFLAGS) -c -o bin/MiscUtil.o src/MiscUtil.cc

bin/Timer.o: src/Timer.cc
	g++ $(CCFLAGS) -c -o bin/Timer.o src/Timer.cc

bin/IterativeDeepening.o: src/IterativeDeepening.cc
	g++ $(CCFLAGS) -c -o bin/IterativeDeepening.o src/IterativeDeepening.cc

bin/Voronoi.o: src/Voronoi.cc
	g++ $(CCFLAGS) -c -o bin/Voronoi.o src/Voronoi.cc

bin/Alphabeta.o: src/Alphabeta.cc
	g++ $(CCFLAGS) -c -o bin/Alphabeta.o src/Alphabeta.cc

#bin/%.o: src/%.c
#	g++ -o $@ -c $< $(CFLAGS)
clean:
	rm bin/*
