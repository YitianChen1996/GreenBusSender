#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	FILE *file;
	int a, b;
	while (1){
		system("cp /run/user/1000/gvfs/mtp:host=%5Busb%3A001%2C006%5D/Internal\\ shared\\ storage/Download/data.txt /home/pi/Desktop/GreenBusSender/cooking/examples/LoRaWAN/phoneData.txt");
		printf("move file from android phone finished\n");
		usleep(5000000);
	}
	return 0;
}
