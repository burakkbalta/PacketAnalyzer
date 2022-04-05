/*
 * DispatcherAgent.h
 *
 *  Created on: 13 Haz 2021
 *      Author: burakbalta
 */

#ifndef DISPATCHERAGENT_H_
#define DISPATCHERAGENT_H_

#include "Common.h"
#include <memory>
#include <vector>
#include <regex>

namespace bb {
namespace detail {

class DispatcherAgent : public bb::detail::interfaces::IDispatcher, public interfaces::NonCopyable {
public:
	DispatcherAgent();
	~DispatcherAgent();

	bool StartDispatching(enumerations::DispatchingRules dispatchRule,
		int numberOfThread, const std::regex& regularExpression) override;
	bool StopDispatching() override;

	void ReceiveCapturedPacket(const std::shared_ptr<struct PacketData>& pckt) override;

private:
	void packetBasedRoundRobinDispatch(const std::shared_ptr<struct PacketData>& pckt);

	std::vector<std::unique_ptr<interfaces::IProcessor>> processorPool;
	unsigned int processorIndex{0};
	enumerations::DispatchingRules dispatchingRule{};
	unsigned int numberOfProcessor{};
	std::regex regularExpression{};
};

}
}



#endif /* DISPATCHERAGENT_H_ */
