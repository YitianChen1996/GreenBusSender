all: moveFile GreenBusSender.cpp_exe
GreenBusSender.cpp_exe: GreenBusSender.cpp
	./cook.sh GreenBusSender.cpp
moveFile: moveFile.cpp
	g++ moveFile.cpp -o moveFile
clean:
	rm -f AdafruitGPS_main moveFile GreenBusSender.cpp_exe	
