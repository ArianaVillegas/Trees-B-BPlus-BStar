#include <iostream>
#include "parser.h"

using namespace std;

int main(){
	
	parse("data/French.txt","data/process_Frech.txt");
	parse("data/German.txt","data/process_German.txt");
	parse("data/Italian.txt","data/process_Italian.txt");
	parse("data/Portuguese.txt","data/process_Portuguese.txt");
	parse("data/Spanish.txt","data/process_Spanish.txt");
	parseLatin("data/Latin.txt","data/processed_Latin.txt");
	
	return 0;
}