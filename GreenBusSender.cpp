//
//                       _oo0oo_
//                      o8888888o
//                      88" . "88
//                      (| -_- |)
//                      0\  =  /0
//                    ___/`---'\___
//                  .' \\|     |// '.
//                 / \\|||  :  |||// \
//                / _||||| -:- |||||- \
//               |   | \\\  -  /// |   |
//               | \_|  ''\---/''  |_/ |
//               \  .-\__  '-'  ___/-. /
//             ___'. .'  /--.--\  `. .'___
//          ."" '<  `.___\_<|>_/___.' >' "".
//         | | :  `- \`.;`\ _ /`;.`/ - ` : | |
//         \  \ `_.   \_ __\ /__ _/   .-` /  /
//     =====`-.____`.___ \_____/___.-`___.-'=====
//                       `=---='
//
//
//     ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//               佛祖保佑         永无BUG

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <iostream>
#include "arduPiLoRaWAN.h"

using namespace std;

// set the freqency and the bus number here!!!!!!!!!!!
// 747 923100000
// 748 923300000
// 762 923500000
// 763 923700000
// reserved 922900000
uint32_t frequency = 922900000;
int busNum = 777;
uint8_t block_size[6] = {58,35,20,12,6,3}; // 6 seconds for all SF

uint8_t error;
uint8_t power = 15;
uint8_t sf_selected = 12; //same initial value as spreading_factor[]
char spreading_factor[5] = "sf12";
char coding_rate[] = "4/5";
uint16_t bandwidth = 125;
char crc_mode[] = "on";
FILE *lockFile, *dataFile;
uint8_t sock = SOCKET0;
uint8_t pkt_num = 0;

uint8_t radioModuleSetup() {
    uint8_t status = 0;
    uint8_t e = 0;

    e = LoRaWAN.ON(sock);
    if (e == 0) {
        printf("1. Switch ON OK\n");
    } else {
        printf("1. Switch ON error = %d\n", e);
        status = 1;
    }
    printf("-------------------------------------------------------\n");

    //disable LoRaWAN mode and enable  P2P mode
    e = LoRaWAN.macPause();

    if (e == 0) {
        printf("2. P2P mode enabled OK\n");
    } else {
        printf("2. Enable P2P mode error = %d\n", e);
        status = 1;
    }
    printf("-------------------------------------------------------\n");

    //set power level,different level correspond to different frequency
    e = LoRaWAN.setRadioPower(power);
    if (e == 0) {
        printf("3.1. Set Radio Power OK\n");
    } else {
        printf("3.1. Set Radio Power error = %d\n", e);
        status = 1;
    }

    //check power level
    e = LoRaWAN.getRadioPower();
    if (e == 0) {
        printf("3.2. Get Radio Power OK. ");
        printf("Power: %d\n", LoRaWAN._radioPower);
    } else {
        printf("3.2. Get Radio Power error = %d\n", e);
        status = 1;
    }
    printf("-------------------------------------------------------\n");

    // Set frequency
    e = LoRaWAN.setRadioFreq(frequency);
    printf("frequency: %d\n", frequency);
    if (e == 0) {
        printf("4.1. Set Radio Frequency OK\n");
    } else {
        printf("4.1. Set Radio Frequency error = %d\n", e);
        status = 1;
    }

    // Get frequency
    e = LoRaWAN.getRadioFreq();
    if (e == 0) {
        printf("4.2. Get Radio Frequency OK. ");
        printf("Frequency: %d\n", LoRaWAN._radioFreq);
    } else {
        printf("4.2. Get Radio Frequency error = %d\n", e);
        status = 1;
    }
    printf("-------------------------------------------------------\n");

    // Set SF
    e = LoRaWAN.setRadioSF(spreading_factor);
    if (e == 0) {
        printf("5.1. Set Radio SF OK\n");
    } else {
        printf("5.1. Set Radio SF error = %e\n", e);
        status = 1;
    }

    // Get SF
    e = LoRaWAN.getRadioSF();
    if (e == 0) {
        printf("5.2. Get Radio SF OK. ");
        printf("Spreading Factor: %s\n", LoRaWAN._radioSF);
    } else {
        printf("5.2. Get Radio SF error = %d\n", e);
        status = 1;
    }
    printf("-------------------------------------------------------\n");

    // Set CR
    e = LoRaWAN.setRadioCR(coding_rate);
    if (e == 0) {
        printf("6.1. Set Radio CR OK\n");
    } else {
        printf("6.1. Set Radio CR error = %d\n", e);
        status = 1;
    }

    // Get CR
    e = LoRaWAN.getRadioCR();
    if (e == 0) {
        printf("6.2. Get Radio CR OK. ");
        printf("Coding Rate: %s\n", LoRaWAN._radioCR);
    } else {
        printf("6.2. Get Radio CR error = %d\n", e);
        status = 1;
    }
    printf("-------------------------------------------------------\n");

    // Set BW
    e = LoRaWAN.setRadioBW(bandwidth);
    if (e == 0) {
        printf("7.1. Set Radio BW OK\n");
    } else {
        printf("7.1. Set Radio BW error = %d\n", e);
    }

    // Get BW
    e = LoRaWAN.getRadioBW();
    if (e == 0) {
        printf("7.2. Get Radio BW OK. ");
        printf("Bandwidth: %u\n", LoRaWAN._radioBW);
    } else {
        printf("7.2. Get Radio BW error = %d\n", e);
        status = 1;
    }
    printf("-------------------------------------------------------\n");

    //enable or disable crc check
    e = LoRaWAN.setRadioCRC(crc_mode);
    if (e == 0) {
        printf("8.1. Set Radio CRC mode OK\n");
    } else {
        printf("8.1. Set Radio CRC mode error = %d\n", e);
        status = 1;
    }

    // Get CRC
    e = LoRaWAN.getRadioCRC();
    if (e == 0) {
        printf("8.2. Get Radio CRC mode OK. ");
        printf("CRC status: %u\n", LoRaWAN._crcStatus);
    } else {
        printf("8.2. Get Radio CRC mode error = %e\n", e);
        status = 1;
    }
    printf("-------------------------------------------------------\n");
    printf("preamble length: %d\n", LoRaWAN.getRadioPreamble());
    return status;
}

void setup() {
    printf("Radio P2P example - Sending packets\n\n");

    error = radioModuleSetup();

    if (error == 0) {
        printf("Module configured OK\n");
    } else {
        printf("Module configured ERROR\n");
    }
}

void init() {
    setup();
}

int sendbuff(char *buff) {
    uint8_t err = LoRaWAN.sendRadio(buff);
    if (err == 0) {
        printf("--> Packet sent OK\n");
        printf("content: %s\nlength: %d\n", buff, strlen(buff));
    } else {
        printf("Error waiting for packets. error = %d\n", err);
    }
    return err;
}

void changeDoubletoIEEE(double *dp, unsigned char *&ret) {
    unsigned char *cp;
    cp = (unsigned char *)dp;
    unsigned char temp[8];
    for (int i = 0; i <= 7; i++) {
        temp[i] = cp[i];
    }
    for (int i = 0; i <= 7; i++) {
        unsigned char tempprint[2];
        sprintf((char *)tempprint, "%02X", temp[i]);
        strcat((char *)ret, (char *)tempprint);
    }
}

void addInttoBuf(int a, unsigned char *&ret) {
    unsigned char temp[8];
    sprintf((char *)temp, "%04d", a);
    strcat((char *)ret, (char *)temp);
}

void restartGPS() {
    system("fuser -k /dev/ttyUSB0");
    system("tput reset > /dev/ttyUSB0");
    system("sudo python ./gpstest.py &");
}

// bool readGPSData(double &GPSlatitudeDegrees, double &GPSlongitudeDegrees,
//                  double &GPSspeed, double &GPSangle, int &GPSFileCount) {
//     char GPSFileline[128];
//     file = fopen("./AdafruitGPSdata.txt", "r");
//     if (file == nullptr) {
//         printf("AdafruitGPSdata.txt open failed!\n");
//         return false;
//     }
//     memset(GPSFileline, 0, sizeof(GPSFileline));
//     fgets(GPSFileline, sizeof(GPSFileline), file);
//     fclose(file);
//     sscanf(GPSFileline, "%d %lf %lf %lf %lf", &GPSFileCount, &GPSlatitudeDegrees, &GPSlongitudeDegrees, &GPSspeed, &GPSangle);
//     return true;
// }

void readAndSend() {
    char PhoneLine[128];
    int PhoneFileCount, GPSFileCount, prevGPSCount = 0, sameGPSCount = 0;
    double GPSlatitudeDegrees, GPSlongitudeDegrees, GPSspeed, GPSangle, Phonelatitude, Phonelongitude, Phonespeed, Phoneangle;
    uint8_t buff[140], ackbuff[8], rxpkt[4], tmpstr[3]={0};
    uint8_t bs, i, err;
    FILE *file;
    while (1) {
        // bool success = true;
        // success = readGPSData(GPSlatitudeDegrees, GPSlongitudeDegrees, GPSspeed, GPSangle, GPSFileCount);

        // if (!ret) {
        //     printf("AdafruitGPSdata.txt open failed!\n");
        //     delay(300);
        //     continue;
        // }

        // if (GPSFileCount == prevGPSCount) {
        //     sameGPSCount++;
        //     if (sameGPSCount == 5) {
        //         printf("GPS seems dead, restarting it\n");
        //         restartGPS();
        //         sameGPSCount = 0;
        //     }
        // } else {
        //     sameGPSCount = 0;
        // }
        // prevGPSCount = GPSFileCount;
        // printf("GPS data: %d %lf %lf %lf %lf\n", GPSFileCount, GPSlatitudeDegrees, GPSlongitudeDegrees, GPSspeed, GPSangle);
        //
        file = fopen("phoneData.txt", "r");
        if (file == NULL) {
            printf("phoneData.txt open failed\n");
            delay(3210); //delay a random time
            continue;
        }
        memset(PhoneLine, 0, sizeof(PhoneLine));
        fgets(PhoneLine, sizeof(PhoneLine), file);
        fclose(file);
        sscanf(PhoneLine, "%d %lf %lf %lf %lf\n", &PhoneFileCount, &Phonelongitude, &Phonelatitude, &Phonespeed, &Phoneangle);
        printf("phone Data: %d %lf %lf %lf %lf\n", PhoneFileCount, Phonelatitude, Phonelongitude, Phonespeed, Phoneangle);
        memset(buff, 0, 140);
        pkt_num++;
        addInttoBuf(busNum, buff);
        addInttoBuf(pkt_num, buff);
        // changeDoubletoIEEE(&GPSlatitudeDegrees, buff);
        // changeDoubletoIEEE(&GPSlongitudeDegrees, buff);
        // changeDoubletoIEEE(&GPSspeed, buff);
        // changeDoubletoIEEE(&GPSangle, buff);
        changeDoubletoIEEE(&Phonelatitude, buff);
        changeDoubletoIEEE(&Phonelongitude, buff);
        changeDoubletoIEEE(&Phonespeed, buff);
        changeDoubletoIEEE(&Phoneangle, buff);
        sendbuff((char *)buff);
        //delay(1000);

        /* Request for SF */
        bs = block_size[sf_selected - 7];
        if (bs == 0 || pkt_num < bs) continue;
        pkt_num = 0;
        LoRaWAN.setRadioSF((char *)"sf12");
        LoRaWAN.setRadioCR((char *)"4/8");
        sprintf(ackbuff, "CAAC%02X", busNum % 100);
        sendbuff(ackbuff);
        err = LoRaWAN.receiveRadio(2000);
        if (err == 0) {
            for (i=0; i<4; i++) {
                memcpy(tmpstr, LoRaWAN._buffer + i*2, 2);
                sscanf((char *)tmpstr, "%X", rxpkt + i);
            }
            if (rxpkt[0]!=0xAC || rxpkt[1]!=0xCA || rxpkt[2]!=(busNum % 100)) {
                printf("RESPONSE missed\n");
                continue;
            }
            sf_selected = rxpkt[3];
            printf("RESPONSE SF = %u\n", sf_selected);
            sprintf(spreading_factor, "sf%u", sf_selected);
        }
        else if (err == 1) printf("RX error\n");
        else if (err == 2) printf("RESPONSE missed\n");
        LoRaWAN.setRadioSF(spreading_factor);
        LoRaWAN.setRadioCR(coding_rate);
        //delay(1000);
    }
}

int main() {
    init();
    readAndSend();
    return 0;
}
