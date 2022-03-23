apt-get -qq -y install cmake
apt-get -qq -y install libpython2.7-dev
apt-get -qq -y install libblas-dev liblapack-dev libglew-dev
apt-get -qq -y install build-essential
apt-get -qq -y install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev  libswscale-dev sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libdc1394-22-dev
apt-get -qq -y install libeigen3-dev
apt -qq -y install snapd
snap install node --channel=14/stable --classic
cd ~
git clone https://github.com/opencv/opencv.git
cd ~/opencv
mkdir release
cd release
cmake -DCMAKE_BUILD_TYPE=RELEASE –DCMAKE_INSTALL_PREFIX=/usr/local ..
make
make install
cd ~
git clone https://github.com/dorian3d/DBoW2.git
cd DBoW2
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
cd ~
cd DBoW2
git clone https://github.com/Emz-programmer/VCL2022MUFYP.git
cd VCL2022MUFYP
cp -R ./build ../
cp ./CMakeLists.txt ../
cp -R ./VCL ../
cp -R ./images ../
cd ..
cd build
cmake ..
make
cd ~
cd DBoW2/VCL/ReactFrontend/vcl_frontend
npm install