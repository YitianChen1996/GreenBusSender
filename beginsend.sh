fuser -k /dev/ttyUSB0
tput reset > /dev/ttyUSB0
./moveFile &
sudo python ./gpstest.py &
sudo ./GreenBusSender.cpp_exe
