//
//  CsvFileForCocos2d.h
//
//
//  Created by OJO on 15/1/11.
//
//

#ifndef __OJO__CsvFileForCocos2d__
#define __OJO__CsvFileForCocos2d__

#include <vector>
#include <string>

class CsvFileForCocos2d
{
public:
    CsvFileForCocos2d() : _cols(0) {};
    ~CsvFileForCocos2d();
    
private:
    std::vector< std::vector<std::string> > _dataTable;
    
    int _cols;
    
    void separatedDataIntoRows(const std::string& data, std::vector<std::string>& rows, const char& sep);
    void separatedLineIntoCols(const std::string& line, std::vector<std::string>& cols, const char& sep);
    
    // quotation marks is a special symbol in the CSV
    int advQuoted(const std::string& line, std::string& fld, const char& sep, int);
    int advNormal(const std::string& line, std::string& fld, const char& sep, int);
    
public:
    bool            openFile(const char * szFileName);
    
    const char *    getData(int rowIndex, int colIndex);
    
    inline  int     getCols() { return _cols; }
    
    inline  int     getRows() { return _dataTable.size(); }
};

#endif /* defined(__OJO__CsvFileForCocos2d__) */
