#include "Graf.hpp"
#include <iostream>
#include <vector>
#include <deque>
#include <stack>
#include <algorithm>
#include <deque>
#include <queue>

//import Graf;

//using namespace Graf;
using std::count;

void lab1_drum_min_parc(IGraf* graf)
{
	auto pct_ctrl = std::vector<int>{ 8, 9 };
	int node_start = 1;
	auto svpath = SVPath();
	auto svparent = SVParents(graf);
	breadth_first_search(graf, { svpath, svparent }, node_start);
	int node = -1;
	for (int n : svpath.path) {
		for (int p : pct_ctrl)
			if (p == n) {
				node = n;
				break;
			}
		if (node != -1)
			break;
	}
	//for (int i = node_start; i < graf->nr_noduri; i++)
	//	std::cout << i << ':' << svparent.parent[i] << ' ';

	print_link(node_start, node, svparent.parent);
	std::cout << '\n';
	//int it = node;
	//while (it != node_start) {
	//	std::cout << it << ' ';
	//	it = svparent.parent[it];
	//}
	//std::cout << node_start;
}

struct SVCiclu : SearchVisitor {
	const SVParents* svp;
	const SVFinalized* svf;
	SVCiclu(IGraf* g, SVParents& p, SVFinalized& f) : svp(&p), svf(&f) {}

	void already_visited(int p, int n) override
	{
		if (svf->finalized[p]) {
			print_link(p, n, svp->parent);
			std::cout << '\n';
		}
		//if (svp->parent[n] != p) {
		//	print_link(p, n, svp->parent);
		//	std::cout << '\n';
		//}
	}
};

int numarComponenteConexe(IGraf* g)
{
	int nrComp = 0;
	auto viz = std::vector<bool>(g->nr_noduri);
	auto svnull = SVNull();
	for (int i = g->start_index; i < g->nr_noduri; i++) {
		if (!viz[i]) {
			nrComp++;
			breadth_first_search(g, svnull, viz, g->start_index);
		}
	}
	return nrComp;
}

// print componente tare conexe
void kosaraju(IGraf* g)
{
	auto svfin = SVFinalizedStack();
	auto viz = std::vector<bool>(g->nr_noduri, false);
	for (int i = g->start_index; i < g->nr_noduri; i++)
		if (!viz[i])
			depth_first_search(g, svfin, viz, i);
	viz.flip();

	auto tr = transposeGraf(g);
	for (auto& stack = svfin.stack; !stack.empty(); stack.pop()) {
		if (!viz[stack.top()]) {
			auto svpath = SVPath();
			auto svparents = SVParents(tr.get());
			depth_first_search(tr.get(), { svpath, svparents }, viz, stack.top());
			for (auto e : svpath.path)
				std::cout << e << ' ';
			std::cout << '\n';
		}
	}
}

void muchiiCritice(IGraf* g)
{
	struct SVCriticalEgde : SearchVisitor {
		const SVLowPath* svlowpath;
		SVCriticalEgde(const SVLowPath& low) : svlowpath(&low) {}
		void post(int p, int n) override
		{
			if (svlowpath->nrMin[n] > svlowpath->nr[p])
				std::cout << p << ' ' << n << '\n';
		}
	};
	auto viz = std::vector<bool>(g->nr_noduri);
	auto svlow = SVLowPath(g);
	auto svcrit = SVCriticalEgde(svlow);
	g->for_nodes([&](int i) {
		if (!viz[i]) {
			depth_first_search(g, { svlow, svcrit }, viz, i);
		}
	});
}

void tarjan(IGraf* g)
{
	struct SVTarjan : SearchVisitor {
		const SVLowPath* svlowpath;
		std::vector<bool> inStack;
		std::stack<int> stack;
		SVTarjan(IGraf* g, const SVLowPath& p) : svlowpath(&p), inStack(g->nr_noduri) {}
		void in(int n) override
		{
			stack.push(n);
			inStack[n] = true;
		}
		void finalize(int n) override
		{
			if (svlowpath->nrMin[n] == svlowpath->nr[n]) {
				int i = 0;
				do {
					i = stack.top();
					std::cout << i << ' ';
					stack.pop();
					inStack[i] = false;
				} while (n != i);
				std::cout << '\n';
			}
		}
	};
	auto svlowpath = SVLowPath(g);
	auto svCompTareConexe = SVTarjan(g, svlowpath);
	auto viz = std::vector<bool>(g->nr_noduri, false);
	g->for_nodes([&](int i) {
		if (!viz[i])
			depth_first_search(g, { svlowpath, svCompTareConexe }, viz, i);
	});
}

class FixedDisjointSet {
	std::vector<int> elems;
	int genId = 0;
	static constexpr int invalidParent = -1;

public:
	FixedDisjointSet(int size) : elems(size, invalidParent) {}

	int find(int i) const { return elems[i]; }
	bool same_set(int a, int b) const { return has(a) && has(b) && (find(a) == find(b)); }
	bool has(int i) const { return find(i) != invalidParent; }
	void make_union(int x, int y)
	{
		int s1 = find(x);
		int s2 = find(y);
		if (s1 == invalidParent && s2 == invalidParent) {
			elems[x] = elems[y] = genId;
			genId += 1;
		}
		else if (s1 != invalidParent && s2 == invalidParent) {
			elems[y] = elems[x];
		}
		else if (s1 == invalidParent && s2 != invalidParent) {
			elems[x] = elems[y];
		}
		else {
			for (auto& setID : elems)
				if (setID == s1)
					setID = s2;
		}
	}
};

auto kruskal(IGrafPonderat* g) -> std::unique_ptr<IGrafPonderat>
{
	/* vector with edges/muchii
	 * sort(edges by pondere)
	 * set = disjoint_set
	 * for edge in edges
	 *   if set.find(e.n1) != set.find(e.n2)
	 *     set.union(n1, n2)
	 *     graf.connect(n1, n2, e.p)
	*/
	struct Muchie {
		int a, b, p;
	};
	auto muchii = std::vector<Muchie>(g->nr_muchii * 2);
	int i = 0;
	g->for_nodes([&](int a) {
		g->for_neighbours(a, [&](int b, int p) {
			muchii[i++] = { a, b, p };
		});
	});

	std::sort(muchii.begin(), muchii.end(), [](const auto& a, const auto& b) { return a.p < b.p; });
	auto set = FixedDisjointSet(g->nr_noduri);
	auto apcm = makeGrafPonderat(g->actual_nr_noduri(), g->nr_noduri - 1, g->one_index, GrafType::ListVector);
	int nr = 0;
	for (const auto& muchie : muchii) {
		if (!set.same_set(muchie.a, muchie.b)) {
			set.make_union(muchie.a, muchie.b);
			apcm->connect(muchie.a, muchie.b, muchie.p);
			nr++;
		}
		if (nr == g->actual_nr_noduri() - 1)
			break;
	}
	return apcm;
	/*
	* polimophic allocator for tree
	* monothonic_alloc
	* reconstruction operation/ func for List Graph
	*/

	/*
	void Initializare(intu) { tata[u] = h[u] = 0; }
	intReprez(intu) { while (tata[u] != 0)u = tata[u]; return u; }
	void Reuneste(intu, intv) { intru, rv; ru = Reprez(u); rv = Reprez(v); if (h[ru] > h[rv])tata[rv] = ru; else { tata[ru] = rv; if (h[ru] == h[rv])h[rv] = h[rv] + 1; } }
	*/
}

auto prim(IGrafPonderat* g) -> std::unique_ptr<IGrafPonderat>
{
	auto parent = std::vector<int>(g->nr_noduri, 0);
	auto viz = std::vector<bool>(g->nr_noduri, false);
	//auto viz = std::vector<int>(g->nr_noduri, false);

	auto apcm = makeGrafPonderat(g->actual_nr_noduri(), g->nr_noduri - 1, g->one_index, GrafType::ListVector);
	auto minPond = std::vector<int>(g->nr_noduri, std::numeric_limits<int>::max());
	minPond[apcm->start_index] = 0;
	auto comp = [&](int a, int b) {
		return minPond[a] > minPond[b];
	};
	auto q = std::vector<int>{g->start_index};
	while (!q.empty()) {
		int a = q.front();
        std::pop_heap(q.begin(), q.end(), comp);
        q.pop_back();
		viz[a] = true;
		//viz[a]++;
		g->for_neighbours(a, [&](int b, int p) {
			if(!viz[b] && p < minPond[b]) {
				// if minPond[a] + p < minPond[b]
				// minPond[b] = minPond[a] + p;
				minPond[b] = p;
				parent[b] = a;
				q.push_back(b);
				std::push_heap(q.begin(), q.end(), comp);
				//std::make_heap(q.begin(), q.end(), comp);
			}
		});
	}
	g->for_nodes([&](int a) {
		if (a != apcm->start_index)
			apcm->connect(a, parent[a], g->pondere(a, parent[a]));
	});
	return apcm;
}

struct DijkstraResult {
	std::vector<int> parent;
	std::vector<int> pondere;
};

auto dijkstra(IGrafPonderat* g, int start) -> DijkstraResult
{
	auto parent = std::vector<int>(g->nr_noduri, 0);
	auto viz = std::vector<int>(g->nr_noduri, 0);

	auto minPond = std::vector<int>(g->nr_noduri, std::numeric_limits<int>::max());
	minPond[start] = 0;
	auto comp = [&](int a, int b) {
		return minPond[a] > minPond[b];
	};
	auto q = std::vector<int>{g->start_index};
	while (!q.empty()) {
		int a = q.front();
        std::pop_heap(q.begin(), q.end(), comp);
        q.pop_back();
		viz[a]++;
		if (viz[a] == 1) {
			g->for_neighbours(a, [&](int b, int p) {
				// q normal pentru bellman-ford
				//if(min[a] != max && minPond[a] + p < minPond[b])
				if (!viz[b] && minPond[a] + p < minPond[b]) {
					minPond[b] = minPond[a] + p;
					parent[b] = a;
					//if(!viz/in_q[b]) q.push viz[b] = true;
					q.push_back(b);
					std::push_heap(q.begin(), q.end(), comp);
					//apar[b]++; if(apartitii[b] > n) return b;
				}
			});
		}
	}
	return { std::move(parent), std::move(minPond) };
}

/*
determina un apcmT al grafului complet G
considera multimea {en-k+1, ... ,en-1} formata cu k-1 muchii cu cele mai mari ponderi în T
fie padurea T'=T-{en-k+1, ... ,en-1} 
defineste clasele k-clustering-ului Cca fiind multimile varfurilor celor k componente conexe ale padurii astfel obtinute
*/

void print_for_site(IGraf* g)
{
	std::cout << g->actual_nr_noduri() << '\n';
	g->for_nodes([](int i) {
		std::cout << i << '\n';
	});
	print_muchii(g);
	std::cout << "\ngata print\n";
}


int main()
{
#if 0
	auto graf = readGraf("GrafCiclic.in", GrafType::Matrix, false, true);
	//lab1_drum_min_parc(graf.get());

	auto svpath = SVPath();
	auto svparents = SVParents(graf.get());
	auto svfinal = SVFinalized(graf.get());
	auto svciclu = SVCiclu(graf.get(), svparents, svfinal);
	auto svfinorder = SVFinalizedStack();

	//dfs(graf.get(), {svpath, svparents, svfinal, svciclu, svfinorder}, 1);
	//dfs(graf.get(), {svpath, svparents, svfinal, svciclu, svfinorder}, 1);

	for (auto e : svpath.path)
		std::cout << e << ' ';
	std::cout << '\n';

	for (int i = svparents.start_index; i < svparents.parent.size(); i++)
		std::cout << svparents.parent[i] << ' ';
	std::cout << '\n';

	for (int s : svfinorder.stack)
		std::cout << s << ' ';
#endif

	GrafConfig conf;
	conf.one_index = true;
	conf.orientat = false;
	conf.type = GrafType::ListVector;
	auto graf = readGrafPonderat("GrafDijkstra.in", conf);
	print_for_site(graf.get());
	std::cout << '\n';
	auto res = dijkstra(graf.get(), 1);
	for (int i = 0; i < graf->nr_noduri; i++) {
		std::cout << i << ' ';
	}
	std::cout << '\n';
	for (auto e : res.parent)
		std::cout << e << ' ';
	std::cout << '\n';
	std::cout << res.pondere[3] << ' ' << res.pondere[5];
	//auto apcm = prim(graf.get()); // arbore partial de cost minim
	//print_for_site(apcm.get());
	std::cout << '\n';
}


