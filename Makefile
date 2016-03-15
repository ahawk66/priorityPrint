all: sim

sim: simulator.o sim.cpp printSim.cpp
  g++ simulator.o sim.cpp printSim.cpp

simulator.o: simulator.cpp
  g++ -c simulator.cpp

sim.o: sim.cpp
  g++ -c sim.cpp
  
printSim.o: testSim.cpp
  g++ -c printSim.cpp
