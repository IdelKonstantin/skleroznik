#ifndef CMD_LINE_PARSER_H
#define CMD_LINE_PARSER_H

#include <string>
#include <vector>
#include <functional>
#include <algorithm>

class CCmdLineParser {
    public:
    CCmdLineParser(int argc, char **argv) {
        for (int i = 1; i < argc; i++) {
            this->m_tokens.push_back(std::string(argv[i]));
        }
    }

    const std::string& getCmdOptionValue(const std::string& option) const {
        auto it =  std::find(this->m_tokens.begin(), this->m_tokens.end(), option);
        if (it != this->m_tokens.end() && ++it != this->m_tokens.end()) {
            return *it;
        }
        static const std::string emptyString("");
        return emptyString;
    }

    bool cmdOptionExist(const std::string& option) const {
        auto it = std::find(this->m_tokens.begin(), this->m_tokens.end(), option); 
        return (it != this->m_tokens.end());
    }

    private:

    std::vector<std::string> m_tokens;
};

#endif // CMD_LINE_PARSER_H
