/**
 * \file oscsendexample.cpp
 */

#include <mipconfig.h>

#if(defined(MIPCONFIG_SUPPORT_OSC))

#include <mipcomponentchain.h>
#include <mipcomponent.h>
#include <miptime.h>
#include <mipaveragetimer.h>
#include <miposcinput.h>
#include <miposcencoder.h>
#include <miprtposcencoder.h>
#include <miprtpcomponent.h>
#include <miprtpdecoder.h>
#include <miprtposcdecoder.h>
#include <miposcoutput.h>
#include <miposcdecoder.h>
#include <miposcmessage.h>

#include <jrtplib3/rtpsession.h>
#include <jrtplib3/rtpsessionparams.h>
#include <jrtplib3/rtpipv4address.h>
#include <jrtplib3/rtpudpv4transmitter.h>
#include <jrtplib3/rtperrors.h>
#include <stdio.h>
#include <iostream>
#include <string>

using namespace jrtplib;

void checkError(bool returnValue, const MIPComponent &component)
{
	if (returnValue == true)
		return;

	std::cerr << "An error occured in component: " << component.getComponentName() << std::endl;
	std::cerr << "Error description: " << component.getErrorString() << std::endl;

	exit(-1);
}

void checkError(bool returnValue, const MIPComponentChain &chain)
{
	if (returnValue == true)
		return;

	std::cerr << "An error occured in chain: " << chain.getName() << std::endl;
	std::cerr << "Error description: " << chain.getErrorString() << std::endl;

	exit(-1);
}

// We'll be using an RTPSession instance from the JRTPLIB library. The following
// function checks the JRTPLIB error code.

void checkError(int status)
{
	if (status >= 0)
		return;

	std::cerr << "An error occured in the RTP component: " << std::endl;
	std::cerr << "Error description: " << RTPGetErrorString(status) << std::endl;

	exit(-1);
}

class MyChain : public MIPComponentChain
{
public:
	MyChain(const std::string &chainName) : MIPComponentChain(chainName)
	{
	}
private:
	void onThreadExit(bool error, const std::string &errorComponent, const std::string &errorDescription)
	{
		if (!error)
			return;

		std::cerr << "An error occured in the background thread." << std::endl;
		std::cerr << "    Component: " << errorComponent << std::endl;
		std::cerr << "    Error description: " << errorDescription << std::endl;
	}
};

int main(void)
{
	int packetsPerSecond = 100;
	MIPTime interval(1.0/(double)packetsPerSecond); // We'll use 10 millisecond intervals.
	MIPAverageTimer timer(interval);
	MIPOSCInput oscInput;
	MIPOSCEncoder oscEnc;
	MIPRTPOSCEncoder rtpEnc;
	MIPRTPComponent rtpComp;
	MIPRTPDecoder rtpDec;
	MIPRTPOSCDecoder rtpOSCDec;
	MIPOSCDecoder oscDec;
	MIPOSCOutput oscOutput;

	MyChain chain("OSC Sender");
	RTPSession rtpSession;
	bool returnValue;


	// Convert Messages to MIPOSCMessages
	returnValue = oscEnc.init();
	checkError(returnValue, oscEnc);

	// Initialize the RTP OSC encoder: this component will create
	// RTP messages which can be sent to the RTP component.

	returnValue = rtpEnc.init();
	checkError(returnValue, rtpEnc);

	// We'll initialize the RTPSession object which is needed by the
	// RTP component.

	RTPUDPv4TransmissionParams transmissionParams;
	RTPSessionParams sessionParams;
	int portBase = 60000;
	int status;

	transmissionParams.SetPortbase(portBase);
	sessionParams.SetOwnTimestampUnit(1.0/((double)packetsPerSecond));
	sessionParams.SetMaximumPacketSize(64000);
	sessionParams.SetAcceptOwnPackets(true);

	status = rtpSession.Create(sessionParams,&transmissionParams);
	checkError(status);

	// Instruct the RTP session to send data to ourselves.
	status = rtpSession.AddDestination(RTPIPv4Address(ntohl(inet_addr("127.0.0.1")),portBase));
	checkError(status);

	// Tell the RTP component to use this RTPSession object.
	returnValue = rtpComp.init(&rtpSession);
	checkError(returnValue, rtpComp);

	returnValue = rtpDec.init(false, 0, &rtpSession);
	checkError(returnValue, rtpDec);

	returnValue = rtpDec.setPacketDecoder(0, &rtpOSCDec);
	checkError(returnValue, rtpDec);

	returnValue = oscDec.init();
	checkError(returnValue, oscDec);

	// Next, we'll create the chain
	returnValue = chain.setChainStart(&timer);
	checkError(returnValue, chain);

	returnValue = chain.addConnection(&timer, &oscInput);
	checkError(returnValue, chain);

	returnValue = chain.addConnection(&oscInput, &oscEnc);
	checkError(returnValue, chain);

	returnValue = chain.addConnection(&oscEnc, &rtpEnc);
	checkError(returnValue, chain);

	returnValue = chain.addConnection(&rtpEnc, &rtpComp);
	checkError(returnValue, chain);

	returnValue = chain.addConnection(&rtpComp, &rtpDec);
	checkError(returnValue, chain);

	returnValue = chain.addConnection(&rtpDec, &oscDec, true);
	checkError(returnValue, chain);

	returnValue = chain.addConnection(&oscDec, &oscOutput);
	checkError(returnValue, chain);

	// Start the chain

	returnValue = chain.start();
	checkError(returnValue, chain);

	// We'll wait until enter is pressed

	int counter = 0;

	sleep(1);
	for(int i=0; i<4; i++) {
		lo_message m = lo_message_new();
		lo_message_add_int32(m,counter++);
		oscInput.push(m, "/testpfad");
		sleep(1);
	}
	getc(stdin);

	returnValue = chain.stop();
	checkError(returnValue, chain);

	rtpSession.Destroy();

	return 0;
}

#else

#include <iostream>

int main(void)
{
	std::cerr << "Not all necessary components are available to run this example." << std::endl;
	return 0;
}

#endif // WIN32 || _WIN32_WCE || MIPCONFIG_SUPPORT_OSS

