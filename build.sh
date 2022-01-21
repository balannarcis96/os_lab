#install gcc
sudo apt-get install gcc

#install cmake
sudo apt-get install cmake

#install make
sudo apt-get install make

rm -f -r ./build
mkdir build
cd build
cmake ../source
make
