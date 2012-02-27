################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../icc/tvms.c \
../icc/x_dag.c \
../icc/x_form.c \
../icc/x_io.c \
../icc/x_list.c \
../icc/x_relax.c 

OBJS += \
./icc/tvms.o \
./icc/x_dag.o \
./icc/x_form.o \
./icc/x_io.o \
./icc/x_list.o \
./icc/x_relax.o 

C_DEPS += \
./icc/tvms.d \
./icc/x_dag.d \
./icc/x_form.d \
./icc/x_io.d \
./icc/x_list.d \
./icc/x_relax.d 


# Each subdirectory must supply rules for building sources it contributes
icc/%.o: ../icc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/opt/xpressmp/include -I/opt/xpressmp/lib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


