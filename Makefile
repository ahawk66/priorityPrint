all: sim

sim: simulator.o sim.cpp testSim.cpp
  g++ simulator.o sim.cpp testSim.cpp

simulator.o: simulator.cpp
  g++ -c simulator.cpp

sim.o: sim.cpp
  g++ -c sim.cpp
  
testSim.o: testSim.cpp
  g++ -c testSim.cpp
