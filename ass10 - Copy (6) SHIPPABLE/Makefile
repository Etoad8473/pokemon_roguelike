SRCS := 0_start.cpp 0_printing.cpp 1_mapWorldGeneration.cpp \
        2_worldMapRunner.cpp 3_heapDijkstra.cpp 4_characters.cpp \
        5_ncurses.cpp 7_pokemonCSV.cpp 8_pokeMechanics.cpp \
        9_battles.cpp 10_raytracing.cpp

OBJS := $(SRCS:.cpp=.o)

poke: $(OBJS)
	g++ -g $^ -lncurses -o $@

%.o: %.cpp
	g++ -g -c $< -o $@

clean:
	rm -f $(OBJS) poke