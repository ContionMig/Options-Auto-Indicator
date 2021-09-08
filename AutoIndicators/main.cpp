// AutoIndicators.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "includes.h"

int main()
{

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Threads::Setup, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Console::Setup, NULL, NULL, NULL);

	while (true) {
		std::this_thread::sleep_for(std::chrono::seconds(20000));
	}
}