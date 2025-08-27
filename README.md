If you use Linux, install SFML's dependencies using your system package manager.  
On Ubuntu and other Debian-based distributions you can use the following commands:

```bash
sudo apt update

sudo apt install \
    libxrandr-dev \
    libxcursor-dev \
    libudev-dev \
    libfreetype-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev \
    libfreetype-dev


Configure and build your project. Most popular IDEs support CMake projects with very little effort on your part.
	•	VS Code via the CMake extension
	•	Visual Studio
	•	CLion
	•	Qt Creator

Using CMake from the command line is straightforward as well. Be sure to run these commands in the root
directory of the project you just created.

cmake -B build
cmake --build build
```
