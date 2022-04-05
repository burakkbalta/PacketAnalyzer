/*
 * CaptureAgent.h
 *
 *  Created on: 13 Haz 2021
 *      Author: burakbalta
 */

#ifndef CAPTUREAGENT_H_
#define CAPTUREAGENT_H_

#include <thread>
#include <pcap.h>
#include <atomic>

#include "Common.h"

namespace bb {
namespace detail {

void my_packet_handler(
	    u_char *args,
	    const struct pcap_pkthdr *header,
	    const u_char *packet
);

void print_packet_info(const u_char *packet, struct pcap_pkthdr packet_header);

class CaptureAgent : public bb::detail::interfaces::ICapture, public interfaces::NonCopyable {
public:
	CaptureAgent();
	~CaptureAgent();

	bool StartCapture(const std::shared_ptr<interfaces::IDispatcher>& dispatcher) override;
	bool StopCapture() override;

private:
	void capturePackets();

	void prepareCapturedPacketAndSendDispatcher(const unsigned char* packet,
			const struct pcap_pkthdr& header);

	int GetPacketNumber();

	std::atomic<bool> isCapturingStopped{false};

	std::thread captureThread;
	pcap_t *handle{nullptr};			/* Session handle */
	char *dev{nullptr};			/* The device to sniff on */
	char errbuf[PCAP_ERRBUF_SIZE];	/* Error string */
	pcap_if_t* allDevices{nullptr};

	std::shared_ptr<interfaces::IDispatcher> dispatcherAgent;
};

}
}



#endif /* CAPTUREAGENT_H_ */
