#include <string>
#include <iostream>
#include <cmath>
//#include <thread>
//#include <chrono>

#include <boost/format.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/thread/thread.hpp>
#include <JointInterpolateAngle.hpp>
#include <MoveTo.hpp>
#include <Avatar.hpp>

#include "SimpleAmqpClient/SimpleAmqpClient.h"

using namespace std;
using namespace boost::numeric;
using namespace AmqpClient;
using namespace lumen;

// Position animation
ublas::vector<double> positionDelta(3);
/**
 * Intended duration of the animation.
 */
double positionDuration(0.0);
/**
 * Intended turn (CCW, degrees) of the position animation.
 */
double positionTurn(0.0);
/**
 * Remaining progress towards end of animation, from 1.0 to 0.0.
 * if > 0 then this will be animated.
 */
double positionProgressRemaining(0.0);

void onUpdate(double deltaTime/*seconds*/, boost::shared_ptr<Avatar> avatar) {
	if (positionProgressRemaining > 0.0) {
		double deltaProgress = min(deltaTime / positionDuration, positionProgressRemaining);
		ublas::vector<double> newPos(avatar->getTilePosition());
		newPos[0] += positionDelta[0] * deltaProgress;
		newPos[1] += positionDelta[1] * deltaProgress;
		newPos[2] += positionDelta[2] * deltaProgress;
		avatar->setTilePosition(newPos);
		double deltaTurn = deltaProgress * positionTurn;
		ublas::vector<double> newRot(avatar->getRotation());
		newRot[1] += deltaTurn; // yaw CCW
		if (newRot[1] < -180.0) {
			newRot[1] += 360.0;
		}
		if (newRot[1] > 180.0) {
			newRot[1] -= 360;
		}
		avatar->setRotation(newRot);
		positionProgressRemaining = max(positionProgressRemaining - deltaProgress, 0.0);
	}
}

int main(int argc, char **argv) {
	cout << "Hello" << "World" << endl;

	using boost::property_tree::ptree;
	ptree pt;
	pt.put("Halo", "juga");
	stringstream ss;
	boost::property_tree::json_parser::write_json(ss, pt, true);
	string jsonStr = ss.str();
	cout << "JSON: " << jsonStr << endl;

	boost::shared_ptr<Avatar> avatar(new Avatar());

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
	cout << boost::format("Consuming %1% (topic=%2% routing=%3%) ... (press Ctrl+C to exit)")
		% consumer_tag % topic % avatarCommandKey << endl;

	double gameTime = 0.0; // seconds
	while (true) {
		cout << avatar->str() << endl;
		Envelope::ptr_t envelope(NULL);
		if (channel->BasicConsumeMessage(consumer_tag, envelope, 0)) {
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

				positionDelta[0] = moveTo.getRightDistance(); // right is x+ axis
				positionDelta[1] = 0;
				positionDelta[2] = moveTo.getBackDistance(); // back is z+ axis
				const double moveSpeed = 0.1; // 0.1 m/s
				positionDuration = std::abs(moveTo.getRightDistance()) + std::abs(moveTo.getBackDistance()) / moveSpeed;
				positionTurn = moveTo.getTurnCcwDeg();
				positionProgressRemaining = 1.0;

//				boost::numeric::ublas::vector<double> newPos(avatar->getTilePosition());
//				newPos[0] += moveTo.getRightDistance(); // right is x+ axis
//				newPos[2] += moveTo.getBackDistance(); // back is z+ axis
//				avatar->setTilePosition(newPos);
			} else if (clazz == "JointInterpolateAngle") {
				JointInterpolateAngle obj = JointInterpolateAngle(inpt);
				cout << "Message object: " << obj.str() << endl;
				if (obj.getJointId() == "HeadYaw") {
					avatar->setHeadYaw(obj.getTargetCcwDeg());
				} else if (obj.getJointId() == "HeadPitch") {
					avatar->setHeadPitch(obj.getTargetCcwDeg());
				}
			}

			//channel->BasicAck(envelope);
		} else {
			//cout << "No message" << endl;
		}

		// Do the time update
		const double deltaTime = 1.0 / 5.0; // 5 fps, because just a simulation
		boost::this_thread::sleep(boost::posix_time::milliseconds(deltaTime*1000));
		//std::this_thread::sleep_for(deltaTime * 1000);
		gameTime += deltaTime;
		onUpdate(deltaTime, avatar);
	}

	cout << "Done" << endl;
}
