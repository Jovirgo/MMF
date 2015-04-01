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
void saveDataByKey_int(const char* szKey, int iData, bool bEncryption = false);

void saveDataByKey_float(const char* szKey, float fData, bool bEncryption = false);

void saveDataByKey_bool(const char* szKey, bool bData, bool bEncryption = false);

void saveDataByKey_string(const char* szKey, std::string strData, bool bEncryption = false);





//--------------------------------
//
// Load data
//
//--------------------------------
int loadDataByKey_int(const char* szKey, bool bDecryption);

float loadDataByKey_float(const char* szKey, bool bDecryption);

bool loadDataByKey_bool(const char* szKey, bool bDecryption);

std::string loadDataByKey_string(const char* szKey, bool bDecryption);


#endif /* defined(__OJO__LoadAndSaveDataForCocos2d__) */
