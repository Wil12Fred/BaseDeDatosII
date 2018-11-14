#include <iostream>
#include <ctime>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

#include "utility.h"
#include "tables.h"
#include "indices.h"
#include "registers.h"
#include "gestor.h"

int main(){
	srand (time(NULL));
	string command;
	string line;
	cout << "BDII >> ";
	while(getline(cin, line)){
		for (int i=0;i<line.size();i++){
			if(line[i]==';'){
				command.push_back(' ');
				cout << ejecutar(command) << endl;
				command="";
				break;
			} else {
				command.push_back(line[i]);
			}
		}
		cout << "     >> ";
	}
}
