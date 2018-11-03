sudo apt-get install libsfml-dev
g++ -c *cpp
g++ *.o -o bridge.out -lsfml-graphics -lsfml-window -lsfml-system
./bridge.out
