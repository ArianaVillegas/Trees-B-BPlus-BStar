#include <iostream>
#include<set>
#include<fstream>
#include<vector>
#include <sstream>
#include <algorithm>
#include <cstdlib>

using namespace std;


int main() {

set<string> all_values;

vector<string> paths = {"data/processed_French.txt", "data/processed_German.txt", "data/processed_Italian.txt", "data/processed_Portuguese.txt", "data/processed_Spanish.txt", "data/processed_Latin.txt"};

for (auto path:paths) {
	ifstream infile(path);
	string line;
	while (getline(infile, line)) {
	    istringstream iss(line);
    	string key;
    	if (!(iss >> key)) { break; } // error
        for_each(key.begin(), key.end(), [](char & c) {c = ::tolower(c);});
    	all_values.insert(key);
	}
	infile.close();
}
vector<string> pool;
for (auto i = all_values.begin(); i != all_values.end(); i++)
    pool.push_back(*i);
int pool_size = pool.size();

srand (7);
vector<int> sizes = {10, 100, 1000, 10000};
ofstream fileout;
for (auto s:sizes){
    string tpath = "data" + to_string(s)+ ".txt";
    fileout.open(tpath, ios::trunc);
    while(s) {
        int i = rand()%pool_size;
        fileout << pool[i]<< std::endl;
        s--;
    }
    fileout.close();
}








}


