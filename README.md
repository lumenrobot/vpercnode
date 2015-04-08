# vpercnode
Virtual Perception C++ based Node that can receive and send commands to Lumen distributed system via RabbitMQ

## Prepare: Windows

Butuh:

1. [CMake 2.8.12+](http://www.cmake.org/)
2. [BOOST 1.54+ - Win64 binaries](http://www.boost.org/)
3. Visual Studio 2013 (bisa Community Edition)
4. [OpenSSL 1.0.2a+](http://slproweb.com/products/Win32OpenSSL.html).
    Ada 2 yang dibutuhkan: "Visual C++ 2008 Redistributables (x64)" + "Win64 OpenSSL v1.0.2a"".
5. [rabbitmq-c 0.6.0](https://github.com/alanxz/rabbitmq-c)

    Use Visual Studio Tools > VS2013 x64 Native Tools Command Prompt:

```bash
cd git\rabbitmq-c
mkdir build
cd build
cmake -G "Visual Studio 12 2013 Win64" -DBUILD_STATIC_LIBS=OFF -DBUILD_TESTS=OFF ..
msbuild rabbitmq-c.sln /p:Configuration=Debug
```

6. [SimpleAmqpClient](https://github.com/alanxz/SimpleAmqpClient)
    (https://github.com/alanxz/SimpleAmqpClient/pull/98 required if not yet merged)

```bash
cd git\SimpleAmqpClient
mkdir build
cd build
cmake -G "Visual Studio 12 2013 Win64" -DBOOST_INCLUDEDIR=C:/local/boost_1_57_0 -DBOOST_LIBRARYDIR=C:/local/boost_1_57_0/lib64-msvc-12.0 -DRabbitmqc_INCLUDE_DIR=../../rabbitmq-c/librabbitmq -DRabbitmqc_LIBRARY=../../rabbitmq-c/build/librabbitmq/Debug/rabbitmq.4.lib ..
msbuild SimpleAmqpClient.sln /p:Configuration=Debug
```

Generate Visual Studio 2013 solution for `vpercnode` inside `build` directory:

```bash
mkdir build
cd build
cmake -G "Visual Studio 12 2013 Win64" -DBOOST_INCLUDEDIR=C:\local\boost_1_57_0 -DBOOST_LIBRARYDIR=C:\local\boost_1_57_0\lib64-msvc-12.0 ..
msbuild vpercnode.sln /p:Configuration=Debug
```
	
## Prepare: Linux

Butuh: Linux Mint 17.1 KDE 64bit

1. [CMake 2.8.12+](http://www.cmake.org/)
2. [BOOST 1.54+](http://www.boost.org/)
3. gcc 4.8.2+ (Linux)
4. [rabbitmq-c 0.6.0](https://github.com/alanxz/rabbitmq-c)
5. [SimpleAmqpClient](https://github.com/alanxz/SimpleAmqpClient)

	```bash
	mkdir ~/tmp/SimpleAmqpClient-2.4.0/build
	cd ~/tmp/SimpleAmqpClient-2.4.0/build
	cmake -D Rabbitmqc_INCLUDE_DIR=~/tmp/rabbitmq-c-0.6.0/librabbitmq/ -D Rabbitmqc_LIBRARY=~/tmp/rabbitmq-c-0.6.0/build/librabbitmq/librabbitmq.so ..
	make -j8
	```

```bash
sudo apt-get install build-essential cmake libboost-all-dev
```

## Generate Eclipse CDT Project

Use in-source build to make it easy:

```bash
cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug .
```

Troubleshooting: [Cannot find `string/iostream` in Eclipse](http://stackoverflow.com/a/21469917), but `make` works:
Go to Project > Properties > C/C++ General > Preprocessor Includes... > Providers
and select "CDT GCC Built-in Compiler Settings"

Why this is not set automatically by CMake? http://answers.ros.org/question/61407/standard-system-include-paths-missing-in-eclipse-from-catkin_make/?answer=61498#post-id-61498
