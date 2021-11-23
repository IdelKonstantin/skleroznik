import copy

def saveToFile(fileName, fileContent):
	with open("./" + fileName, "w") as f:
		f.write(fileContent)

def makeGitignore(projType, projName):
	
	gitignoreContent = """*.a
*.so
CMakeFiles/
CMakeCache.txt
*.cmake
Makefile"""

	if(projType == "e"):
		gitignoreContent += ("\n" + projName)

	saveToFile(".gitignore", gitignoreContent)


def makeCmakeListsTxt(projType, projName):

	cmakeListsContent = "cmake_minimum_required(VERSION 3.7)\n\nproject("
	cmakeListsContent += (projName + ")\n\n")
	cmakeListsContent += """#find_package(---SOME_LIB--- REQUIRED)

set(INC_PATH "include")
set(SRC_PATH "src")

file(GLOB HEADERS ${INC_PATH}/*.h)
file(GLOB SOURCES ${SRC_PATH}/*.cpp)

#include_directories(---SOME_INCLUDE_DIRS---)
include_directories(${INC_PATH})\n\n"""

	if(projType == "e"):

		cmakeListsContent += "add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} main.cpp)\n\n"
		cmakeListsContent += """#target_link_libraries(${PROJECT_NAME} PUBLIC
#
#	---SOME_LIBS_HERE---
#
#   )

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -std=c++1z -g -O0)

#install(TARGETS ${PROJECT_NAME} DESTINATION /usr/bin)"""

	elif(projType == "s"):

		cmakeListsContent += "add_library(${PROJECT_NAME} STATIC ${SOURCES} ${HEADERS})\n\n"
		cmakeListsContent += """#target_link_libraries(${PROJECT_NAME} PUBLIC
#
#	---SOME_LIBS_HERE---
#
#   )

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -std=c++1z -g -O0)"""

	elif(projType == "d"):
		
		cmakeListsContent += "add_library(${PROJECT_NAME} SHARED ${SOURCES} ${HEADERS})\n\n"
		cmakeListsContent += """#target_link_libraries(${PROJECT_NAME} PUBLIC
#
#	---SOME_LIBS_HERE---
#
#   )

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -std=c++1z -g -O0)

#install(TARGETS ${PROJECT_NAME} DESTINATION /usr/lib)"""

	saveToFile("CMakeLists.txt", cmakeListsContent)


def makeMainCpp(projType):

	if(projType != "e"):
		return

	mainCppContent = """#include <>

int main(int argc, char** argv) {

return 0;
}"""
	saveToFile("main.cpp", mainCppContent)


def successCreation(fileName_):
	print("Файл [" + fileName_ + "] успешно создан" )


def creationFail(fileName_):
	print("Не удалось создать файл [" + fileName_ + "]" )


def makeHeaderGuard(fileName):

	headerGuard = copy.deepcopy(fileName)
	headerGuard = headerGuard.upper()
	headerGuard.replace(' ', '_')
	return headerGuard


def makeHeaderFile(fileName):
	
	fileName_ = fileName + ".h"
	headerGuard = makeHeaderGuard(fileName)

	headerFileContent = "#ifndef " + headerGuard
	headerFileContent += ("\n#define " + headerGuard + "\n\n")
	headerFileContent += "#include <>\n"
	headerFileContent += "#include <>\n"
	headerFileContent += "#include <>\n\n"
	headerFileContent += "/* Your code here */\n\n"
	headerFileContent += ("#endif /*" + headerGuard + "*/\n")

	try:
		saveToFile(fileName_, headerFileContent)
		successCreation(fileName_)
	except:
		creationFail(fileName_)


def makeCppFile(fileName):
	
	fileName_ = fileName + ".cpp"

	cppContent = '#include "' + fileName +'.h"\n\n'
	cppContent += "/* Your code here */\n"

	try:
		saveToFile(fileName_, cppContent)
		successCreation(fileName_)
	except:
		creationFail(fileName_)


def makeRequestedFile(fileType, fileName):

	if fileType == "h":
		makeHeaderFile(fileName)
	else:
		makeCppFile(fileName)