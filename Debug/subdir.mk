################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../CaptureAgent.cpp \
../DispatcherAgent.cpp \
../Processor.cpp \
../main.cpp 

OBJS += \
./CaptureAgent.o \
./DispatcherAgent.o \
./Processor.o \
./main.o 

CPP_DEPS += \
./CaptureAgent.d \
./DispatcherAgent.d \
./Processor.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -O2 -g3 -Wall -c -fmessage-length=0 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


