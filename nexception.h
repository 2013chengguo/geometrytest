/*	Nehe SDK
	Copyright 2002 Jeff Molofee, Gregory Austwick and Others

	Title:
		Exception class

	Version History:
		v1.00	New file		(GA 24/11/02)

	Licence:

	This SDK/library is free software, written primarily for
	teaching purposes. You can redistribute and modify it to 
	your heart's content. However, any use of this library is 
	at the user's own risk. There is no warranty. The authors 
	will not be held responsible for ANYTHING that happens as 
	a result of the use of this software. All financial/emotional/health 
	problems or anything else that happens after using this 
	software is therefore the user's own responsibility. 
	Nevertheless, if the user finds it useful, an email or a 
	mention in any credits would be appreciated, but is not 
	necessary.
*/

#ifndef NEXCEPTION_H
#define NEXCEPTION_H

#include <string>

namespace NeHe {

class NeHeException {
	std :: string	error;		// the error causing the exception
	std :: string	filename;	// filename of source file containing error
	int				lineno;		// line number of the source file containing the error
public:
	NeHeException();
	NeHeException(std :: string err);
	NeHeException(int line,std :: string file,std :: string err);

	//
	//	Get error information
	//

	int				GetLine()		{ return lineno; };
	std :: string	GetFileName()	{ return filename; };
	std :: string	GetError()		{ return error; }
};

// throw macro
#define NTHROW(str)	throw NeHeException(__LINE__,__FILE__,(str))

};	// namespace NeHe

#endif