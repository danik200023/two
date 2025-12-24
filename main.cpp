#include <iostream>

struct BigInteger {
    bool negative;
    std::vector<int> digits;
    int base = 1e9;

    BigInteger(): negative(false), digits(0) {}

    explicit BigInteger(long long value) {
        if (value < 0) {
            negative = true;
        } else {
            negative = false;
        }
        while (value != 0) {
            digits.push_back(abs(value % base));
            value /= base;
        }
    }

    explicit BigInteger(std::string str) {
        if (str.empty() || (str.length() == 1 && str[0] == '-')) {
            negative = false;
            digits.push_back(0);
            return;
        } else if (str[0] == '-') {
            negative = true;
            str.erase(0, 1);
        } else {
            negative = false;
        }
        if (!std::all_of(str.begin(), str.end(), isdigit)) {
            throw std::invalid_argument("Invalid number");
        }
        int i = str[0] - '0';
        while (!str.empty()) {
            if (i == 0) {
                str.erase(0, 1);
                i = str[0] - '0';
            } else {
                break;
            }
        }
        if (str.empty()) {
            digits.push_back(0);
            return;
        }
        while (!str.empty()) {
            if (str.length() > 9) {
                std::string substr = str.substr(str.length() - 9, 9);
                str.erase(str.length() - 9, 9);
                digits.push_back(std::stoi(substr));
            } else {
                digits.push_back(std::stoi(str));
                str.erase(0, str.length());
            }
        }
    }

    BigInteger operator+(const BigInteger& rhs) const {
        BigInteger result;
        if (this->negative == rhs.negative) {
            result.negative = this->negative;
            result.digits = addAbs(this->digits, rhs.digits);
        } else {
            int cmp = compareAbs(*this, rhs);
            if (cmp >= 0) {
                result.negative = this->negative;
                result.digits = subAbs(this->digits, rhs.digits);
            } else {
                result.negative = rhs.negative;
                result.digits = subAbs(rhs.digits, this->digits);
            }
        }
        if (result.digits.size() == 1 && result.digits[0] == 0) {
            result.negative = false;
        }

        return result;
    }

    BigInteger operator-(BigInteger rhs) const {
        rhs.negative = !rhs.negative;
        return *this+rhs;
    }

    std::vector<int> addAbs(const std::vector<int>& lhs, const std::vector<int>& rhs) const {
        std::vector<int> result;

        int razryads = std::max(lhs.size(), rhs.size());

        int carry = 0;
        for (int i = 0; i < razryads; i++) {
            const int a = (i < lhs.size() ? lhs[i] : 0);
            const int b = (i < rhs.size() ? rhs[i] : 0);
            result.push_back(a + b + carry);
            carry = result.back()/base;
            result.back() = result.back() % base;
        }
        if (carry) {
            result.push_back(carry);
        }
        return result;
    }

    std::vector<int> subAbs(std::vector<int> lhs, const std::vector<int>& rhs) const {
        std::vector<int> result;
        int borrow = 0;
        for (int i = 0; i < lhs.size(); i++) {
            lhs[i] -= borrow;
            int b = (i < rhs.size() ? rhs[i] : 0);
            if (lhs[i] < b) {
                lhs[i] += base;
                result.push_back(lhs[i] - b);
                borrow = 1;
            } else {
                result.push_back(lhs[i] - b);
                borrow = 0;
            }
        }
        while (result.size() > 1 && result.back() == 0) {
            result.pop_back();
        }
        return result;
    }

    bool operator>(const BigInteger& rhs) const {
        if (!this->negative && rhs.negative) {
            return true;
        }
        if (this->negative && rhs.negative) {
            const int cmp = compareAbs(*this, rhs);
            return cmp < 0 ? true : false;
        }
        if (!this->negative && !rhs.negative) {
            const int cmp = compareAbs(*this, rhs);
            return cmp > 0 ? true : false;
        }
        return false;
    }

    bool operator==(const BigInteger &rhs) const {
        if (this->negative && rhs.negative) {
            const int cmp = compareAbs(*this, rhs);
            return cmp == 0 ? true : false;
        }
        return false;
    }

    bool operator<(const BigInteger& rhs) const {
        return rhs > *this;
    }

    bool operator>=(const BigInteger& rhs) const {
        return !(*this < rhs);
    }

    bool operator<=(const BigInteger& rhs) const {
        return !(*this > rhs);
    }

private:
    int compareAbs(const BigInteger& lhs, const BigInteger& rhs) const {
        if (lhs.digits.size() != rhs.digits.size()) {
            return lhs.digits.size() > rhs.digits.size() ? 1 : -1;
        }

        for (int i = lhs.digits.size() - 1; i >= 0; --i) {
            if (lhs.digits[i] != rhs.digits[i]) {
                return lhs.digits[i] > rhs.digits[i] ? 1 : -1;
            }
        }

        return 0;
    }
};

int main() {
    // long long t = LLONG_MAX;
    BigInteger a("999999999999999999");
    BigInteger b("1");
    BigInteger c = a + b;
    BigInteger d = a - b;
    std::cout << 52 << std::endl;
}