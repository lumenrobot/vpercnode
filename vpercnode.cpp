#include <string>
#include <iostream>
#include "SimpleAmqpClient/SimpleAmqpClient.h"

using namespace std;
using namespace AmqpClient;

int main(int argc, char **argv) {
	cout << "Hello" << "World" << "\n";
	cout << "Connecting..." << "\n";
	Channel::ptr_t channel = Channel::CreateFromUri("amqp://localhost");
	string queue = channel->DeclareQueue("");
	cout << "Publishing to '" << queue << "'..." << "\n";
	BasicMessage::ptr_t message = BasicMessage::Create("wah keren");
	channel->BasicPublish("", queue, message);
	cout << "OK" << "\n";
}
