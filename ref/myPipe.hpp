#ifndef PIPE_HPP_INCLUDED
#define PIPE_HPP_INCLUDED


void connectPipe(HANDLE &tar, const char* pipeName);

void createPipe(HANDLE &tar, const char* pipeName);

void getMsg(char* str, HANDLE &tar);

void sendMsg(HANDLE &tar, const char* value);

#endif // PIPE_HPP_INCLUDED
