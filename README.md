# vpercnode
Virtual Perception C++ based Node that can receive and send commands to Lumen distributed system via RabbitMQ

## Prepare: General

Butuh:

1. CMake 
2. BOOST 1.54
3. gcc 4.8.2+ (Linux)
4. [rabbitmq-c 0.6.0](https://github.com/alanxz/rabbitmq-c)
5. [SimpleAmqpClient](https://github.com/alanxz/SimpleAmqpClient)

	mkdir ~/tmp/SimpleAmqpClient-2.4.0/build
	cd ~/tmp/SimpleAmqpClient-2.4.0/build
	cmake -D Rabbitmqc_INCLUDE_DIR=~/tmp/rabbitmq-c-0.6.0/librabbitmq/ -D Rabbitmqc_LIBRARY=~/tmp/rabbitmq-c-0.6.0/build/librabbitmq/librabbitmq.so ..
	make -j8

## Prepare: Linux

```
sudo apt-get install build-essential cmake libboost-all-dev
```

## Generate Eclipse Project

Use in-source build to make it easy:

```
cmake -G"Eclipse CDT4 - Unix Makefiles" -D CMAKE_BUILD_TYPE=Debug .
```

