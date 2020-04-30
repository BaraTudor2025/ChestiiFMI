#include <iostream>
#include <memory>
#include <functional>
#include <type_traits>

template <typename T, typename TComp = std::less<T> >
class BinaryTree {

	struct node_t {
		std::unique_ptr<node_t> left = nullptr;
		std::unique_ptr<node_t> right = nullptr;
		T value;

		node_t(T data) :value(data) {}
	};

	std::unique_ptr<node_t> root = nullptr;

	bool comp(const T& a, const T& b) { return TComp()(a, b); }

	template <typename F>
	void implForEach(int mode, node_t* node, F f)
	{
		if (node) {
			if (mode == 0) {
				implForEach(mode, node->left.get(), f);
				f(node->value);
				implForEach(mode, node->right.get(), f);
			} else if (mode == 1) {
				f(node->value);
				implForEach(mode, node->left.get(), f);
				implForEach(mode, node->right.get(), f);
			} else if (mode == 2) {
				implForEach(mode, node->left.get(), f);
				implForEach(mode, node->right.get(), f);
				f(node->value);
			}
		}
	}

	bool simpleErase(node_t* parent, node_t* node)
	{
		bool isLeft = parent->left.get() == node;
		std::unique_ptr<node_t> node_t::* childPtr = isLeft ? &node_t::left : &node_t::right;
		if (!node->left && !node->right) {
			parent->*childPtr = nullptr;
		} else if (node->left && !node->right) {
			parent->*childPtr = std::move(node->left);
		} else if (!node->left && node->right) {
			parent->*childPtr = std::move(node->right);
		} else {
			return false;
		}
		return true;
	}

	void implInterval(node_t* node, T lb, T ub)
	{
		if (!node)
			return;
		if (lb < node->value)
			implInterval(node->left.get(), lb, ub);
		if (lb < node->value && node->value < ub)
			std::cout << node->value << ' ';
		if (node->value < ub)
			implInterval(node->right.get(), lb, ub);
	}

public:

	BinaryTree() = default;

	BinaryTree(std::initializer_list<T> list)
	{
		for (T x : list)
			insert(x);
	}

	void insert(T value)
	{
		if (!root)
			root = std::make_unique<node_t>(value);
		else {
			node_t* node = root.get();
			while (true) {
				if (comp(value, node->value)) {
					if (node->left) {
						node = node->left.get();
					} else {
						node->left = std::make_unique<node_t>(value);
						break;
					}
				} else {
					if (node->right) {
						node = node->right.get();
					} else {
						node->right = std::make_unique<node_t>(value);
						break;
					}
				}
			}
		}
	}

	bool contains(T value)
	{
		node_t* node = root.get();
		while (node && node->value != value) {
			if (comp(value, node->value))
				node = node->left.get();
			else
				node = node->right.get();
		}
		return (node && node->value == value);
	}

	template<typename F>
	void inOrder(F f) {
		implForEach(0, root.get(), f);
	}

	template<typename F>
	void preOrder(F f) {
		implForEach(1, root.get(), f);
	}

	template<typename F>
	void postOrder(F f) {
		implForEach(2, root.get(), f);
	}

	void interval(T lb, T ub)
	{
		implInterval(root.get(), lb, ub);
	}

	void erase(T value)
	{
		node_t* node = root.get();
		node_t* parent = node;
		while (node && node->value != value) {
			parent = node;
			if (comp(value, node->value))
				node = node->left.get();
			else
				node = node->right.get();
		}

		bool exists = (node && node->value == value);
		if (!exists)
			return;

		bool deepRootChild = false;
		if (parent == node) { // root needs to be erased
			if (!root->left && !root->right)
				root = nullptr;
			else if (root->left && !root->right)
				root = std::move(root->left);
			else if (!root->left && root->right)
				root = std::move(root->right);
			else
				deepRootChild = true;
		}
		if (deepRootChild || !simpleErase(parent, node)) {
			node_t* candidate = node->left.get();
			node_t* candidateParent = candidate;
			while (candidate->right) {
				candidateParent = candidate;
				candidate = candidate->right.get();
			}
			node->value = std::move(candidate->value);
			if (candidate == candidateParent)
				simpleErase(node, candidate);
			else
				simpleErase(candidateParent, candidate);
		}
	}

};

int main()
{
	BinaryTree<int> tree{4, 5, 2, 3, 6, 1, 9, 7, 8};
	std::cout << std::boolalpha;
	tree.preOrder([&](auto x) {std::cout << x << ':' << tree.contains(x) << ", "; });
	std::cout << '\n';

	tree.erase(4);
	tree.erase(5);
	tree.inOrder([&](auto x) {std::cout << x << ' '; });
	std::cout << '\n';

	tree.interval(3, 8);
	std::cout << '\n';

	std::cout << tree.contains(6) << ' ' << tree.contains(5) << '\n';

	BinaryTree<std::string> stree{"mama", "tata", "aaaa", "bbbb", "aaab", "nush", "ceva", "altceva", "incaceva"};
	stree.inOrder([&](auto x) {std::cout << x << ' '; });
}

/*
	Output:
	4:true, 2 : true, 1 : true, 3 : true, 5 : true, 6 : true, 9 : true, 7 : true, 8 : true,
	1 2 3 6 7 8 9
	6 7
	true false
	aaaa aaab altceva bbbb ceva incaceva mama nush tata
*/


