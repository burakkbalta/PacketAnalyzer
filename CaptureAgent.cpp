/*
 * CaptureAgent.cpp
 *
 *  Created on: 13 Haz 2021
 *      Author: burakbalta
 */

#include <stdio.h>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <vector>

#include "CaptureAgent.h"

namespace bb {
namespace detail {

void my_packet_handler(
    u_char *args,
    const struct pcap_pkthdr *packet_header,
    const u_char *packet_body
)
{
    print_packet_info(packet_body, *packet_header);
    return;
}

void print_packet_info(const u_char *packet, struct pcap_pkthdr packet_header) {
    printf("Packet capture length: %d\n", packet_header.caplen);
    printf("Packet total length %d\n", packet_header.len);
}

CaptureAgent::CaptureAgent()
{
}

CaptureAgent::~CaptureAgent() {
	if(handle) {
		pcap_close(handle);
	}

	if(allDevices) {
		pcap_freealldevs(allDevices);
	}

	if(this->captureThread.joinable()) {
		this->captureThread.join();
	}
}

bool CaptureAgent::StartCapture(const std::shared_ptr<interfaces::IDispatcher>& dispatcher) {
	bool result{true};
	this->dispatcherAgent = dispatcher;
	/* Define the device */
	//dev = pcap_lookupdev(errbuf);
	if(pcap_findalldevs(&allDevices, errbuf) == 0) {
		dev = allDevices->name;
	}
	if (dev == nullptr) {
		fprintf(stderr, "Couldn't find default device: %s\n", errbuf);
		result = false;
	} else {
		handle = pcap_open_live(dev, 8192, 1, 1000, errbuf);
		if (handle == nullptr) {
			fprintf(stderr, "Couldn't open device %s: %s\n", dev, errbuf);
			result = false;
		}
	}


	if(result) {
		capturePackets();
	}

	return result;
}

bool CaptureAgent::StopCapture() {
	if(handle) {
		pcap_breakloop(handle);
	}
	isCapturingStopped = true;
	return true;
}

void CaptureAgent::capturePackets() {
	this->captureThread = std::thread([this](){
		while(!isCapturingStopped.load()) {
			std::shared_ptr<PacketData> pcktPtr = std::make_shared<PacketData>();
			const unsigned char* packet = pcap_next(handle, &pcktPtr->header);
			if(packet != nullptr) {
				std::string str(reinterpret_cast<char*>(const_cast<unsigned char*>(packet)),
						pcktPtr->header.caplen);
				pcktPtr->packet = std::move(str);
				pcktPtr->packetNumber = GetPacketNumber();
				this->dispatcherAgent->ReceiveCapturedPacket(pcktPtr);
				//prepareCapturedPacketAndSendDispatcher(packet, pcktPtr->header);
			}


		}

		//pcap_loop(handle, -1, my_packet_handler, nullptr);
	});
}

void CaptureAgent::prepareCapturedPacketAndSendDispatcher(const unsigned char* packet,
		const struct pcap_pkthdr& header) {

	printf("Packet capture length: %d\n", header.caplen);
	printf("Packet total length %d\n", header.len);
}

int CaptureAgent::GetPacketNumber() {
	static int pcktNumber{0};
	return pcktNumber++;
}


}
}




