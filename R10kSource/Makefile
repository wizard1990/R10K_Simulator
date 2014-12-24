CC = g++
CXXFLAGS = -g

SOURCES = main.cpp register_manager.cpp trace_reader.cpp active_list_manager.cpp instruction_queue_manager.cpp\
		  ALU.cpp arithmetic_queue.cpp address_queue.cpp R10K_system_manager.cpp

OBJECTS = $(SOURCES:.cpp=.o)

all: exec

.PHONY: clean
clean:
	rm -rf *.o R10K_Simulator

.PHONY: exec
exec: $(OBJECTS)
	$(CC) $(CXXFLAGS) -o R10K_Simulator $(OBJECTS)

.cpp.o:
	$(CC) -c $(CXXFLAGS) $< -o $@

main.o: trace_reader.h R10K_system_manager.h

trace_reader.o: trace_reader.h global.h

active_list_manager.o: active_list_manager.h global.h

instruction_queue_manager.o: instruction_queue_manager.h arithmetic_queue.h address_queue.h

ALU.o: ALU.h arithmetic_queue.h active_list_manager.h

arithmetic_queue.o: arithmetic_queue.h instruction.h

address_queue.o: address_queue.h instruction.h active_list_manager.h

R10K_system_manager.o: R10K_system_manager.h active_list_manager.h instruction_queue_manager.h ALU_manager.h trace_reader.h

register_manager.o: register_manager.h

