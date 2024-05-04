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

## Note
In the case of trouble opening files, you may try manually copying the files.
1. Copy the contents of a file, and delete the whole file from the folder.
2. Add a new item within the folder, name it the same as the deleted file.
3. Paste the contents inside.
4. Do this for every file.

We really have no explanation as to why this is happening, but it seems to be the most effective fix.
