# cshass

# [Install Hassbian](https://home-assistant.io/getting-started/hassbian-installation/)


## Install InfluxDB
```sh
wget https://dl.influxdata.com/influxdb/releases/influxdb_1.2.0_armhf.deb
sudo dpkg -i influxdb_1.2.0_armhf.deb
sudo service influxd start

influx
CREATE DATABASE home_assistant
```
## Install Grafana
```sh
wget https://github.com/fg2it/grafana-on-raspberry/releases/download/v4.1.2/grafana_4.1.2-1487023783_armhf.deb
sudo dpkg -i grafana_4.1.2-1487023783_armhf.deb
sudo /bin/systemctl daemon-reload
sudo /bin/systemctl enable grafana-server
sudo /bin/systemctl start grafana-server
```

## Install Mosquitto
```sh
sudo wget http://repo.mosquitto.org/debian/mosquitto-repo.gpg.key
sudo apt-key add mosquitto-repo.gpg.key
sudo rm mosquitto-repo.gpg.key

cd /etc/apt/sources.list.d/

sudo wget http://repo.mosquitto.org/debian/mosquitto-jessie.list

sudo apt-get update
sudo apt-get install mosquitto

sudo nano /etc/mosquitto/conf.d/mosquitto.conf
```
Paste following and change to your needs
```sh
max_queued_messages 1000
message_size_limit 0
allow_zero_length_clientid true
allow_duplicate_messages false

port 1883
protocol mqtt

listener 9001
protocol websockets

autosave_interval 900
autosave_on_changes false

persistence_file mosquitto.db

allow_anonymous false
password_file /etc/mosquitto/passwd
```

Generate user `admin` for MQTT
```
sudo /usr/bin/mosquitto_passwd -c /etc/mosquitto/passwd admin
sudo systemctl enable mosquitto.service
sudo systemctl start mosquitto.service
```

## Install SAMBA
```sh
sudo apt-get install samba samba-common-bin
sudo nano /etc/samba/smb.conf
```
Append to the end:
```ini
[homeassistant]
path = /home/homeassistant/.homeassistant/
read only = no
browsable = yes
writable = yes
valid users = homeassistant
user = homeassistant
```
Create user `homeassistant`and password
```sh
sudo smbpasswd -a homeassistant
```
And restart
```sh
sudo update-rc.d smbd enable
sudo update-rc.d nmbd enable
sudo /etc/init.d/samba restart
```

## Install MySensors gateway
```sh
git clone https://github.com/mysensors/MySensors.git --branch master
cd MySensors
```
Add this to ./configure
```cpp
--my-mqtt-user=*)
    CPPFLAGS="-DMY_MQTT_USER=\\\"${optarg}\\\" $CPPFLAGS"
    ;;
--my-mqtt-password=*)
    CPPFLAGS="-DMY_MQTT_PASSWORD=\\\"${optarg}\\\" $CPPFLAGS"
    ;;
```
After this
```cpp
--my-signing-request-gw-signatures-from-all*)
        signing_request_signatures=true
        CPPFLAGS="-DMY_SIGNING_GW_REQUEST_SIGNATURES_FROM_ALL $CPPFLAGS"
        ;;
```
```sh
./configure --my-gateway=mqtt --my-controller-ip-address=127.0.0.1 --my-port=1883 --my-mqtt-publish-topic-prefix=mysensors-out --my-mqtt-subscribe-topic-prefix=mysensors-in --my-mqtt-client-id=mygateway --my-mqtt-user=admin --my-mqtt-password=admin --my-transport=nrf24 --my-rf24-irq-pin=15
make
sudo make install
sudo systemctl enable mysgw.service
sudo systemctl start mysgw.service
```


## To install LIRC
```sh
sudo apt-get install lirc liblircclient-dev
sudo nano /etc/lirc/hardware.conf
```
Paste following:

```
########################################################
# /etc/lirc/hardware.conf
#
# Arguments which will be used when launching lircd
LIRCD_ARGS="--uinput"

# Don't start lircmd even if there seems to be a good config file
# START_LIRCMD=false

# Don't start irexec, even if a good config file seems to exist.
# START_IREXEC=false

# Try to load appropriate kernel modules
LOAD_MODULES=true

# Run "lircd --driver=help" for a list of supported drivers.
DRIVER="default"
# usually /dev/lirc0 is the correct setting for systems using udev
DEVICE="/dev/lirc0"
MODULES="lirc_rpi"

# Default configuration files for your hardware if any
LIRCD_CONF=""
LIRCMD_CONF=""
########################################################
```

```sh
sudo nano /etc/modules
```
Append to the end:
```
lirc_dev
lirc_rpi gpio_in_pin=16 gpio_out_pin=20
```

```sh
sudo nano /etc/lirc/lircd.conf
# paste content of lircd.conf from this repo

sudo nano /etc/lirc/lircrc
# paste content of .lircrc from this repo
```

```sh
sudo /etc/init.d/lirc stop
sudo /etc/init.d/lirc start
sudo nano /boot/config.txt
```
Append to the end:
```
dtoverlay=lirc-rpi,gpio_in_pin=16,gpio_out_pin=20,gpio_in_pull=up
```

## GPIO Serial port
```sh
sudo nano /boot/config.txt
#find enable_uart=0 and change it to
enable_uart=1
```
```sh
sudo nano /boot/cmdline.txt
#remove the word phase
"console=serial0,115200" or "console=ttyAMA0,115200"
```

### If you want to test serial port
```sh
sudo apt-get install minicom
minicom -b 57600 -o -D /dev/serial0

# to exit press ctrl+a x
```
## Enable serial port to `homeassistant` user
```
sudo usermod -G dialout -a homeassistant
```

## Install `NMAP`
```sh
sudo apt-get install nmap
```
