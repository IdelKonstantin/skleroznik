#pragma once

/********************************************

	Парсер файлов *.ini простейшего вида:

	host=127.0.0.1
	port=5555

	Строки не разделенные "=" игнорируются,
	секции не обрабатываются

********************************************/

#include <fstream>
#include <algorithm>
#include <map>

class iniFileParser {

private:
	std::map<std::string, std::string> iniFileContent{};
	std::string iniFilePath{};

public:
	iniFileParser() = delete;
	iniFileParser(const iniFileParser&) = delete;
	iniFileParser(const iniFileParser&&) = delete;     
	iniFileParser(const std::string& iniFilePath) : iniFilePath(iniFilePath) {};

	bool parseIniFile(void) {

		std::ifstream fin;
		fin.open(iniFilePath);

		if(!fin) {

			return false;		
		}
		else {

			try {

				auto removeWhitespaces = [](std::string& text) {

					text.erase(std::remove_if(text.begin(), text.end(), 
					[](decltype(text[0]) symbol) {return std::isspace(symbol);}), text.end());
				};

				/* Разделение строк по знаку равенства и очистка получаемых частей от пробельных символов */
				for (std::string iniLine, paramName, paramValue;  std::getline(fin, iniLine);) {
	        		
					size_t pos = iniLine.find('=');

					if(pos != std::string::npos) {

						paramName = iniLine.substr(0, pos++);
						paramValue = iniLine.substr(pos, iniLine.size() - pos);
						removeWhitespaces(paramName);
						removeWhitespaces(paramValue);

						/* Добавление пар "имя - значение" в словарь параметров ТБД аналогичный db::connparams_t */
						iniFileContent.insert(std::make_pair(paramName, paramValue));
					}
				}
			}
			catch(...) {

				fin.close();
				return false;				
			}
		}
		fin.close();
		return true;
	}

	std::string getParamValue(const std::string& paramName) {

		try {

			return iniFileContent.at(paramName);
		}
		catch(...) {

			return std::string("");
		}
	}

	std::map<std::string, std::string> getAllParams(void) {

		return iniFileContent;
	}
};