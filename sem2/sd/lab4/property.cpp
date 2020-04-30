#include <functional>
#include <iostream>
#include <type_traits>

template <typename T>//, typename getter_t, typename setter_t>
class Property {

	//using getter_t = T& (*)();
	//using setter_t = void (*)(const T&);
	using getter_t = std::function<T& ()>;
	using setter_t = std::function<void(const T&)>;
	getter_t get;
	setter_t set;

	//T data;

public:
	Property(T& value)
	{
		get = [&value]()->T& {return value; };
		set = [&value](const T& newValue) {value = newValue; };
	}

	Property(getter_t get, setter_t set) :get(get), set(set) {}

	T& operator=(const T& t)
	{
		set(t);
		return get();
	}

	//operator T() { return get(); }

	operator T& () { return get(); }
};

//template <typename T, typename getter_t, typename setter_t>
//Property(getter_t, setter_t)->Property<T, getter_t, setter_t>;

struct Prop : std::pair<int, int> {

};

int main2()
{
	int i;
	//auto get = [&i]() -> int& { return (i); };
	//auto set = [&i](int x) { i = x; };
	Property<int> p(i); //(get, set);

	p = 4;
	int x = p;
	x = 3;
	std::cout << i;
	return 0;
}
