#include "utils.h"

using std::string;
using std::vector;
using std::istringstream;

vector<std::string> split(const string& s, char delim)
{
    vector<string> tokens;
    istringstream f(s);
    string t;
    while (getline(f, t, delim)) {
        tokens.push_back(t);
    }
    return tokens;
}
