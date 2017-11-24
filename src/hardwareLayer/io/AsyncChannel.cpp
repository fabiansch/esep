/*
 * AsyncChannel.cpp
 *
 *  Created on: 20.10.2017
 *      Author: abj240
 */

#include "AsyncChannel.h"
#include "Header.h"

#include <sys/neutrino.h>

using namespace std;

namespace hardwareLayer {
namespace io {

AsyncMsg::AsyncMsg()
: type(0)
, value(0)
{}

AsyncMsg::AsyncMsg(char type, int value)
: type(type)
, value(value)
{}

AsyncChannel::AsyncChannel() {
	LOG_SCOPE
	// creating channel
	channelId = ChannelCreate_r(0);
	if(channelId < 0) {
		LOG_ERROR<<"Channel create failed. Channel id: "<< channelId << endl;
		exit(EXIT_FAILURE);
	}

	// creating connection to channel
	connectionId = ConnectAttach_r(0, 0, channelId, 0, 0);
	if(connectionId < 0){
		LOG_ERROR<<"Connection attach failed. Connection id: " << connectionId << endl;
		exit(EXIT_FAILURE);
	}

	LOG_DEBUG<<__FUNCTION__<<" channel created. Channel id: " + to_string(channelId) + " ( connection id: " + to_string(connectionId)+ ")"<<endl;
}

AsyncChannel::~AsyncChannel() {
	LOG_SCOPE

	ConnectDetach(connectionId);
	ChannelDestroy(channelId);

	LOG_DEBUG<<__FUNCTION__ << " channel destroyed." << endl;;
}

AsyncMsg AsyncChannel::nextMessage(){
	LOG_SCOPE

	AsyncMsg msg;
	struct _pulse pulse;

	// wait for next message
	int ret = MsgReceivePulse_r(channelId, &pulse, sizeof(pulse), NULL);
	if(ret < 0){
		LOG_ERROR<<__FUNCTION__ << " MsgReceivePulse failed. Error id: " << ret << endl;
		exit(EXIT_FAILURE);
	}

	// put data in own structure
	msg.type = pulse.code;
	msg.value = pulse.value.sival_int;

	return msg;
}

void AsyncChannel::sendMessage(AsyncMsg msg){
	LOG_SCOPE
	// send message to channel via connectionId
    int ret = MsgSendPulse_r(
		connectionId,
		sched_get_priority_max(0),
    	msg.type,
		msg.value );
   	if(ret < 0){
   		LOG_ERROR<<__FUNCTION__<<" MsgSendPulse failed. Error id: "<<ret<<endl;
   		exit(EXIT_FAILURE);
   	}
}

int AsyncChannel::getConnectionId(){
	LOG_SCOPE
	return connectionId;
}

AsyncChannel& AsyncChannel::instance(){
	LOG_SCOPE
	static AsyncChannel instance;
	return instance;
}

} /* io */
} /* hardwareLayer */
