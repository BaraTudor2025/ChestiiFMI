#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <map>
#include <unordered_map>
#include <set>
#include <queue>
using namespace std;

class AutomatFD {

public:
	AutomatFD(int nrs) : states(nrs) { }

	bool isValid(std::string_view s) const {
		auto state = getState(s);
		return state ? state->isFinale : false;
	}

	static AutomatFD make(std::string s)
	{
		std::stringstream ss(s);
		return AutomatFD::make(ss);
	}

	static AutomatFD make(std::istream& in)
	{
		int nrStates, nrAlfabet;
		in >> nrStates;
		AutomatFD automat(nrStates);
		for(State& state : automat.states){
			in >> state.name >> state.isFinale >> nrAlfabet;
			char letter;
			int nextState;
			for (int i = 0; i < nrAlfabet; i++) {
				in >> letter >> nextState;
				state.transitions[letter] = nextState;
			}
		}
		return automat;
	}

	void run(std::string_view str) const
	{
		const char* prefix = isValid(str) ? "  " : "in";
		std::printf("%svalid: %s\n", prefix, str.data());
	}

private:

	struct State {
		char name{};
		bool isFinale{};
		std::map<char, int> transitions;
	};

	std::vector<State> states;

	const State* getState(std::string_view str) const
	{
		const State* state = &states[0];
		for (char inputChar : str) {
			auto it = state->transitions.find(inputChar);
			if (it == state->transitions.end())
				return nullptr;
			int nextState = it->second;
			state = &states[nextState];
		}
		return state;
	}
};


class AutomatFN {

public:

	AutomatFN(int statesNum) : states(statesNum) { }

	static AutomatFN make(std::string s)
	{
		std::stringstream ss(s);
		return AutomatFN::make(ss);
	}

	static AutomatFN make(std::istream& in)
	{
		int statesNum, transNum;
		in >> statesNum;
		AutomatFN automat(statesNum);
		for (State& state : automat.states) {
			in >> state.name >> state.isFinale >> transNum;
			char transName;
			int nextState;
			for (int i = 0; i < transNum; i++) {
				in >> transName >> nextState;
				state.transitions[transName].push_back(nextState);
			}
		}
		return automat;
	}

	AutomatFD makeDeterminist()
	{
	}

	auto isValid(std::string_view s) const
	{
		auto statesQ = getStates(states[0], s);
		for (auto state : statesQ)
			if (state->isFinale)
				return std::pair{true, statesQ};
		return std::pair{false, statesQ};
	}

	void run(std::string_view str) const
	{
		auto [valid, statesQ] = isValid(str);
		std::cout << str << ':' << valid;
		std::cout << '\n';

		//const char* prefix = valid ? "  " : "in";
		//std::printf("%svalid: %s\n", prefix, str.data());

		//std::cout << "states:";
		//for (auto state : statesQ) {
		//	std::cout << state->name << " ";
		//}
		//std::cout << "\n\n";
	}

	friend AutomatFN makeAFNfromGrammar(std::istream&);

private:

	struct State {
		int name{};
		bool isFinale{};
		std::map<char, std::vector<int>> transitions;
	};

	std::vector<AutomatFN::State> states;

	std::set<const State*> getStates(const State& firstState, std::string_view str) const
	{
		std::set<const State*> statesQ = {&firstState};
		std::set<const State*> nextStatesQ;
		for(char inputChar : str) {
			for (const State* state : statesQ) {
				auto it = state->transitions.find(inputChar);
				if (it != state->transitions.end()) {
					for (int nextState : it->second) {
						nextStatesQ.insert(&states[nextState]);
					}
				}
			}
			statesQ = std::move(nextStatesQ);
		}
		return statesQ;
	}
};

template <typename Auto>
void runAutomata(const Auto& automata, const std::vector<std::string>& strings)
{
	for (const auto& str : strings) {
		automata.run(str);
	}
}

void runAutomatAndExamples(std::istream& in)
{
	auto automata = AutomatFD::make(in);
	int exampleNum;
	in >> exampleNum;
	std::string data;
	for (int i = 0; i < exampleNum; i++) {
		in >> data;
		automata.run(data);
	}
}

void runAutomatAndExamples(std::string s)
{
	std::stringstream ss(s);
	runAutomatAndExamples(ss);
}

AutomatFN makeAFNfromGrammar(std::istream& in)
{
	int statesNum;
	in >> statesNum;
	AutomatFN automata(statesNum);

	int UniqueIndex = 0;
	std::map<char, int> grammarState2Index; // index of automata state

	auto getIndex = [&](char c) {
		if (grammarState2Index.count(c) == 0)
			grammarState2Index[c] = UniqueIndex++;
		return grammarState2Index[c];
	};
	
	while (in) {
		std::string str;
		getline(in, str);
		char state, inputElem, nextState;
		if (str.size() > 1) {
			int charCitite = sscanf(str.c_str(), "%c %c%c", &state, &inputElem, &nextState);
			//printf("1:%c 2:%c 3:%c\n", state, elem, nextState);
			auto& s = automata.states[getIndex(state)];

			if (charCitite == 2) {
				if (inputElem == '0') {
					s.isFinale = true;
					s.name = getIndex(state);
				}
				else {
					auto& fs = automata.states[getIndex('F')];
					fs.name = getIndex('F');
					fs.isFinale = true;
					s.transitions[inputElem].push_back(getIndex('F'));
					s.name = getIndex(state);
				}
				continue;
			}
			s.name = getIndex(state);
			s.transitions[inputElem].push_back(getIndex(nextState));
		}
	}
	return automata;
}

int main()
{
	// 'f' inseamana ca e final
	stringstream ss(R"(
3
S aS
S bA
A 0
A cA
)");

	/*
A 0
A cA

S aS | bA
A cA
A 0
	*/
	
	stringstream ssPrez(R"(
6
S aA
S bB
S 0
A aS
B bC
C bD
C b
D bB
)");
	AutomatFN automat = makeAFNfromGrammar(ssPrez);
	runAutomata(automat, { "aaaabbbbbb", "aaabb", "bbbaa"});

	cout << "\n\n automat nedet:\n";
	AutomatFN afn = AutomatFN::make(R"(
7
0 0 2 a 1 a 2
1 0 1 c 1
2 0 3 c 3 c 4 d 4
3 1 2 a 3 d 2
4 0 1 b 5
5 0 2 b 4 a 6
6 1 b 4
)");
	runAutomata(afn, { "acdcbbba", "ac", "abaa" });
	return 0;
}

int main2()
{
	runAutomatAndExamples(R"(
	4
	0 0 2 b 0 a 1
	1 0 2 a 1 b 2
	2 0 2 a 3 b 0
	3 1 2 a 3 b 3
	4
	bbabaaa
	baabbbaaababbababb
	bbbabb
	bbbbbbbbbbbbaba
	)");


	cout << "\n\n automat 1:\n";
	auto automat1 = AutomatFD::make(R"(
	4
	0 0 2 b 0 a 1
	1 0 2 a 1 b 2
	2 0 2 a 3 b 0
	3 1 2 a 3 b 3
	)");
	runAutomata(automat1, {"bbabaaa", "baabbbaaababbababb", "bbbabb", "bbbbbbbbbbbbaba"});


	cout << "\n\n automat 2:\n";
	auto automat2 = AutomatFD::make(R"(
	3
	0 1 2 a 1 b 0
	1 1 2 a 2 b 0
	2 0 2 a 2 b 2
	)");
	runAutomata(automat2, {"b", "bbababbba", "", "aa"});


	cout << "\n\n automat nedet:\n\n";
	AutomatFN afn = AutomatFN::make(R"(
	8
	0 0 2 a 1 a 2
	1 0 1 b 1
	2 0 4 d 0 b 3 c 3 c 5
	3 0 2 a 2 d 4
	4 1 0
	5 0 3 a 6 a 7 c 6
	6 0 2 c 5 d 7
	7 1 1 d 7
	)");
	runAutomata(afn, {"acacd", "acacaddd", "acad", "ab", "acd", "acdd"});


	return 0;
}
