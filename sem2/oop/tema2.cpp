#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

class Monom {
	int grad;
	int coef;

public:
	Monom() :grad(0), coef(0) {}
	Monom(int grad, int coef) :grad(grad), coef(coef) {}
	Monom(const Monom& m) :grad(m.grad), coef(m.coef) {}
	Monom& operator=(const Monom& m) { grad = m.grad; coef = m.coef; return *this; }

	friend class Polinom;
	friend class PolinomIreductibil;

	friend std::istream& operator>>(std::istream& is, Monom& m)
	{
		return is >> m.coef >> m.grad;
	}

	friend std::ostream& operator<<(std::ostream& out, const Monom& m)
	{
		if (m.coef == 0)
			return out;
		if (m.grad == 0)
			return out << m.coef;
		if (m.grad == 1)
			return out << m.coef << "x";
		if (m.coef == 1)
			return out << "x^" << m.grad;

		return out << m.coef << "x^" << m.grad;
	}
};

class Polinom {
protected:
	std::vector<Monom> data;

public:
	Polinom() = default;
	Polinom(std::initializer_list<std::pair<int, int>> list)
	{
		for (auto x : list)
			data.push_back({x.first, x.second});
	}

	virtual ~Polinom() = default;

	virtual void print(std::ostream& out) const
	{
		for (int i = 0; i < data.size() - 1; i++)
			out << (data[i].coef > 0 ? " +" : " ") << data[i];
		out << data.back();
	}

	friend std::istream& operator>>(std::istream& is, Polinom& p)
	{
		int n;
		is >> n;
		p.data.resize(n);
		for (Monom& m : p.data)
			is >> m;
		return is;
	}

	friend std::ostream& operator<<(std::ostream& out, const Polinom& p)
	{
		p.print(out);
		return out;
	}
};


int getPrime(int i)
{
	auto isPrime = [](int n) {
		if (n == 0 || n == 1 || n % 2 == 0)
			return false;
		for (int d = 3; d < n / 2; d += 2)
			if (n % d == 0)
				return false;
		return true;
	};

	static std::vector<int> primes{2};
	if (i >= primes.size()) {
		for (int p = primes.back()+1; ; p++) {
			if (isPrime(p)) {
				primes.push_back(p);
				break;
			}
		}
	}
	return primes[i];
}


int gcd(int a, int b)
{
	if (b == 0)
		return a;
	return gcd(b, a % b);

}

class PolinomIreductibil : public Polinom{

public:
	PolinomIreductibil() = default;
	PolinomIreductibil(std::initializer_list<std::pair<int, int>> list) : Polinom(list) { }
	virtual ~PolinomIreductibil() = default;

	void print(std::ostream& out) const override
	{
		out << "polinomul ";
		this->Polinom::print(out);
		out << " este " << (this->verifIreduct() ? "i" : "") << "reductibil";
	}

	bool verifIreduct() const
	{
		Monom cmmdc = std::accumulate( data.begin() + 1, data.end(), data[0],
			[](Monom a, Monom b) { 
				return Monom(1, gcd(std::abs(a.coef), std::abs(b.coef))); 
			});

		//printf("gcd %d\n", cmmdc.coef);
		if (cmmdc.coef > 1) {
			return false;
		}

		Monom min = *std::min_element(this->data.begin(), this->data.end()-1, 
									  [](Monom a, Monom b) {return std::abs(a.coef) < std::abs(b.coef); });
		if (min.coef == 1)
			return true;

		int minCoef = std::abs(min.coef);
		for (int primePos = 0; ; primePos++) {
			int prime = getPrime(primePos);
			if (prime > minCoef)
				break;
			bool ok = true;
			for (int i = 0; i < data.size() - 1; i++) {
				if (std::abs((int) data[i].coef) % prime != 0) {
					ok = false;
					break;
				}
			}
			if (std::abs((int) data.back().coef) % prime == 0)
				ok = false;
			if (((int) data[0].coef) % (prime * prime) == 0)
				ok = false;
			if (ok)
				return true;
		}
		return false;
	}

	/*
	friend std::istream& operator>>(std::istream& is, PolinomIreductibil& m)
	{
		Polinom* p = &m;
		is >> *p;
		return is;
	}
	*/

	friend std::ostream& operator<<(std::ostream& out, const PolinomIreductibil& m)
	{
		m.print(out);
		return out;
	}
};

template <typename T>
void read_print(int n, std::istream& in=std::cin, std::ostream& out=std::cout)
{
	for (int i = 0; i < n; i++) {
		T p;
		in >> p;
		out << p;
	}
}

int main()
{
	//printf("gcd %d\n", gcd(2, 1));
	int v[3] = {4, 2, 3};
	int cmmdc = std::accumulate(v + 1, v + 3, v[0], gcd);
	//printf("gcd %d\n", cmmdc);

	PolinomIreductibil pol{{0, 2}, { 1, -4 }, {2, 6}, {3, -7}};
	//PolinomIreductibil pol{{0, 13}, {1, -7},{2, 0}, { 3, 1 }, {4, 1}};
	//PolinomIreductibil pol{{0, 1}, {1, 1},{2, 1}};
	std::cout << pol << '\n';
	//std::cout << pol.verifIreduct();
	//for (int i = 0; i < 10; i++)
	//	std::cout << getPrime(i) << ' ';
	//std::cout << 2 % 4;
	read_print<Polinom>(0);
	read_print<PolinomIreductibil>(0);
}