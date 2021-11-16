#include "inoty_wrapper.h"

void inotifyRAIIwrapper::initInotyService(inoty_event_t eventType) {

    m_fd = inotify_init();

    if(m_fd == -1) {

        initDone = false;
        const std::string errorMsg{"Ошибка инициализации службы inotify"};
        throw std::logic_error(errorMsg);
    }

    m_wd = inotify_add_watch(m_fd, m_rawDirPath.c_str(), eventType);

    if(m_wd == -1) {

        initDone = false;
        const std::string errorMsg{"Ошибка при создании наблюдателя inotify"};
        throw std::logic_error(errorMsg);
    }
}

void inotifyRAIIwrapper::destroyInotyService() {

    if(initDone) {

        auto ret = inotify_rm_watch(m_fd, m_wd);

        if(ret) {

            const std::string errorMsg{"Ошибка при удалении наблюдателя inotify"};
            throw std::logic_error(errorMsg);
        }


        if(m_fd == -1) {

            const std::string errorMsg{"Ошибка деструкции inotify ->  invalid fd == -1"};
            throw std::logic_error(errorMsg);
        }
        else {

            ret = close(m_fd);

            if(ret) {

                const std::string errorMsg{"Ошибка деструкции inotify -> !close(fd)"};
                throw std::logic_error(errorMsg);
            }
        }
    }
}

inotifyRAIIwrapper::inotifyRAIIwrapper(const std::string& rawDirPath, inoty_event_t eventType) : m_rawDirPath(rawDirPath) {

    try {

        initInotyService(eventType);
    }
    catch(std::logic_error& le) {

        std::cerr << "Ошибка в работе файлового наблюдателя СИУ: " << le.what() << std::endl;
    }
}

bool inotifyRAIIwrapper::isInitDone() const {

    return initDone;
}

deleted_files_t inotifyRAIIwrapper::readInotyEvents() {

    std::vector<std::string> deletedFiles{};
    deletedFiles.clear();

    auto len = read(m_fd, m_buf, BUF_LEN);

    if(len != -1) {

        while(i < len) {

            struct inotify_event *event = (struct inotify_event*)&m_buf[i];

            if(event->len) {

                std::cout << "Обнаружено, что в папке [" << m_rawDirPath
                << "] taskLaunchDaemon'ом был удален файл [" 
                << std::string(event->name) << "]" << std::endl;

                deletedFiles.push_back(std::string(event->name));
            }

            i += sizeof(struct inotify_event) + event->len;
        }
    }
    
    return deletedFiles;
}

inotifyRAIIwrapper::~inotifyRAIIwrapper() {

    try {

        destroyInotyService();
    }
    catch(std::logic_error& le) {

        std::cerr <<  "Ошибка в работе файлового наблюдателя СИУ: " << le.what() << std::endl;
    }
}
