// SimpitX.h
#pragma once

#ifndef SIMPITX_H
#define SIMPITX_H

#include <string>
#include <Windows.h>

#include "ipc.h"

// IPC DLL for SimpitX plugins

#define EXTERN_DLL_EXPORT __declspec(dllexport)


// IPC Factory
EXTERN_DLL_EXPORT IPC::Server* CreateServer(std::string connectionName);

EXTERN_DLL_EXPORT IPC::Client* CreateClient(std::string connectionName);


#endif