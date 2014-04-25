#ifndef alphautils_VERSION_H
#define alphautils_VERSION_H

namespace alphautils_AutoVersion{
	
	//Date Version Types
	static const char alphautils_DATE[] = "24";
	static const char alphautils_MONTH[] = "04";
	static const char alphautils_YEAR[] = "2014";
	static const char alphautils_UBUNTU_VERSION_STYLE[] =  "14.04";
	
	//Software Status
	static const char alphautils_STATUS[] =  "Alpha";
	static const char alphautils_STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long alphautils_MAJOR  = 1;
	static const long alphautils_MINOR  = 4;
	static const long alphautils_BUILD  = 59;
	static const long alphautils_REVISION  = 0;
	
	//Miscellaneous Version Types
	static const long alphautils_BUILDS_COUNT  = 1;
	#define alphautils_RC_FILEVERSION 1,4,59,0
	#define alphautils_RC_FILEVERSION_STRING "1, 4, 59, 0\0"
	static const char alphautils_FULLVERSION_STRING [] = "1.4.59.0";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long alphautils_BUILD_HISTORY  = 0;
	

}
#endif //alphautils_VERSION_H
