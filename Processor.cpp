/*
 * Processor.cpp
 *
 *  Created on: 14 Haz 2021
 *      Author: burakbalta
 */

#include "Processor.h"

#include <iostream>

namespace bb {
namespace detail {

std::mutex Processor::coutMutex;

Processor::Processor(int id) :
tag{"Process#" + std::to_string(id) + ": "}
{
}

Processor::~Processor() {
	if(this->engineThread.joinable()) {
		this->engineThread.join();
	}
	decltype(this->packetQueue) empty;
	std::swap(this->packetQueue, empty);
}

void Processor::Process(const std::shared_ptr<struct PacketData>& pckt) {
	std::unique_lock<std::mutex> lck(this->packetQueueMutex);
	this->packetQueue.emplace(pckt);
	lck.unlock();
	cond.notify_one();
}

void Processor::StartProcessing(const std::regex& regularExpression) {
	this->regularExpression = regularExpression;
	this->engineThread = std::thread([this](){
		this->engine();
	});
}

void Processor::StopProcessing() {
	this->isStopped = true;
	cond.notify_one();
}

void Processor::engine() {
	while(!this->isStopped.load() ) {
		std::unique_lock<std::mutex> lck(this->packetQueueMutex);
		cond.wait(lck, [this](){
			return (this->isStopped.load() || this->packetQueue.size() > 0);
		});

		if(this->isStopped) {
			break;
		} else {
			auto pckt = this->packetQueue.front();
			this->packetQueue.pop();
			if(std::regex_match(pckt->packet, this->regularExpression)) {
				std::lock_guard<std::mutex> lck(coutMutex);
				std::cout << tag << "Expression is matched..."  << "\n";
				std::cout << tag << "PacketNumber: "  << pckt->packetNumber << " Length: " <<
						pckt->header.caplen << " bytes...\n";
				std::cout << tag << "Packet: ";
				printPacketAsHex(pckt->packet);
			} else {
				//std::lock_guard<std::mutex> lck(coutMutex);
				//std::cout << tag << "Expression is not matched..\n";
				//std::cout << tag << "PacketNumber: "  << pckt->packetNumber << " Length: " <<
				//		pckt->header.caplen << " bytes...\n";
				//std::cout << tag << "Packet: ";
				//printPacketAsHex(pckt->packet);
			}
		}
	}
}

void Processor::printPacketAsHex(const std::string& s) {
	 static constexpr char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

	for(auto elem : s) {
		std::string printStr;
		printStr += hex_chars[ ( elem & 0xF0 ) >> 4 ];
		printStr += hex_chars[ ( elem & 0x0F ) >> 0 ];
		std::cout << printStr << " ";
	}
	printf("\n");
}

}
}

