#include "../../inc/input_utils.h"
#include <cstring>

namespace UI {
	char target[MAX_TEXT_BUFF];		
};

char* fontAdapter::toRussian(const char *source) {
	
	int i,j,k;
	unsigned char n;
	char m[2] = { '0', '\0' };

	strcpy(UI::target, ""); k = strlen(source); i = j = 0;

	while (i < k) {
			
		n = source[i]; i++;

		if (n >= 0xBF) {
		
			switch (n) {
				case 0xD0: {
					n = source[i]; i++;
					if (n == 0x81) { n = 0xA8; break; }
					if (n >= 0x90 && n <= 0xBF) n = n + 0x2F;
					break;
				}
				case 0xD1: {
						n = source[i]; i++;
						if (n == 0x91) { n = 0xB7; break; }
						if (n >= 0x80 && n <= 0x8F) n = n + 0x6F;
						break;
				}
			}
		}

		m[0] = n; strcat(UI::target, m);
		j++; if (j >= MAX_TEXT_BUFF) break;
	}
	return UI::target;
}