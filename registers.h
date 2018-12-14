#include <ctime>

vector<string> T;
vector<bool> E;
//map<pair<string, int> string, nodoAVL<int>* > Indices;
//map</*pair<string, string>*/string, int > Base;

AVL<int> A;

bool select(string name, string pos, string cond, bool tablescanning=false){//R
	cout << "select" << name << pos << cond << endl;
	string nameIndex;
	bool existQuery=false;
	int jump;
	int prow;
	int tipo=obtener_tipo(name, pos, &jump, &prow);
	if(!tablescanning && tipo==0 && existIndice(name, pos, nameIndex, cond)){
		cout << "usando índice: " << nameIndex << endl;
		A.m_head=NULL;
		int base=stoi(cond)/1000000;
		if(Indices.find(make_pair(nameIndex,base))!=Indices.end()){
			A.m_head = Indices[make_pair(name,base)];
		}
		if(!A.m_head){
			cout << "cargar avl" << endl;
			A.m_head=loadAVL<int>(nameIndex+to_string(base));
			Indices[make_pair(name,base)]=A.m_head;
			cout << "100%" << endl;
		}
		unsigned t0, t1;
		t0=clock();
		nodoAVL<int>* f=find(A.m_head, stoi(cond));
		if(f && f->dato==stoi(cond)){
			FILE* pFile, *pFile2;
			pFile2 = fopen(string(nameIndex+to_string(base)+".txt").c_str(), "r");
			pFile = fopen(string(name+".txt").c_str(), "r");
			fseek(pFile2, f->value[0], SEEK_SET);
			int n;
			fscanf(pFile2, "%d", &n);
			cout << n << endl;
			for (int i=0; i<n; i++){
				int pos;
				fscanf(pFile2, "%d", &pos);
				fseek(pFile, pos, SEEK_SET);
				char c;
				string line;
				while((c=char(fgetc(pFile)))!='\n'){
					line.push_back(c);
				}
				if(line[1]!='-'){
					cout << line << endl;
					existQuery=true;
				}
			}
			fclose(pFile2);
			fclose(pFile);
		}
		t1=clock();
		double time = (double(t1-t0)/CLOCKS_PER_SEC);
		cout << "Query Time: " << time << endl;
	} else {
		int t=get_MaxId(name);
		std::ifstream ifs (string(name+".txt").c_str(), std::ifstream::in);
		string line;
		vector<string> H(3);
		for (int i=0;i<3;i++){
			ifs >> H[i];
		}
		getline(ifs, line);
		while(getline(ifs, line)){
			//getline(ifs, line);
			if(line[0]!=' ' && line[1]!='-'){
				stringstream iss(line);
				for (int i=0;i<3;i++){
					string next;
					iss >> next;
					if(pos==H[i] && next==cond){
						cout << line << endl;
						existQuery=true;
					}
				}
			}
		}
		ifs.close();
	}
	return existQuery;
}

void add_register(string& line_table, string& name, int maxId){//R
	FILE * pFile;
	long lSize;
	pFile = fopen (string(name+".txt").c_str(),"r+w");
	if(pFile==NULL){
		return;
	}
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	string number=" " + to_string(maxId);
	line_table+=number;
	if(lSize>0){
		fseek ( pFile , lSize-1 , SEEK_SET );
		int c;
		while ( lSize-1>-1 && (c=char(fgetc(pFile)))!=' '){
			lSize--;
			fseek ( pFile , lSize-1 , SEEK_SET );
		}
		lSize--;
		fseek ( pFile , lSize , SEEK_SET );
		//cout << "\'" << line_table << "\'" << endl;
		fputs ( line_table.c_str(), pFile );
	}
	fclose (pFile);
}

int evaluate_register(string table, string name_register, string types, string& line_table, int maxId=0, bool key=false){
	string type;
	if(types.size()<6){
		int tipo=obtener_tipo(table,name_register);
		if(tipo==-1){
			return -1;
		}
		if(tipo==0){
			if(key && select(table, name_register, types)){
				return -1;
			}
			to_size(types, tamEntero);
			line_table+=types;
			return stoi(types);
		} else {
			to_size(types, tamString);
			line_table+=types;
			return maxId;
		}
	}
	for (int j=0;j<6;j++){
		type.push_back(types[j]);
	}
	if(type=="increm" || type=="INCREM"){
		maxId++;
		string increm = to_string(maxId);
		to_size(increm, tamEntero);
		line_table+=increm;
	}else if(type=="aleato" || type=="ALEATO"){
		int j=6;
		while(types[j]==' '){//fixear limite j de types[i][j]
			j++;
		}
		if(types[j]!='('){
			return -1;
		}
		j++;
		while(types[j]==' '){
			j++;
		}
		int base=0;
		while(types[j]>='0' && types[j]<='9'){
			base*=10;
			base+=types[j]-'0';
			j++;
		}
		while(types[j]==' '){
			j++;
		}
		if(types[j]!=','){
			return -1;
		}
		j++;
		while(types[j]==' '){
			j++;
		}
		int top=0;
		while(types[j]>='0' && types[j]<='9'){
			top*=10;
			top+=types[j]-'0';
			j++;
		}
		while(types[j]==' '){
			j++;
		}
		if(types[j]!=')'){
			return -1;
		}
		string aleat=to_string(rand()%(top-base)+base);
		to_size(aleat, tamEntero);
		line_table+=aleat;
	}else if(type=="basico" || type=="BASICO"){
		string basic=name_register+to_string(maxId);
		to_size(basic, tamString);
		line_table+=basic;
	} else if(types[0]>='0' && types[0]<='9'){
		string number=to_string(stoi(types));
		to_size(number, tamEntero);
		line_table+=number;
	} else {
		string basic=types;
		cout << basic << endl;
		to_size(basic, tamString);
		line_table+=basic;
	}
	return maxId;
}

int tamBloque=1000000;

string insertar (string& name, vector<string>& name_registers, vector<string>& types, int fori=1){//R
	int maxId= get_MaxId(name);
	for (int i=0;i<name_registers.size();i++){
		cout << name_registers[i] << " ";
	}
	cout << endl;
	int max=fori;
	for (int iii=0; iii<max; iii+=tamBloque){
		string line_table;
		int forii=min(fori,tamBloque);
		for (int ii=0;ii<forii;ii++){
			int lineId=-1;
			for (int i=0;i<name_registers.size();i++){
				string new_reg;
				if(i==0){
					int newId=evaluate_register(name, name_registers[i],types[i],new_reg,maxId, true);
					if(newId==-1){
						return "Error de Syntaxis";
					}
					lineId=newId;
					if(newId>maxId){
						maxId=newId;
					}
				} else {
					int newId=evaluate_register(name, name_registers[i],types[i],new_reg,lineId);
					if(newId==-1){
						return "Error de Syntaxis";
					}
				}
				line_table+=new_reg;
			}
			line_table+="\n";
			//maxId++;
		}
		fori-=forii;
		add_register(line_table, name, maxId);
	}
	for (int i=0; i<name_registers.size(); i++){
		string nameIndex;
		if(existIndice(name, name_registers[i],nameIndex)){
			crear_indice(name, nameIndex, name_registers[i]);
		}
	}
	return "Operación ejecutada exitosamente";
}

string insertar_en_tabla (string& command, int begin=0, int ifor=1){//R
	for (int i=begin;i<command.size();i++){
		if(command[i]!=' '){
			begin=i;
			break;
		}
	}
	if(command[begin]==' ' || command[begin]=='('){
		return "Error de syntaxis";
	}
	string name;
	for (int i=begin;i<command.size();i++){
		if(command[i]=='('){
			begin=i;
			break;
		} else {
			name.push_back(command[i]);
		}
	}
	if(command[begin]!='('){
		return "Error de syntaxis";
	}
	vector<string> name_registers;
	vector<string> type_registers;
	string name_reg;
	string type_reg;
	while(command[begin]!=')'){
		name_reg="";
		type_reg="";
		begin++;
		for (int i=begin;i<command.size();i++){
			if(command[i]!=' '){
				begin=i;
				break;
			}
		}
		if(begin==command.size()){
			return "Error de syntaxis";
		}
		for (int i=begin;i<command.size();i++){
			if(command[i]==':'){
				begin=i;
				break;
			} else {
				name_reg.push_back(command[i]);
			}
		}
		if(command[begin]!=':'){
			return "Error de syntaxis";
		}
		begin++;
		for (int i=begin;i<command.size();i++){
			if(command[i]!=' '){
				begin=i;
				break;
			}
		}
		if(begin==command.size()){
			return "Error de syntaxis";
		}
		int par=0;
		for (int i=begin;i<command.size();i++){
			if(command[i]==',' and par==0) { 
				begin=i;
				break;
			} else if (command[i]==')'){
				par--;
				if(par==-1){
					begin=i;
					break;
				} else {
					type_reg.push_back(command[i]);
				}
			} else {
				if(command[i]=='('){
					par++;
				}
				type_reg.push_back(command[i]);
			}
		}
		if(command[begin]==':'){
			return "Error de syntaxis";
		}
		name_registers.push_back(name_reg);
		type_registers.push_back(type_reg);
	}
	return insertar(name, name_registers, type_registers,ifor);
}

void eliminar(string name, string cond, string comp){//R
	string nameIndex;
	//bool existQuery=false;
	int jump;
	int prow;
	int tipo=obtener_tipo(name, cond, &jump, &prow);
	if(tipo==0 && existIndice(name, cond, nameIndex, comp)){
		cout << "usando indice: " << nameIndex << endl;
		int base=stoi(comp)/1000000;
		A.m_head=loadAVL<int>(nameIndex+to_string(base));
		nodoAVL<int>* f=find(A.m_head, stoi(comp));
		if(f && f->dato==stoi(comp)){
			FILE* pFile, *pFile2;
			pFile2 = fopen(string(nameIndex+to_string(base)+".txt").c_str(), "r");
			pFile = fopen(string(name+".txt").c_str(), "r+w");
			fseek(pFile2, f->value[0], SEEK_SET);
			int n;
			fscanf(pFile2, "%d", &n);
			for (int i=0; i<n; i++){
				int pos;
				fscanf(pFile2, "%d", &pos);
				fseek(pFile, pos+1, SEEK_SET);
				fputs("-", pFile);
			}
			fclose(pFile2);
			fclose(pFile);
			//crear_indice(name, nameIndex, cond);
			/*cout << f->value.size() << endl;
			FILE* pFile;
			pFile = fopen(string(name+".txt").c_str(), "r+w");
			for (int i=0;i<f->value.size();i++){
				fseek(pFile, f->value[i]+1, SEEK_SET);
				fputs("-",pFile);
			}
			fclose(pFile);
			crear_indice(name, nameIndex, cond);*/
		}
	} else {
		int t=get_MaxId(name);
		FILE * pFile;
		long lSize;
		fpos_t pos;
		pFile = fopen (string(name+".txt").c_str(),"r+w");
		if(pFile==NULL){
			return;
		}
		char line[256];
		fgets(line, sizeof(line), pFile);
		string Line(line);
		stringstream iss(Line);
		vector<string> H(3);
		for (int i=0;i<3;i++){
			iss >> H[i];
		}
		vector<long> Positions;
		while (t){
			fgetpos (pFile,&pos);
			fgets(line, sizeof(line), pFile);
			Line=string(line);
			if(Line[0]==' '){
				break;
			}
			if(Line[1]=='-'){
				continue;
			}
			stringstream iss(Line);
			string value;
			for (int i=0;i<3;i++){
				iss >> value;
				if(H[i]==cond && value==comp){
					Positions.push_back(pos);
				}
			}
		}
		for (int i=0;i<Positions.size();i++){
			fseek ( pFile , Positions[i]+1 , SEEK_SET );
			fputs ("-",pFile);
		}
		fclose (pFile);
	}
}

void actualizar(string name, string cond, string comp,string cond2,string new_value){
	AVL<int> B;
	string nameIndex;
	if(existIndice(name, cond, nameIndex, comp)){
		B.m_head=loadAVL<int>(nameIndex+to_string( stoi(comp)/1000000));
	}
	vector<string>H=obtener_cabecera(name);
	int ipos=-1;
	string value;
	for (int i=0;i<H.size();i++){///<-----------------
		if(H[i]==cond2){
			ipos=i;
		}
	}
	cout << ipos << endl;
	vector<pair<long,string> > Positions;
	if(B.m_head==NULL){
		cout << "nv " <<  new_value << endl;
		int t=get_MaxId(name);
		FILE * pFile;
		long lSize;
		fpos_t pos;
		pFile = fopen (string(name+".txt").c_str(),"r+w");
		if(pFile==NULL){
			return;
		}
		char line[256];
		fgets(line, sizeof(line), pFile);
		string Line(line);
		while (t){
			fgetpos (pFile,&pos);
			fgets(line, sizeof(line), pFile);
			Line=string(line);
			if(Line[0]==' '){
				break;
			}
			if(Line[1]=='-'){
				continue;
			}
			string Line2=Line;
			stringstream iss(Line);
			for (int i=0;i<H.size();i++){///<-------------------
				iss >> value;
				if(H[i]==cond && value==comp){
					int spaces=0;
					bool update=0;
					for (int j=0;j<Line2.size();j++){
						if(Line2[j]==' '){
							spaces++;
							while(Line2[j]==' '){
								j++;
							}
							j--;
						} else {
							if(spaces==ipos){
								//cout << new_value << endl;
								if(update==0){
									for (int k=0;k<new_value.size();k++){
										Line2[j]=new_value[k];
										j++;
									}
									update=1;
								} else {
									Line2[j]=' ';
								}
							}
						}
					}
					cout << Line2;
					Positions.push_back(make_pair(pos, Line2));
				}
			}
		}
		for (int i=0;i<Positions.size();i++){
			fseek ( pFile , Positions[i].first , SEEK_SET );
			fputs (Positions[i].second.c_str(),pFile);
		}
		fclose (pFile);
	} else {
		cout << "usando indice: " << nameIndex << endl;
		int base=stoi(comp)/1000000;
		nodoAVL<int>* f=find(B.m_head, stoi(comp));
		char line[256];
		if(f && f->dato==stoi(comp)){
			FILE* pFile, *pFile2;
			pFile2 = fopen(string(nameIndex+to_string(base)+".txt").c_str(), "r");
			pFile = fopen(string(name+".txt").c_str(), "r+w");
			fseek(pFile2, f->value[0], SEEK_SET);
			int n;
			fscanf(pFile2, "%d", &n);
			for (int i=0; i<n; i++){
				int pos;
				fscanf(pFile2, "%d", &pos);
				fseek(pFile, pos, SEEK_SET);
				fgets(line, sizeof(line), pFile);
				string Line=string(line);
				if(Line[0]==' '){
					break;
				}
				if(Line[1]=='-'){
					continue;
				}
				string Line2=Line;
				stringstream iss(Line);
				for (int i=0;i<H.size();i++){///<-------------------
					iss >> value;
					if(H[i]==cond && value==comp){
						int spaces=0;
						bool update=0;
						for (int j=0;j<Line2.size();j++){
							if(Line2[j]==' '){
								spaces++;
								while(Line2[j]==' '){
									j++;
								}
								j--;
							} else {
								if(spaces==ipos){
									//cout << new_value << endl;
									if(update==0){
										for (int k=0;k<new_value.size();k++){
											Line2[j]=new_value[k];
											j++;
										}
										update=1;
									} else {
										Line2[j]=' ';
									}
								}
							}
						}
						cout << Line2;
						Positions.push_back(make_pair(pos, Line2));
					}
				}
			}
			for (int i=0;i<Positions.size();i++){
				fseek ( pFile , Positions[i].first , SEEK_SET );
				fputs (Positions[i].second.c_str(),pFile);
			}
			fclose (pFile);
			//fclose(pFile2);
			fclose(pFile);
			//crear_indice(name, nameIndex, cond);
			/*cout << f->value.size() << endl;
			FILE* pFile;
			pFile = fopen(string(name+".txt").c_str(), "r+w");
			for (int i=0;i<f->value.size();i++){
				fseek(pFile, f->value[i]+1, SEEK_SET);
				fputs("-",pFile);
			}
			fclose(pFile);
			crear_indice(name, nameIndex, cond);*/
		}
	}
	if(existIndice(name, cond2, nameIndex)){
		crear_indice(name, nameIndex, cond2, true);
		//pFile = fopen (string(name+".txt").c_str(),"r+w");
		//
		//fclose(pFile);
	}
}

void select(string name){//R
	int t=get_MaxId(name);
	std::ifstream ifs (string(name+".txt").c_str(), std::ifstream::in);
	string line;
	vector<string> H=obtener_cabecera(name);//(3);
	for (int i=0;i<H.size();i++){
		ifs >> H[i];
	}
	getline(ifs, line);
	while(getline(ifs, line)){
		if(line[0]!=' ' && line[1]!='-'){
			cout << line << endl;
		}
	}
	ifs.close();
}
