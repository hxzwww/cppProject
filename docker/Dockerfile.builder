# docker/Dockerfile.builder
FROM nvidia/cuda:11.8.0-devel-ubuntu20.04

ENV DEBIAN_FRONTEND=noninteractive
# Install dependencies
RUN apt-get update && \
    apt-get install -y vim build-essential unzip git cmake g++ libssl-dev libcpprest-dev libgtest-dev libtbb-dev && \
    apt-get clean

# Set working directory
WORKDIR /app

# Copy source files to the container
COPY ../ /app

# Run cmake and make to build the server
RUN cmake -S . -B build && cmake --build build
