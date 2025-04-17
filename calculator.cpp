#include "calculator.hpp"
#include <algorithm>
#include <cctype>
#include <cmath>
#include <iostream>

static std::string trim(const std::string &s) {
    size_t a = 0; while (a < s.size() && std::isspace(static_cast<unsigned char>(s[a]))) ++a;
    size_t b = s.size(); while (b > a && std::isspace(static_cast<unsigned char>(s[b - 1]))) --b;
    return s.substr(a, b - a);
}

// validate (+|-)A(.B) where B has ≥1 digit if dot present
bool isValidDouble(const std::string &raw) {
    std::string s = trim(raw); if (s.empty()) return false;
    size_t i = 0; if (s[i] == '+' || s[i] == '-') ++i;
    bool int_ok = false; while (i < s.size() && std::isdigit(static_cast<unsigned char>(s[i]))) { int_ok = true; ++i; }
    bool frac_ok = false;
    if (i < s.size() && s[i] == '.') {
        ++i;
        while (i < s.size() && std::isdigit(static_cast<unsigned char>(s[i]))) { frac_ok = true; ++i; }
        if (!frac_ok) return false;                 // dot needs digit after
    }
    return int_ok && i == s.size();
}


struct Parts { int sign; std::string intPart, fracPart; };

static std::string lstripZeros(const std::string &s) {
    size_t i = 0; while (i + 1 < s.size() && s[i] == '0') ++i;
    return s.substr(i);
}
static std::string rstripZeros(const std::string &s) {
    size_t i = s.size(); while (i && s[i - 1] == '0') --i;
    return s.substr(0, i);
}
static Parts split(const std::string &raw) {
    Parts p; p.sign = 1; std::string s = trim(raw); size_t i = 0;
    if (s[i] == '+' || s[i] == '-') { p.sign = s[i] == '-' ? -1 : 1; ++i; }
    size_t dot = s.find('.', i);
    if (dot == std::string::npos) { p.intPart = s.substr(i); }
    else { p.intPart = s.substr(i, dot - i); p.fracPart = s.substr(dot + 1); }
    if (p.intPart.empty()) p.intPart = "0";
    p.intPart = lstripZeros(p.intPart); p.fracPart = rstripZeros(p.fracPart);
    return p;
}
static int cmpAbs(const std::string &a, const std::string &b) {
    if (a.size() != b.size()) return a.size() < b.size() ? -1 : 1;
    return a == b ? 0 : (a < b ? -1 : 1);
}
static std::string addAbs(const std::string &a,const std::string &b) {
    std::string r; int c = 0, i=a.size()-1, j=b.size()-1;
    while (i>=0 || j>=0 || c) {
        int s = (i>=0?a[i]-'0':0)+(j>=0?b[j]-'0':0)+c;
        r.push_back('0'+(s%10)); c=s/10; --i; --j;
    }
    std::reverse(r.begin(), r.end()); return r;
}
static std::string subAbs(const std::string &a,const std::string &b) { // |a|≥|b|
    std::string r; int br=0, i=a.size()-1, j=b.size()-1;
    while (i>=0) {
        int d=a[i]-'0'-br-(j>=0?b[j]-'0':0);
        if (d<0){ d+=10; br=1;} else br=0;
        r.push_back('0'+d); --i; --j;
    }
    while(r.size()>1&&r.back()=='0') r.pop_back();
    std::reverse(r.begin(), r.end()); return r;
}


std::string add(const std::string &lhs, const std::string &rhs) {
    Parts a = split(lhs), b = split(rhs);
    size_t len = std::max(a.fracPart.size(), b.fracPart.size());
    a.fracPart.append(len - a.fracPart.size(), '0');
    b.fracPart.append(len - b.fracPart.size(), '0');

    std::string wa = lstripZeros(a.intPart + a.fracPart);
    std::string wb = lstripZeros(b.intPart + b.fracPart);

    std::string digits; int sign = 1;
    if (a.sign == b.sign) { digits = addAbs(wa, wb); sign = a.sign; }
    else {
        int cmp = cmpAbs(wa, wb);
        if (cmp == 0) return "0";
        if (cmp > 0) { digits = subAbs(wa, wb); sign = a.sign; }
        else         { digits = subAbs(wb, wa); sign = b.sign; }
    }

    if (digits.size() <= len) digits.insert(0, len + 1 - digits.size(), '0');
    std::string intPart  = lstripZeros(digits.substr(0, digits.size() - len));
    std::string fracPart = rstripZeros(digits.substr(digits.size() - len));

    std::string out; if (sign == -1) out.push_back('-');
    out += intPart.empty() ? "0" : intPart;
    if (!fracPart.empty()) { out.push_back('.'); out += fracPart; }
    return out;
}

double parse_number(const std::string &expression) {
    long long int_part = 0, frac_part = 0; int frac_len = 0; int sign = 1; size_t i = 0;

    if (expression[i] == '+' || expression[i] == '-') { sign = (expression[i] == '-') ? -1 : 1; ++i; }

    while (i < expression.size() && std::isdigit(static_cast<unsigned char>(expression[i]))) {
        int_part = int_part * 10 + (expression[i] - '0'); ++i;
    }

    if (i < expression.size() && expression[i] == '.') {
        ++i;
        while (i < expression.size() && std::isdigit(static_cast<unsigned char>(expression[i]))) {
            frac_part = frac_part * 10 + (expression[i] - '0');
            ++frac_len; ++i;
        }
    }

    double value = static_cast<double>(int_part);
    if (frac_len) value += static_cast<double>(frac_part) / std::pow(10.0, frac_len);
    return sign * value;
}
