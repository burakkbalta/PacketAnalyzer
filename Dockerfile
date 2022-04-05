FROM ubuntu:latest
RUN apt-get -y update && apt-get install -y
RUN apt-get install -y build-essential
RUN apt-get install -y libpcap-dev
RUN apt-get install -y make
RUN apt-get update && \
    apt-get install -y gcc 
ENV NAME VAR1
ENV NAME VAR2    
COPY . /usr/src/packetAnalyzer
WORKDIR /usr/src/packetAnalyzer/Debug/
RUN make clean
RUN make all
CMD ["/bin/sh", "./run_packetanalyzer.sh"]