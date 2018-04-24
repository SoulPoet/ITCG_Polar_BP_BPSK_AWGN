CC = g++
OBJECTS = AWGN_Channel.o PolarCodec.o SourceCreate.o BPSK_Modulator.o CommonFunc.o Simulator.o main.o
PROJECT = ITCG_Polar_Project
COMMONHEADERFILE = CommonFunc.hpp

$(PROJECT) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

%.o : %.cpp %.hpp $(COMMONHEADERFILE)
	$(CC) -c $< -o $@

clean :
	rm -rf $(OBJECTS) $(PROJECT)