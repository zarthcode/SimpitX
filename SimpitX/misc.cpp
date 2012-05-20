

#include "stdafx.h"

#include "misc.h"

using namespace std;


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