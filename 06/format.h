#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::string &format(std::string &fmt, const std::vector<std::string> &strs)
{
    int state = 0;
    int start = 0;
    int end = -1;
    int ind = 0;
    std::stringstream stream;
    int i = 0;
    for (auto ch : fmt) {
        if (state == 0) {
            if (ch == '{') {
                state = 1;
                start = i;
                stream << fmt.substr(end + 1, start - (end + 1));
            }
            if (ch == '}') {
                throw std::runtime_error("unmatched }");
            }
        } else if (state == 1) {
            if (ch == '}') {
                end = i;
                ind = stoi(fmt.substr(start + 1, end - (start + 1)));
                if (ind >= strs.size())
                    throw std::runtime_error("missing argument");
                stream << strs[ind];

                state = 0;
            } else if (48 > (int)ch || (int)ch > 57) {
                throw std::runtime_error("illigable symbol");
            }
        }
        i++;
    }

    if (state == 1)
        throw std::runtime_error("unmatched {");
    if (end != fmt.size() - 1)
        stream << fmt.substr(end + 1, fmt.size() - (end + 1));

    fmt = stream.str();
    return fmt;
}

void unravel(std::vector<std::string> &strs) {}

template <class T, class... Args>
void unravel(std::vector<std::string> &strs, T &&first, Args &&... args)
{
    std::stringstream stream;
    stream << first;
    strs.push_back(stream.str());
    unravel(strs, std::forward<Args>(args)...);
}

template <class... Args> std::string format(const char *str, Args &&... args)
{
    std::string fmt(str);
    std::vector<std::string> strs;
    unravel(strs, std::forward<Args>(args)...);
    return format(fmt, strs);
}
