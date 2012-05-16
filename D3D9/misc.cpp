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
#include "misc.h"
#include <exception>
#include <locale>
#include <iostream>
#include <sstream>
#include "main.h"
#include <fstream>

using namespace std;


// Output directory
char dlldir[320];
#define BUFFER_SIZE 1024*1024
char callbuffer[BUFFER_SIZE];
char detailbuffer[BUFFER_SIZE];

// Generic output log
ofstream ofile;	

// Call log
unsigned long D3D9CallID;
ofstream callIDLog;

// Call detail log
ofstream callDetailLog;

string currentCall;
// Texture Index

// 

void open_log( char * currdir )
{

	strcpy_s(dlldir, currdir);
	// Generic output log
	ofile.open(GetDirectoryFile("d3d9hook.log"), ios::trunc);
	cout.rdbuf(ofile.rdbuf());

	// D3D Call Log
	D3D9CallID = 0;
	callIDLog.open(GetDirectoryFile("call.log"), ios::trunc);
	callIDLog.rdbuf()->pubsetbuf(callbuffer, BUFFER_SIZE);
	clog.rdbuf(callIDLog.rdbuf());


	// D3D Call detail log
	callDetailLog.open(GetDirectoryFile("calldetail.log"), ios::trunc);
	callDetailLog.rdbuf()->pubsetbuf(detailbuffer, BUFFER_SIZE);
	cerr.rdbuf(callDetailLog.rdbuf());

}

void close_log()
{
	if(ofile)
		ofile.close();
	cout.rdbuf(NULL);

	if (callDetailLog)
	{
		callDetailLog.flush();
		callDetailLog.close();
	}

	if (callIDLog)
	{
		callIDLog.flush();
		callIDLog.close();
	}
}

unsigned long callID()
{
	return D3D9CallID;
}

unsigned long inc_callID(char* currCallID)
{
	currentCall.assign(currCallID);
	return ++D3D9CallID;
}



const char* callInfo()
{
	return currentCall.c_str();
}


void __cdecl add_log (const char *fmt, ...)
{

	if(!fmt) { return; }

	va_list va_alist;
	char logbuf[256] = {0};

	va_start (va_alist, fmt);
	_vsnprintf_s (logbuf+strlen(logbuf), sizeof(logbuf) - strlen(logbuf), sizeof(logbuf) - strlen(logbuf), fmt, va_alist);
	va_end (va_alist);

	cout << timestamp() << " - " << logbuf << endl;

}

std::string __cdecl throw_log (const char *fmt, ...)
{
	std::stringstream outbuf;

	if(!fmt) { return ""; }

	va_list va_alist;
	char logbuf[256] = {0};

	va_start (va_alist, fmt);
	_vsnprintf_s (logbuf+strlen(logbuf), sizeof(logbuf) - strlen(logbuf), sizeof(logbuf) - strlen(logbuf), fmt, va_alist);
	va_end (va_alist);

	outbuf << timestamp() << " - " << logbuf << endl;
	cout << outbuf.str();

	return outbuf.str();

}

void __cdecl call_log (const char *fmt, ...)
{

	if(!fmt) { return; }

	va_list va_alist;
	char logbuf[256] = {0};

	va_start (va_alist, fmt);
	_vsnprintf_s (logbuf+strlen(logbuf), sizeof(logbuf) - strlen(logbuf), sizeof(logbuf) - strlen(logbuf), fmt, va_alist);
	va_end (va_alist);

	clog << "\t#" << D3D9CallID << " : " << logbuf << endl;

}

void __cdecl detail_log (const char *fmt, ...)
{
	static unsigned long prevCallID = 0;

	if(!fmt) { return; }

	va_list va_alist;
	char logbuf[256] = {0};

	va_start (va_alist, fmt);
	_vsnprintf_s (logbuf+strlen(logbuf), sizeof(logbuf) - strlen(logbuf), sizeof(logbuf) - strlen(logbuf), fmt, va_alist);
	va_end (va_alist);

	if(prevCallID != callID())
	{
		// Insert a quick header
		cerr << "\n\nCall #" << callID() << " " << currentCall << " Details:" << endl;
		prevCallID = callID();
	}

	cerr << "\t" << logbuf << endl;

}

wstring widen( const string& str )
{
	wostringstream wstm ;
	const ctype<wchar_t>& ctfacet = 
		use_facet< ctype<wchar_t> >( wstm.getloc() ) ;
	for( size_t i=0 ; i<str.size() ; ++i ) 
		wstm << ctfacet.widen( str[i] ) ;
	return wstm.str() ;
}

string narrow( const wstring& str )
{
	ostringstream stm ;
	const ctype<char>& ctfacet = 
		use_facet< ctype<char> >( stm.getloc() ) ;
	for( size_t i=0 ; i<str.size() ; ++i ) 
		stm << ctfacet.narrow((std::ctype<char>::_Elem) str[i], 0 ) ;
	return stm.str() ;
}

// This function will convert a char string to a wchar_t string.
wchar_t *__stdcall char_to_wchar_t(const char *src)
{
	wchar_t *out_str = 0;
	size_t len = 0;
	int i = 0;

	if(src == 0)
	{
		return 0;
	}

	len = (strlen(src) + 1) * sizeof(wchar_t);
	out_str = (wchar_t*)malloc(len);

	// memory allocation failed
	if(!out_str)
	{
		printf("Error: Memory allocation failed.\n");
		return 0;
	}

	memset(out_str, 0, len);

	while(src[i] != '\0')
	{
		out_str[i] = (wchar_t)src[i];
		++i;
	}

	return out_str;
}

int parse_int(const char *str)
{
	int int_ret = 0;
	if(str == 0)
	{
		return 0;
	}

	// try to get dec format
	sscanf_s(str, "%d", &int_ret);

	// try to get hex format
	if(int_ret == 0 && str[0] == '0' && (str[1] == 'x' || str[1] == 'X'))
	{
		sscanf_s(str, "%x", &int_ret);
	}

	return int_ret;
}

char *alloc_stra(char *in_str)
{
	size_t len = 0;
	char *out_str = 0;

	if(in_str == 0)
	{
		return 0;
	}

	len = strlen(in_str) + 1;
	out_str = (char*)malloc(len);

	// memory allocation failed
	if(!out_str)
	{
		printf("Error: Memory allocation failed.\n");
		return 0;
	}

	// strcpy_s failed
	if(strcpy_s(out_str, len, in_str) != 0)
	{
		free(out_str);
		return 0;
	}

	return out_str;
}

std::string timestamp()
{
	char buff[20];
	time_t now = time(NULL);
	struct tm newtime;

	localtime_s(&newtime, &now);
	strftime(buff, 20, "%Y-%m-%d %H:%M:%S", &newtime);

	std::string timestr(buff);

	return timestr;
}


char *GetDirectoryFile(char *filename)
{
	static char path[320];
	strcpy_s(path, dlldir);
	strcat_s(path, filename);
	return path;
}