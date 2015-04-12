//
//  NKStringUtil.cpp
//  NodeKittenX
//
//  Created by Leif Shackelford on 1/18/15.
//  Copyright (c) 2015 structuresound. All rights reserved.
//

#include "NKStringUtil.h"

#include <cctype>
#include <bitset>

namespace Poco {
    
    
    const int Ascii::CHARACTER_PROPERTIES[128] =
    {
        /* 00 . */ ACP_CONTROL,
        /* 01 . */ ACP_CONTROL,
        /* 02 . */ ACP_CONTROL,
        /* 03 . */ ACP_CONTROL,
        /* 04 . */ ACP_CONTROL,
        /* 05 . */ ACP_CONTROL,
        /* 06 . */ ACP_CONTROL,
        /* 07 . */ ACP_CONTROL,
        /* 08 . */ ACP_CONTROL,
        /* 09 . */ ACP_CONTROL | ACP_SPACE,
        /* 0a . */ ACP_CONTROL | ACP_SPACE,
        /* 0b . */ ACP_CONTROL | ACP_SPACE,
        /* 0c . */ ACP_CONTROL | ACP_SPACE,
        /* 0d . */ ACP_CONTROL | ACP_SPACE,
        /* 0e . */ ACP_CONTROL,
        /* 0f . */ ACP_CONTROL,
        /* 10 . */ ACP_CONTROL,
        /* 11 . */ ACP_CONTROL,
        /* 12 . */ ACP_CONTROL,
        /* 13 . */ ACP_CONTROL,
        /* 14 . */ ACP_CONTROL,
        /* 15 . */ ACP_CONTROL,
        /* 16 . */ ACP_CONTROL,
        /* 17 . */ ACP_CONTROL,
        /* 18 . */ ACP_CONTROL,
        /* 19 . */ ACP_CONTROL,
        /* 1a . */ ACP_CONTROL,
        /* 1b . */ ACP_CONTROL,
        /* 1c . */ ACP_CONTROL,
        /* 1d . */ ACP_CONTROL,
        /* 1e . */ ACP_CONTROL,
        /* 1f . */ ACP_CONTROL,
        /* 20   */ ACP_SPACE | ACP_PRINT,
        /* 21 ! */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 22 " */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 23 # */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 24 $ */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 25 % */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 26 & */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 27 ' */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 28 ( */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 29 ) */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 2a * */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 2b + */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 2c , */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 2d - */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 2e . */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 2f / */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 30 0 */ ACP_DIGIT | ACP_HEXDIGIT | ACP_GRAPH | ACP_PRINT,
        /* 31 1 */ ACP_DIGIT | ACP_HEXDIGIT | ACP_GRAPH | ACP_PRINT,
        /* 32 2 */ ACP_DIGIT | ACP_HEXDIGIT | ACP_GRAPH | ACP_PRINT,
        /* 33 3 */ ACP_DIGIT | ACP_HEXDIGIT | ACP_GRAPH | ACP_PRINT,
        /* 34 4 */ ACP_DIGIT | ACP_HEXDIGIT | ACP_GRAPH | ACP_PRINT,
        /* 35 5 */ ACP_DIGIT | ACP_HEXDIGIT | ACP_GRAPH | ACP_PRINT,
        /* 36 6 */ ACP_DIGIT | ACP_HEXDIGIT | ACP_GRAPH | ACP_PRINT,
        /* 37 7 */ ACP_DIGIT | ACP_HEXDIGIT | ACP_GRAPH | ACP_PRINT,
        /* 38 8 */ ACP_DIGIT | ACP_HEXDIGIT | ACP_GRAPH | ACP_PRINT,
        /* 39 9 */ ACP_DIGIT | ACP_HEXDIGIT | ACP_GRAPH | ACP_PRINT,
        /* 3a : */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 3b ; */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 3c < */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 3d = */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 3e > */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 3f ? */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 40 @ */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 41 A */ ACP_HEXDIGIT | ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 42 B */ ACP_HEXDIGIT | ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 43 C */ ACP_HEXDIGIT | ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 44 D */ ACP_HEXDIGIT | ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 45 E */ ACP_HEXDIGIT | ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 46 F */ ACP_HEXDIGIT | ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 47 G */ ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 48 H */ ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 49 I */ ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 4a J */ ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 4b K */ ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 4c L */ ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 4d M */ ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 4e N */ ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 4f O */ ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 50 P */ ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 51 Q */ ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 52 R */ ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 53 S */ ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 54 T */ ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 55 U */ ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 56 V */ ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 57 W */ ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 58 X */ ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 59 Y */ ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 5a Z */ ACP_ALPHA | ACP_UPPER | ACP_GRAPH | ACP_PRINT,
        /* 5b [ */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 5c \ */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 5d ] */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 5e ^ */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 5f _ */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 60 ` */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 61 a */ ACP_HEXDIGIT | ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 62 b */ ACP_HEXDIGIT | ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 63 c */ ACP_HEXDIGIT | ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 64 d */ ACP_HEXDIGIT | ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 65 e */ ACP_HEXDIGIT | ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 66 f */ ACP_HEXDIGIT | ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 67 g */ ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 68 h */ ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 69 i */ ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 6a j */ ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 6b k */ ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 6c l */ ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 6d m */ ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 6e n */ ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 6f o */ ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 70 p */ ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 71 q */ ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 72 r */ ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 73 s */ ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 74 t */ ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 75 u */ ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 76 v */ ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 77 w */ ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 78 x */ ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 79 y */ ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 7a z */ ACP_ALPHA | ACP_LOWER | ACP_GRAPH | ACP_PRINT,
        /* 7b { */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 7c | */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 7d } */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 7e ~ */ ACP_PUNCT | ACP_GRAPH | ACP_PRINT,
        /* 7f . */ ACP_CONTROL
    };
    
}

using namespace Poco;

namespace String {
    
    // POCO UTILS
    
    template <class S>
    S trimLeft(const S& str)
    /// Returns a copy of str with all leading
    /// whitespace removed.
    {
        typename S::const_iterator it  = str.begin();
        typename S::const_iterator end = str.end();
        
        while (it != end && Ascii::isSpace(*it)) ++it;
        return S(it, end);
    }
    
    
    template <class S>
    S& trimLeftInPlace(S& str)
    /// Removes all leading whitespace in str.
    {
        typename S::iterator it  = str.begin();
        typename S::iterator end = str.end();
        
        while (it != end && Ascii::isSpace(*it)) ++it;
        str.erase(str.begin(), it);
        return str;
    }
    
    
    template <class S>
    S trimRight(const S& str)
    /// Returns a copy of str with all trailing
    /// whitespace removed.
    {
        int pos = int(str.size()) - 1;
        
        while (pos >= 0 && Ascii::isSpace(str[pos])) --pos;
        return S(str, 0, pos + 1);
    }
    
    
    template <class S>
    S& trimRightInPlace(S& str)
    /// Removes all trailing whitespace in str.
    {
        int pos = int(str.size()) - 1;
        
        while (pos >= 0 && Ascii::isSpace(str[pos])) --pos;
        str.resize(pos + 1);
        
        return str;
    }
    
    
    template <class S>
    S trim(const S& str)
    /// Returns a copy of str with all leading and
    /// trailing whitespace removed.
    {
        int first = 0;
        int last  = int(str.size()) - 1;
        
        while (first <= last && Ascii::isSpace(str[first])) ++first;
        while (last >= first && Ascii::isSpace(str[last])) --last;
        
        return S(str, first, last - first + 1);
    }
    
    
    template <class S>
    S& trimInPlace(S& str)
    /// Removes all leading and trailing whitespace in str.
    {
        int first = 0;
        int last  = int(str.size()) - 1;
        
        while (first <= last && Ascii::isSpace(str[first])) ++first;
        while (last >= first && Ascii::isSpace(str[last])) --last;
        
        str.resize(last + 1);
        str.erase(0, first);
        
        return str;
    }
    
    
    template <class S>
    S toUpper(const S& str)
    /// Returns a copy of str containing all upper-case characters.
    {
        typename S::const_iterator it  = str.begin();
        typename S::const_iterator end = str.end();
        
        S result;
        result.reserve(str.size());
        while (it != end) result += Ascii::toUpper(*it++);
        return result;
    }
    
    
    template <class S>
    S& toUpperInPlace(S& str)
    /// Replaces all characters in str with their upper-case counterparts.
    {
        typename S::iterator it  = str.begin();
        typename S::iterator end = str.end();
        
        while (it != end) { *it = Ascii::toUpper(*it); ++it; }
        return str;
    }
    
    
    template <class S>
    S toLower(const S& str)
    /// Returns a copy of str containing all lower-case characters.
    {
        typename S::const_iterator it  = str.begin();
        typename S::const_iterator end = str.end();
        
        S result;
        result.reserve(str.size());
        while (it != end) result += Ascii::toLower(*it++);
        return result;
    }
    
    
    template <class S>
    S& toLowerInPlace(S& str)
    /// Replaces all characters in str with their lower-case counterparts.
    {
        typename S::iterator it  = str.begin();
        typename S::iterator end = str.end();
        
        while (it != end) { *it = Ascii::toLower(*it); ++it; }
        return str;
    }
    
    
    
    // CONVENIENCE FUNCTIONS
    
    
    string& getfile(const string& filename, string& buffer) {
        ifstream in(filename.c_str(), ios_base::binary | ios_base::ate);
        in.exceptions(ios_base::badbit | ios_base::failbit | ios_base::eofbit);
        buffer.resize(in.tellg());
        in.seekg(0, ios_base::beg);
        in.read(&buffer[0], buffer.size());
        return buffer;
    }
    
    
    //--------------------------------------------------
    vector <string> split(const string & source, const string & delimiter, bool ignoreEmpty, bool trim) {
        vector<string> result;
        if (delimiter.empty()) {
            result.push_back(source);
            return result;
        }
        string::const_iterator substart = source.begin(), subend;
        while (true) {
            subend = search(substart, source.end(), delimiter.begin(), delimiter.end());
            string sub(substart, subend);
            if(trim) {
                trimInPlace(sub);
            }
            if (!ignoreEmpty || !sub.empty()) {
                result.push_back(sub);
            }
            if (subend == source.end()) {
                break;
            }
            substart = subend + delimiter.size();
        }
        return result;
    }
    
    //--------------------------------------------------
    string join(vector <string> stringElements, const string & delimiter){
        string resultString = "";
        int numElements = stringElements.size();
        
        for(int k = 0; k < numElements; k++){
            if( k < numElements-1 ){
                resultString += stringElements[k] + delimiter;
            } else {
                resultString += stringElements[k];
            }
        }
        
        return resultString;
    }
    
    //--------------------------------------------------
    void replace(string& input, string searchStr, string replaceStr){
        size_t uPos = 0;
        size_t uFindLen = searchStr.length();
        size_t uReplaceLen = replaceStr.length();
        
        if( uFindLen == 0 ){
            return;
        }
        
        for( ;(uPos = input.find( searchStr, uPos )) != std::string::npos; ){
            input.replace( uPos, uFindLen, replaceStr );
            uPos += uReplaceLen;
        }
    }
    
    
    int occurancesOfSubstring(string haystack, string needle){
        const size_t step = needle.size();
        
        size_t count(0);
        size_t pos(0) ;
        
        while( (pos=haystack.find(needle, pos)) != std::string::npos) {
            pos +=step;
            ++count ;
        }
        
        return count;
    }
    
    //--------------------------------------------------
    string lowerCase(const string & src){
        string dst(src);
        transform(src.begin(),src.end(),dst.begin(),::tolower);
        return dst;
    }
    
    //--------------------------------------------------
    string upperCase(const string & src){
        string dst(src);
        transform(src.begin(),src.end(),dst.begin(),::toupper);
        return dst;
    }
    
    string filePath( const string & fullPath){
        return fullPath.substr(0, fullPath.find_last_of("/") + 1);
    }
    
    string lastPathExtension( const string & fullPath){
         return fullPath.substr(fullPath.find_last_of("/") + 1);
    }
    
    string fileType( const string & fullPath, bool upperCase){
        if (upperCase) {
            return String::upperCase(fullPath.substr(fullPath.find_last_of(".") + 1));
        }
        return fullPath.substr(fullPath.find_last_of(".") + 1);
    }
    
    //--------------------------------------------------
    string format(const char * format, ...){
        // variadic args to string:
        // http://www.codeproject.com/KB/string/string_format.aspx
        static char aux_buffer[10000];
        string retStr("");
        if (NULL != format){
            
            va_list marker;
            
            // initialize variable arguments
            va_start(marker, format);
            
            // Get formatted string length adding one for NULL
            size_t len = vsprintf(aux_buffer, format, marker) + 1;
            
            // Reset variable arguments
            va_end(marker);
            
            if (len > 0)
            {
                va_list args;
                
                // initialize variable arguments
                va_start(args, format);
                
                // Create a char vector to hold the formatted string.
                vector<char> buffer(len, '\0');
                vsprintf(&buffer[0], format, args);
                retStr = &buffer[0];
                va_end(args);
            }
            
        }
        return retStr;
    }
    
    string format(const char * format, va_list args){
        // variadic args to string:
        // http://www.codeproject.com/KB/string/string_format.aspx
        char aux_buffer[10000];
        string retStr("");
        if (NULL != format){
            
            // Get formatted string length adding one for NULL
            vsprintf(aux_buffer, format, args);
            retStr = aux_buffer;
            
        }
        return retStr;
    }
    
    
    
    
    //----------------------------------------
    string toHex(const string& value) {
        ostringstream out;
        // how many bytes are in the string
        int numBytes = value.size();
        for(int i = 0; i < numBytes; i++) {
            // print each byte as a 2-character wide hex value
            out << setfill('0') << setw(2) << hex << (unsigned int) ((unsigned char)value[i]);
        }
        return out.str();
    }
    
    //----------------------------------------
    string ofToHex(const char* value) {
        // this function is necessary if you want to print a string
        // using a syntax like ofToHex("test")
        return toHex((string) value);
    }
    
    //----------------------------------------
    int toInt(const string& intString) {
        int x = 0;
        istringstream cur(intString);
        cur >> x;
        return x;
    }
    
    //----------------------------------------
    int hexToInt(const string& intHexString) {
        int x = 0;
        istringstream cur(intHexString);
        cur >> hex >> x;
        return x;
    }
    
    //----------------------------------------
    char hexToChar(const string& charHexString) {
        int x = 0;
        istringstream cur(charHexString);
        cur >> hex >> x;
        return (char) x;
    }
    
    //----------------------------------------
    float hexToFloat(const string& floatHexString) {
        union intFloatUnion {
            int x;
            float f;
        } myUnion;
        myUnion.x = 0;
        istringstream cur(floatHexString);
        cur >> hex >> myUnion.x;
        return myUnion.f;
    }
    
    
    
    //----------------------------------------
    float toFloat(const string& floatString) {
        float x = 0;
        istringstream cur(floatString);
        cur >> x;
        return x;
    }
    
    //----------------------------------------
    double toDouble(const string& doubleString) {
        double x = 0;
        istringstream cur(doubleString);
        cur >> x;
        return x;
    }
    
    //----------------------------------------
    bool toBool(const string& boolString) {
        static const string trueString = "true";
        static const string falseString = "false";
        string lower = toLower(boolString);
        if(lower == trueString) {
            return true;
        }
        if(lower == falseString) {
            return false;
        }
        bool x = false;
        istringstream cur(lower);
        cur >> x;
        return x;
    }
    
    //----------------------------------------
    char toChar(const string& charString) {
        char x = '\0';
        istringstream cur(charString);
        cur >> x;
        return x;
    }
    
    //----------------------------------------
    string toBinary(const string& value) {
        stringstream out;
        int numBytes = value.size();
        for(int i = 0; i < numBytes; i++) {
            bitset<8> bitBuffer(value[i]);
            out << bitBuffer;
        }
        return out.str();
    }
    
    //----------------------------------------
    string toBinary(const char* value) {
        // this function is necessary if you want to print a string
        // using a syntax like ofToBinary("test")
        return toBinary((string) value);
    }
    
    //----------------------------------------
    int binaryToInt(const string& value) {
        const int intSize = sizeof(int) * 8;
        bitset<intSize> binaryString(value);
        return (int) binaryString.to_ulong();
    }
    
    //----------------------------------------
    char binaryToChar(const string& value) {
        const int charSize = sizeof(char) * 8;
        bitset<charSize> binaryString(value);
        return (char) binaryString.to_ulong();
    }
    
    //----------------------------------------
    float binaryToFloat(const string& value) {
        const int floatSize = sizeof(float) * 8;
        bitset<floatSize> binaryString(value);
        union ulongFloatUnion {
            unsigned long result;
            float f;
        } myUFUnion;
        myUFUnion.result = binaryString.to_ulong();
        return myUFUnion.f;
    }
    //----------------------------------------
    string binaryToString(const string& value) {
        ostringstream out;
        stringstream stream(value);
        bitset<8> byteString;
        int numBytes = value.size() / 8;
        for(int i = 0; i < numBytes; i++) {
            stream >> byteString;
            out << (char) byteString.to_ulong();
        }
        return out.str();
    }
    
    //----------------------------------------
    string fromHex(const string& stringHexString) {
        stringstream out;
        stringstream stream(stringHexString);
        // a hex string has two characters per byte
        int numBytes = stringHexString.size() / 2;
        for(int i = 0; i < numBytes; i++) {
            string curByte;
            // grab two characters from the hex string
            stream >> setw(2) >> curByte;
            // prepare to parse the two characters
            stringstream curByteStream(curByte);
            int cur = 0;
            // parse the two characters as a hex-encoded int
            curByteStream >> hex >> cur;
            // add the int as a char to our output stream
            out << (char) cur;
        }
        return out.str();
    }
    
}


