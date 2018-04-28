FROM ubuntu:16.04

ARG port=8080

# Install build environment
RUN apt-get -y update && \
    apt-get -y install build-essential software-properties-common vim && \
    apt-get -y install git cmake libboost-all-dev


# Clone Corvusoft/restbed and dependencies
RUN cd /tmp && \
    git clone --recursive https://github.com/corvusoft/restbed.git && \
    cd restbed && \
    git reset --hard c79c407


# Build Corvusoft/restbed (ignore tests, disable openssl) & install
RUN cd /tmp/restbed && \
    sed -i 's/enable_testing( )//g' CMakeLists.txt && \
    sed -i 's/add_subdirectory.*test.*//g' CMakeLists.txt && \
    rm -rf test && \
    mkdir build && \
    cd build && \
    cmake -DBUILD_SSL=NO .. && \
    make install -j$(nproc) && \
    cd ../distribution && \
    cp -R include/* /usr/include/ && \
    cp -R library/* /usr/lib/ && \
    rm -rf /tmp/restbed


# Copy wplexserver sources
COPY src            /wplexserver_source/src
COPY library        /wplexserver_source/library
COPY CMakeLists.txt /wplexserver_source/CMakeLists.txt


# Build wplexserver
RUN cd /wplexserver_source && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j$(nproc) && \
    mkdir /wplexserver && \
    cp wplexserver-demo /wplexserver/ && \
    rm -rf /wplexserver_source


WORKDIR /wplexserver

CMD /wplexserver/wplexserver-demo

EXPOSE ${port}
