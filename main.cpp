/*
 * main.cpp
 *
 *  Created on: 10 Haz 2021
 *      Author: burakbalta
 */

#include <iostream>
#include <regex>
#include <stdexcept>
#include <csignal>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <pcap.h>

#include "CaptureAgent.h"
#include "DispatcherAgent.h"

std::condition_variable cond;
bool isStop{false};
std::mutex stopMutex;


void signalHandler( int signum ) {
   std::cout << "\nInterrupt signal (" << signum << ") received.\n";
   {
	  std::lock_guard<std::mutex> guard(stopMutex);
	  isStop = true;
   }

   cond.notify_one();
}


bool checkCommandLineArguments(int argc, char** argv, std::regex& expression,
		unsigned int& threadNumber) {
	bool result{false};

	if(argc == 3) {
		std::string argument{argv[1]};
		try {
			expression = std::regex(argument);
			argument = argv[2];
			try {
			  std::size_t pos;
			  threadNumber = std::stoi(argument, &pos);
			  if (pos < argument.size()) {
			    std::cerr << "Trailing characters after number: " << argument << '\n';
			  } else {
				  if(threadNumber < 0) {
					  std::cout << "second parameter should be greater than zero" << '\n';
					  result = false;
				  } else {
					  result = true;
				  }
			  }
			} catch (std::invalid_argument const &ex) {
			  std::cerr << "Invalid number: " << argument << '\n';
			} catch (std::out_of_range const &ex) {
			  std::cerr << "Number out of range: " << argument << '\n';
			}
		}
		catch (const std::regex_error& e) {
			std::cout << "first parameter should be regular expression!!!" << '\n';
			std::cerr << "regex_error caught: " << e.what() << '\n';
		}
	} else {
		std::cout << "Wrong number of command line arguments are given!!!!" << '\n';
		result = false;
	}

	return result;
}

int main(int argc, char **argv) {
	auto returnResult{0};

	std::regex regExpression;
	unsigned int numberOfThread;
	if(checkCommandLineArguments(argc, argv, regExpression, numberOfThread)) {
		//start Dispatcher thread.
		const auto& dispatcherAgent = std::make_shared<bb::detail::DispatcherAgent>();
		const auto& captureAgent = std::make_shared<bb::detail::CaptureAgent>();
		bool b = dispatcherAgent->StartDispatching(bb::detail::enumerations::DispatchingRules::PacketBasedRoundRobin,
			numberOfThread, regExpression);
		b = captureAgent->StartCapture(dispatcherAgent);

		if(!b) {
			std::cout << "Please run application with sudo command!!!\n";
		} else {
			signal(SIGINT, signalHandler);

			std::unique_lock<std::mutex> lck{stopMutex};
			cond.wait(lck, []{return isStop;});
		}

		b = captureAgent->StopCapture();
		b = dispatcherAgent->StopDispatching();
	} else {

		returnResult = -1;
		std::cout << "Program is terminated." << '\n';
		std::cerr << "Usage: " << argv[0] << " <Regular Expression> <Thread Number>" << '\n';
	}

	return returnResult;
}


