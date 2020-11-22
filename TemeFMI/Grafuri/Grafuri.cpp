#include "Graf.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string_view>
#include <string>
#include <functional>
#include <set>
#include <algorithm>
#include <queue>
using std::cout;


class GrafMatrice : public IGraf {
	std::vector<std::vector<bool>> m_graf;

public:
	GrafMatrice(int nrNoduri, int nrMuchii, bool one_index, GrafType type) 
		: IGraf(nrNoduri, nrMuchii, one_index, type), 
		m_graf(nr_noduri, std::vector<bool>(nr_muchii, 0))
	{
	}

	void connect(int n1, int n2) override
	{
		m_graf[n1][n2] = true;
	}

	bool is_connected(int n1, int n2) const override
	{
		return m_graf[n1][n2];
	}

	void for_neighbours(int n, std::function<void(int)> f) const override
	{
		for (int i = start_index; i < nr_muchii; i++) {
			if (m_graf[n][i])
				f(i);
		}
	}
};

class GrafListaVector : public IGraf {
	std::vector<std::vector<int>> m_lista;

public:
	GrafListaVector(int nrNoduri, int nrMuchii, bool one_index, GrafType type)
		: IGraf(nrNoduri, nrMuchii, one_index, type), 
		m_lista(nr_noduri)
	{
	}

	void connect(int n1, int n2) override
	{
		m_lista[n1].push_back(n2);
	}

	bool is_connected(int n1, int n2) const override
	{
		return m_lista[n1].end() != std::find(m_lista[n1].begin(), m_lista[n1].end(), n2) ;
	}

	void for_neighbours(int n, std::function<void(int)> f) const override
	{
		for (auto e : m_lista[n]) {
			f(e);
		}
	}
};

class GrafListaSet : public IGraf {
	std::vector<std::set<int>> m_lista;

public:
	GrafListaSet(int nrNoduri, int nrMuchii, bool one_index, GrafType type) 
		: IGraf(nrNoduri, nrMuchii, one_index, type), 
		m_lista(nr_noduri)
	{
	}

	void connect(int n1, int n2) override
	{
		m_lista[n1].insert(n2);
	}

	bool is_connected(int n1, int n2) const override
	{
		return m_lista[n1].find(n2) != m_lista[n1].end();
	}

	void for_neighbours(int n, std::function<void(int)> f) const override
	{
		for (auto e : m_lista[n]) {
			f(e);
		}
	}
};



class GrafMatricePonderat : public IGrafPonderat {
	std::vector<std::vector<int>> m_graf;

public:
	GrafMatricePonderat(int nrNoduri, int nrMuchii, bool one_index, GrafType type) 
		: IGrafPonderat(nrNoduri, nrMuchii, one_index, type), 
		m_graf(nr_noduri, std::vector<int>(nr_muchii, UNREACHABLE_NODE))
	{
	}

	void connect(int n1, int n2, int p) override
	{
		m_graf[n1][n2] = p;
	}

	int pondere(int n1, int n2) const override
	{
		return m_graf[n1][n2];
	}

	bool is_connected(int n1, int n2) const override
	{
		return m_graf[n1][n2] == UNREACHABLE_NODE;
	}

	void for_neighbours(int n, std::function<void(int, int)> f) const override
	{
		for (int i = start_index; i < nr_muchii; i++) {
			if (m_graf[n][i] != UNREACHABLE_NODE)
				f(i, m_graf[n][i]);
		}
	}
};

struct NodeNeighbour {
	int node;
	int pondere;
	friend bool operator==(const NodeNeighbour& a, const NodeNeighbour& b)
	{
		return a.node == b.node;
	}
	friend bool operator<(const NodeNeighbour& a, const NodeNeighbour& b)
	{
		return a.node < b.node;
	}
};

class GrafListaVectorPonderat : public IGrafPonderat {
	std::vector<std::vector<NodeNeighbour>> m_lista;

public:
	GrafListaVectorPonderat(int nrNoduri, int nrMuchii, bool one_index, GrafType type)
		: IGrafPonderat(nrNoduri, nrMuchii, one_index, type), 
		m_lista(nr_noduri)
	{
	}

	void connect(int n1, int n2, int p) override
	{
		m_lista[n1].push_back({ n2, p });
	}

	int pondere(int n1, int n2) const override
	{
		auto it = std::find(m_lista[n1].begin(), m_lista[n1].end(), NodeNeighbour{n2, 0});
		if (it == m_lista[n1].end())
			return UNREACHABLE_NODE;
		else
			return it->pondere;
	}

	bool is_connected(int n1, int n2) const override
	{
		return pondere(n1, n2) != UNREACHABLE_NODE;
	}

	void for_neighbours(int n, std::function<void(int, int)> f) const override
	{
		for (const auto& e : m_lista[n]) {
			f(e.node, e.pondere);
		}
	}
};

class GrafListaSetPonderat : public IGrafPonderat {
	std::vector<std::set<NodeNeighbour>> m_lista;

public:
	GrafListaSetPonderat(int nrNoduri, int nrMuchii, bool one_index, GrafType type) 
		: IGrafPonderat(nrNoduri, nrMuchii, one_index, type), 
		m_lista(nr_noduri)
	{
	}

	void connect(int n1, int n2, int p) override
	{
		m_lista[n1].insert({ n2, p });
	}

	int pondere(int n1, int n2) const override
	{
		if (auto it = m_lista[n1].find({ n2, 0 }); it != m_lista[n1].end())
			return it->pondere;
		else
			return UNREACHABLE_NODE;
	}

	bool is_connected(int n1, int n2) const override
	{
		return pondere(n1, n2) != UNREACHABLE_NODE;
	}

	void for_neighbours(int n, std::function<void(int, int)> f) const override
	{
		for (const auto& e : m_lista[n]) {
			f(e.node, e.pondere);
		}
	}
};



auto makeGraf(int nrNoduri, int nrMuchii, bool oneIndex, GrafType type) -> std::unique_ptr<IGraf>
{
	switch (type) {
	case GrafType::Matrix:
		return std::make_unique<GrafMatrice>(nrNoduri, nrMuchii, oneIndex, type);
	case GrafType::ListVector:
		return std::make_unique<GrafListaVector>(nrNoduri, nrMuchii, oneIndex, type);
	case GrafType::ListSet:
		return std::make_unique<GrafListaSet>(nrNoduri, nrMuchii, oneIndex, type);
	default:
		return nullptr;
	}
}

auto readGraf(std::string file, GrafConfig conf) -> std::unique_ptr<IGraf>
{
	using namespace std;

	int nrNoduri, nrMuchii;;
	auto fin = ifstream(file);

	fin >> nrNoduri >> nrMuchii;

	auto graf = makeGraf(nrNoduri, nrMuchii, conf.one_index, conf.type);

	for (int i = 0; i < nrMuchii; i++) {
		int a, b;
		fin >> a >> b;
		graf->connect(a, b);
		if (not conf.orientat)
			graf->connect(b, a);
	}
	return graf;
}

auto makeGrafPonderat(int nrNoduri, int nrMuchii, bool oneIndex, GrafType type)->std::unique_ptr<IGrafPonderat>
{
	switch (type) {
	case GrafType::Matrix:
		return std::make_unique<GrafMatricePonderat>(nrNoduri, nrMuchii, oneIndex, type);
	case GrafType::ListVector:
		return std::make_unique<GrafListaVectorPonderat>(nrNoduri, nrMuchii, oneIndex, type);
	case GrafType::ListSet:
		return std::make_unique<GrafListaSetPonderat>(nrNoduri, nrMuchii, oneIndex, type);
	default:
		return nullptr;
	}
}

auto readGrafPonderat(std::string file, GrafConfig config)->std::unique_ptr<IGrafPonderat>
{
	using namespace std;

	int nrNoduri, nrMuchii;;
	auto fin = ifstream(file);

	fin >> nrNoduri >> nrMuchii;

	auto graf = makeGrafPonderat(nrNoduri, nrMuchii, config.one_index, config.type);

	for (int i = 0; i < nrMuchii; i++) {
		int a, b, p;
		fin >> a >> b >> p;
		graf->connect(a, b, p);
		if (not config.orientat)
			graf->connect(b, a, p);
	}
	return graf;
}


auto transposeGraf(IGraf* g) -> std::unique_ptr<IGraf>
{
	auto tr = makeGraf(g->nr_noduri, g->nr_muchii, g->one_index, g->type);
	for (int i = g->start_index; i < g->nr_noduri; i++) {
		g->for_neighbours(i, [&](int n) {
			tr->connect(n, i);
		});
	}
	return tr;
}

auto transposeGrafPonderat(IGrafPonderat* g) -> std::unique_ptr<IGrafPonderat>
{
	auto tr = makeGrafPonderat(g->nr_noduri, g->nr_muchii, g->one_index, g->type);
	for (int i = g->start_index; i < g->nr_noduri; i++) {
		g->for_neighbours(i, [&](int n, int p) {
			tr->connect(n, i, p);
		});
	}
	return tr;
}

void dfs_impl(IGraf* g, int p, std::vector<bool>& viz, SearchVisitor& visitor)
{
	viz[p] = true;
	visitor.in(p);
	g->for_neighbours(p, [&](int n) {
		if (!viz[n]) {
			visitor.pre(p, n);
			dfs_impl(g, n, viz, visitor);
			visitor.post(p, n);
		}
		else {
			visitor.already_visited(p, n);
		}
	});
	visitor.finalize(p);
}

void depth_first_search(IGraf* g, SearchVisitor& visitor, std::vector<bool>& viz, int start_node)
{
	if (start_node == -1)
		start_node = g->start_index;
	visitor.start(start_node);
	dfs_impl(g, start_node, viz, visitor);
}

void breadth_first_search(IGraf* g, SearchVisitor& visitor, std::vector<bool>& viz, int start_node)
{
	if (start_node == -1)
		start_node = g->start_index;
	auto c = std::queue<int>();
	c.push(start_node);
	viz[start_node] = true;
	visitor.start(start_node);
	while (c.size() > 0) {
		int p = c.front(); c.pop(); // p-nodul curent
		visitor.in(p);
		g->for_neighbours(p, [&](int n) {
			if (!viz[n]) {
				viz[n] = true;
				visitor.pre(p, n);
				c.push(n);
			}
			else
				visitor.already_visited(p, n);
		});
		visitor.finalize(p);
	}
}

void dfs_impl_pon(IGrafPonderat* g, int a, std::vector<bool>& viz, SearchVisitorPonderat& visitor)
{
	viz[a] = true;
	visitor.in(a);
	g->for_neighbours(a, [&](int b, int p) {
		if (!viz[b]) {
			visitor.pre(a, b, p);
			dfs_impl_pon(g, b, viz, visitor);
			visitor.post(a, b, p);
		}
		else {
			visitor.already_visited(a, b, p);
		}
	});
	visitor.finalize(a);
}

void depth_first_search(IGrafPonderat* g, SearchVisitorPonderat& visitor, std::vector<bool>& viz, int start_node)
{
	if (start_node == -1)
		start_node = g->start_index;
	visitor.start(start_node);
	dfs_impl_pon(g, start_node, viz, visitor);
}
void breadth_first_search(IGrafPonderat* g, SearchVisitorPonderat& visitor, std::vector<bool>& viz, int start_node)
{
	if (start_node == -1)
		start_node = g->start_index;
	auto c = std::queue<int>();
	c.push(start_node);
	viz[start_node] = true;
	visitor.start(start_node);
	while (c.size() > 0) {
		int a = c.front(); c.pop(); // p-nodul curent
		visitor.in(a);
		g->for_neighbours(a, [&](int b, int p) {
			if (!viz[b]) {
				viz[b] = true;
				visitor.pre(a, b, p);
				c.push(b);
			}
			else
				visitor.already_visited(a, b, p);
		});
		visitor.finalize(a);
	}
}

struct SVCompose : SearchVisitor {

	std::vector<SearchVisitor*> visitors;

	SVCompose(std::initializer_list<std::reference_wrapper<SearchVisitor>> list)
	{
		for (auto v : list)
			visitors.push_back(&v.get());
	}

	void start(int n) override
	{
		for (auto& v : visitors)
			v->start(n);
	}
	void in(int n) override
	{
		for (auto& v : visitors)
			v->in(n);
	}
	void pre(int p, int n) override
	{
		for (auto& v : visitors)
			v->pre(p, n);
	}
	void finalize(int n) override
	{
		for (auto& v : visitors)
			v->finalize(n);
	}
	void post(int p, int n) override
	{
		for (auto& v : visitors)
			v->post(p, n);
	}
	void already_visited(int p, int n) override
	{
		for (auto& v : visitors)
			v->already_visited(p, n);
	}
};

struct SVComposePonderat : SearchVisitorPonderat {

	std::vector<SearchVisitorPonderat*> visitors;

	SVComposePonderat(SearchVisitorListPonderat list)
	{
		for (auto v : list)
			visitors.push_back(&v.get());
	}

	void start(int n) override
	{
		for (auto& v : visitors)
			v->start(n);
	}
	void in(int n) override
	{
		for (auto& v : visitors)
			v->in(n);
	}
	void pre(int a, int b, int p) override
	{
		for (auto& v : visitors)
			v->pre(a, b, p);
	}
	void finalize(int n) override
	{
		for (auto& v : visitors)
			v->finalize(n);
	}
	void post(int a, int b, int p) override
	{
		for (auto& v : visitors)
			v->post(a, b, p);
	}
	void already_visited(int a, int b, int p) override
	{
		for (auto& v : visitors)
			v->already_visited(a, b, p);
	}
};


void depth_first_search(IGraf* g, SearchVisitorList list, std::vector<bool>& viz, int start_node)
{
	auto comp = SVCompose(list);
	depth_first_search(g, comp, viz, start_node);
}
void breadth_first_search(IGraf* g, SearchVisitorList list, std::vector<bool>& viz, int start_node)
{
	auto comp = SVCompose(list);
	breadth_first_search(g, comp, viz, start_node);
}
void depth_first_search(IGraf* g, SearchVisitorList list, int start_node)
{
	auto viz = std::vector<bool>(g->nr_noduri, false);
	depth_first_search(g, list, viz, start_node);
}
void breadth_first_search(IGraf* g, SearchVisitorList list, int start_node)
{
	auto viz = std::vector<bool>(g->nr_noduri, false);
	breadth_first_search(g, list, viz, start_node);
}


void depth_first_search(IGrafPonderat* g, SearchVisitorListPonderat list, std::vector<bool>& viz, int start_node)
{
	auto comp = SVComposePonderat(list);
	depth_first_search(g, comp, viz, start_node);
}
void breadth_first_search(IGrafPonderat* g, SearchVisitorListPonderat list, std::vector<bool>& viz, int start_node)
{
	auto comp = SVComposePonderat(list);
	breadth_first_search(g, comp, viz, start_node);
}
void depth_first_search(IGrafPonderat* g, SearchVisitorListPonderat list, int start_node)
{
	auto viz = std::vector<bool>(g->nr_noduri);
	depth_first_search(g, list, viz, start_node);
}
void breadth_first_search(IGrafPonderat* g, SearchVisitorListPonderat list, int start_node)
{
	auto viz = std::vector<bool>(g->nr_noduri);
	breadth_first_search(g, list, viz, start_node);
}

