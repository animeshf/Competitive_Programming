#include "bits/stdc++.h"
using namespace std;

const int N = 205;

// Dinic's Algorithm to compute Maximum Flow
const int INF = 1000000000;

struct Edge {
    int from, to, cap, flow, index;
    Edge(int from, int to, int cap, int flow, int index) :
    from(from), to(to), cap(cap), flow(flow), index(index) {}
};

struct Dinic {
    int N;
    vector < vector < Edge > > G;
    vector < Edge * > dad;
    vector < int > Q;

    Dinic(int N) : N(N), G(N), dad(N), Q(N) {}

    void AddEdge(int from, int to, int cap) {
        G[from].push_back(Edge(from, to, cap, 0, G[to].size()));
        if (from == to) G[from].back().index++;
        G[to].push_back(Edge(to, from, 0, 0, G[from].size() - 1));
    }

    long long BlockingFlow(int s, int t) {
        fill(dad.begin(), dad.end(), (Edge *) NULL);
        dad[s] = &G[0][0] - 1;

        int head = 0, tail = 0;
        Q[tail++] = s;

        while(head < tail){
            int x = Q[head++];
            for (int i = 0; i < G[x].size(); i++){
                Edge &e = G[x][i];
                if(!dad[e.to] && e.cap - e.flow > 0){
                    dad[e.to] = &G[x][i];
                    Q[tail++] = e.to;
                }
            }
        }
        if (!dad[t]) return 0;

        long long totflow = 0;
        for (int i = 0; i < G[t].size(); i++){
            Edge *start = &G[G[t][i].to][G[t][i].index];
            int amt = INF;
            for (Edge *e = start; amt && e != dad[s]; e = dad[e->from]){
                if (!e) { amt = 0; break; }
                amt = min(amt, e->cap - e->flow);
            }
            if (amt == 0) continue;
            for (Edge *e = start; amt && e != dad[s]; e = dad[e->from]) {
                e->flow += amt;
                G[e->to][e->index].flow -= amt;
            }
            totflow += amt;
        }
        return totflow;
    }

    long long GetMaxFlow(int s, int t) {
        long long totflow = 0;
        while (long long flow = BlockingFlow(s, t)) totflow += flow;
        return totflow;
    }
};
// End of Maximum Flow Template

int n;
int sentence[N];
string str[N];
set < string > words;
map < string, set < int > > in_sentence;
map < string, int > in;
map < string, int > out;

vector < string > split(string work, char delim) {
    vector < string > flds;
    if (!flds.empty()) flds.clear();
    string buf = "";
    int i = 0;
    while (i < (int) work.size()) {
        if (work[i] != delim) {
            buf += work[i];
        } else if (((int) buf.size()) > 0) {
            flds.push_back(buf);
            buf = "";
        }
        i++;
    }
    if ((int) buf.size()) {
        flds.push_back(buf);
    }
    return flds;
}

int main() {
    freopen ("inp.in", "r", stdin);
    freopen ("C.out", "w", stdout);
    int tc; cin >> tc;
    for (int qq = 1; qq <= tc; qq++) {
        cout << "Case #" << qq << ": ";
        cin >> n;
        cin.ignore();
        words.clear();
        in_sentence.clear();
        in.clear();
        out.clear();
        for (int i = 1; i <= n; i++) {
            getline(cin, str[i]);
            vector < string > wordlist = split(str[i], ' ');
            for (string word : wordlist) {
                words.insert(word);
                in_sentence[word].insert(i);
            }
        }
        int ticks = 0;
        for (string word : words) {
            in[word] = ++ticks;
            out[word] = ++ticks;
        }
        for (int i = 1; i <= n; i++) {
            sentence[i] = ++ticks;
        }
        Dinic graph(ticks + 10);
        for (string word : words) {
            graph.AddEdge(in[word], out[word], 1);
            for (int sentence_id : in_sentence[word]) {
                graph.AddEdge(sentence[sentence_id], in[word], INF);
                graph.AddEdge(out[word], sentence[sentence_id], INF);
            }
        }
        cout << graph.GetMaxFlow(sentence[1], sentence[2]) << "\n";
    }
}