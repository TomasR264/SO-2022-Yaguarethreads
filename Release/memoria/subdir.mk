################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../memoria/memoria.c 

OBJS += \
./memoria/memoria.o 

C_DEPS += \
./memoria/memoria.d 


# Each subdirectory must supply rules for building sources it contributes
memoria/%.o: ../memoria/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


