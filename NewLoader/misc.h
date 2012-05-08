////////////////////////////////////////////////////////////////////////////////////////////
// loader: command-line interface dll injector
// Copyright (C) 2011  Wadim Egorov
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MISC_H
#define _MISC_H

#include <Windows.h>
#include <stdio.h>
#include <string>

#if defined(_WIN64)

#define CHECK_TARGET_PROC(pid) IsProcess64(pid) != 0
#define PRINT_TARGET_PROC_ERROR() ThrowErrorMsgA("Error: x64 loader doesn't support x86 processes.")

#elif defined(_WIN32)

#define CHECK_TARGET_PROC(pid) IsProcess64(pid) != 1
#define PRINT_TARGET_PROC_ERROR() ThrowErrorMsgA("Error: x86 loader doesn't support x64 processes.")

#endif


void __cdecl ThrowErrorMsgA(char *format, ...);
void __cdecl ThrowErrorMsgW(wchar_t *format, ...);

#define THROW_ERROR_MSGA(...) { ThrowErrorMsgA("Exception: ", __FUNCTION__, __VA_ARGS__); }
#define THROW_ERROR_MSGW(...) { ThrowErrorMsgW(L"Exception: ", __FUNCTIONW__, __VA_ARGS__); }

std::wstring widen( const std::string& str );
std::string narrow( const std::wstring& str );

wchar_t *__stdcall char_to_wchar_t(const char *src);

char *alloc_stra(char *in_str);

int parse_int(const char *str);

#endif // _MISC_H