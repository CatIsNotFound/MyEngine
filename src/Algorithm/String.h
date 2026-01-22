#pragma once
#ifndef MYENGINE_ALGORITHM_STRING_H
#define MYENGINE_ALGORITHM_STRING_H
#include "../Basic.h"

namespace MyEngine {
    namespace Algorithm {
        inline static std::string mergeStringList(const StringList& string_list, bool reverse = false) {
            std::string output;
            for (auto& s : string_list) {
                if (reverse) output.assign(s + output);
                else output += s;
            }
            return output;
        }

        inline static std::string multiplicationString(const char* str, uint32_t count = 1) {
            std::string output;
            while (count--) {
                output += str;
            }
            return output;
        }

        inline static StringList splitUTF_8(const std::string& utf8_string) {
            StringList _ret;
            size_t char_length = 0;
            for (size_t i = 0; i < utf8_string.size();) {
                const auto byte = static_cast<uint8_t>(utf8_string[i]);
                if ((byte & 0x80) == 0) {
                    char_length = 1;
                } else if ((byte & 0xe0) == 0xc0) {
                    char_length = 2;
                } else if ((byte & 0xf0) == 0xe0) {
                    char_length = 3;
                } else if ((byte & 0xf8) == 0xf0) {
                    char_length = 4;
                } else {
                    Logger::log(Logger::Warn, "splitUTF_8: The specified character is not valid! Returned null!");
                    return {};
                }
                _ret.emplace_back(utf8_string.substr(i, char_length));
                i += char_length;
            }
            return _ret;
        }


    }
}

#endif //MYENGINE_ALGORITHM_STRING_H
