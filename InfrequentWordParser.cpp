/*
ID: jpl102
PROG: Wordle
LANG: C++11
*/
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <bitset>
#include <tuple>
#include <cmath>
#include <unordered_map>
#include <map>
#include <set>
#include <unordered_set>
#include <queue>
#include <math.h>
#include <assert.h>
#include <stack>
#include <cstdlib>
#include <time.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef vector<vector<int>> vvi;
typedef vector<vector<vector<int>>> vvvi;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef vector<ull> vull;
typedef vector<pair<int, int>> vpi;
typedef vector<vpi> vvpi;
typedef vector<vector<tuple<int, int>>> vvti;
typedef vector<tuple<int, int>> vti;
typedef vector<tuple<ll, ll>> vtll;
typedef vector<bool> vb;
typedef vector<vector<long long>> vvll;
typedef vector<string> vs;
typedef unordered_set<int> usi;
typedef vector<set<int>> vsi;
typedef unordered_map<int, int> umi;
typedef multiset<int> msi;
typedef vector<unordered_set<int>> vusi;
typedef multimap<int, int> mmi;
typedef vector<umi> vumi;
typedef vector<unordered_map<int, ll>> vumll;
typedef unordered_map<int, ll> umll;
typedef vector<double> vd;
typedef vector<vector<string>> vvs;
typedef unordered_map<string, int> umsi;
typedef vector<pair<int, int>> vpii;
typedef vector<tuple<string, int>> vtsi;
typedef vector<tuple<string, double>> vtsd;
typedef vector<unordered_map<string, int>> vumsi;
typedef vector<vector<vector<string>>> vvvs;

int main() {
	ifstream ain("words_answer.in");
	ifstream allin("words_possible");
	ofstream fout("words_rare.in");
	unordered_set<string> answers;
	unordered_set<string> words;
	vector<string> rareWords;
	while (!ain.eof()) {
		string a;
		ain >> a;
		answers.insert(a);
	}
	while (!allin.eof()) {
		string a;
		allin >> a;
		words.insert(a);
	}
	for (auto cur : words) {
		if (answers.find(cur) == answers.end())
			rareWords.push_back(cur);
	}
	for (auto c : rareWords)
		fout << c << " ";
	return 0;
}