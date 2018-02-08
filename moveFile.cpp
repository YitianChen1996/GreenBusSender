#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <vector>

using namespace std;

string portNum, busNum;

struct port {
    int portNum, busNum;
    string portName;
};

vector<string> splitString(const string &origs) {
    vector<string> v;
    int p1 = 0, p2 = 0;
    for (int i = 0; i <= (int)origs.length() - 1; i++) {
        if (origs[i] == ' ') {
            v.push_back(origs.substr(p1, i - p1));
            p1 = i + 1;
        }
    }
    return v;
}

bool updateNum(const vector<string> &v) {
    for (int i = 0; i <= (int)v.size() - 1; i++) {
        if (v[i] == "Google") {
            portNum = v[3].substr(0, (int)v[3].length() - 1);
            busNum = v[1];
            return true;
        }
    }
    return false;
}

void findPort() {
    system("lsusb > portList.txt");
    ifstream fp("portList.txt");
    string thisLine;
    vector<string> v;
    while (getline(fp, thisLine)) {
        v.clear();
        v = splitString(thisLine);
        if (updateNum(v)) {
            break;
        }
    }
    fp.close();
}

bool checkValidFile() { //to check phoneData.txt is a valid file, if not, make up some data to satisfy the main program
    ifstream fp("phoneData.txt");
    string thisLine;
    int cnt = 0;
    getline(fp, thisLine);
    for (int i = 0; i <= (int)thisLine.length() - 1; i++) {
        if (thisLine[i] == ' ') {
            cnt++;
        }
    }
    fp.close();
    if (cnt >= 4) {
        return true;
    } else {
        ofstream fp("phoneData.txt");
        fp << "0 0 0 0 0";
        fp.close();
		return false;
    }
	return false;
}

int main() {
    FILE *file;
    int a, b;
    while (1) {
        findPort();
        string cmds = "cp /run/user/1000/gvfs/mtp:host=%5Busb%3A" + busNum +
                      "%2C" + portNum +
                      "%5D/Internal\\ shared\\ storage/Download/data.txt /home/pi/Desktop/GreenBusSender/phoneData.txt";
        cout << busNum << endl;
        cout << portNum << endl;
        char *cmdc;
        cmdc = (char *)cmds.data();
        system(cmdc);
		checkValidFile();
        usleep(5000000);
    }
    return 0;
}
