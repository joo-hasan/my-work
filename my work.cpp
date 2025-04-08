#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>

using namespace std;

enum TokenType {
    KEYWORD, IDENTIFIER, NUMBER, OPERATOR, PUNCTUATION, UNKNOWN
};

struct Token {
    string value;
    TokenType type;
};

vector<string> keywords = {"int", "for", "return"};

bool isKeyword(const string& s) {
    return find(keywords.begin(), keywords.end(), s) != keywords.end();
}

bool isNumber(const string& s) {
    return regex_match(s, regex("[0-9]+(\\.[0-9]+)?"));
}

bool isIdentifier(const string& s) {
    return regex_match(s, regex("[A-Za-z_][A-Za-z0-9_]*"));
}

bool isOperator(const string& s) {
    vector<string> ops = {"=", "+", "++", "<"};
    return find(ops.begin(), ops.end(), s) != ops.end();
}

bool isPunctuation(char c) {
    return c == ';' || c == '(' || c == ')' || c == '{' || c == '}';
}

vector<Token> lexicalAnalyze(const string& code) {
    vector<Token> tokens;
    string current;
    size_t i = 0;

    while (i < code.length()) {
        char c = code[i];

        if (isspace(c)) {
            ++i;
            continue;
        }

        if (isPunctuation(c)) {
            tokens.push_back({string(1, c), PUNCTUATION});
            ++i;
            continue;
        }

        if (c == '+' || c == '=' || c == '<') {
            string op(1, c);
            if (c == '+' && i + 1 < code.size() && code[i + 1] == '+') {
                op += '+';
                ++i;
            }
            tokens.push_back({op, OPERATOR});
            ++i;
            continue;
        }

        if (isalnum(c) || c == '.' || c == '_') {
            current.clear();
            while (i < code.size() && (isalnum(code[i]) || code[i] == '.' || code[i] == '_')) {
                current += code[i++];
            }

            if (isKeyword(current))
                tokens.push_back({current, KEYWORD});
            else if (isNumber(current))
                tokens.push_back({current, NUMBER});
            else if (isIdentifier(current))
                tokens.push_back({current, IDENTIFIER});
            else
                tokens.push_back({current, UNKNOWN});
        } else {
            tokens.push_back({string(1, c), UNKNOWN});
            ++i;
        }
    }

    return tokens;
}

int main() {
    string code = R"(Pi = 3.14;
for(Int I = 0; I < 10; ++)
{
Pi + 1.0;
}
Return Pi;)";

    vector<Token> tokens = lexicalAnalyze(code);

    for (const auto& token : tokens) {
        cout << "Token: " << token.value << " | Type: ";
        switch (token.type) {
            case KEYWORD: cout << "KEYWORD"; break;
            case IDENTIFIER: cout << "IDENTIFIER"; break;
            case NUMBER: cout << "NUMBER"; break;
            case OPERATOR: cout << "OPERATOR"; break;
            case PUNCTUATION: cout << "PUNCTUATION"; break;
            default: cout << "UNKNOWN"; break;
        }
        cout << endl;
    }

    return 0;
}
