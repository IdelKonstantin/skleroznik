#include <server_logger.h>

serverLogger::serverLogger() {

    this->addTimepointToPath();
}

time_t serverLogger::getCurrentTimepoint() {

    auto now = std::chrono::system_clock::now();
    return std::chrono::system_clock::to_time_t(now);   
}

std::string serverLogger::getdateAndTime() {

    ss.str(std::string());
    ss << boost::posix_time::from_time_t(this->getCurrentTimepoint());
    return ss.str();
}

const std::string& serverLogger::buildLogTimeMark() {

    this->logTimeMark.clear();
    logTimeMark.append("[");
    logTimeMark.append(this->getdateAndTime());
    logTimeMark.append("]");

    return logTimeMark;
}

void serverLogger::addTimepointToPath() {

    this->path.append("S2_service_log_");
    this->path.append(this->getdateAndTime());
    this->path.append(".log");
}

void serverLogger::logoutData(const std::string& serviceMessage, const log_type_t logType) {

    std::ofstream fout;

    fout.open(this->path, std::fstream::out | std::ofstream::app);

    if(!fout.is_open()) {

        std::cerr << "Can't open file for logging..." << std::endl;
        return;
    }

    this->logLine.clear();

    logLine.append(this->buildLogTimeMark());

    switch(logType) {
        
        case logType::REQUEST:
        logLine.append(" Service have got a message: [");
        break;

        case logType::RESPONSE:
        logLine.append(" Service have sent a result: [");
        break;
        
        default:
        break;          
    }
    
    logLine.append(serviceMessage);
    logLine.append("]\n");

    try{  
        
        fout << logLine << "\n";
        fout.close();
    }
    catch(...) {

        fout.close();
    }
}
