#pragma once
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <windows.h>
#include <string>

bool validatePositiveNumber(const std::string& input);
bool validateFilename(const std::string& filename);
bool validateChoice(const std::string& choice);

void getNumber(int& number, const std::string& welcomeMessage);
void getFilename(std::string& filename);
void getChoice(int& choice, const std::string& options);

HANDLE createMutex(const std::wstring& name);
HANDLE createSemaphore(const std::wstring& name, int initialCount, int maxCount);
HANDLE openMutex(const std::wstring& name);
HANDLE openSemaphore(const std::wstring& name);

bool waitForMultipleHandles(HANDLE* handles, int count, DWORD timeout = INFINITE);
bool waitForSingleHandle(HANDLE handle, DWORD timeout = INFINITE);

#endif // FUNCTIONS_H