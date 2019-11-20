#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::string &format(std::string &fmt, std::vector<std::string> &strs)
{
    int state = 0;
    std::vector<int> start;
    std::vector<int> end;
    std::vector<int> inds;
    int i = 0;
    for (auto ch : fmt) {
        if (state == 0) {
            if (ch == '{') {
                state = 1;
                start.push_back(i);
            }
            if (ch == '}') {
                throw std::runtime_error("unmatched }");
            }
        } else if (state == 1) {
            if (ch == '}') {
                end.push_back(i);
                inds.push_back(stoi(fmt.substr(start.back() + 1, end.back())));
                state = 0;
            } else if (48 > (int)ch || (int)ch > 57) {
                throw std::runtime_error("elligable symbol");
            }
        }
        i++;
    }
    if (state == 1)
        throw std::runtime_error("unmatched {");
    int offset = 0;
    for (int i = 0; i < inds.size(); ++i) {
        if (inds[i] >= strs.size())
            throw std::runtime_error("missing argument");
        fmt.replace(start[i] + offset, end[i] - start[i] + 1, strs[inds[i]]);
        offset -= end[i] - start[i] + 1 - strs[inds[i]].size();
    }
    if (state == 1)
        throw std::runtime_error("unmatched parenthesis");

    return fmt;
}

template <class T, class... Agrs>
std::string &format(std::string &fmt, std::vector<std::string> &strs, T &&first,
                    Agrs &&... args)
{
    std::stringstream stream;
    stream << first;
    strs.push_back(stream.str());
    return format(fmt, strs, std::forward<Agrs>(args)...);
}

template <class... Agrs>
std::string format(const char *str, Agrs &&... args)
{
    std::string fmt(str);
    std::vector<std::string> strs;
    return format(fmt, strs, std::forward<Agrs>(args)...);
}
