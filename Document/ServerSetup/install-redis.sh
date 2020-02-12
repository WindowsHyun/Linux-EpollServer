#!/bin/bash
# from here: http://www.codingsteps.com/install-redis-2-6-on-amazon-ec2-linux-ami-or-centos/
# and here: https://raw.github.com/gist/257849/9f1e627e0b7dbe68882fa2b7bdb1b2b263522004/redis-server
###############################################
# To use: 
# wget https://gist.github.com/andzdroid/5848701/raw/f4c16767c0e04ba22f83088fe204640d10a8f87a/install-redis.sh
# chmod 777 install-redis.sh
# ./install-redis.sh
###############################################
echo "*****************************************"
echo " 1. Prerequisites: Install updates, set time zones, install GCC and make"
echo "*****************************************"
#sudo yum -y update
#sudo ln -sf /usr/share/zoneinfo/America/Los_Angeles \/etc/localtime
#sudo yum -y install gcc gcc-c++ make 
sudo apt-get update
sudo apt-get -y install make
sudo apt-get -y install make-guile
sudo apt-get -y install linux-kernel-headers kernel-package
sudo apt-get -y install libjemalloc-dev
sudo apt-get -y install gcc
echo "*****************************************"
echo " 2. Download, Untar and Make Redis 5.0.7"
echo "*****************************************"
cd /usr/local/src
sudo wget http://download.redis.io/releases/redis-5.0.7.tar.gz
sudo tar xzf redis-5.0.7.tar.gz
sudo rm redis-5.0.7.tar.gz
cd redis-5.0.7
sudo make
echo "*****************************************"
echo " 3. Create Directories and Copy Redis Files"
echo "*****************************************"
sudo mkdir /etc/redis /var/lib/redis
sudo mkdir /etc/redis/logs
sudo mkdir /etc/redis/data
sudo chmod 777 /etc/redis/logs
sudo chmod 777 /etc/redis/data
sudo mkdir /etc/redis/data/6380
sudo cp src/redis-server src/redis-cli /usr/local/bin
echo "*****************************************"
echo " 4. Configure Redis.Conf"
echo "*****************************************"
echo " Edit redis.conf as follows:"
echo " 1: ... daemonize yes"
echo " 2: ... bind 127.0.0.1"
echo " 3: ... dir /var/lib/redis"
echo " 4: ... loglevel notice"
echo " 5: ... logfile /var/log/redis.log"
echo "*****************************************"
sudo sed -e "s/^daemonize no$/daemonize yes/" -e "s/^# bind 127.0.0.1$/bind 127.0.0.1/" -e "s/^dir \.\//dir \/var\/lib\/redis\//" -e "s/^loglevel verbose$/loglevel notice/" -e "s/^logfile stdout$/logfile \/var\/log\/redis.log/" redis.conf | sudo tee /etc/redis/redis.conf
echo "*****************************************"
echo " 5. Download init Script"
echo "*****************************************"
sudo wget https://gist.github.com/andzdroid/6815460/raw/78226b74ce4ce07809ad6635cb4027b593deaed6/redis-server
echo "*****************************************"
echo " 6. Move and Configure Redis-Server"
echo "*****************************************"
sudo mv redis-server /etc/init.d
sudo chmod 755 /etc/init.d/redis-server
echo "*****************************************"
echo " 7. Auto-Enable Redis-Server"
echo "*****************************************"
sudo update-rc.d redis-server defaults
echo "*****************************************"
echo " 8. Start Redis Server"
echo "*****************************************"
sudo service redis-server start
echo "*****************************************"
echo " Complete!"
echo " You can test your redis installation using the redis console:"
echo "   $ src/redis-cli"
echo "   redis> set foo bar"
echo "   OK"
echo "   redis> get foo"
echo "   bar"
echo "*****************************************"
read -p "Press [Enter] to continue..."
