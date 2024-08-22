FROM debian:bookworm

# required libs
RUN apt update \
	&& apt install --no-install-recommends --no-install-suggests -y build-essential cmake \
	libgtk-3-dev checkinstall libwxgtk3.0-gtk3-dev \
	git pkg-config libavcodec-dev libavformat-dev libswscale-dev libopencv-dev \
	libjsoncpp-dev apt-transport-https ca-certificates \
	autoconf automake libtool

RUN apt update \
	&& apt install --no-install-recommends --no-install-suggests -y curl gdb cmake-extras \
	bc bison flex libssl-dev make \
	libboost-atomic-dev libboost-thread-dev libboost-system-dev  libboost-date-time-dev libboost-regex-dev  \
	libboost-filesystem-dev libboost-random-dev libboost-chrono-dev libboost-serialization-dev libwebsocketpp-dev \
	openssl ninja-build g++  \
	libcpprest-dev gpiod libgpiod-dev rsync sshpass\
	libpng-dev texlive doxygen 
	
RUN apt update \
	&& apt-cache pkgnames | grep -i crypto++ \
	&& apt install --no-install-recommends --no-install-suggests -y libcrypto++8 libcrypto++-utils libcrypto++-dev
	
# ceutil lib
RUN git clone --depth 1 https://github.com/yan9a/ceutil.git \
	&& cd ceutil && mkdir -p build && cd build \
	&& cmake -D CMAKE_BUILD_TYPE=Release -D BUILD_SHARED_LIBS=ON -D CMAKE_INSTALL_PREFIX=/usr/local .. \
	&& make && make install \
	&& sh -c "echo /usr/local/lib > /etc/ld.so.conf.d/ceutil.conf" \
	&& ldconfig  && cd ../..

# nfc lib
RUN git clone --depth 1 https://github.com/NXPNFCLinux/linux_libnfc-nci.git \
	&& cd linux_libnfc-nci && ./bootstrap \
	&& ./configure --enable-alt && make && make install \
	&& sh -c "echo /usr/local/lib/ > /etc/ld.so.conf.d/nfc-nci.conf" \
	&& ldconfig && cd ..

# zint lib
RUN git clone https://github.com/woo-j/zint.git \
	&& cd zint && mkdir -p build && cd build \
	&& cmake ..  && make && make install \
	&& sh -c "echo /usr/local/lib/ > /etc/ld.so.conf.d/zint.conf" \
	&& ldconfig && cd ../..

EXPOSE 80 443

