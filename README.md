# VCL2022MUFYP
The visual campus locator is a place recognition tool designed for use on the Maynooth University campus. The git repo contains downscaled images, however it does contain the full resolution database. Requires NodeJS 14.12.
## Automated Installation
Compatible with Ubuntu 20.04. Installation will take approx 1hr 30mins depending on system performance and network speeds.  
```
git clone git clone https://github.com/Emz-programmer/VCL2022MUFYP.git
cd VCL2022MUFYP
chmod + x ./setup.sh
sudo -E ./setup.sh

```
## Manual Installation
Compatible with Ubuntu 20.04. Compatibility with other OS's may vary. DBoW2 Requires OpenCV to run and compile. Installation will take approx 1hr 30mins depending on system performance and network speeds.  
### OpenCV
#### Install Dependencies
```
sudo apt-get install cmake
sudo apt-get install libpython2.7-dev
sudo apt-get install libblas-dev liblapack-dev libglew-dev
sudo apt-get install build-essential
sudo apt-get install librestbed-dev
sudo apt install sqlite3
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
```
## Usage
### Running the VCL
VCL consists of 2 main programs: **VCL** and **BuildDB**. A prebuild image database is provided in the repo. **WARNING: BuildDB program will overwrite vcl_db.yml.gz and vcl_voc.yml.gz!** To run the VCL api:
```
cd DBoW2/build
./VCL
```
The VCL will now listen on localhost:2022 for requests. Ensure NodeJS 14.12 is installed along with Axios. To initialise the frontend components:
```
cd DBoW2/VCL/ReactFrontend/vcl_frontend
npm start
```
### External Devices
The vcl is configured to run on localhost:3000. Nginx (or similar) is required to enable external devices to connect. The app.js (VCL/ReactFrontend/vcl_frontend/src/app.js) code will need to be modified as follows:
```
const api = axios.create({
    baseURL: 'http://<ip address>:2022/2022fypVCL'

});

```