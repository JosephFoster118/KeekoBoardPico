FROM debian:11

RUN apt update
RUN apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi libstdc++-arm-none-eabi-newlib git python3 g++ -y
RUN git clone https://github.com/raspberrypi/pico-sdk.git
RUN git clone --branch smp https://github.com/FreeRTOS/FreeRTOS-Kernel.git; cd /FreeRTOS-Kernel; git checkout 7ce8266
RUN cd /pico-sdk; git submodule update --init --recursive
ENV PICO_SDK_PATH="/pico-sdk"
ENV FREERTOS_SRC_DIRECTORY="/FreeRTOS-Kernel"
COPY ./compile.sh /compile.sh
RUN chmod 777 /compile.sh

#ENTRYPOINT [ "./compile.sh" ]
