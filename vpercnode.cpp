#include <string>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "SimpleAmqpClient/SimpleAmqpClient.h"

using namespace std;
using namespace AmqpClient;

int main(int argc, char **argv) {
	cout << "Hello" << "World" << "\n";

	using boost::property_tree::ptree;
	ptree pt;
	pt.put("Halo", "juga");
	stringstream ss;
	boost::property_tree::json_parser::write_json(ss, pt, true);
	string jsonStr = ss.str();
	cout << "JSON: " << jsonStr << "\n";

	cout << "Connecting..." << "\n";
	Channel::ptr_t channel = Channel::CreateFromUri("amqp://localhost");
	string queue = channel->DeclareQueue("");
	cout << "Publishing to '" << queue << "'..." << "\n";
	BasicMessage::ptr_t message = BasicMessage::Create("wah keren");
	channel->BasicPublish("", queue, message);
	cout << "OK" << "\n";
}
