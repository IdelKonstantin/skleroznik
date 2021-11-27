//#ifndef _STRING_PURE_C_H_
//#define _STRING_PURE_C_H_ 

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_SIZE 0xFF

typedef uint8_t position_t;

static const position_t NPOS = -1;
static const position_t EMPTY = 0;

struct CString;

/********************************************************************/

//Privates methods...
void copyText(struct CString*, const char* data);
void concatText(struct CString*, const char* data);
bool compareTexts(struct CString*, const char* data);
bool checkTextPresense(struct CString*, const char* data);
bool checkCharPresense(struct CString*, const char char_);

//Public methods...

size_t length(struct CString*);
const char* c_str(struct CString*);
void clear(struct CString*);
size_t capacity(struct CString*);
void append_chars(struct CString*, const char* text);
void append_cstr(struct CString*, struct CString* cstr);
size_t max_size(struct CString*);
bool empty(struct CString*);
char at (struct CString*, size_t pos);
bool contains_cstr (struct CString*, struct CString* cstr);  
bool contains_chars (struct CString*, const char* data);    
bool contains_char (struct CString*, const char symbol);

/********************************************************************/
typedef struct CStrPriv {

    char m_data[MAX_SIZE];
    void (*m_copyText)(struct CString*, const char* data);
    void (*m_concatText)(struct CString*, const char* data);
    bool (*m_compareTexts)(struct CString*, const char* data);
    bool (*m_checkTextPresense)(struct CString*, const char* data);
    bool (*m_checkCharPresense)(struct CString*, const char char_);

} CStrPriv;

typedef struct CString {

    struct CStrPriv* privates;

    size_t (*length)(struct CString*);
    const char* (*c_str)(struct CString*);
    void (*clear)(struct CString*);
    size_t (*capacity)(struct CString*);
    void (*append_chars)(struct CString*, const char* text);
    void (*append_cstr)(struct CString*, struct CString*);
    size_t (*max_size)(struct CString*);
    bool (*empty)(struct CString*);
    char (*at)(struct CString*, size_t pos);
    bool (*contains_cstr)(struct CString*, struct CString*);   
    bool (*contains_chars)(struct CString*, const char* data);    
    bool (*contains_char)(struct CString*, const char symbol);
} CString;


void initAllMethods(CString* this_) {

    this_->privates->m_copyText = copyText;
    this_->privates->m_concatText = concatText;
    this_->privates->m_compareTexts = compareTexts;
    this_->privates->m_checkTextPresense = checkTextPresense;
    this_->privates->m_checkCharPresense = checkCharPresense;

    /* Init publics */
    this_->length = length;
    this_->c_str = c_str;
    this_->clear = clear;
    this_->capacity = capacity;
    this_->append_chars = append_chars;
    this_->append_cstr = append_cstr;
    this_->max_size = max_size;
    this_->empty = empty;
    this_->at = at;
    this_->contains_cstr = contains_cstr;
    this_->contains_chars = contains_chars;
    this_->contains_char = contains_char;
}

void initAllPrivates(CString* this_) {

    CStrPriv* this_privates_ = (CStrPriv*)malloc(sizeof(CStrPriv));
    assert(this_privates_);

    /* Init privates */
    this_->privates = this_privates_;
}

void initAllFields(CString* this_) {

    initAllPrivates(this_);
    initAllMethods(this_);
}

CString CStr_construct_default() {

    puts("Default constructor called");

    CString* this_ = (CString*)malloc(sizeof(CString));
    assert(this_);

    initAllFields(this_);
    
    this_->privates->m_concatText(this_, this_->privates->m_data);
    return *this_;
}

CString CStr_construct_c_char(const char* text) {

    puts("Constructor of const char* called");

    CString* this_ = (CString*)malloc(sizeof(CString));
    assert(this_);

    initAllFields(this_); 
    this_->privates->m_copyText(this_, text);
    return *this_;
}

CString CStr_construct_CStr(const CString* const cstr) {

    puts("Copy Constructor called");

    CString* this_ = (CString*)malloc(sizeof(CString));
    assert(this_);

    initAllFields(this_);

    this_->privates->m_copyText(this_, cstr->privates->m_data);
    return *this_;  
}

/**************************************************************************************/

//Privates methods...
void copyText(struct CString* this_, const char* data) {

    strncpy(this_->privates->m_data, data, MAX_SIZE);
}

void concatText(struct CString* this_, const char* data) {

    strncat(this_->privates->m_data, data, MAX_SIZE);
}

bool compareTexts(struct CString* this_, const char* data) {
    
    return strcmp(this_->privates->m_data, data) == 0;
}

bool checkTextPresense(struct CString* this_, const char* data) {

    return (strstr(this_->privates->m_data, data)) 
    && (strlen(this_->privates->m_data) == strlen(data));
}

bool checkCharPresense(struct CString* this_, const char symbol) {

    return strchr(this_->privates->m_data, symbol);
}

//Public methods...

size_t length(struct CString* this_) {

    return strlen(this_->privates->m_data);
}

const char* c_str(struct CString* this_) {

    return this_->privates->m_data;
}

void clear(struct CString* this_) {

    memset(this_->privates->m_data, '\0', sizeof(char)*MAX_SIZE);
}

size_t capacity(struct CString* this_) {

    return MAX_SIZE - strlen(this_->privates->m_data);
}

void append_chars(struct CString* this_, const char* text) {

    this_->privates->m_concatText(this_, text);
}

void append_cstr(struct CString* this_, struct CString* cstr) {

    this_->privates->m_concatText(this_, cstr->privates->m_data);
}

size_t max_size(struct CString* this_) {

    return MAX_SIZE;
}

bool empty(struct CString* this_) {

    return strlen(this_->privates->m_data) == 0;
}

char at (struct CString* this_, size_t pos) {

    return this_->privates->m_data[pos];
}

bool contains_cstr (struct CString* this_, struct CString* cstr) {

    return this_->privates->m_checkTextPresense(this_, cstr->privates->m_data);
}  

bool contains_chars (struct CString* this_, const char* data) {

    return this_->privates->m_checkTextPresense(this_, data);
}   

bool contains_char (struct CString* this_, const char symbol) {

    return this_->privates->m_checkCharPresense(this_, symbol);
}


/**************************************************************************************/

int main () {

    CString str1 = CStr_construct_default();
    CString str2 = CStr_construct_c_char("Text");
    CString str3 = CStr_construct_CStr(&str2);

    puts("Before");
    printf("String_1 = %s\n", str1.c_str(&str1));
    printf("String_2 = %s\n", str2.c_str(&str2));
    printf("String_3 = %s\n", str3.c_str(&str3));

    str1.append_cstr(&str1, &str2);
    str2.append_chars(&str2, " abyrvalg");

    puts("After");
    printf("String_1 = %s\n", str1.c_str(&str1));
    printf("String_2 = %s\n", str2.c_str(&str2));
    printf("String_3 = %s\n", str3.c_str(&str3));

    return 0;
}

//#define /* _STRING_PURE_C_H_ */