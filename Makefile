
SRC=$(filter-out %main.cpp, Source/*)
OBJ=claim.o edge.o grbaserouter.o grgrid.o grid.o grnetList.o grnode.o grrouterone.o grroutertwo.o leerouter.o netlist.o node.o path.o pathsegment.o problem_object.o segmentgroup.o 

vpath %.cpp Source/

all: $(OBJ) main.cpp
	g++ -o grid_router $^ Utilities/JSON_parser/json_parser.so 

test: all
	./grid_router Tests/test_sample.json
	
%.o: %.cpp
	g++ -c $^

cleanup:
	rm -f *.o

clean: cleanup
	rm -f grid_router

