#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

typedef long long ll;
typedef unsigned int u_int;
typedef unsigned long long u_ll;

u_int const BIT = 32;
u_ll const RADIX = (1ll << BIT);

class big_integer {
	vector<u_int> data;
	bool sign;

	void delete_zeroes() {
		while (data.size() > 1 && data.back() == 0) {
			data.pop_back();
		}
	}

	vector<u_int> make_two_complement() const {
		vector<u_int> res = data;
		res.push_back(0);
		if (sign) {
			for (size_t i = 0; i < res.size(); ++i) {
				res[i] = ~res[i];
			}
			for (size_t i = 0; i < res.size(); ++i) {
				if (res[i] == RADIX - 1) {
					res[i] = 0;
				} else {
					res[i]++;
					break;
				}
			}
		}
		return res;
	}

	big_integer(vector<u_int> const &d, bool const &s) {
		data = d;
		sign = s;
		delete_zeroes();
	}

	big_integer(vector<u_int> const &arr) {
		vector<u_int> copy = arr;
		if (copy.back() == 0) {
			copy.pop_back();
			sign = false;
		} else {
			copy.pop_back();
			size_t i = 0;
			while (i < copy.size() && copy[i] == 0) {
				copy[i++]--;
			}
			copy[i]--;
			for (size_t i = 0; i < copy.size(); ++i) {
				copy[i] = ~copy[i];
			}
			sign = true;
		}
		data = copy;
		delete_zeroes();
	}

	u_ll make_long_from_int(u_int const &a, u_int const &b) const {
		return (u_ll)a * RADIX + (u_ll)b;
	}

	bool operator== (u_int const &b) const {
		return data.size() == 1 && data[0] == b;
	}

	bool operator!= (u_int const &b) const {
		return !(*this == b);
	}

	big_integer operator* (u_int const &b) const {
		u_ll prev = 0;
		big_integer res = *this;
		for (size_t i = 0; i < res.data.size(); ++i) {
			u_ll cur = (u_ll)res.data[i] * (u_ll)b + prev;
			res.data[i] = (u_int)(cur % RADIX);
			prev = cur / RADIX;
		}
		if (prev) {
			res.data.push_back(prev % RADIX);
		}
		res.delete_zeroes();
		return res;
	}

	void operator*= (u_int const &b) {
		*this = *this * b;
	}

	void operator+= (u_int const &b) {
		u_ll prev = (u_ll)b;
		for (size_t i = 0; i < data.size(); ++i) {
			u_ll cur = data[i] + prev;
			data[i] = (u_int)(cur % RADIX);
			prev = cur / RADIX;
			if (prev == 0) {
				break;
			}
		}
		if (prev) {
			data.push_back(prev % RADIX);
		}
		delete_zeroes();
	}

	void operator-= (u_int const &b) {
		if (sign) {
			
		} else {

		}
	}

	u_int operator/= (u_int const &b) {
		u_int prev = 0;
		for (ptrdiff_t i = data.size() - 1; i >= 0; --i) {
			u_ll cur = make_long_from_int(prev, data[i]);
			prev = (u_int)(cur % (u_ll)b);
			data[i] = (u_int)(cur / (u_ll)b);
		}
		delete_zeroes();
		return prev;
	}

public:
	big_integer() {
		sign = false;
		data.resize(1, 0);
	}

	big_integer(int const &n) {
		data.resize(1);
		sign = n < 0;
		data[0] = abs(n);
	}

	big_integer(big_integer const &other) {
		data = other.data;
		sign = other.sign;
	}

	explicit big_integer(string const &s) {
		if (s.empty()) {
			data.resize(1, 0);
			sign = false;
		} else {
			data.resize(1, 0);
			if (s == "-0") {
				sign = false;
				return;
			} 
			sign = s[0] == '-';
			for (size_t i = sign; i < s.length(); ++i) {
				*this *= 10;
				*this += (u_int)(s[i] - '0');
			}
			delete_zeroes();
		}
	}

	~big_integer() {}

	string to_string() const {
		string res = "";
		big_integer copy = *this;
		while (copy != 0) {
			u_int mod = copy /= 10;
			res += (char)(mod + '0');
		}
		if (res.empty()) {
			res = "0";
		}
		if (sign) {
			res += '-';
		}
		reverse(res.begin(), res.end());
		return res;
	}

	big_integer& operator= (big_integer const &other) {
		sign = other.sign;
		data.resize(other.data.size());
		for (size_t i = 0; i < data.size(); ++i) {
			data[i] = other.data[i];
		}
		return *this;
	}

	friend const bool operator== (big_integer const &a, big_integer const &b);

	friend const bool operator!= (big_integer const &a, big_integer const &b);

	friend const bool operator< (big_integer const &a, big_integer const &b);
	
	friend const bool operator<= (big_integer const &a, big_integer const &b);

	friend const bool operator> (big_integer const &a, big_integer const &b);

	friend const bool operator>= (big_integer const &a, big_integer const &b);

	friend const big_integer operator+ (big_integer const &a, big_integer const &b);

	friend const big_integer operator- (big_integer const &a, big_integer const &b);

	friend const big_integer operator- (big_integer const &a);

	void swap(big_integer &other) {
		bool buff = sign;
		sign = other.sign;
		other.sign = buff;
		data.swap(other.data);
	}

	friend const big_integer operator+ (big_integer const &a);

	friend ostream& operator<< (ostream& os, big_integer const &number);

	friend const big_integer operator* (big_integer const &a, big_integer const &b);

	friend big_integer& operator+= (big_integer &a, big_integer const &b);

	friend big_integer& operator-= (big_integer &a, big_integer const &b);

	friend big_integer& operator*= (big_integer &a, big_integer const &b);

	friend const big_integer& operator++ (big_integer &a);

	friend const big_integer operator++ (big_integer &a, int);

	friend const big_integer& operator-- (big_integer &a);

	friend const big_integer operator-- (big_integer &a, int);

	friend const big_integer operator& (big_integer const &a, big_integer const &b);

	friend const big_integer operator| (big_integer const &a, big_integer const &b);

	friend const big_integer operator^ (big_integer const &a, big_integer const &b);

	friend big_integer& operator&= (big_integer &a, big_integer const &b);

	friend big_integer& operator|= (big_integer &a, big_integer const &b);

	friend big_integer& operator^= (big_integer &a, big_integer const &b);

	friend const big_integer operator~ (big_integer const &a);

	friend const big_integer operator<< (big_integer const &a, u_int const &shift);

	friend const big_integer operator>> (big_integer const &a, u_int const &shift);

	friend big_integer& operator>>= (big_integer &a, u_int const &shift);

	friend big_integer& operator<<= (big_integer &a, u_int const &shift);

	friend const big_integer operator/ (big_integer const &a, big_integer const &b);

	friend const big_integer operator% (big_integer const &a, big_integer const  &b);

	friend big_integer& operator/= (big_integer &a, big_integer const &b);

	friend big_integer& operator%= (big_integer &a, big_integer const &b);
};

const bool operator== (big_integer const &a, big_integer const &b) {
	if (a.sign != b.sign || a.data.size() != b.data.size()) {
		return false;
	}
	for (size_t i = 0; i < a.data.size(); ++i) {
		if (a.data[i] != b.data[i]) {
			return false;
		}
	}
	return true;
}

const bool operator!= (big_integer const &a, big_integer const &b) {
	return !(a == b);
}

const bool operator< (big_integer const &a, big_integer const &b) {
	if (a.sign && !b.sign) {
		return true;
	}
	if (!a.sign && b.sign) {
		return false;
	}
	bool res = false;
	size_t a_size = a.data.size(), b_size = b.data.size();
	if (a_size != b_size) {
		res = a_size < b_size;
	} else {
		res = true;
		ptrdiff_t i = a_size - 1;
		for (; i >= 0; --i) {
			if (a.data[i] != b.data[i]) {
				res = a.data[i] < b.data[i];
				break;
			}
		}
		if (i == -1) {
			return false;
		}
	}
	return res ^ a.sign;
}

const bool operator<= (big_integer const &a, big_integer const &b) {
	return a < b || a == b;
}

const bool operator> (big_integer const &a, big_integer const &b) {
	return !(a <= b);
}

const bool operator>= (big_integer const &a, big_integer const &b) {
	return !(a < b);
}

const big_integer operator+ (big_integer const &a, big_integer const &b) {
	big_integer res;
	if (a.sign != b.sign) {
		if (a.sign) {
			res = b - -a;
		} else {
			res = a - -b;
		}
	} else {
		res.sign = a.sign;
		u_int prev = 0;
		size_t a_size = a.data.size(), b_size = b.data.size();
		res.data.resize(max(a_size, b_size));
		for (size_t i = 0; i < max(a_size, b_size); ++i) {
			u_ll cur = (u_ll)prev;
			if (i < a_size) {
				cur += (u_ll)a.data[i];
			}
			if (i < b_size) {
				cur += (u_ll)b.data[i];
			}
			res.data[i] = (u_int)(cur % RADIX);
			prev = (u_int)(cur / RADIX);
		}
		if (prev) {
			res.data.push_back((u_int)prev);
		}
	}
	if (res == 0) {
		res.sign = false;
	}
	res.delete_zeroes();
	return res;
}

const big_integer operator- (big_integer const &a, big_integer const &b) {
	big_integer res;
	if (a.sign == b.sign) {
		big_integer subt;
		if (a.sign) {
			res = b;
			subt = a;
			res.sign ^= true;
			subt.sign ^= true;
		} else {
			res = a;
			subt = b;
		}
		bool swap_sign = false;
		if (res < subt) {
			swap_sign = true;
			res.swap(subt);
		}
		bool borrowed = false;
		for (size_t i = 0; i < res.data.size(); ++i) {
			ll first = (ll)res.data[i], second = 0ll;
			if (subt.data.size() > i) {
				second = (ll)subt.data[i];
			}
			first -= second + borrowed;
			if (first < 0) {
				borrowed = true;
				first += RADIX;
			} else {
				borrowed = false;
			}
			res.data[i] = (u_int)first;
		}
		res.sign ^= swap_sign;
	} else {
		if (a.sign) {
			res = -a + b;
		} else {
			res = a + -b;
		}
	}
	if (res == 0) {
		res.sign = false;
	}
	res.delete_zeroes();
	return res;
}

const big_integer operator- (big_integer const &a) {
	big_integer res = a;
	res.sign ^= true;
	return res;
}

const big_integer operator+ (big_integer const &a) {
	big_integer res = a;
	return res;
}

ostream& operator<< (ostream& os, big_integer const &number) {
	os << number.to_string();
	return os;
}

const big_integer operator* (big_integer const &a, big_integer const &b) {
	big_integer res;
	for (ptrdiff_t i = b.data.size() - 1; i >= 0; --i) {
		res <<= 32;
		res += a * b.data[i];
	}
	res.sign = a.sign ^ b.sign;
	if (res == 0) {
		res.sign = false;
	}
	return res;
}

big_integer& operator+= (big_integer &a, big_integer const &b) {
	return a = a + b;
}

big_integer& operator-= (big_integer &a, big_integer const &b) {
	return a = a - b;
}

big_integer& operator*= (big_integer &a, big_integer const &b) {
	return a = a * b;
}

const big_integer& operator++ (big_integer &a) {
	if (a.sign) {
		a.sign ^= true;
		--a;
		a.sign ^= true;
	} else {
		bool was = true;
		for (size_t i = 0; i < a.data.size(); ++i) {
			u_ll cur = (u_ll)a.data[i] + (u_ll)was;
			if (cur == RADIX) {
				a.data[i] = 0;
			} else {
				a.data[i]++;
				was = false;
				break;
			}
		}
		if (was) {
			a.data.push_back(1);
		}
	}
	if (a == 0) {
		a.sign = false;
	}
	a.delete_zeroes();
	return a;
}

const big_integer operator++ (big_integer &a, int) {
	big_integer res = a;
	++a;
	return res;
}

const big_integer& operator-- (big_integer &a) {
	if (a.sign) {
		a.sign ^= true;
		++a;
		a.sign ^= true;
	} else {
		if (a == 0) {
			a.sign = true;
			a.data[0] = 1;
		} else {
			ptrdiff_t i = 0;
			while (a.data[i] == 0) {
				++i;
			}
			a.data[i--]--;
			for (; i >= 0; --i) {
				a.data[i] = (u_int)(RADIX - 1);
			}
		}
	}
	if (a == 0) {
		a.sign = false;
	}
	a.delete_zeroes();
	return a;
}

const big_integer operator-- (big_integer &a, int) {
	big_integer res = a;
	--a;
	return res;
}

const big_integer operator& (big_integer const &a, big_integer const &b) {
	vector<u_int> first = a.make_two_complement(), second = b.make_two_complement();
	size_t n = max(first.size(), second.size());
	vector<u_int> res(n);
	for (size_t i = 0; i < n; ++i) {
		u_int c =  i < first.size() ? first[i] : 0;
		u_int d = i < second.size() ? second[i] : 0;
		res[i] = c & d;
	}
	return big_integer(res);
}

const big_integer operator| (big_integer const &a, big_integer const &b) {
	vector<u_int> first = a.make_two_complement(), second = b.make_two_complement();
	size_t n = max(first.size(), second.size());
	vector<u_int> res(n);
	for (size_t i = 0; i < n; ++i) {
		u_int c = i < first.size() ? first[i] : 0;
		u_int d = i < second.size() ? second[i] : 0;
		res[i] = c | d;
	}
	return big_integer(res);
}

const big_integer operator^ (big_integer const &a, big_integer const &b) {
	vector<u_int> first = a.make_two_complement(), second = b.make_two_complement();
	size_t n = max(first.size(), second.size());
	vector<u_int> res(n);
	for (size_t i = 0; i < n; ++i) {
		u_int c = i < first.size() ? first[i] : 0;
		u_int d = i < second.size() ? second[i] : 0;
		res[i] = c ^ d;
	}
	return big_integer(res);
}

big_integer& operator&= (big_integer &a, big_integer const &b) {
	return a = a & b;
}

big_integer& operator|= (big_integer &a, big_integer const &b) {
return a = a | b;
}

big_integer& operator^= (big_integer &a, big_integer const &b) {
	return a = a ^ b;
}

const big_integer operator~ (big_integer const &a) {
	vector<u_int> res = a.make_two_complement();
	for (size_t i = 0; i < res.size(); ++i) {
		res[i] = ~res[i];
	}
	return big_integer(res);
}

const big_integer operator<< (big_integer const &a, u_int const &shift) {
	size_t big = shift / BIT, small = shift % BIT;
	vector<u_int> res(big + a.data.size());
	u_int prev = 0;
	for (size_t i = big; i < res.size(); ++i) {
		res[i] = (a.data[i - big] << small) | prev;
		if (small > 0) {
			prev = a.data[i - big] >> (BIT - small);
		} else {
			prev = 0;
		}
	}
	if (prev) {
		res.push_back(prev);
	}
	return big_integer(res, a.sign);
}

const big_integer operator >> (big_integer const &a, u_int const &shift) {
	size_t big = shift / BIT, small = shift % BIT, shift1 = BIT - small;
	if (big >= a.data.size()) {
		return 0;
	}
	vector<u_int> res(a.data.size() - big);
	for (size_t i = 0; i < res.size(); ++i) {
		res[i] = a.data[i + big] >> small;
		if (i + 1 < res.size()) {
			res[i] |= (a.data[i + big + 1] >> shift1) << shift1;
		}
	}
	while (res.size() > 1 && res.back() == 0) {
		res.pop_back();
	}
	bool s = a.sign;
	if (res.size() == 1 && res[0] == 0) {
		s = false;
	}
	return big_integer(res, s);
}

big_integer& operator>>= (big_integer &a, u_int const &shift) {
	return a = a >> shift;
}

big_integer& operator<<= (big_integer &a, u_int const &shift) {
	return a = a << shift;
}

const big_integer operator/ (big_integer const &a, big_integer const &b) {
	big_integer a_copy = a, b_copy = b;
	while (b_copy.data.back() < (RADIX >> 1)) {
		b_copy <<= 1;
		a_copy <<= 1;
	}
	big_integer res;
	size_t n = b_copy.data.size(), m = a_copy.data.size() - n;
	res.data.reserve(m + 1);
	big_integer shift = b_copy << (BIT * m);
	if (a_copy < shift) {
		res.data.back() = 0;
	} else {
		res.data.back() = 1;
		a_copy -= shift;
	}
	for (ptrdiff_t j = m - 1; j >= 0; --j) {
		u_int qj = min(RADIX - 1, (((u_ll)a_copy.data[n + j] * RADIX + a_copy.data[n + j - 1]) / (u_ll)b_copy.data.back()));
		shift >>= BIT;
		a_copy -= shift * qj;
		while (a_copy.sign) {
			qj--;
			a_copy += shift;
		}
		res.data[j] = qj;
	}
	res.sign = a.sign ^ b.sign;
	res.delete_zeroes();
	if (res == 0) {
		res.sign = false;
	}
	return res;
}
const big_integer operator% (big_integer const &a, big_integer const &b) {
	return a - b * (a / b);
}

big_integer& operator/= (big_integer &a, big_integer const &b) {
	return a = a / b;
}

big_integer& operator%= (big_integer &a, big_integer const &b) {
	return a = a % b;
}

//----------------------------------------------------------------------------------------
char rnd() {
	return (char)((rand() % 10) + '0');
}

int main() {
	/*string s1 = "", s2 = "";	
	for (size_t i = 0; i < 100000; ++i) {
		s1 += rnd();
		s2 += rnd();
	}
	big_integer a(s1), b(s2);
	a & b;
	cerr << clock() * 1. / CLOCKS_PER_SEC << "\n";*/
	big_integer a("10"), b("2");
	cout << a % b;

	return 0;
}