//
//  LoadAndSaveDataForCocos2d.h
//
//  Created by OJO on 13-6-5.
//
//

#ifndef __OJO__LoadAndSaveDataForCocos2d__
#define __OJO__LoadAndSaveDataForCocos2d__

#include <string>

//--------------------------------
//
// Save data
//
//--------------------------------

/**
* when key not found, return 0
*/
void saveDataByKey_int(const char* szKey, int iData, bool bEncryption = false);

/**
* when key not found, return 0.0
*/
void saveDataByKey_float(const char* szKey, float fData, bool bEncryption = false);

/**
* when key not found, return false
*/
void saveDataByKey_bool(const char* szKey, bool bData, bool bEncryption = false);

/**
* when key not found, return ""
*/
void saveDataByKey_string(const char* szKey, std::string strData, bool bEncryption = false);

void saveDataFlush();



//--------------------------------
//
// Load data
//
//--------------------------------
int loadDataByKey_int(const char* szKey, bool bDecryption = false);

float loadDataByKey_float(const char* szKey, bool bDecryption = false);

bool loadDataByKey_bool(const char* szKey, bool bDecryption = false);

std::string loadDataByKey_string(const char* szKey, bool bDecryption = false);


#endif /* defined(__OJO__LoadAndSaveDataForCocos2d__) */
