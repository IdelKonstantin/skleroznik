#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char** argv) {
	
	mkdir(argv[1], S_IRWXU | S_IRWXG | S_IRWXO);

	//Права доступа к файлам и каталогам Т4.12 p.189
}