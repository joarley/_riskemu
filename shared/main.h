#ifndef MAIN_H
#define MAIN_H

#include "log/Logger.h"
#include "Splash.h"

#include <string>
#include <vector>

#ifdef STARTUP_CLASS

int main(int argc, char *argv[]) {
	LOG->ClearDefaultLogFile();
    LOG->ShowMessage(Splash, MODULE_NAME, 0);

	LOG->ShowInfo("Creating module\n");
    std::vector<std::string> params;
    for (int i = 0; i < argc; i++)
	{
        params.push_back(argv[i]);
    }
    STARTUP_CLASS Main(params);

    LOG->ShowInfo("Starting module configuration\n");
    if (!Main.LoadConfig())
	{
        return -1;
    }

    LOG->ShowInfo("Start module\n");
	int startRet = Main.Start();	

	LOG->ShowMessage(CL_RESET);
	Logger::DestroyInstance();
    return startRet;
}
#else
#error STARTUP_CLASS not defined
#endif

#endif
