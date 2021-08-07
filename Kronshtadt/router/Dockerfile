FROM debian:buster

RUN apt-get update
RUN apt-get install net-tools
RUN apt-get install python3 -y
RUN apt-get install build-essential -y
RUN DEBIAN_FRONTEND='noninteractive' apt-get install cmake -y

RUN mkdir router
RUN mkdir router/doc
RUN mkdir router/lemon
RUN mkdir router/server
RUN mkdir router/srcs
RUN mkdir router/build

COPY ./doc ./router/doc
COPY ./lemon ./router/lemon
COPY ./server ./router/server
COPY ./src ./router/src
COPY ./CMakeLists.txt ./router
COPY ./LICENSE ./router

COPY ./start.sh ./
CMD bash start.sh

EXPOSE 80
