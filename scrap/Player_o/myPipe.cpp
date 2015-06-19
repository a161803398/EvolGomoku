#include <stdio.h>
#include <windows.h>
#include <string.h>

void connectPipe(HANDLE &tar, const char* pipeName) {
    tar = CreateFile(TEXT(pipeName),GENERIC_WRITE,
                     0,
                     NULL,
                     OPEN_EXISTING,
                     FILE_ATTRIBUTE_NORMAL,
                     NULL
                    );
}

void createPipe(HANDLE &tar, const char* pipeName) {
    tar = CreateNamedPipe(TEXT(pipeName), 	// Name
                          PIPE_ACCESS_DUPLEX | WRITE_DAC, // OpenMode
                          PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT, // PipeMode
                          2, // MaxInstances
                          1024, // OutBufferSize
                          1024, // InBuffersize
                          2000, // TimeOut
                          NULL); // Security
}



void getMsg(char* str, HANDLE &tar) {
    DWORD dwRead;
    char buffer[1024];


    for (;;) {
        if (!ReadFile(tar, buffer, sizeof(buffer), &dwRead, NULL)) {
            printf("ReadFile failed -- probably EOF\n");
            break;
        }
        //printf("%s\n", buffer);
        if(buffer[0] != '\0') {
            strcpy(str, buffer);
            return;
        }
    }
    /*

        while (ReadFile(tar, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE) {
            buffer[dwRead] = '\0';

            if(buffer[0] != '\0') {
                //printf("%s\n", buffer);
                strcpy(str, buffer);
                return;
            }
        }*/
}

void sendMsg(HANDLE &tar, const char* value) {
    DWORD dwWritten;
    char buffer[1024];
    strcpy(buffer, value);
    if(!WriteFile(tar, value, sizeof(buffer), &dwWritten, NULL)) {
        printf("WriteFile failed\n");
    }
    //CloseHandle(tar);
}
