#include <cstdlib>
#include <sstream>
#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>

using err_t = uint16_t;

namespace addrList {

    const err_t NORMAL_EXIT = 0;
    const err_t GETIFADDR_FAIL = 1;    
}

class IPaddrsListing final {

private:
    struct ifaddrs* address{nullptr};
    std::stringstream ss{};
    char ap[100];

    void clearSstream() {

        ss.str(std::string());
    }

    auto getIPfamilyNameAndAddr(int family) {

        auto IPname = family == AF_INET ? std::string{"IPv4"} : std::string{"IPv6"};

        const auto family_size = family == AF_INET ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
        getnameinfo(address->ifa_addr, family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);    
        
        return std::make_pair(std::move(IPname), std::move(ap));
    }

public:

    IPaddrsListing() {

        if (getifaddrs(&address) == -1) {
            
            std::cout << "getifaddrs call failed" << std::endl;
            std::exit(addrList::GETIFADDR_FAIL);
        }
    }

    void printIPaddrs() {

        while(address) {
            
            if (address->ifa_addr == NULL) { 
                
                address = address->ifa_next;
                continue;
            }
            int family = address->ifa_addr->sa_family;
            if (family == AF_INET || family == AF_INET6) {

                clearSstream();
                
                auto [IPname, IPadrr] = getIPfamilyNameAndAddr(family);
                
                ss << address->ifa_name << "\t" << IPname << "\t" << IPadrr;
                std::cout << ss.str() << std::endl;
            }
            address = address->ifa_next;
        }
    }

    ~IPaddrsListing() {

        if(address) {

            freeifaddrs(address);
        }
    }
};


int main() {

    IPaddrsListing().printIPaddrs();
    return addrList::NORMAL_EXIT;
}
