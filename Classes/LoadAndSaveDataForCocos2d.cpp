//
//  LoadAndSaveDataForCocos2d.cpp
//
//
//  Created by OJO on 13-6-5.
//
//

#include "LoadAndSaveDataForCocos2d.h"

#include "cocos2d.h"

USING_NS_CC;

std::string EncryptionByBase64(unsigned char const* , unsigned int len);
std::string DecryptionByBase64(std::string const& s);




//--------------------------------
//
// Save data
//
//--------------------------------
void saveDataByKey_int(const char * szKey, int iData, bool bEncryption)
{
    auto userDefault = CCUserDefault::sharedUserDefault();
    
    if (bEncryption)
    {
        char szBuffer[20];
        sprintf(szBuffer, "%d", iData);
        std::string strData(szBuffer);
        std::string strCode = EncryptionByBase64(
                                                 reinterpret_cast<const unsigned char *>(strData.c_str()),
                                                 strData.length()
                                                 );
        
        userDefault->setStringForKey(szKey, strCode);
    }
    else
    {
        userDefault->setIntegerForKey(szKey, iData);
    }
    
    userDefault->flush();
}

void saveDataByKey_float(const char * szKey, float fData, bool bEncryption)
{
    auto userDefault = CCUserDefault::sharedUserDefault();
    
    if (bEncryption)
    {
        char szBuffer[20];
        sprintf(szBuffer, "%f", fData);
        std::string strData(szBuffer);
        std::string strCode = EncryptionByBase64(
                                                 reinterpret_cast<const unsigned char *>(strData.c_str()),
                                                 strData.length()
                                                 );
        
        userDefault->setStringForKey(szKey, strCode);
    }
    else
    {
        userDefault->setFloatForKey(szKey, fData);
    }
    
    userDefault->flush();
}

void saveDataByKey_bool(const char * szKey, bool bData, bool bEncryption)
{
    auto userDefault = CCUserDefault::sharedUserDefault();
    
    if (bEncryption)
    {
        const char * szBuffer = bData ? "true" : "false";
        std::string strData(szBuffer);
        std::string strCode = EncryptionByBase64(
                                                 reinterpret_cast<const unsigned char *>(strData.c_str()),
                                                 strData.length()
                                                 );
        
        userDefault->setStringForKey(szKey, strCode);
    }
    else
    {
        userDefault->setBoolForKey(szKey, bData);
    }
    
    userDefault->flush();
}

void saveDataByKey_string(const char * szKey, std::string strData, bool bEncryption)
{
    auto userDefault = CCUserDefault::sharedUserDefault();
    
    if (bEncryption)
    {
        std::string strCode = EncryptionByBase64(
                                                 reinterpret_cast<const unsigned char *>(strData.c_str()),
                                                 strData.length()
                                                 );
        
        userDefault->setStringForKey(szKey, strCode);
    }
    else
    {
        userDefault->setStringForKey(szKey, strData);
    }
    
    userDefault->flush();
}




//--------------------------------
//
// Load data
//
//--------------------------------
int loadDataByKey_int(const char * szKey, bool bDecryption)
{
    auto userDefault = CCUserDefault::sharedUserDefault();
    
    if (bDecryption)
    {
        std::string strCode = userDefault->getStringForKey(szKey);
        std::string strData = DecryptionByBase64(strCode);
        
        return atoi(strData.c_str());
    }
    else
    {
        return userDefault->getIntegerForKey(szKey);
    }
}

float loadDataByKey_float(const char * szKey, bool bDecryption)
{
    auto userDefault = CCUserDefault::sharedUserDefault();
    
    if (bDecryption)
    {
        std::string strCode = userDefault->getStringForKey(szKey);
        std::string strData = DecryptionByBase64(strCode);
        
        return atof(strData.c_str());
    }
    else
    {
        return userDefault->getFloatForKey(szKey);
    }
}

bool loadDataByKey_bool(const char * szKey, bool bDecryption)
{
    auto userDefault = CCUserDefault::sharedUserDefault();
    
    if (bDecryption)
    {
        std::string strCode = userDefault->getStringForKey(szKey);
        std::string strData = DecryptionByBase64(strCode);
        
        return strData == std::string("true");
    }
    else
    {
        return userDefault->getBoolForKey(szKey);
    }
}

std::string loadDataByKey_string(const char * szKey, bool bDecryption)
{
    auto userDefault = CCUserDefault::sharedUserDefault();
    
    if (bDecryption)
    {
        std::string strCode = userDefault->getStringForKey(szKey);
        std::string strData = DecryptionByBase64(strCode);
        
        return strData;
    }
    else
    {
        return userDefault->getStringForKey(szKey);
    }
}





//--------------------------------------------------
//
// Encryption / Decryption
//
//--------------------------------------------------

static const std::string dataChars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";


static inline bool baseData(unsigned char c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string EncryptionByBase64(unsigned char const* bytes_to_encode, unsigned int in_len)
{
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    
    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            
            for(i = 0; (i <4) ; i++)
                ret += dataChars[char_array_4[i]];
            i = 0;
        }
    }
    
    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';
        
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;
        
        for (j = 0; (j < i + 1); j++)
            ret += dataChars[char_array_4[j]];
        
        while((i++ < 3))
            ret += '=';
        
    }
    
    return ret;
    
}

std::string DecryptionByBase64(std::string const& encoded_string)
{
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;
    
    while (in_len-- && ( encoded_string[in_] != '=') && baseData(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i ==4) {
            for (i = 0; i <4; i++)
                char_array_4[i] = dataChars.find(char_array_4[i]);
            
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            
            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }
    
    if (i) {
        for (j = i; j <4; j++)
            char_array_4[j] = 0;
        
        for (j = 0; j <4; j++)
            char_array_4[j] = dataChars.find(char_array_4[j]);
        
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        
        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }
    
    return ret;
}




