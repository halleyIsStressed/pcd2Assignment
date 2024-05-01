# PCD2 Assignment
This is a step-by-step guide on installing the prerequisites to be able to run this code. They are as follows:

- Git
- libcurl
- vcpkg

Also, this guide assumes you are using Windows instead of Linux.

## Guide
1. [Install Git](https://git-scm.com/download/win). Go with the 64-bit version.
2. Install `vcpkg`. Run the below commands on cmd:
    ```cmd
    git clone https://github.com/microsoft/vcpkg
    cd vcpkg
    bootstrap-vcpkg.bat
    ```
3. Install `libcurl`. Run the below command on cmd:
    ```cmd
    vcpkg install curl:x64-windows
    ```
4. Integrate `libcurl` into Visual Studio so that we may use the library.
    ```cmd
    vcpkg integrate install
    ```
You should be able to run the program now.