/*
 * Channel_test.cpp
 *
 *  Created on: Nov 26, 2017
 *      Author: stammtisch
 */

#include "Test.h"
#include "Channel.h"

namespace logicLayer {
namespace test {


typedef Signal Message;

// test signals
Signal signals[] = {Signal(Signalname::BUTTON_START_PUSHED),
					Signal(Signalname::BUTTON_START_PULLED),
					Signal(Signalname::BUTTON_STOP_PUSHED),
					Signal(Signalname::BUTTON_STOP_PULLED),
					Signal(Signalname::BUTTON_RESET_PUSHED),
					Signal(Signalname::BUTTON_RESET_PULLED),
					Signal(Signalname::BUTTON_E_STOP_PUSHED),
					Signal(Signalname::BUTTON_E_STOP_PULLED),
					Signal(Signalname::SENSOR_TEST_START),
					Signal(Signalname::SENSOR_TEST_SUCCESSFUL)};
int signalsSize = sizeof(signals) / sizeof(Signal);

class Receiver {
public:
	explicit Receiver(Channel<Message>& c)
	: ch1(c)
	, running(true)
	{
		static int ID = 0;
		id = ++ID;
	}
	Channel<Message>& ch1;
	void operator()() {
		Signal signal;
		while(running){
			std::this_thread::sleep_for(std::chrono::seconds(1));
			signal<<ch1;//read values from channel
			cout<<"\t\t\t\treceiver "<<id<<" read from channel = "<<(int)signal.name<<endl;
		}
	}
	void terminate() { running = false; }
private:
	bool running;
	int id;
};

class Sender {
public:
	explicit Sender(Channel<Message>& c)
	: ch1(c)
	, running(true)
	{
		static int ID = 0;
		id = ++ID;
	}
	Channel<Message>& ch1;
	void operator()() {
		int i = -1;
		while(running) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			try {
				ch1<<signals[++i % signalsSize];
				cout<<"sender "<<id<<" sent to channel = "<<(int)signals[i % signalsSize].name<<endl;
			} catch (const std::range_error& e) {
				cout<<e.what()<<endl;
				running = false;
				continue;
			}
		}
	}
	void terminate() { running = false; }
private:
	bool running;
	int id;
};


class SenderReceiver {
public:
	explicit SenderReceiver(Channel<Message>& ch_sender, Channel<Message>& ch_receiver)
	: ch_sender(ch_sender)
	, ch_receiver(ch_receiver)
	, running(true)
	{
		static int ID = 0;
		id = ++ID;
	}
	Channel<Message>& ch_sender;// reference to channel
	Channel<Message>& ch_receiver;// reference to channel
	void operator()() {// this makes the class a functor. Put your event-loop in here.
		Signal signal;
		int i = -1;
		while(running) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			try {
				ch_sender<<signals[++i % signalsSize];
				cout<<"senderReceiver "<<id<<" sent to channel " << ch_sender.getId() <<" : "<<(int)signals[i % signalsSize].name<<endl;
			} catch (const std::range_error& e ) {
				cout<<e.what()<<endl;
			}
			std::this_thread::sleep_for(std::chrono::seconds(1));
			try {
				ch_sender<<signals[++i % signalsSize];
				cout<<"senderReceiver "<<id<<" sent to channel " << ch_sender.getId() <<" : "<<(int)signals[i % signalsSize].name<<endl;
			} catch (const std::range_error& e ) {
				cout<<e.what()<<endl;
			}

			signal<<ch_receiver;//read values from channel
			cout<<"\t\t\t\t\tsenderReceiver "<<id<<" read from channel " << ch_receiver.getId() <<" : "<<(int)signal.name<<endl;
		}
	}
	void terminate() { running = false; }
private:
	bool running;
	int id;
};

void Test::channelTest() {

	cout << "################ start " << __FUNCTION__ << " ####################" << endl;
	cout << "hit return to start."<<endl;
	cin.get();

	constexpr int SEC_10 = 10;
	constexpr int BUFFER_SIZE = 3;

	cout<<"## Start Channel Test with buffer size: "<<BUFFER_SIZE<<" ##"<<endl;

	{
		cout<<"one channel, one sender, one receiver"<<endl<<endl;
		Channel<Message> c(BUFFER_SIZE);
		Receiver receiver1(c);
		thread ReceiverThread1( std::ref(receiver1) );
		Sender sender1(c);
		thread SenderThread1( std::ref(sender1) );
		cout<<"### Abort in "<<SEC_10<<" seconds."<<endl;
		std::this_thread::sleep_for(std::chrono::seconds(SEC_10));
		cout<<"### Aborting."<<endl;
		sender1.terminate();
		receiver1.terminate();
		c.destroy();
		SenderThread1.join();
		ReceiverThread1.join();
		cout<<"done."<<endl<<endl;

	}

	{
		cout<<"one channel, two sender, one receiver"<<endl<<endl;
		Channel<Message> c(BUFFER_SIZE);
		Receiver receiver1(c);
		thread ReceiverThread1( std::ref(receiver1) );
		Sender sender1(c);
		thread SenderThread1( std::ref(sender1) );
		Sender sender2(c);
		thread SenderThread2( std::ref(sender2) );
		cout<<"### Abort in "<<SEC_10<<" seconds."<<endl;
		std::this_thread::sleep_for(std::chrono::seconds(SEC_10));
		cout<<"### Aborting."<<endl;
		sender1.terminate();
		sender2.terminate();
		receiver1.terminate();
		c.destroy();
		SenderThread1.join();
		SenderThread2.join();
		ReceiverThread1.join();
		cout<<"done."<<endl<<endl;
	}

	{
		cout<<"one channel, two sender, no receiver (simulating two blocked waiting sender)"<<endl<<endl;
		Channel<Message> c(BUFFER_SIZE);
		Sender sender1(c);
		thread SenderThread1( std::ref(sender1) );
		Sender sender2(c);
		thread SenderThread2( std::ref(sender2) );
		cout<<"### Abort in "<<SEC_10<<" seconds."<<endl;
		std::this_thread::sleep_for(std::chrono::seconds(SEC_10));
		cout<<"### Aborting."<<endl;
		sender1.terminate();
		sender2.terminate();
		c.destroy();
		SenderThread1.join();
		SenderThread2.join();
		cout<<"done."<<endl<<endl;
	}

	{
		cout<<"one channel, no sender, two receiver (simulating two blocked waiting receiver)"<<endl<<endl;
		Channel<Message> c(BUFFER_SIZE);
		Receiver receiver1(c);
		Receiver receiver2(c);
		thread ReceiverThread1( std::ref(receiver1) );
		thread ReceiverThread2( std::ref(receiver1) );
		cout<<"### Abort in "<<SEC_10<<" seconds."<<endl;
		std::this_thread::sleep_for(std::chrono::seconds(SEC_10));
		cout<<"### Aborting."<<endl;
		receiver1.terminate();
		receiver2.terminate();
		c.destroy();
		ReceiverThread1.join();
		ReceiverThread2.join();
		cout<<"done."<<endl<<endl;
	}

	{
		cout<<"two channel, two SenderReceiver (simulationg deadlock)"<<endl<<endl;
		Channel<Message> c1(BUFFER_SIZE);
		Channel<Message> c2(BUFFER_SIZE);
		SenderReceiver senderReceiver1(c1,c2);
		SenderReceiver senderReceiver2(c2,c1);
		thread SenderReceiverThread1( std::ref(senderReceiver1) );
		thread SenderReceiverThread2( std::ref(senderReceiver2) );
		cout<<"### Abort in "<<SEC_10<<" seconds."<<endl;
		std::this_thread::sleep_for(std::chrono::seconds(SEC_10));
		cout<<"### Aborting."<<endl;
		senderReceiver1.terminate();
		senderReceiver2.terminate();
		c1.destroy();
		c2.destroy();
		SenderReceiverThread1.join();
		SenderReceiverThread2.join();
		cout<<"done."<<endl<<endl;
	}


	cout<<"## Finish Channel Test ##"<<endl<<endl;
	nextTest(__FUNCTION__);
}

} /* namespace test */
} /* namespace logicLayer */
