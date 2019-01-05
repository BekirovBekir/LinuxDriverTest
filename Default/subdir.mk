################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CharDev.c \
../CharDev.mod.c 

O_SRCS += \
../CharDev.mod.o \
../CharDev.o 

OBJS += \
./CharDev.o \
./CharDev.mod.o 

C_DEPS += \
./CharDev.d \
./CharDev.mod.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -std=c99 -nostdinc -D__KERNEL__=1 -I/usr/src/linux-headers-4.15.0-42-generic/arch/x86/include -I/usr/src/linux-headers-4.15.0-42-generic/include -I/usr/lib/gcc/x86_64-linux-gnu/5.4.0/include -I/usr/src/linux-headers-4.15.0-43-generic/arch/x86/include -I/usr/src/linux-headers-4.15.0-43-generic/arch/x86/include/uapi -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)"  -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


