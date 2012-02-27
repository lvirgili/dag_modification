################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../x_dag.o \
../x_io.o 

C_SRCS += \
../heft.c \
../setPart.c \
../tvms.c \
../x_dag\ -\ antes\ de\ modificar\ insercion\ de\ valores\ en\ la\ matriz\ B.c \
../x_dag.c \
../x_form.c \
../x_io.c \
../x_list.c \
../x_relax.c 

OBJS += \
./heft.o \
./setPart.o \
./tvms.o \
./x_dag\ -\ antes\ de\ modificar\ insercion\ de\ valores\ en\ la\ matriz\ B.o \
./x_dag.o \
./x_form.o \
./x_io.o \
./x_list.o \
./x_relax.o 

C_DEPS += \
./heft.d \
./setPart.d \
./tvms.d \
./x_dag\ -\ antes\ de\ modificar\ insercion\ de\ valores\ en\ la\ matriz\ B.d \
./x_dag.d \
./x_form.d \
./x_io.d \
./x_list.d \
./x_relax.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/opt/xpressmp/include -I/opt/xpressmp/lib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

x_dag\ -\ antes\ de\ modificar\ insercion\ de\ valores\ en\ la\ matriz\ B.o: ../x_dag\ -\ antes\ de\ modificar\ insercion\ de\ valores\ en\ la\ matriz\ B.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/opt/xpressmp/include -I/opt/xpressmp/lib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"x_dag - antes de modificar insercion de valores en la matriz B.d" -MT"x_dag\ -\ antes\ de\ modificar\ insercion\ de\ valores\ en\ la\ matriz\ B.d" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


