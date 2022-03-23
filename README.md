# VCL2022MUFYP
The visual campus locator is a place recognition tool designed for use on the Maynooth University campus.

## Installation
DBoW2 Requires OpenCV to run and compile. Installation will take approx 1hr 30mins. 
### OpenCV
#### Install Dependencies
```
sudo apt-get install cmake
sudo apt-get install libpython2.7-dev
sudo apt-get install libblas-dev liblapack-dev libglew-dev
sudo apt-get install build-essential
sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev  libswscale-dev sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libdc1394-22-dev
sudo apt-get install libeigen3-dev

```
#### Compile OpenCV
```
git clone https://github.com/opencv/opencv.git
cd ~/opencv
mkdir release
cd release
cmake -DCMAKE_BUILD_TYPE=RELEASE –DCMAKE_INSTALL_PREFIX=/usr/local ..
make
sudo make install
```
### DBoW2
#### Compile DBoW2
```
git clone https://github.com/dorian3d/DBoW2.git
cd DBoW2
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```
### VCL
```
cd ~
cd DBoW2
git clone https://github.com/Emz-programmer/VCL2022MUFYP.git
cd VCL2022MUFYP
```
