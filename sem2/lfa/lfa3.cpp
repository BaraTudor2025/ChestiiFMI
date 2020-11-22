#include <stack>
#include <unordered_map>
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <sstream>
#include <deque>
#include <fstream>

class APD {
public:

	using StateNameType = char;
	using StackType = char;

	struct State {
		struct Input {
			char inputChar{};
			StackType topOfStack{};

			friend bool operator<(Input k1, Input k2) {
				if (k1.inputChar < k2.inputChar)
					return true;
				else if (k1.inputChar > k2.inputChar)
					return false;
				else
					return k1.topOfStack < k2.topOfStack;
			}
		};

		struct Transition {
			StateNameType nextState{};
			std::deque<StackType> toPush;
		};

		bool isFinale{};
		std::map<Input, Transition> transitions;
		std::map<Input, Transition> lambdaTransitions;
	};

	std::map<StateNameType, State> states;
	StateNameType initState = 'a';
	static inline StateNameType lambdaInput = 'L';

	mutable std::stack<StackType> stack;
	StackType initStackValue = -1;

	const State* getState(std::string_view input) const {

		while (!stack.empty())
			stack.pop();
		stack.push(initStackValue);

		auto gotoNextState = [this](auto& currState, const auto& trans) {
			this->stack.pop();
			for (const auto& v : trans.toPush)
				this->stack.push(v);
			currState = &this->states.at(trans.nextState);
		};

		const State* currentState = &states.at(initState);
		for (char inputChar : input) {
			auto it = currentState->transitions.find({ inputChar, stack.top() });
			if (it == currentState->transitions.end())
				return nullptr;

			auto& trans = it->second;
			gotoNextState(currentState, trans);
		}

		while (!currentState->lambdaTransitions.empty()) {
			auto it = currentState->lambdaTransitions.find({ lambdaInput, stack.top() });
			if (it == currentState->lambdaTransitions.end())
				break;
			auto& trans = it->second;
			gotoNextState(currentState, trans);
		}
		return currentState;
	}

	bool isValid(std::string_view str) {
		auto state = getState(str);
		return state && state->isFinale;
	}

public:
	APD() = default;

	static void readSimplu(std::stringstream& in, APD& automat) 
	{
		/* specificatii
		0 - initState
		1 - numFinalStates
		5 - listFinalStates
		11 - numTrans
		<from> <to> <input> <top-of-stack> <list-to-push>
		0 1 a A AA
		*/
		in >> automat.initState;

		int numFinalStates;
		in >> numFinalStates;
		for (int i = 0; i < numFinalStates; i++) {
			StateNameType c;
			in >> c;
			automat.states[c].isFinale = true;
		}
		automat.initStackValue = 'Z';

		int numTrans;
		in >> numTrans;
		for (int i = 0; i < numTrans; i++) {
			StateNameType fromState, toState;
			char inputChar;
			StackType stackTop;
			std::string stackList;
			in >> fromState >> toState >> inputChar >> stackTop >> stackList;

			APD::State::Transition* pTrans;
			if (inputChar != '$') {
				pTrans = &automat.states[fromState].transitions[{inputChar, stackTop}];
				pTrans->nextState = toState;
			} else {
				pTrans = &automat.states[fromState].lambdaTransitions[{lambdaInput, stackTop}];
				pTrans->nextState = toState;
			}

			if(stackList != "$")
				for (char c : stackList)
					pTrans->toPush.push_front(c);
		}
	}
};


class APDnedet {
public:

	using StateNameType = char;
	using StackType = char;


	struct State {
		struct Input {
			StateNameType inputChar{};
			StackType topOfStack{};

			friend bool operator<(Input k1, Input k2) {
				if (k1.inputChar < k2.inputChar)
					return true;
				else if (k1.inputChar > k2.inputChar)
					return false;
				else
					return k1.topOfStack < k2.topOfStack;
			}
		};

		struct Transition {
			StateNameType nextState{};
			std::deque<StackType> toPush;
		};

		bool isFinale{};
		StateNameType name;
		//mutable std::stack<StackType> stack;
		std::map<Input, std::vector<Transition>> transitions;
		std::map<Input, std::vector<Transition>> lambdaTransitions;
	};

	std::map<StateNameType, State> states;
	StateNameType initState = 'a';
	static inline StateNameType lambdaInput = 'L';
	StackType initStackValue = -1;

	mutable std::stack<StackType> stack;

	auto getState(std::string input) const {

		//for (auto& [key, state] : states) {
		//	while(!state.stack.empty())
		//}
		//while (!stack.empty())
		//	stack.pop();
		//stack.push(initStackValue);

		//std::set<const State*> stateSet = { &states.at(initState) };
		//std::set<const State*> nextStateSet;
		//std::set<std::pair<const State*, std::stack<StackType>>> stateSet;
		//stateSet.emplace( &states.at(initState), std::vector<StackType>{initStackValue} );

#if 0
		auto addNextState = [this, &nextStateSet](const auto& state, const auto& trans) {
			nextStateSet.insert(&this->states.at(trans.nextState));
			//state->stack.pop();
			//for (const auto& v : trans.toPush)
			//	state->stack.push(v);
			this->stack.pop();
			for (const auto& v : trans.toPush)
				this->stack.push(v);
		};

		for (char inputChar : input) {
			for (const auto& state : stateSet) {

				//auto it = state->transitions.find({ inputChar, stack.top() });
				auto it = state->transitions.find({ inputChar, state->stack.top() });
				if (it != state->transitions.end())
					for (const auto& trans : it->second)
						addNextState(state, trans);

				auto itL = state->lambdaTransitions.find({lambdaInput, stack.top() });
				if (itL != state->lambdaTransitions.end())
					for (const auto& trans : itL->second)
						addNextState(state, trans);
			}
			stateSet = std::move(nextStateSet);
		}

		decltype(stateSet) nonLambdaSet;

		bool exaustLambdaTrans = false;
		while (!exaustLambdaTrans) {
			exaustLambdaTrans = true;
			for (const auto& state : stateSet) {
				if (!state->lambdaTransitions.empty()) {
					exaustLambdaTrans = false;
					auto it = state->lambdaTransitions.find({lambdaInput, stack.top() });
					if (it != state->lambdaTransitions.end()) {
						for (const auto& trans : it->second)
							addNextState(state, trans);
						for (auto& s : nextStateSet)
							if (s->isFinale)
								return {s};
					}
				}
				else
					nonLambdaSet.insert(state);
			}
			if(!exaustLambdaTrans)
				stateSet = std::move(nextStateSet);
		}
		stateSet.merge(nonLambdaSet);
#endif
		int stateSet=0;
		return stateSet;
	}

	bool isValid(std::string str) {
		auto states = getState(str);
		//for (auto& state : states)
		//	if (state->isFinale)
		//		return true;
		return false;
	}

public:
	APDnedet() = default;

	/*
	static void read(std::istream& in, APDnedet& automat)
	{
		int numStates; // sau numStari
		in >> numStates;
		for (int i = 0; i < numStates; i++) {
			char c;
			in >> c;
			volatile auto& _ = automat.states[c];
			if (i == 0)
				automat.initState = c;
		}

		int numFinalStates;
		in >> numFinalStates;
		for (int i = 0; i < numFinalStates; i++) {
			char c;
			in >> c;
			automat.states[c].isFinale = true;
		}
		in >> automat.initStackValue;

		int transNum;
		in >> transNum;
		for (int i = 0; i < transNum; i++) {
			StateNameType currState, nextState;
			int transRuleNum;
			in >> currState >> nextState >> transRuleNum;

			for (int j = 0; j < transRuleNum; j++) {
				StateNameType input;
				StackType topOfStack;
				int toPushNum;
				in >> input >> topOfStack >> toPushNum;

				if (input == lambdaInput) {
					auto& trans = automat.states.at(currState).lambdaTransitions[{input, topOfStack}];
					trans.nextState = nextState;
					for (int k = 0; k < toPushNum; k++) {
						StackType nush;
						in >> nush;
						trans.toPush.push_front(nush);
					}
				}
				else {
					auto& trans = automat.states.at(currState).transitions[{input, topOfStack}];
					trans.nextState = nextState;
					for (int k = 0; k < toPushNum; k++) {
						StackType nush;
						in >> nush;
						trans.toPush.push_front(nush);
					}
				}
			}
		}
	}
	*/


	static void readSimplu(std::stringstream& in, APDnedet& automat) 
	{
		/*
		5 - numStates
		0 - initState
		1 5 - numFinalStates, listFinalStates
		11 - numTrans

		0 - initState
		1 - numFinalStates
		5 - listFinalStates
		11 - numTrans
		0 1 a A AA
		0 1 a Z AZ
		1 0 a A AAA
		0 2 b A $
		2 2 b A $
		2 3 c D DD
		2 3 c Z DZ
		3 2 c D D
		2 4 d D $
		4 4 d D $
		4 5 $ Z $
		*/
		in >> automat.initState;

		int numFinalStates;
		in >> numFinalStates;
		for (int i = 0; i < numFinalStates; i++) {
			StateNameType c;
			in >> c;
			automat.states[c].isFinale = true;
		}
		automat.initStackValue = 'Z';

		int numTrans;
		in >> numTrans;
		for (int i = 0; i < numTrans; i++) {
			StateNameType fromState, toState;
			char inputChar;
			StackType stackTop;
			std::string stackList;
			in >> fromState >> toState >> inputChar >> stackTop >> stackList;

			std::vector<APDnedet::State::Transition>* pTrans;
			automat.states[fromState].name = fromState;
			if (inputChar != '$') {
				pTrans = &automat.states[fromState].transitions[{inputChar, stackTop}];
				pTrans->push_back({ toState, {} });
				//pTrans->nextState = toState;
			} else {
				pTrans = &automat.states[fromState].lambdaTransitions[{lambdaInput, stackTop}];
				pTrans->push_back({ toState, {} });
				//pTrans->nextState = toState;
			}

			if (stackList != "$")
				for (char c : stackList)
					pTrans->back().toPush.push_front(c);
		}
	}
};

int main()
{
	/*
	3 --<num-states/stari>
	a b c
	1 --<num-final-states>
	c
	z -- <init-stack-value>
	4 -- <num-of-trans>

	-- trans din 'a' in 'a', 6 reguli de tranzitie
	a a 6     --<from-state> <to-state> <num-of-rules>
	0 0 2 0 z --<input> <top-of-stack> <length> <list-to-push-on-stack>
	1 z 2 1 z
	0 0 2 0 0
	0 1 2 0 1
	1 0 2 1 0
	1 1 2 1 1

	a b 3
	c z 1 z
	c 0 1 0
	c 1 1 1

	b b 2
	0 0 0 -- <length>==0 adica dam pop fara sa pusham nimic
	1 1 0

	b c 1
	L z 1 z

	w = 01c10 incl L(A)
	w = 023 not-incl L(A)
	w = 0010101c1010100 incl L(A)
	w = 011c110 not-incl L(A)
	*/

/*
aabbbccd-da
aaaabbbbbbccccdd-da
ccdddd-nu
abbb-nu

bccc - nu
aaaabcccd - nu
aaaabcccdd - da
*/


	std::stringstream ssPalim(R"(
0 1 2
12
0 0 0 Z 0Z
0 0 1 Z 1Z
0 0 0 0 00 
0 0 0 1 01
0 0 1 0 10
0 0 1 1 11
0 1 c Z Z
0 1 c 0 0
0 1 c 1 1
1 1 0 0 $
1 1 1 1 $
1 2 $ Z Z
)");

	std::stringstream ss2(R"(
0 
1 4 
9
0 1 a A AA
0 1 a Z AZ
0 2 b A BBBA
1 0 a A A
2 2 b B BBBB
2 3 c B $
3 3 d A $
3 3 c B $
3 4 $ Z $
)");

	std::stringstream ssSimplu(R"(
0
1
5
11
0 1 a A AA
0 1 a Z AZ
1 0 a A AAA
0 2 b A $
2 2 b A $
2 3 c D DD
2 3 c Z DZ
3 2 c D D
2 4 d D $
4 4 d D $
4 5 $ Z $
)");

	auto listTest = {"aabbbccd", "aaaabbbbbbccccdd", "ccdddd", "abbb"};
	auto listPalim = { "001c100", "032", "10101c10101", "1c1", "100c000", "100c100" };
	auto listTest2 = { "bccc", "aaaabcccd", "aaaabcccdd" };
	auto prezentare = {"aaaabbbbbb", "aabbbccd", "aaaabbbbbbccccccddd"};
	using std::cout;

	APD automat;
	APD::readSimplu(ssSimplu, automat);
	for (const char* str : prezentare)
		cout << str << ':' << automat.isValid(str) << '\n';

	// listTest
	//-da
	//-da
	//-nu
	//-nu

	// listTest2
	//bccc - nu
	//aaaabcccd - nu
	//aaaabcccdd - da


//	std::stringstream ssNedet(R"(
//0
//1 2
//12
//0 0 0 Z 0Z
//0 0 1 Z 1Z
//0 0 0 0 00 
//0 0 0 1 01
//0 0 1 0 10
//0 0 1 1 11
//0 1 $ 0 0
//0 1 $ 1 1
//0 1 $ Z Z
//1 1 0 0 $
//1 1 1 1 $
//1 2 $ Z Z
//)");

	//auto listPalimNedet = { "0110", "00", "11", "10100101", "1011" };
	//APDnedet automat;
	//APDnedet::readSimplu(ssNedet, automat);
	//for (const char* str : listPalimNedet)
	//	cout << str << ':' << automat.isValid(str) << '\n';

	//for (const auto& [stateName, state] : automat.states) {
	//	std::printf("state: %c \n", stateName);
	//	for (const auto& [input, trans] : state.transitions) {
	//		std::printf("inputChar(%c), topOfStack(%c) ", input.inputChar, input.topOfStack);
	//	}
	//}

	//automat.initStackValue = 'z';
	//automat.initState = 'a';
	//automat.states['a'].transitions[{'0', 'z'}] = { 'a', {'0', 'z'} };
}