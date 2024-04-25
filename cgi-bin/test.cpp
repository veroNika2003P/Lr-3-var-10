#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <regex>
#include <vector>

using namespace std;

struct cgipar {
    string name, value;
};

string decode_url(const char *str) {
    string result = "";
    for (size_t i = 0; i < strlen(str); i++) {
        if (str[i] == '+') {
            result += " ";
        } else if (str[i] == '%') {
            char buf[3] = {str[i + 1], str[i + 2], 0};
            int number;
            sscanf(buf, "%x", &number);
            result += static_cast<char>(number);
            i += 2;
        } else {
            result += str[i];
        }
    }
    return result;
}

vector<cgipar> cgisplit(char *p) {
    vector<cgipar> result;
    char *buf = strdup(p);
    char *token = strtok(buf, "&");

    while (token != NULL) {
        char *equal = strchr(token, '=');
        if (equal) {
            *equal = '\0';
            cgipar pair = {decode_url(token), decode_url(equal + 1)};
            result.push_back(pair);
        }
        token = strtok(NULL, "&");
    }

    free(buf);
    return result;
}

bool is_valid_email(const string &email) {
    const regex pattern("(\\w+)(\\.\\w+)*@(\\w+)(\\.\\w+)+");
    return regex_match(email, pattern);
}

int main() {
    cout << "Content-type: text/html\n\n";

    char *query_string = getenv("QUERY_STRING");
    if (!query_string) {
        cout << "<html><body>Некоректний запит.</body></html>";
        return 1;
    }

    vector<cgipar> data = cgisplit(query_string);
    string email;
    for (auto &item : data) {
        if (item.name == "email") {
            email = item.value;
        }
    }

    cout << "<!DOCTYPE html><html lang=\"uk\"><head><meta charset=\"UTF-8\"><title>Результат валідації</title></head><body>";
    if (!email.empty() && is_valid_email(email)) {
        cout << "<p>Email <strong>" << email << "</strong> є валідним.</p>";
    } else {
        cout << "<p>Email <strong>" << email << "</strong> не є валідним.</p>";
    }
    cout << "</body></html>";
    
    return 0;
}
