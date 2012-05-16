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
#include <exception>
#include <sstream>
#include <iostream>

// #define EXTREME_LOGGING


#ifdef EXTREME_LOGGING
	
	#define INC_CALL_LOG() std::clog << "Call #" << inc_callID(__FUNCSIG__) << ":" << __FUNCSIG__ << std::endl; std::clog.flush();
	#define RESULT_LOG() detail_log("HRESULT = %s", ToString(result).c_str())
	
#else
	
	#define INC_CALL_LOG() inc_callID(__FUNCSIG__)
	#define RESULT_LOG() if( result != D3D_OK) { detail_log("HRESULT = %s", ToString(result).c_str()); }

#endif

#define CALL_LOG() std::clog << "Call #" << callID() << ":" << __FUNCSIG__ << std::endl; 

void open_log(char * dlldir);
void close_log();

void __cdecl add_log (const char * fmt, ...);


unsigned long callID();
unsigned long inc_callID(char* currCallID);
const char* callInfo();

void __cdecl call_log (const char *fmt, ...);
void __cdecl detail_log (const char *fmt, ...);

std::string __cdecl throw_log(const char *fmt, ...);

std::string timestamp();

std::wstring widen( const std::string& str );
std::string narrow( const std::wstring& str );

wchar_t *__stdcall char_to_wchar_t(const char *src);

char *alloc_stra(char *in_str);

int parse_int(const char *str);
char *GetDirectoryFile(char *filename);
#endif // _MISC_H