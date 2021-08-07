# shellcheck disable=SC2164
mkdir build


cd build
cmake ..
make
cd src
./router >> /router_log &
cd ../../server
python3 server.py >> /proxy_log &
