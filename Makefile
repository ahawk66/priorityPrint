all: make

make: simulator.o sim.cpp testSim.cpp
  g++ simulator.o sim.cpp testSim.cpp

simulator.o
  g++ -c simulator.cpp

sim.cpp
  g++ -c sim.cpp
  
testSim.o
  g++ -c testSim.cpp
