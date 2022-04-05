/*
 * DispatcherAgent.cpp
 *
 *  Created on: 14 Haz 2021
 *      Author: burakbalta
 */

#include "DispatcherAgent.h"
#include "Processor.h"

#include <iostream>

namespace bb {
namespace detail {

DispatcherAgent::DispatcherAgent()
{
}

DispatcherAgent::~DispatcherAgent() {
	this->processorPool.clear();
}

bool DispatcherAgent::StartDispatching(enumerations::DispatchingRules dispatchRule,
	int numberOfThread, const std::regex& regularExpression) {
	this->dispatchingRule = dispatchRule;
	this->numberOfProcessor = numberOfThread;
	this->regularExpression = regularExpression;
	for(decltype(this->numberOfProcessor) i = 0; i<this->numberOfProcessor; ++i) {
		auto proc = std::make_unique<Processor>(i);
		proc->StartProcessing(regularExpression);
		this->processorPool.push_back(std::move(proc));
	}
	return true;
}

bool DispatcherAgent::StopDispatching() {
	for(const auto& elem : this->processorPool) {
		elem->StopProcessing();
	}
	return true;
}
void DispatcherAgent::ReceiveCapturedPacket(const std::shared_ptr<struct PacketData>& pckt) {
	switch(this->dispatchingRule) {
	case enumerations::DispatchingRules::PacketBasedRoundRobin:
		packetBasedRoundRobinDispatch(pckt);
		break;
	default:
		break;
	}
}

void DispatcherAgent::packetBasedRoundRobinDispatch(const std::shared_ptr<struct PacketData>& pckt) {
	unsigned int index{this->processorIndex++};
	if(this->processorIndex == this->numberOfProcessor) {
		this->processorIndex = this->processorIndex % this->numberOfProcessor;

	}
	this->processorPool[index]->Process(pckt);
}

}
}

