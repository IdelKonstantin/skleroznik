#pragma once

#include <cstring>

#define MAX_SIZE 0xFF

namespace Str {

    const auto NPOS{-1};
    const auto EMPTY{0};   
}  

class String {

    private:

        char data[MAX_SIZE]{};
        
        void copyText(const char* data);
        void concatText(const char* data);
        bool compareTexts(const char* data);
        bool checkTextPresense(const char* data);
        bool checkCharPresense(const char char_);
    
    public:
        String() = default;  
        String(const char* str);
        String(String& str);
        
        String& operator=(String& str);
        String& operator=(const char* data);
        String& operator+=(const char* data);
        String& operator+=(String& str);
        bool operator==(String& str);
        bool operator==(const char* data);
        char& operator[] (const size_t index);

        size_t length() const;
        const char* c_str() const;
        void clear();
        size_t capacity() const;
        void append(const char* text);
        void append(String& str);
        size_t max_size() const;
        bool empty();
        char& at (size_t pos);
        bool contains (String& str);   
        bool contains (const char* data);    
        bool contains (const char char_);
};