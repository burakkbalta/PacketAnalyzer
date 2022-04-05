# PacketAnalyzer
The packet analysis project responsible for checking if the captured packets are matched with the given regular expression.

The project file structure is as follows:
  - PacketAnalyzer
		- Debug
			- makefile
			- objects.mk
			- run_packetanalyzer.sh
			- sources.mk
			- subdir.mk
		- Release
			- makefile
			- objects.mk
			- run_packetanalyzer.sh
			- sources.mk
			- subdir.mk
		- CaptureAgent.cpp
		- CaptureAgent.h
		- Common.h
		- DispatcherAgent.cpp
		- DispatcherAgent.h
		- Dockerfile
		- main.cpp
		- Processor.cpp
		- Processor.h
    
The tasks for which the above files are responsible are as follows:
  - Debug : 
		- It contains the necessary files for the application to be compiled in this configuration. 
		- run_packetanalyzer.sh is the bash script file used for the docker image.
	- Release : 
		- It contains the necessary files for the application to be compiled in this configuration. 
		- rrun_packetanalyzer.sh is the bash script file used for the docker image.
	- CaptureAgent.h & CaptureAgent.cpp : 
		- It is the unit where the packets are captured and then forwarded to the relevant unit.
	- Common.h : 
		- It is the file containing general interface and enum classes. 
	- DispatcherAgent.h & DispatcherAgent.cpp : 
		- Creates the work unit according to given parameter.
		- It is the unit that forwards the captured packets to relevant business unit using a packet-based RoundRobin scheduling mechanism.
	- Dockerfile: 
		- It is the docker image file created for the project to run.
	- main.cpp : 
		- The file containing the main function.
		- It contains the functions which are responsible to check if the given parameters are conformed.
		- The required units are created here.
	- Processor.h & Processor.cpp : 
		- It is the unit where the captured packets are processed.
		- Responsible whether there is a match between the given regular expression and the packet.
		
The steps required to run project are as follows:
  - Running from source code:
    - 
