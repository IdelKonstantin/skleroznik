#ifndef _BC_DEVICE_INFO_H_
#define _BC_DEVICE_INFO_H_

#define FIRMWARE_VERSION "1.0.0.1"
#define RELEASE_DATE "2024-04-21"
#define WEB_SITE "www.s2ballistix.com"
#define DEVICE_NAME "Alpha 7"

#include <Arduino.h>

namespace bc_data {
	String getChipName(); 
};

class chip {

private:
	
	const String m_chipID;
	const String m_firmwareVersion{FIRMWARE_VERSION};
	const String m_releaseDate{RELEASE_DATE};
	const String m_webSite{WEB_SITE};
	const String m_devieceName{DEVICE_NAME};

	chip();

public:

	static chip& info();
	const String& chipID() const;
	const String& firmwareVersion() const;
	const String& releaseDate() const;
	const String& webSite() const;
	const String& deviceName() const;
};

#endif /* _BC_DEVICE_INFO_H_ */