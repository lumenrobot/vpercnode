#include <string>
#include <iostream>
#include <iomanip>

#include <boost/format.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <JointInterpolateAngle.hpp>
#include <MoveTo.hpp>

#include "SimpleAmqpClient/SimpleAmqpClient.h"

using namespace std;
using namespace AmqpClient;
using namespace lumen;

int main(int argc, char **argv) {
	cout << "Hello" << "World" << endl;

	using boost::property_tree::ptree;
	ptree pt;
	pt.put("Halo", "juga");
	stringstream ss;
	boost::property_tree::json_parser::write_json(ss, pt, true);
	string jsonStr = ss.str();
	cout << "JSON: " << jsonStr << endl;

	cout << "Connecting..." << "\n";
	Channel::ptr_t channel = Channel::CreateFromUri("amqp://localhost");

	string queue = channel->DeclareQueue("");
	cout << "Publishing to '" << queue << "'..." << "\n";
	BasicMessage::ptr_t message = BasicMessage::Create("wah keren");
	channel->BasicPublish("", queue, message);

	string avatarCommandQueue;
	channel->DeclareQueue(avatarCommandQueue);
	cout << "Declared queue '" << avatarCommandQueue << "'" << endl;
	string topic = "amq.topic";
	string avatarCommandKey = "avatar.NAO.command";
	channel->BindQueue(avatarCommandQueue, topic, avatarCommandKey);
	string consumer_tag = channel->BasicConsume(avatarCommandQueue, "");

	while (true) {
		cout << boost::format("Consuming %1% (topic=%2% routing=%3%) ... (press Ctrl+C to exit)")
			% consumer_tag % topic % avatarCommandKey << endl;
		Envelope::ptr_t envelope;
		if (channel->BasicConsumeMessage(consumer_tag, envelope, 1000)) {
			BasicMessage::ptr_t msg = envelope->Message();
			cout << boost::format("Got message: id=%1% appId=%2% correlationId=%3% contentType=%4% replyTo=%5% body=%6%") %
					msg->MessageId() % msg->AppId() % msg->CorrelationId() % msg->ContentType() % msg->ReplyTo() % msg->Body() << endl;
			for (auto& entry: msg->HeaderTable()) {
				cout << boost::format("%1% = %2% (%3%)") % entry.first % entry.second.GetString() % entry.second.GetType() << endl;
			}
			ptree inpt;
			istringstream bodyStream(msg->Body());
			boost::property_tree::read_json(bodyStream, inpt);
			for (auto& entry: inpt) {
				cout << boost::format("%1% = %2%") % entry.first % entry.second.get_value("") << endl;
			}

			string clazz = inpt.get("@type", "");
			if (clazz == "MoveTo") {
				MoveTo moveTo = MoveTo(inpt);
				cout << "MoveTo object: " << moveTo.str() << endl;
			} else if (clazz == "JointInterpolateAngle") {
				JointInterpolateAngle obj = JointInterpolateAngle(inpt);
				cout << "Message object: " << obj.str() << endl;
			}

			channel->BasicAck(envelope);
			break;
		} else {
			cout << "No message" << endl;
		}
	}

	cout << "Done" << endl;
}
