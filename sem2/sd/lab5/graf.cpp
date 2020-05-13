#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>

class Graf {
	using GrafType = std::unordered_map<int, std::unordered_set<int>>;
	GrafType data;

	void DFS_impl(int node, std::unordered_set<int>& visited) const
	{
		std::cout << node << " ";
		visited.insert(node);
		const auto& neighbours = data.at(node);
		for (int neighbour : neighbours) {
			if(!visited.count(neighbour))
				DFS_impl(neighbour, visited);
		}
	}

public:
	Graf() = default;

	void addNode(int node)
	{
		// operatorul [] construieste vectorul daca nu exista
		volatile auto& _ = data[node];
	}

	void addEdge(int source, int target)
	{
		if (data.count(source) && data.count(target))
			data[source].insert(target);
		else // nush
			throw std::runtime_error("nu-s nodurile");
	}

	//lookup-ul este constant, O(1), din moment ce graful este implementat ca un hash-table-uri
	bool hasEdge(int source, int target) const
	{
		return data.at(source).count(target);
	}

	void BFS(int startNode) const
	{
		std::queue<int> q;
		std::unordered_set<int> visited;
		q.push(startNode);
		visited.insert(startNode);
		int node = startNode;
		do {
			const auto& set = data.at(node);
			for (int neightbour : set) {
				if (!visited.count(neightbour)) {
					q.push(neightbour);
					visited.insert(neightbour);
				}
			}
			node = q.front();
			std::cout << node << " ";
			q.pop();
		} while (!q.empty());
	}

	void DFS(int startNode) const
	{
		std::unordered_set<int> visited;
		DFS_impl(startNode, visited);
	}

	int twoNodes() const
	{
		int cycles = 0;
		auto nodes = this->data;
		for (auto& [node, neighbours] : nodes) {
			while (!neighbours.empty()) {
				// extract e ca un pop
				auto hNode = neighbours.extract(neighbours.begin());
				int neighbour = hNode.value();
				auto& neighbourSet = nodes.at(neighbour);
				if (neighbourSet.count(node)) {
					cycles += 1;
					neighbourSet.erase(node);
				}
			}
		}
		return cycles;
	}

	friend std::istream& operator>>(std::istream& in, Graf& graf)
	{
		int nodeNum;
		in >> nodeNum;
		int node;
		for (int i = 0; i < nodeNum; i++) {
			in >> node;
			graf.addNode(node);
		}
		std::string line;
		std::getline(in, line); 
		for (int i = 0; i < nodeNum; i++) {
			std::getline(in, line);
			std::stringstream ssline(line);
			int node, neighbour;
			ssline >> node;
			while (ssline >> neighbour)
				graf.addEdge(node, neighbour);
		}
		return in;
	}

	friend std::ostream& operator<<(std::ostream& out, const Graf& graf)
	{
		for (const auto& [node, neighbours] : graf.data) {
			out << node << " - ";
			for (int n : neighbours)
				out << n << ' ';
			out << '\n';
		}
		return out;
	}

};

int main()
{
	std::stringstream ss(R"(
	7
	1 2 3 4 5 6 7
	1 2 3 4
	2 1 4 7 
	3 1 4
	4 1 2 3 7
	5 6
	6 5
	7 2 4
	)");
	Graf graf;
	ss >> graf;
	std::cout << graf;
	std::cout << "DFS: "; graf.DFS(1); std::cout << '\n';
	std::cout << "BFS: "; graf.BFS(1);  std::cout << '\n';
	std::cout << "2-cicluri: " << graf.twoNodes();
	/*
	output:
	1 - 2 3 4
	2 - 1 4 7
	3 - 1 4
	4 - 1 2 3 7
	5 - 6
	6 - 5
	7 - 2 4
	DFS: 1 2 4 3 7
	BFS: 1 2 3 4 7
	2-cicluri: 8
	*/
}

	//std::string line;
	//while (std::getline(ss, line)) {
	//	std::stringstream ssline(line);
	//	int node, neighbour;
	//	ssline >> node;
	//	while (ssline >> neighbour)
	//		graf.addEdge(node, neighbour);
	//}