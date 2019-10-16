#include <cctype>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

class Calc
{
    vector<string> tokens;
    vector<string>::iterator token;

  public:
    Calc(string &str)
    {
        parse_string(str);
        token = tokens.begin();
    }

    int eval()
    {
        return binary_low();
    }

  private:
    void parse_string(string &str)
    {
        tokens.push_back("phony");
        auto it = str.begin();
        while (it != str.end()) {
            if (*it == ' ' || *it == '\t') {
                it++;
            } else if (isdigit(*it)) {
                auto start = it;
                it++;
                while (isdigit(*it)) {
                    it++;
                }
                tokens.push_back(string(start, it));
            } else if ((*it == '+') || (*it == '-') || (*it == '*') ||
                       (*it == '/')) {
                tokens.push_back(string(1, *it));
                it++;
            } else {
                throw runtime_error(to_string(__LINE__));
            }
        }
    }

    int to_number(string &token)
    {
        char *p;
        long res = strtol(token.c_str(), &p, 10);
        if (*p) {
            throw runtime_error(to_string(__LINE__));
        } else {
            return res;
        }
    }

    int unary()
    {
        vector<string>::iterator t = next(token);
        if (t == tokens.end())
            throw runtime_error(to_string(__LINE__));
        if (!t->compare("-")) {
            token++;
            return -unary();
        } else {
            token++;
            return to_number(*t);
        }
    }

    int binary_high()
    {
        int res = unary();
        vector<string>::iterator t;
        while (true) {
            if ((t = next(token)) == tokens.end()) {
                return res;
            } else if (!t->compare("*")) {
                token++;
                res *= unary();
            } else if (!t->compare("/")) {
                token++;
                auto u = unary();
                if (u == 0)
                    throw runtime_error(to_string(__LINE__));
                res /= u;
            } else {
                return res;
            }
        }
        return res;
    }

    int binary_low()
    {
        int res = binary_high();
        vector<string>::iterator t;
        while (true) {
            if ((t = next(token)) == tokens.end()) {
                return res;
            } else if (t->compare("+") && t->compare("-")) {
                throw runtime_error(to_string(__LINE__));
            } else if (!t->compare("+")) {
                token++;
                res += binary_high();
            } else if (!t->compare("-")) {
                token++;
                res -= binary_high();
            }
        }
    }
};

int main(int argc, char **argv)
{
    try {
        if (argc != 2) {
            throw runtime_error(to_string(__LINE__));
        }

        string input(reinterpret_cast<char *>(argv[1]));
        Calc p(input);
        cout << p.eval();

    } catch (exception &e) {
        // cout << e.what();
        cout << "error";
        return 1;
    }
    return 0;
}
