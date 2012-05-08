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

using namespace std;


void __cdecl PrintErrorMsgA(char *format, ...)
{
	va_list ap;
	DWORD dwLastError = 0;
	va_start(ap, format);
	
	//printf("Error: ");
	vprintf(format, ap);
	va_end(ap);

	dwLastError = GetLastError();
	if(dwLastError)
	{
		printf(" [LastError: %d]", GetLastError());
	}
	SetLastError(0);
	printf("\n");
}

void __cdecl PrintErrorMsgW(wchar_t *format, ...)
{
	va_list ap;
	DWORD dwLastError = 0;
	va_start(ap, format);
	
	//printf("Error: ");
	vwprintf(format, ap);
	va_end(ap);

	dwLastError = GetLastError();
	if(dwLastError)
	{
		printf(" [LastError: %d]", GetLastError());
	}
	SetLastError(0);
	printf("\n");
}

void __cdecl ThrowErrorMsgA(char *format, ...)
{
	va_list ap;
	DWORD dwLastError = 0;
	std::stringstream error_msg;
	error_msg << format;
	va_start(ap, format);

	error_msg << ap;
	
	va_end(ap);

	dwLastError = GetLastError();
	if(dwLastError)
	{
		error_msg << "\n\tLastError: " << GetLastError();
	}
	SetLastError(0);
	error_msg << "\n";

	throw exception(error_msg.str().c_str());

}

void __cdecl ThrowErrorMsgW(wchar_t *format, ...)
{
	va_list ap;
	DWORD dwLastError = 0;
	std::wstringstream error_msg;
	error_msg << format;
	va_start(ap, format);

	error_msg << ap;

	va_end(ap);

	dwLastError = GetLastError();
	if(dwLastError)
	{
		error_msg << L"\n\tLastError: " << GetLastError();
	}
	SetLastError(0);
	error_msg << L"\n";

	throw exception(narrow(error_msg.str()).c_str());


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
		stm << ctfacet.narrow( str[i], 0 ) ;
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