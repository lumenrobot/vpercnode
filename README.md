# vpercnode
Virtual Perception C++ based Node that can receive and send commands to Lumen distributed system via RabbitMQ

## Prepare: General

Butuh:

1. CMake 2.8.12+
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

## Prepare: Linux

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
