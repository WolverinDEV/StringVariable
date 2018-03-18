#include "../include/StringVariable.h"

#define dbg(message) std::cout << "[DEBUG] " << message << std::endl;
#define dbg(...)

namespace strvar {
    size_t transform0(std::string& in, const StringProperties& properties);
    std::string transform(std::string in, const StringProperties& properties) {
        auto counter = properties.max_deep;
        do {
            if(transform0(in, properties) == 0) break;
        } while(counter-- > 1);

        return in;
    }

    size_t transform0(std::string& in, const StringProperties& properties) {
        size_t variableCounter = 0;

        size_t index = 0;
        while(true) {
            index = in.find('$', index);
            const auto begIndex = index;

            if(index == std::string::npos || index + 2 >= in.length()) return variableCounter;

            if(in[++index] != '{')
                continue;

            std::string key{};
            while(index + 1 < in.length() && in[++index] != '}')
                key += in[index];
            if(index + 1 == in.length() && in[index] != '}') return variableCounter;
            dbg("Got key '" << key << "'");

            std::deque<std::string> parms{};

            if(index + 1 < in.length() && in[++index] == '(') {
                std::string currentParm{};
                bool escaped = false;
                bool quoted = false;
                while(index + 1 < in.length() && (in[++index] != ')' || escaped || quoted)) {
                    if(escaped) {
                        escaped = false;
                        currentParm += in[index];
                        continue;
                    }
                    if(quoted) {
                        if(in[index] == '"') {
                            quoted = false;
                            //TODO check if var ended!
                            continue;
                        }
                        currentParm += in[index];
                        continue;
                    }

                    if(in[index] == ',') { //Next parm
                        parms.push_back(currentParm);
                        currentParm = "";
                        continue;
                    }

                    if(currentParm.empty() && in[index] == ' ') continue; //Dont read the spaces!

                    if(in[index] == '"') {
                        quoted = true;
                        continue;
                    }
                    if(in[index] == '\\') {
                        escaped = true;
                        continue;
                    }
                    currentParm += in[index];
                }
                if(!currentParm.empty()) parms.push_back(currentParm);
                if(in[index] == ')') index+
            }

            dbg("Got " << parms.size() << " parms");
            for(const auto& entry : parms)
                dbg(" - " << entry);

            auto value = properties.findValue(key);
            std::string result{};
            if(!value)
                result = "<unknown variable '" + key + "'>";
            else
                result = value->value(properties, parms);

            in.replace(begIndex, index - begIndex, result);
            index = begIndex + result.length();
            variableCounter++;
        }
    }
}