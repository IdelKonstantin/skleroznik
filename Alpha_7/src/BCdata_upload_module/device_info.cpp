#include "../../inc/device_info.h"

String bc_data::getChipName() {

	const size_t buffLength{11};
	char ssid[buffLength];
	snprintf(ssid, buffLength, "S2%llX", ESP.getEfuseMac());
	return ssid;
}

chip::chip() : m_chipID(bc_data::getChipName()) {}

chip& chip::info() {

	static chip c;
	return c;
}

const String& chip::chipID() const {

	return m_chipID;
}

const String& chip::firmwareVersion() const {

	return m_firmwareVersion;
}

const String& chip::releaseDate() const {

	return m_releaseDate;
}

const String& chip::webSite() const {

	return m_webSite;
}

const String& chip::deviceName() const {

	return m_devieceName;
}