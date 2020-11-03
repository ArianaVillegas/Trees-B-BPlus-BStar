#include <iostream>
#include "parser.h"

using namespace std;

int main(){
	
	parse("data/French.txt","data/processed_Frech.txt");
	parse("data/German.txt","data/processed_German.txt");
	parse("data/Italian.txt","data/processed_Italian.txt");
	parse("data/Portuguese.txt","data/processed_Portuguese.txt");
	parse("data/Spanish.txt","data/processed_Spanish.txt");
	parseLatin("data/Latin.txt","data/processed_Latin.txt");
	
	return 0;
}