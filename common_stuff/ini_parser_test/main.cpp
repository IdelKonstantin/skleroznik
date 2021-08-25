#include <iostream>
#include "./ini_file_parser.h"

int main () {

	iniFileParser ifp("./stat_serv.ini");
	
	if(ifp.parseIniFile()) {

		std::cout << ifp.getParamValue("host") << std::endl;
		std::cout << ifp.getParamValue("port") << std::endl;
		std::cout << ifp.getParamValue("user") << std::endl;
		std::cout << ifp.getParamValue("dbname") << std::endl;
		std::cout << ifp.getParamValue("passwd") << std::endl;
		//std::cout << ifp.getParamValue("абырвалг!!!") << std::endl;
	}
}
