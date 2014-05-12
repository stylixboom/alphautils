#ifndef alphautils_VERSION_H
#define alphautils_VERSION_H

namespace alphautils_AutoVersion{
	
	//Date Version Types
	static const char alphautils_DATE[] = "11";
	static const char alphautils_MONTH[] = "05";
	static const char alphautils_YEAR[] = "2014";
	static const char alphautils_UBUNTU_VERSION_STYLE[] =  "14.05";
	
	//Software Status
	static const char alphautils_STATUS[] =  "Alpha";
	static const char alphautils_STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long alphautils_MAJOR  = 1;
	static const long alphautils_MINOR  = 4;
	static const long alphautils_BUILD  = 67;
	static const long alphautils_REVISION  = 24;
	
	//Miscellaneous Version Types
	static const long alphautils_BUILDS_COUNT  = 8;
	#define alphautils_RC_FILEVERSION 1,4,67,24
	#define alphautils_RC_FILEVERSION_STRING "1, 4, 67, 24\0"
	static const char alphautils_FULLVERSION_STRING [] = "1.4.67.24";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long alphautils_BUILD_HISTORY  = 8;
	

}
#endif //alphautils_VERSION_H
