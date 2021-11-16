#pragma once 

#include <iostream>
#include <vector>
#include <sys/inotify.h>
#include <unistd.h>

using descriptor_t = int;
using inoty_event_t = size_t;
using deleted_files_t = std::vector<std::string>;

#define BUF_LEN 0xFF*sizeof(struct inotify_event)
#define NOT_FOUND std::string::npos

class inotifyRAIIwrapper final {
	
private:

    descriptor_t m_fd{};
    descriptor_t m_wd{};
    std::string m_rawDirPath{};
    bool initDone{true};

    char m_buf[BUF_LEN] __attribute__((aligned(4)));
    ssize_t i{0};

    void initInotyService(inoty_event_t eventType);
    void destroyInotyService();

public:	

    inotifyRAIIwrapper() = delete;
    inotifyRAIIwrapper(const inotifyRAIIwrapper&) = delete;
    inotifyRAIIwrapper& operator=(const inotifyRAIIwrapper&) = delete;
    
    inotifyRAIIwrapper(const std::string& rawDirPath, inoty_event_t eventType = IN_DELETE);
    ~inotifyRAIIwrapper();

    bool isInitDone() const;
    deleted_files_t readInotyEvents();
};
