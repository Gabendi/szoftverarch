#include "IOHandler.h"

bool IOHandler::getSimulationShouldPause() {
	return (GetAsyncKeyState(VK_CONTROL) & 0x01);
}

bool IOHandler::getSimulationShouldContinue() {
	return (GetAsyncKeyState(VK_SPACE) & 0x01);
}

bool IOHandler::getSimulationShouldExit() {
	return (GetAsyncKeyState(VK_ESCAPE) & 0x01);
}

bool IOHandler::getSimulationShouldIterate() {
	return (GetAsyncKeyState(VK_RIGHT) & 0x01);
}