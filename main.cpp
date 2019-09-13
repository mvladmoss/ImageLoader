#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <winsock2.h>
#include <thread>
#include <vector>

using namespace std;
char logFileName[80];
CRITICAL_SECTION csConsole;
CRITICAL_SECTION csLogFile;
int currentAddressNumber = -1;
string addressArray[100];

class ImageLink {
public: string hostName;
public: string imagePath;

    ImageLink(string hostName, string imagePath)
    {
        this -> hostName = hostName;
        this -> imagePath = imagePath;
    }
};

void WriteLogFile(string, int);
ImageLink* convertStrToLink(string inputLink);



int main()
{
    time_t epochSeconds = time(nullptr);
    tm* time = localtime(&epochSeconds);
    char format[] = "%H.%M.%S %d %b %Y.txt";
    strftime(logFileName, 80, format, time);
    CreateDirectoryA("./img", nullptr);

    string inputAddress;

    InitializeCriticalSection(&csConsole);
    InitializeCriticalSection(&csLogFile);

    vector<thread> threads;
    int clientChoose;

    cout << "Chose one of the next option:" << endl
         << "1) Input link" << endl
         << "2) Run test link-list" << endl
         << "3) Exit" << endl;
    while (true) {
        cin >> clientChoose;
        if (clientChoose == 1) {
            cin >> inputAddress;
            ImageLink* imageLink = convertStrToLink(inputAddress);
        } else if (clientChoose == 2) {
            ifstream link("./input.txt", ios::in);
            while (!link.eof()) {
                getline(link, inputAddress);
            }
            link.close();
        } else if(clientChoose == 3){
            break;
        }
    }

    for (int numLink = threads.size() - 1; numLink >= 0; numLink--) {
        if (WaitForSingleObject(reinterpret_cast<HANDLE>(threads[numLink].native_handle()), INFINITE) == WAIT_OBJECT_0) {
            threads[numLink].join();
        }
    }

    DeleteCriticalSection(&csLogFile);
    DeleteCriticalSection(&csConsole);
    system("pause");
    return 0;
}

ImageLink* convertStrToLink(string inputLink) {
    char hostName[100];
    char imagePath[100];
    char temporaryBuff[200];
    char transformBuff1[200];
    char transformBuff2[200];

    for (int i = 0; i < inputLink.length(); i++) {
        transformBuff1[i] = inputLink[i];
    }
    transformBuff1[inputLink.length()] = '\0';
    sscanf(transformBuff1, "%*[^//]%s", transformBuff2);
    sscanf(transformBuff2, "%*2c%s", transformBuff1);

    inputLink = (string)transformBuff1;

    currentAddressNumber++;
    addressArray[currentAddressNumber] = inputLink;
    int lenght = inputLink.length();
    for (int j = 0; j < lenght; j++) {
        temporaryBuff[j] = inputLink[j];
        if (temporaryBuff[j] == '/') {
            addressArray[currentAddressNumber][j] = '.';
        } else {
            addressArray[currentAddressNumber][j] = inputLink[j];
        }
    }
    temporaryBuff[lenght] = '\0';
    sscanf(temporaryBuff, "%[^/]%s", hostName, imagePath);
    return new ImageLink(hostName, imagePath);
}

void writeLogFile(string file, int bites)
{
    time_t seconds = time(NULL);
    tm* timeinfo = localtime(&seconds);
    char buffTime[] = "%H.%M.%S";
    char timeForAnswer[10];
    strftime(timeForAnswer, 80, buffTime, timeinfo);

    char answer_char[200];
    string answer;

    FILE* pFile = fopen(logFileName, "a");

    if (bites > 0) {
        answer = "File: " + file + ", Bites: " + to_string(bites) + ", Time: " + (string)timeForAnswer;
    } else if (bites < 0) {
        answer = "File: " + file + ", ERROR!" + ", Time: " + (string)timeForAnswer;
    } else {
        answer = "<---" + file + " is download" + "--->" + " Time: " + (string)timeForAnswer;
    }

    for (int j = 0; j < answer.length(); j++) {
        answer_char[j] = answer[j];
    }
    answer_char[answer.length()] = '\0';

    fprintf(pFile, "%s\n", answer_char);
    fclose(pFile);
}
