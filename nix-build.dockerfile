FROM gcc:10.2

# Build uses this env var for finding Gtest and GMock
ENV GOOGLE_TEST_SRC_DIR=/usr/googletest

# Set a timezone to avoid possible complications installing prereqs
ENV TZ=America/New_York
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Install Prereqs (CMake)
RUN apt-get update
RUN apt-get install -y cmake

# Clone Google Test and Mock
WORKDIR /usr
RUN git clone https://github.com/google/googletest.git -b release-1.10.0 

# Set Working Dir
WORKDIR /usr/proj
