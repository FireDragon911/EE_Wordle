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
template<int M, template<typename> class F = std::less>
struct TupleCompare
{
	template<typename T>
	bool operator()(T const &t1, T const &t2)
	{
		return F<typename tuple_element<M, T>::type>()(std::get<M>(t1), std::get<M>(t2));
	}
};



const int max_int = numeric_limits<int>::max() / 10;


string random_guess(unsigned int seed, vs &words)
{
	if (seed >= 0)
		srand(seed);

	string curguess = words[rand() % words.size()];

	return curguess;
}

//match word with the green-yellow-gray coloring
string evaluate(string actual_word, string curguess)
{
	//auto set to grey since if it is neither green nor yellow it must be grey
	string mask = "00000";

	vi count(26, 0);
	for (int i = 0; i < actual_word.size(); i++)
	{
		count[actual_word[i] - 'a']++;
	}
	// Found green first
	for (int i = 0; i < actual_word.size(); i++)
	{
		if (curguess[i] == actual_word[i])
		{
			mask[i] = '2';
			count[curguess[i] - 'a']--;
		}
	}
	// Deal with yellows 
	for (int i = 0; i < curguess.size(); i++)
	{
		if (mask[i] == '2')
			//no need to recheck when it is already green
			continue;


		if (count[curguess[i]-'a'] > 0)
		{
			//this letter both shows up in the real word and in this word
			mask[i] = '1';
			count[curguess[i] - 'a']--;
		}
	}
	return mask;
}
void get_freq(vs& words, vi& freq)
{
	for (auto oneword : words)
	{
		for (auto c : oneword)
			freq[c - 'a']++;
	}
}

void sort_word_by_freq(vs& words, vs&sorted_words)
{
	vi freq(26, 0);
	get_freq(words, freq);

	vpii scores;
	for (int i = 0; i < words.size(); i++)
	{
		int onescore = 0;
		vb seen(26, false);
		for (auto c : words[i])
		{
			if (!seen[c - 'a'])
			{
				seen[c - 'a'] = true;
				onescore += freq[c - 'a'];
			}
		}
		scores.push_back(make_pair(-onescore, i));
	}
	
	sort(scores.begin(), scores.end());
	for (auto pair : scores)
	{
		sorted_words.push_back(words[pair.second]);
	}

}

void sort_word_by_info(vs& words, vs& sorted_words){//, vumsi& wordCounts) {
	int wordCount = (int)words.size();
	vtsd sort_double;
	for (int curnum = 0; curnum < words.size(); curnum++) {
		string cword = words[curnum];
		double totalInformation = 0;
		umsi lcCount;
		for(int i = 0; i < 3; i++)
			for(int j = 0; j < 3; j++)
				for(int k = 0; k < 3; k++)
					for(int l = 0; l < 3; l++)
						for (int m = 0; m < 3; m++) {
							string cmask = "" + i + j + k + l + m;
							lcCount[cmask] = 0;
							//wordCounts[curnum][cmask] = 0;
						}

		for (int j = 0; j < words.size(); j++) {
			//if we assume that each word has equal probability
			//lets first find how many greens and yellows there are
			string eval = evaluate(cword, words[j]);
			lcCount[eval]++;
			//wordCounts[curnum][eval]++;
		}
		//caclulate info gained from these guesses
		for (auto c : lcCount) {
			if (c.second == 0)
				continue;
			double p = (1.0 * c.second) / wordCount;
			double info = -1.0 * log2(p);
			totalInformation += (p * info);
		}
		
		// negative so it sorts in reverse order 
		sort_double.push_back(make_tuple(cword, -1.0 * totalInformation));
	}
	sort(sort_double.begin(), sort_double.end(), TupleCompare<1>());
	for (auto curtup : sort_double) {
		sorted_words.push_back(get<0>(curtup));
	}
}


void sort_wordle_by_info(vs& words, vs& sorted_words, vs& words_answers) {//, vumsi& wordCounts) {
	int wordCount = (int)words.size();
	vtsd sort_double;
	for (int curnum = 0; curnum < words.size(); curnum++) {
		string cword = words[curnum];
		double totalInformation = 0;
		umsi lcCount;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				for (int k = 0; k < 3; k++)
					for (int l = 0; l < 3; l++)
						for (int m = 0; m < 3; m++) {
							string cmask = "" + i + j + k + l + m;
							lcCount[cmask] = 0;
							//wordCounts[curnum][cmask] = 0;
						}

		for (int j = 0; j < words_answers.size(); j++) {
			//if we assume that each word has equal probability
			//lets first find how many greens and yellows there are
			string eval = evaluate(cword, words[j]);
			lcCount[eval]++;
			//wordCounts[curnum][eval]++;
		}
		//caclulate info gained from these guesses
		for (auto c : lcCount) {
			if (c.second == 0)
				continue;
			double p = (1.0 * c.second) / wordCount;
			double info = -1.0 * log2(p);
			totalInformation += (p * info);
		}

		// negative so it sorts in reverse order 
		sort_double.push_back(make_tuple(cword, -1.0 * totalInformation));
	}
	sort(sort_double.begin(), sort_double.end(), TupleCompare<1>());
	for (auto curtup : sort_double) {
		sorted_words.push_back(get<0>(curtup));
	}
}


int interactive_solver1(vs& in_words, int step = 0)
{
	// greedy algorithm

	vs words;
	sort_word_by_freq(in_words, words);

	/*cout << "Possible: ";
	for (int i = 0; i < 10; i++)
	{
		if (i >= words.size())
			break;
		cout << words[i] << " ";
	}
	cout << endl;*/
	string curguess = words[0];
	cout << "Guess " << step << " = " << curguess << endl;
	cout << "Evaluation == ";
	string mask;
	cin >> mask;
	if (mask == "22222")
	{
		//cout << "I have guessed correctly" << endl;
		return step;
	}

	string pos = "     ";
	vi char_cnt(26, 0);
	vb nonexist_char(26, false);

	for (int i = 0; i < 5; i++)
	{
		switch (mask[i])
		{
		case '2':
			pos[i] = curguess[i];
			char_cnt[curguess[i] - 'a']++;
			continue;
		case '1':
			char_cnt[curguess[i] - 'a']++;
			continue;
		case '0':
			nonexist_char[curguess[i] - 'a'] = true;
			continue;
		}
	}

	vs possible_list;
	for (auto oneword : words)
	{
		vi cnt(26, 0);
		bool possible = true;
		for (int i = 0; i < 5; i++)
		{
			if (pos[i] != ' ' && pos[i] != oneword[i])
			{
				possible = false;
				break;
			}
			if (curguess[i] == oneword[i] && pos[i] != oneword[i])
			{
				possible = false;
				break;
			}
			cnt[oneword[i] - 'a']++;
		}
		// Green character doesn't match
		if (!possible)
			continue;

		for (int c = 0; c < 26; c++)
		{
			if (nonexist_char[c])
			{
				if (cnt[c] != char_cnt[c])
				{
					possible = false;
					break;
				}
			}
			else {
				if (cnt[c] < char_cnt[c])
				{
					possible = false;
					break;
				}
			}
		}

		if (possible)
			possible_list.push_back(oneword);
	}

	if (possible_list.size() == 0)
	{
		//cout << "I have run out of word in my dictionary" << endl;
		return step;
	}
	else {
		interactive_solver1(possible_list, step + 1);
	}

}


int auto_solver1(string actual_word, vs in_words, int step = 0, int startindex = -1)
{
	// greedy algorithm

	vs &words = in_words;
	vs out_words;
	
	if (step > 0)
	{
		//sort_word_by_freq(in_words, out_words);
		//vumsi placeholder(in_words.size());
		sort_word_by_info(in_words, out_words);//, placeholder);
		words = out_words;
	}
	if (startindex == -1)
		startindex = 0;

	/* cout << "Possible: ";
	for (int i = 0; i < 10; i++)
	{
		if (i >= words.size())
			break;
		cout << words[i] << " ";
	}
	cout << endl; */
	string curguess = words[0];
	if (step == 0)
		curguess = words[startindex];
	// cout << "Guess " << step << " = " << curguess << endl;
	// cout << "Evaluation == ";
	string mask = evaluate(actual_word, curguess);
	if (mask == "22222")
	{
		// cout << "I have guessed correctly" << endl;
		return step;
	}

	string pos = "     ";
	vi char_cnt(26, 0);
	vb nonexist_char(26, false);

	for (int i = 0; i < 5; i++)
	{
		switch (mask[i])
		{
		case '2':
			pos[i] = curguess[i];
			char_cnt[curguess[i] - 'a']++;
			continue;
		case '1':
			char_cnt[curguess[i] - 'a']++;
			continue;
		case '0':
			nonexist_char[curguess[i] - 'a'] = true;
			continue;
		}
	}

	vs possible_list;
	for (auto oneword : words)
	{
		vi cnt(26, 0);
		bool possible = true;
		for (int i = 0; i < 5; i++)
		{
			if (pos[i] != ' ' && pos[i] != oneword[i])
			{
				possible = false;
				break;
			}
			if (curguess[i] == oneword[i] && pos[i] != oneword[i])
			{
				possible = false;
				break;
			}
			cnt[oneword[i] - 'a']++;
		}
		// Green character doesn't match
		if (!possible)
			continue;

		for (int c = 0; c < 26; c++)
		{
			if (nonexist_char[c])
			{
				if (cnt[c] != char_cnt[c])
				{
					possible = false;
					break;
				}
			}
			else {
				if (cnt[c] < char_cnt[c])
				{
					possible = false;
					break;
				}
			}
		}

		if (possible)
			possible_list.push_back(oneword);
	}

	if (possible_list.size() == 0)
	{
		// cout << "I have run out of word in my dictionary" << endl;
		return -step;
	}
	else {
		return auto_solver1(actual_word, possible_list, step + 1, startindex);
	}

}
int auto_solver_wordle(string actual_word, vs in_words, vs words_answers, int step = 0, int startindex = -1)
{
	// greedy algorithm

	vs &words = in_words;
	vs out_words;

	if (step > 0)
	{
		//sort_word_by_freq(in_words, out_words);
		//vumsi placeholder(in_words.size());
		sort_wordle_by_info(in_words, out_words,words_answers);//, placeholder);
		words = out_words;
	}
	if (startindex == -1)
		startindex = 0;

	/* cout << "Possible: ";
	for (int i = 0; i < 10; i++)
	{
		if (i >= words.size())
			break;
		cout << words[i] << " ";
	}
	cout << endl; */
	string curguess = words[0];
	if (step == 0)
		curguess = words[startindex];
	// cout << "Guess " << step << " = " << curguess << endl;
	// cout << "Evaluation == ";
	string mask = evaluate(actual_word, curguess);
	if (mask == "22222")
	{
		// cout << "I have guessed correctly" << endl;
		return step;
	}

	string pos = "     ";
	vi char_cnt(26, 0);
	vb nonexist_char(26, false);

	for (int i = 0; i < 5; i++)
	{
		switch (mask[i])
		{
		case '2':
			pos[i] = curguess[i];
			char_cnt[curguess[i] - 'a']++;
			continue;
		case '1':
			char_cnt[curguess[i] - 'a']++;
			continue;
		case '0':
			nonexist_char[curguess[i] - 'a'] = true;
			continue;
		}
	}

	vs possible_list;
	for (auto oneword : words)
	{
		vi cnt(26, 0);
		bool possible = true;
		for (int i = 0; i < 5; i++)
		{
			if (pos[i] != ' ' && pos[i] != oneword[i])
			{
				possible = false;
				break;
			}
			if (curguess[i] == oneword[i] && pos[i] != oneword[i])
			{
				possible = false;
				break;
			}
			cnt[oneword[i] - 'a']++;
		}
		// Green character doesn't match
		if (!possible)
			continue;

		for (int c = 0; c < 26; c++)
		{
			if (nonexist_char[c])
			{
				if (cnt[c] != char_cnt[c])
				{
					possible = false;
					break;
				}
			}
			else {
				if (cnt[c] < char_cnt[c])
				{
					possible = false;
					break;
				}
			}
		}

		if (possible)
			possible_list.push_back(oneword);
	}

	if (possible_list.size() == 0)
	{
		// cout << "I have run out of word in my dictionary" << endl;
		return -step;
	}
	else {
		return auto_solver1(actual_word, possible_list, step + 1, startindex);
	}

}

/*




int auto_solver2(vs in_words, vumsi wordCounts)
{
	// uses breadth-first search

	//we are testing the top 10% of answers in terms of information given to see which paths are most optimal

	//top 10% is to make things faster for now, especially with the potential 12900+ data set

	//then we will score and return the one that finds the most optimal general path
	for (int start_index = 0; start_index < in_words.size() / 10; start_index++) {

		vs &words = in_words;
		
		int step = 0;

		
		queue < tuple<string, int>> bfs;
		vi char_cnt(26, 0);
		vb nonexist_char(26, false);
		bfs.push(make_tuple(words[start_index], 0));
		//to assess each guess, we are going to be using golf scores according to the following scale:
		//  4 is par, so any guess less than a 4 (or) on step 3, is -1, -2, etc. 
		//anything above is the opposite, until any guess that is 7 or higher, which is given a +4 instead for being inefficient
		while (!bfs.empty()) {
			string curguess = get<0>(bfs.front());
			int curstep = get<1>(bfs.front());
			bfs.pop();
			
			// no need to check since we aren't guessing
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					for (int k = 0; k < 3; k++) {
						for (int l = 0; l < 3; l++)
						{
							for (int m = 0; m < 3; m++) {
								
							}
						}
					}
				}
			}

		}
	}
}*/
int main()
{
	ifstream in("words_possible");
	ifstream cin("words_answer.in");
	//ifstream in("words_rare.in");
	vs words;
	vi freq(26, 0);
	umsi perf;

	
	while (!in.eof())
	{
		string cur;
		in >> cur;
		if (cur.length() <= 0)
			break;
		words.push_back(cur);
		for (auto c : cur)
		{
			freq[c - 'a']++;
		}
	}

	vs words_answers;
	while (!cin.eof()) {
		string cur;
		cin >> cur;
		if (cur.length() <= 0)
			break;
		words_answers.push_back(cur);
	}

	cout << "Total words = " << words.size() << endl;

	for (int i = 0; i < 26; i++)
		cout << "Frequency " << (char)('a' + i) << " : " << freq[i] << endl;

	vs sorted_words;
	//sort_word_by_freq(words, sorted_words);
	// 
	// The next code lets us curate the word to start with if
	//
	//sorted_words[0] = sorted_words[sorted_words.size()-1];
	//sorted_words.push_back("later");
	//vumsi wordCounts(words.size());
	sort_wordle_by_info(words, sorted_words, words_answers); //, wordCounts);

	int max_step = 10;
	vvvs solution(sorted_words.size(),vvs(3 * max_step + 1, vs()));
	//interactive_solver1(sorted_words, 0);
	vti totalScore(sorted_words.size());
	for (int curindex = 0; curindex < sorted_words.size()/1000; curindex++) {
		
		for (auto oneword : sorted_words)
		{
			int nstep = auto_solver_wordle(oneword, sorted_words, words_answers, 0, curindex);
			//cout << count << " Solve " << oneword << " = " << nstep << endl;
			//cout << sorted_words[curindex] << " testing " << oneword << " score "<< nstep << endl;
			solution[curindex][nstep + max_step].push_back(oneword);
		}
		// sum up the total
		int cScore = 0;
		for (int i = 0; i < 11; i++) {
			if(i <= 6)
				cScore += (i-3)*solution[curindex][i + max_step].size();
			else
				cScore += 9 * solution[curindex][i + max_step].size();
			
			
		}
		totalScore[curindex] = make_tuple(curindex, cScore);
	} 
	//auto_solver2(sorted_words, wordCounts);
	sort(totalScore.begin(), totalScore.end(), TupleCompare<1>());
	for (int i = 0; i < totalScore.size() / 1000; i++) {
		cout << "Word #" << i + 1 << ": " << sorted_words[i] << "  --- Score:  " << get<1>(totalScore[i]) << endl;
	}
	cout << "========= Solution Statistics ============= " << endl;
	for (int nstep = -max_step; nstep <= max_step; nstep++)
	{
		if (solution[get<0>(totalScore[0])][nstep + max_step].size() > 0)
		{
			cout << "Step " << nstep+1 << " = " << solution[get<0>(totalScore[0])][nstep + max_step].size() << " :";
			for (int j = 0; j < 10; j++)
			{
				if (j >= solution[get<0>(totalScore[0])][nstep + max_step].size())
					break;
				cout << " " << solution[get<0>(totalScore[0])][nstep + max_step][j];
			}
			cout << endl;
		}

	}
	return 0;
}