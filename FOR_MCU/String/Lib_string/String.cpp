#include <String.h>

//Privates
  	
void String::copyText(const char* data) {

	std::strncpy(this->data, data, MAX_SIZE);
}

void String::concatText(const char* data) {

	std::strncat(this->data, data, MAX_SIZE);
}

bool String::compareTexts(const char* data) {

	return std::strcmp(this->data, data) == 0;
}

bool String::checkTextPresense(const char* data) {

	return (std::strstr(this->data, data)) && (std::strlen(this->data) == std::strlen(data));
}

bool String::checkCharPresense(const char char_) {

	return std::strchr(this->data, char_);    		
}

//Publics

String::String(const char* str) {
    
	this->copyText(str);
}

String::String(String& str) {
    
	this->copyText(str.c_str());
}

String& String::operator=(String& str) {

	this->copyText(str.c_str());
	return *this;
}

String& String::operator=(const char* data) {

	this->copyText(data);
	return *this;
}

String& String::operator+=(const char* data) {

	this->concatText(data);
	return *this;
}

String& String::operator+=(String& str) {

	this->concatText(str.data);
	return *this;
}        

bool String::operator==(String& str) {

	return compareTexts(str.c_str());
}

bool String::operator==(const char* data) {

	return compareTexts(data);
}

char& String::operator[] (const size_t index) {

	return this->data[index];
} 

size_t String::length() const {

	return std::strlen(this->data);
}

const char* String::c_str() const {

	return this->data;
}

void String::clear() {

	std::memset(this->data, '\0', MAX_SIZE);
}

size_t String::capacity() const {

	return sizeof(this->data);
}

void String::append(const char* text) {

	this->concatText(text);
}

void String::append(String& str) {

	this->concatText(str.c_str());
}

size_t String::max_size() const {

	return MAX_SIZE;
}

bool String::empty() {

	return std::strlen(this->data) == 0;
}

char& String::at (size_t pos) {

	return this->data[pos];
}

bool String::contains (String& str) {

	return this->checkTextPresense(str.c_str());
}

bool String::contains (const char* data) {

	return this->checkTextPresense(data);
}

bool String::contains (const char char_) {

	return this->checkCharPresense(char_);
}