This is a CPP implementation of the simulator. 

# Build
This code using CMake to build the project and it was tested with Visual Studio Community Edition 2022.

To build this simulator from source, the folloing applications and libraries should be installed

1. [CMake](https://cmake.org/download/) (Versin 3.24 of higher)
1. Visual Studio 2022
1. [vcpkg](https://vcpkg.io/en/)
1. [Jimatech Library](https://github.com/mans-ahmadian/JimatechLibrary)

after installing these applications/libraries, set the following two enviroment variables:

__VCPKG_ROOT__ : point to the root folder of where the vcpkg is installed.    
__JIMATECH_LIB__ : set the root folder of where jimatech library is installed. Please follow the instruction in the readme file of this library to properly setup your environment. 

Then run __build.run__ and wait it finishes building OpenCV and building the project in the build directory.Open *.sln on build directory and compile and run the application. 