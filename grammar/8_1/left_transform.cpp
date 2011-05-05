#include <iostream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include <iterator>
#include <cctype>

using namespace std;

class Automaton;

class Grammar
{
    string start_s;
    unsigned int non_term_cnt;

    set<char> terminals;
    map<char, unsigned int> non_terminals;
    vector<pair<string, string> > rules;

    void add_fill_set(const pair<string, string>& p);
public:
    Grammar() : start_s("S"), non_term_cnt(1) {
    }

    friend istream& operator>>(istream& is, Grammar& g);
    friend class Automaton;
};

inline void Grammar::add_fill_set(const pair<string, string>& p) {
    if (isupper(p.first[0])) {
        if (non_terminals.find(p.first[0]) == non_terminals.end()) {
            non_terminals.insert(make_pair(p.first[0], non_term_cnt++));
            cout << "d: " << p.first[0] << ' ' << non_term_cnt << '\n';
        }
    }
    for (unsigned int j = 0; j < p.second.length(); j++) {
        if (isupper(p.second[j])) {
            if (non_terminals.find(p.second[j]) == non_terminals.end()) {
                non_terminals.insert(make_pair(p.second[j], non_term_cnt++));
                cout << "d: " << p.second[j] << ' ' << non_term_cnt << '\n';
            }
        } else {
            terminals.insert(p.second[j]);
        }
    }
}

istream& operator>>(istream& is, Grammar& g) {
    istream_iterator<string> in(is);
    istream_iterator<string> all;
    pair<string, string> p;

    while (in != all) {
        p.first = *(in++);
        p.second = *(in++);
        g.rules.push_back(p);
        g.add_fill_set(p);
    }

    return is;
}


class Automaton
{
    typedef map<char, set<unsigned int> > NodeInfo;
    typedef vector<NodeInfo> Transitions;

    string alphabet;
    unsigned int state_num;
    unsigned int final_state_num;
    unsigned int start_state;

    set<unsigned int> final_states;
    Transitions transitions;
public:
    Automaton() : state_num(0), final_state_num(0), start_state(0) {
    }
    Automaton(Grammar& g);

    void convert();

    friend ostream& operator<<(ostream& os, Automaton& g);
};

ostream& operator<<(ostream& os, Automaton& a) {
    set<unsigned int>::const_iterator it;
    Automaton::NodeInfo::iterator n_it;

    unsigned int i, j;

    os << a.alphabet << '\n';
    os << a.state_num << '\n' << a.start_state + 1 << ' ' << a.final_state_num << ' ';
    for (it = a.final_states.begin(); it != a.final_states.end(); ++it)
        os << *it + 1 << ' ';
    os << '\n';
    for (i = 0; i < a.transitions.size(); i++) {
        for (j = 0; j < a.alphabet.size(); j++) {
            if ((n_it = a.transitions[i].find(a.alphabet[j])) != a.transitions[i].end()) {
                if (n_it->second.size() != 1) {
                    os << "FFFUUUU: node " << i << " symbol " << a.alphabet[j] << endl;;
                }
                os << *(n_it->second.begin()) + 1 << ' ';
            } else {
                os << a.state_num << ' ';
            }
        }
        os << '\n';
    }
    return os;
}

void Automaton::convert() {
    Transitions dfa_transitions;
    NodeInfo::iterator it;
    queue<unsigned int> new_states;
    set<unsigned int> old_states;

    unsigned int i;
    unsigned int cur_state, state_cnt;

    new_states.push(start_state);

    state_cnt = 1;
    while (!new_states.empty()) {
        cur_state = new_states.pop();
        if (old_states.find(cut_state) != old_states.end())
            continue;

        for (i = 0; i < alphabet.size(); i++) {
            if ((it = transitions[cur_state].find(alphabet[i])) != transitions[cur_state].end()) {
                dfa_transitions.push_back(
            }
        }

        old_states.insert(cur_state);
    }
}

Automaton::Automaton(Grammar& g) {
    state_num = g.non_term_cnt + 1;
    start_state = 0;
    final_state_num = 1;
    final_states.insert(g.non_terminals['S']);
    transitions.resize(state_num);

    for (set<char>::const_iterator it = g.terminals.begin(); it != g.terminals.end(); it++)
        alphabet.push_back(*it);

    for (vector<pair<string, string> >::const_iterator it = g.rules.begin(); it != g.rules.end(); it++) {
        if (it->second.length() == 1) {
            transitions[start_state][it->second[0]].insert(g.non_terminals[it->first[0]]);
        } else {
            transitions[g.non_terminals[it->second[0]]][it->second[1]].insert(g.non_terminals[it->first[0]]);
        }
    }
}

int main()
{
    Grammar g;

    cin >> g;

    Automaton a(g);

    cout << a;

    return 0;
}