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

#include "nexception.h"

namespace NeHe {

NeHeException :: NeHeException()
{
	// set to unknown values
	error="Unknown Error.";
	filename="Unknown File.";
	lineno=-1;
}

NeHeException :: NeHeException(std :: string err)
{
	// copy over the error value
	error=err;
	// set to unknown values
	filename="Unknown File.";
	lineno=-1;
}

NeHeException :: NeHeException(int line,std :: string file,std :: string err)
{
	// copy over the error values
	error=err;
	filename=file;
	lineno=line;
}

};	// namespace NeHe
