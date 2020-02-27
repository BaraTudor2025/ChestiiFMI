#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std;
/*

4 2
0 0  1 b 0  1 a 1
1 0  1 a 1  1 b 2
2 0  1 a 3  1 b 0
3 1  1 a 3  1 b 3

4 2
s 0 b 0 a 1
p 0 a 1 b 2
q 0 a 3 b 0
r 1 a 3 b 3

0
1
2
3
4
5
6
7

*/
struct Transition {
    std::vector<char> letters;
    int state_index;
};

struct State {
    State(int nr) : trans(nr) { }

    char name;
    bool isFinale;
    std::vector<Transition> trans;
};

struct AutomatDet {
    AutomatDet(int nrs, int nra) : states(nrs, nra) { }

    std::vector<State> states;

    bool isValid(std::string s){
        return getState(states[0], s).isFinale;
    }
private:
    const State& getState(State& state, std::string& str)
    {
        if(str.size() == 0)
            return state;

        for(const auto& trans : state.trans) {
            for(const auto& letter : trans.letters)
            if(trans.letter == str.back()) {
                str.pop_back();
                return getState(this->states[trans.state_index], str);
            }
        }
    }
};

int main()
{
    std::ifstream fin("date.txt");
    int nrStates, nrAlfabet;
    fin >> nrStates >> nrAlfabet;
    AutomatDet automat(nrStates, nrAlfabet);
    for(State& state : automat.states){
        fin >> state.name >> state.isFinale;
        for(Transition& tr : state.trans){
            fin >> tr.letter >> tr.state_index;
        }
    }

    std::string s = "bbabaaa";
    std::reverse(s.begin(), s.end());
    //auto st = automat.getState(automat.states[0], s);
    //cout << st.name;
    cout << automat.isValid(s);
    return 0;
}
