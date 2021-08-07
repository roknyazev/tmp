apt-get update
apt-get install net-tools
apt-get install python3 -y
apt install python3-pip -y

pip3 install requests

python3 main.py >> /hubs_log &
cd ../
