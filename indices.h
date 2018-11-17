#include "avl.h"

bool existIndice(string tabla, string col, string& nameIndex){
	std::ifstream ifs("indexes.txt", std::ifstream::in);
	string line;
	nameIndex="";
	while (getline(ifs,line)){
		stringstream iss(line);
		int co=0;
		string el;
		while(iss >> el){
			if(co==0) {
				if(tabla!=el){
					break;
				}
			} else if(co==1){
				if(col!=el){
					break;
				}
			} else {
				nameIndex=el;
			}
			co++;
		}
		
	}
	if(nameIndex==""){
		return false;
	}
	return true;
}
