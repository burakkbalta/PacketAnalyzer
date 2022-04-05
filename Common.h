/*
 * Common.h
 *
 *  Created on: 13 Haz 2021
 *      Author: burakbalta
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <memory>
#include <regex>
#include <pcap.h>

namespace bb {
namespace detail {

namespace enumerations {

enum class DispatchingRules {
	PacketBasedRoundRobin,
};

}

struct PacketData {
	int packetNumber;
	struct pcap_pkthdr header;
	std::string packet;
};

namespace interfaces {

struct NonCopyable {
	NonCopyable() = default;
	virtual ~NonCopyable() = default;
	NonCopyable(NonCopyable&&) = default;
	NonCopyable& operator=(NonCopyable&&) = default;

	NonCopyable(NonCopyable&) = delete;
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;

};

class IDispatcher{
public:
	virtual ~IDispatcher() = default;
	virtual bool StartDispatching(enumerations::DispatchingRules dispatchRule, int numberOfThread,
		const std::regex& regularExpression) = 0;
	virtual bool StopDispatching() = 0;
	virtual void ReceiveCapturedPacket(const std::shared_ptr<struct PacketData>& pckt) = 0;
};

class ICapture {
public:
	virtual ~ICapture() = default;
	virtual bool StartCapture(const std::shared_ptr<IDispatcher>& dispatcher) = 0;
	virtual bool StopCapture() = 0;
};


class IProcessor {
public:
	virtual ~IProcessor() = default;
	virtual void Process(const std::shared_ptr<struct PacketData>& pckt) = 0;
	virtual void StartProcessing(const std::regex& regularExpression) = 0;
	virtual void StopProcessing() = 0;
};

}

}
}


#endif /* COMMON_H_ */
