# -*- mode: ruby -*-
# vi: set ft=ruby :

# All Vagrant configuration is done below. The "2" in Vagrant.configure
# configures the configuration version (we support older styles for
# backwards compatibility). Please don't change it unless you know what
# you're doing.
Vagrant.configure("2") do |config|
  config.vm.box = "geerlingguy/ubuntu1604"
  config.vm.synced_folder "", "/var/www/html"

   # Configurate the virtual machine to use 256MB of RAM
  config.vm.provider :virtualbox do |vb|
    vb.customize ["modifyvm", :id, "--memory", "480"]
    vb.customize [ "modifyvm", :id, "--uartmode1", "disconnected" ]
  end
  # Forward the Rails server default port to the host
  config.vm.network "forwarded_port", guest: 22, host: 2200
  config.vm.network "forwarded_port", guest: 80, host: 8081
  config.vm.network "private_network", ip: "192.168.56.43"
end
