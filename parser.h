#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

void parse(string input_filename, string output_filename){
	ifstream infile(input_filename);
	ofstream outfile(output_filename, ios::trunc);

	string line;
	string word, definition;
	string cur_word;
	vector<string> defs;
	while (getline(infile, line)){

	    istringstream iss(line);
	    getline(iss, word, '\t');
	    getline(iss, definition, '\n');
	    
	    if(cur_word.empty()){
	    	cur_word = word;
	    	defs.push_back(definition);
	    	continue;
	    }

	    if(cur_word != word){
	    	outfile << cur_word << '\t';
	    	for(string s:defs){
	    		outfile << s << '\t';
	    	}
	    	outfile << endl;

	    	cur_word = word;
	    	defs.clear();
	    }
	    defs.push_back(definition);

	}

	if(!defs.empty()){
		outfile << cur_word << '\t';
    	for(string s:defs){
    		outfile << s << '\t';
    	}
    	outfile << endl;
	}

}

void parseLatin(string input_filename, string output_filename){
	ifstream infile(input_filename);
	ofstream outfile(output_filename, ios::trunc);

	string line;
	string word, definition;
	vector<string> words, defs;
	map<string,vector<string>> dict;
	while (getline(infile, line)){

	    istringstream iss(line);
	    getline(iss, word, '\t');
	    getline(iss, definition, '\n');
	    word.pop_back();
	    word.erase(remove(word.begin(),word.end(),'\"'),word.end());
	    definition.erase(remove(definition.begin(),definition.end(),'\"'),definition.end());
	    
	    istringstream iss_w(word);
	    while (getline(iss_w, word, ',')){
	    	if(word[0] < 'a') continue;
	    	words.push_back(word);
	    }

	    istringstream iss_d(definition);
	    while (getline(iss_d, definition, ',')){
	    	if(word[0] < 'a') continue;
	    	defs.push_back(definition);
	    }

	    for(string k:words){
	    	for(string val:defs){
	    		dict[k].push_back(val);
	    	}
	    }

	    words.clear();
	    defs.clear();

	}

	for (auto pp:dict){
		outfile << pp.first << '\t';
		for (string val:pp.second){
			outfile << val << '\t';
		}
		outfile << endl;
	}

}