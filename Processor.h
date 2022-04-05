/*
 * Processor.h
 *
 *  Created on: 14 Haz 2021
 *      Author: burakbalta
 */

#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include "Common.h"
#include <thread>
#include <mutex>
#include <regex>
#include <queue>
#include <string>
#include <condition_variable>
#include <atomic>

namespace bb {
namespace detail {

class Processor : public interfaces::IProcessor, public interfaces::NonCopyable {
public:
	Processor(int id);
	~Processor();
	void Process(const std::shared_ptr<struct PacketData>& pckt) override;
	void StartProcessing(const std::regex& regularExpression) override;
	void StopProcessing() override;
private:
	void engine();

	void printPacketAsHex(const std::string& s);

	std::mutex packetQueueMutex;
	std::queue<std::shared_ptr<struct PacketData>> packetQueue;
	std::thread engineThread;
	std::condition_variable cond;
	std::atomic<bool> isStopped{false};
	std::regex regularExpression;

	static std::mutex coutMutex;

	const std::string tag;
};


}
}


#endif /* PROCESSOR_H_ */
