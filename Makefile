SRC=$(filter-out %main.cpp, src/*, Headers/* )

OBJ=claim.o edge.o grid.o netlist.o node.o path.o pathsegment.o problem_object.o segmentgroup.o grnode.o grgrid.o grnetlist.o grbaserouter.o soukups_gr_one.o soukups_gr_two.o soukups_executor.o main.o json_name_value.o json_file.o json_value.o json_object.o json_parser.o json_parser_bison.o json_parser_lex.o 

OBJ1=claim.o edge.o grid.o netlist.o node.o path.o pathsegment.o problem_object.o segmentgroup.o grnode.o grgrid.o grnetlist.o grbaserouter.o soukups_gr_one.o soukups_gr_two.o soukups_executor.o maingr2.o json_name_value.o json_file.o json_value.o json_object.o json_parser.o json_parser_bison.o json_parser_lex.o 

OBJ2=claim.o edge.o grid.o netlist.o node.o path.o pathsegment.o problem_object.o segmentgroup.o grnode.o grgrid.o grnetlist.o grbaserouter.o soukups_gr_one.o soukups_gr_two.o soukups_executor.o maingr22.o json_name_value.o json_file.o json_value.o json_object.o json_parser.o json_parser_bison.o json_parser_lex.o 
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

vpath %.cc Source/

gr1: $(OBJ) 
	$(CC) $(LFLAGS) -o routergr1 $(OBJ)

gr2: $(OBJ1) 
	$(CC) $(LFLAGS) -o routergr2 $(OBJ1)

gr22: $(OBJ2) 
	$(CC) $(LFLAGS) -o routergr22 $(OBJ2)

claim.o : Headers/claim.h src/claim.cpp 
	$(CC) $(CFLAGS)  src/claim.cpp

main.o : main.cpp 
	$(CC) $(CFLAGS)  main.cpp 

maingr2.o : maingr2.cpp 
	$(CC) $(CFLAGS)  maingr2.cpp 

maingr22.o : maingr22.cpp 
	$(CC) $(CFLAGS)  maingr22.cpp 

edge.o : Headers/edge.h src/edge.cpp 
	$(CC) $(CFLAGS)  src/edge.cpp

grid.o : Headers/grid.h src/grid.cpp
	$(CC) $(CFLAGS) src/grid.cpp

grgrid.o : Headers/grgrid.h src/grgrid.cpp
	$(CC) $(CFLAGS) src/grgrid.cpp

netlist.o : Headers/netlist.h src/netlist.cpp
	$(CC) $(CFLAGS) src/netlist.cpp

node.o : Headers/node.h src/node.cpp
	$(CC) $(CFLAGS) src/node.cpp

grnode.o : Headers/grnode.h src/grnode.cpp
	$(CC) $(CFLAGS) src/grnode.cpp

path.o : Headers/path.h src/path.cpp
	$(CC) $(CFLAGS) src/path.cpp

pathsegment.o : Headers/pathsegment.h src/pathsegment.cpp
	$(CC) $(CFLAGS) src/pathsegment.cpp

problem_object.o : Headers/problem_object.h src/problem_object.cpp 
	$(CC) $(CFLAGS) src/problem_object.cpp 

segmentgroup.o : Headers/segmentgroup.h src/segmentgroup.cpp
	$(CC) $(CFLAGS) src/segmentgroup.cpp 

grnetlist.o : Headers/grnetlist.h src/grnetlist.cpp
	$(CC) $(CFLAGS) src/grnetlist.cpp

grbaserouter.o : Headers/grbaserouter.h src/grbaserouter.cpp
	$(CC) $(CFLAGS) src/grbaserouter.cpp

soukups_gr_one.o : Headers/soukups_gr_one.h src/soukups_gr_one.cpp
	$(CC) $(CFLAGS) src/soukups_gr_one.cpp

soukups_gr_two.o : Headers/soukups_gr_two.h src/soukups_gr_two.cpp
	$(CC) $(CFLAGS) src/soukups_gr_two.cpp

soukups_executor.o : Headers/soukups_executor.h src/soukups_executor.cpp
	$(CC) $(CFLAGS) src/soukups_executor.cpp

# ------------------------ JSON FILES --------------------------------------

json_name_value.o : Utilities/JSON_parser/Headers/json_name_value.h Utilities/JSON_parser/Source/json_name_value.cpp
	$(CC) $(CFLAGS) Utilities/JSON_parser/Source/json_name_value.cpp

json_file.o : Utilities/JSON_parser/json_file.h Utilities/JSON_parser/Source/json_file.cpp
	$(CC) $(CFLAGS) Utilities/JSON_parser/Source/json_file.cpp

json_value.o : Utilities/JSON_parser/Headers/json_value.h Utilities/JSON_parser/Source/json_value.cpp
	$(CC) $(CFLAGS) Utilities/JSON_parser/Source/json_value.cpp

json_object.o : Utilities/JSON_parser/Headers/json_object.h Utilities/JSON_parser/Source/json_object.cpp
	$(CC) $(CFLAGS) Utilities/JSON_parser/Source/json_object.cpp

json_parser.o : Utilities/JSON_parser/Headers/json_parser.h Utilities/JSON_parser/Source/json_parser.cpp
	$(CC) $(CFLAGS) Utilities/JSON_parser/Source/json_parser.cpp

json_parser_bison.o : Utilities/JSON_parser/json_parser_bison.h Utilities/JSON_parser/json_parser_bison.cpp
	$(CC) $(CFLAGS) Utilities/JSON_parser/json_parser_bison.cpp

json_parser_lex.o : Utilities/JSON_parser/json_parser_lex.cc
	$(CC) $(CFLAGS) Utilities/JSON_parser/json_parser_lex.cc

test: all
	./grid_router Tests/test_sample.json
	
%.o: %.cc
	g++ -c $

cleanup:
	rm -f *.o

clean: cleanup
	rm -f grid_router
