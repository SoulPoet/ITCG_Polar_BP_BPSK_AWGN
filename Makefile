CC = g++
OBJECTS = AWGN_Channel.o PolarCodec.o SourceCreate.o BPSK_Modulator.o CommonFunc.o Simulator.o main.o
PROJECT = ITCG_Polar_Project

$(PROJECT) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

main.o : main.cpp
	$(CC) -c $< -o $@

%.o : %.cpp %.hpp CommonFunc.hpp
	$(CC) -c $< -o $@

clean :
	rm -rf $(OBJECTS) $(PROJECT)