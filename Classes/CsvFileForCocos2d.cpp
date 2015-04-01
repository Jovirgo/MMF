//
//  CsvFileForCocos2d.cpp
//
//
//  Created by OJO on 15/1/11.
//
//

#include "CsvFileForCocos2d.h"
#include "cocos2d.h"

using namespace std;
USING_NS_CC;

bool CsvFileForCocos2d::openFile(const char * szFileName)
{
    string fullPath = FileUtils::getInstance()->fullPathForFilename(szFileName);
    
    unsigned char * pBuffer = nullptr;
    ssize_t         nBufferSize = 0;
    
    pBuffer = FileUtils::getInstance()->getFileData(fullPath.c_str(), "r", &nBufferSize);
    
    string strBuffer = (char *)pBuffer;
    string strData = strBuffer.substr(0, nBufferSize);
    
    vector<string> rows;
    separatedDataIntoRows(strData, rows, '\n');
    
    for (int index = 0
         ; index != rows.size()
         ; index++)
    {
        vector<string> cols;
        separatedLineIntoCols(rows[index], cols, ',');
        _dataTable.push_back(cols);
        
        _cols = max(_cols, (int)cols.size());
    }
    
    return true;
}

const char * CsvFileForCocos2d::getData(int rowIndex, int colIndex)
{
    if (rowIndex < 0
        || rowIndex >= _dataTable.size()
        || colIndex < 0
        || colIndex >= _dataTable[rowIndex].size())
    {
        return "";
    }
    
    return _dataTable[rowIndex][colIndex].c_str();
}

void CsvFileForCocos2d::separatedDataIntoRows(const string & data, vector<string> & rows, const char& sep)
{
    string::size_type headPos = data.find_first_not_of(sep, 0);
    string::size_type tailPos = data.find_first_of(sep, headPos);
    
    while (string::npos != tailPos || string::npos != headPos)
    {
        rows.push_back(data.substr(headPos, tailPos-headPos));
        
        headPos = data.find_first_not_of(sep, tailPos);
        tailPos = data.find_first_of(sep, headPos);
    }
}

void CsvFileForCocos2d::separatedLineIntoCols(const string & line, vector<string> & cols, const char& sep)
{
    if (line.length() == 0)
    {
        return;
    }
    
    string field;
    
    int i = 0;
    int j = 0;
    
    do
    {
        if (i < line.length() && line[i] == '"')
        {
            j = advQuoted(line, field, sep, ++i);
        }
        else
        {
            j = advNormal(line, field, sep, i);
        }
        
        cols.push_back(field);
        i = j + 1;
        
    } while (j < line.length());
}

int CsvFileForCocos2d::advQuoted(const string & line, string & fld, const char & sep, int i)
{
    int j;
    fld = "";
    
    for (j = i; j < line.length(); j++)
    {
        if (line[j] == '"' && line[++j] != '"')
        {
            int k = line.find_first_of(sep, j);
            
            if (k > line.length())
            {
                k = line.length();
            }
            
            for (k -= j; k > 0; k--)
            {
                fld += line[j++];
            }
            
            break;
        }
        
        fld += line[j];
    }
    
    return j;
}

int CsvFileForCocos2d::advNormal(const string & line, string & fld, const char & sep, int i)
{
    int j = line.find_first_of(sep, i);
    
    if (j > line.length())
    {
        j = line.length();
    }
    
    fld = string(line, i, j-i);
    return j;
}


CsvFileForCocos2d::~CsvFileForCocos2d()
{
    for (int i = 0; i < _dataTable.size(); i++)
    {
        _dataTable[i].clear();
    }
    
    _dataTable.clear();
}

