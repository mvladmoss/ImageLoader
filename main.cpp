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

void WriteLogFile(string, int);

int main()
{
    time_t seconds = time(nullptr);
    tm* tm = localtime(&seconds);
    char format[] = "%H.%M.%S %d %b %Y.txt";
    strftime(logFileName, 80, format, tm);
    CreateDirectoryA("./img", nullptr);

    string str;

    InitializeCriticalSection(&csConsole);
    InitializeCriticalSection(&csLogFile);

    vector<thread> threads;
    int var;

    cout << "Chose need:" << endl
         << "1. Input link" << endl
         << "2. Run test link-list" << endl
         << "3. Exit" << endl;
    while (true) {
        cin >> var;
        if (var == 1) {
            cin >> str;
        } else if (var == 2) {
            ifstream link("./input.txt", ios::in);
            while (!link.eof()) {
                getline(link, str);
            }
            link.close();
        } else if(var == 3){
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

void WriteLogFile(string file, int bites)
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