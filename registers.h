vector<string> T;
vector<bool> E;

int obtener_tipo(string tabla, string cond){
	vector<pair<int,int> > ES = obtener_tabla(tabla);
	if(ES.size()==0){
		return -1;
	}
	std::ifstream ifs (string(tabla+".txt").c_str(), std::ifstream::in);
	int tipo=-1;
	for (int i=0; i<ES.size(); i++){
		string name_atributo;
		ifs >> name_atributo;
		if(name_atributo==cond){
			if(ES[i].first){
				tipo=1;
			} else {
				tipo=0;
			}
		}
	}
	ifs.close();
	return tipo;
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
		cout << "\'" << line_table << "\'" << endl;
		fputs ( line_table.c_str(), pFile );
	}
	fclose (pFile);
}

string insertar (string& name, vector<string>& name_registers, vector<string>& types){
	int maxId= get_MaxId(name);
	string line_table;
	for (int i=0;i<name_registers.size();i++){
		string type;
		if(types[i].size()<6){
			return "Error de syntaxis";
		}
		for (int j=0;j<6;j++){
			type.push_back(types[i][j]);
		}
		if(type=="increm" || type=="INCREM"){
			string increm=to_string(maxId+1);
			to_size(increm, 7);
			line_table+=increm;
		}else if(type=="aleato" || type=="ALEATO"){
			int j=6;
			while(types[i][j]==' '){//fixear limite j de types[i][j]
				j++;
			}
			if(types[i][j]!='('){
				return "Error de syntaxis";
			}
			j++;
			while(types[i][j]==' '){
				j++;
			}
			int base=0;
			while(types[i][j]>='0' && types[i][j]<='9'){
				base*=10;
				base+=types[i][j]-'0';
				j++;
			}
			while(types[i][j]==' '){
				j++;
			}
			if(types[i][j]!=','){
				return "Error de syntaxis";
			}
			j++;
			while(types[i][j]==' '){
				j++;
			}
			int top=0;
			while(types[i][j]>='0' && types[i][j]<='9'){
				top*=10;
				top+=types[i][j]-'0';
				j++;
			}
			while(types[i][j]==' '){
				j++;
			}
			if(types[i][j]!=')'){
				return "Error de syntaxis";
			}
			string aleat=to_string(rand()%(top-base)+base);
			to_size(aleat, 7);
			line_table+=aleat;
		}else if(type=="basico" || type=="BASICO"){
			string basic=name_registers[i]+to_string(maxId+1);
			to_size(basic, 30);
			line_table+=basic;
		} else if(types[i][0]>=0 && types[i][0]<=9){
			string number=to_string(stoi(types[i]));
			to_size(number, 7);
			line_table+=number;
		} else {
			string basic=types[i];
			to_size(basic, 30);
			line_table+=basic;
		}
	}
	line_table+="\n";
	add_register(line_table, name, maxId+1);
	return "Operación ejecutada exitosamente";
}

string insertar_en_tabla (string& command, int begin=0){
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
	return insertar(name, name_registers, type_registers);
}

void eliminar(string name, string cond, string comp){
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
	while (t--){
		fgetpos (pFile,&pos);
		fgets(line, sizeof(line), pFile);
		Line=string(line);
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

void actualizar(string name, string cond, string comp,string cond2,string new_value){
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
	stringstream iss(Line);
	vector<string> H(3);
	for (int i=0;i<3;i++){
		iss >> H[i];
	}
	vector<pair<long,string> > Positions;
	while (t--){
		fgetpos (pFile,&pos);
		fgets(line, sizeof(line), pFile);
		Line=string(line);
		string Line2=Line;
		stringstream iss(Line);
		string value;
		for (int i=0;i<3;i++){
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
						if(spaces==i+1){
							cout << new_value << endl;
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
				cout << Line2 << endl;
				Positions.push_back(make_pair(pos, Line2));
			}
		}
	}
	cout << Line.size();
	for (int i=0;i<Positions.size();i++){
		fseek ( pFile , Positions[i].first , SEEK_SET );
		fputs (Positions[i].second.c_str(),pFile);
	}
	fclose (pFile);
}

void select(string name){
	int t=get_MaxId(name);
	std::ifstream ifs (string(name+".txt").c_str(), std::ifstream::in);
	string line;
	vector<string> H(3);
	for (int i=0;i<3;i++){
		ifs >> H[i];
	}
	getline(ifs, line);
	while(t--){
		getline(ifs, line);
		if(line[1]!='-'){
			cout << line << endl;
		}
	}
	ifs.close();
}

void select(string name, string pos, string cond){
	string nameIndex;
	if(existIndice(name, pos, nameIndex)){
		cout << "usando indice: " << nameIndex << endl;
		AVL<int> A;
		A.m_head=loadAVL<int>(nameIndex);
		nodoAVL<int>* f=find(A.m_head, stoi(cond));
		cout << f->value.size() << endl;
		FILE* pFile;
		pFile = fopen(string(name+".txt").c_str(), "r");
		for (int i=0;i<f->value.size();i++){
			fseek(pFile, f->value[i], SEEK_SET);
			char c;
			string line;
			while((c=char(fgetc(pFile)))!='\n'){
				line.push_back(c);
			}
			cout << line << endl;
		}
		fclose(pFile);
	} else {
		int t=get_MaxId(name);
		std::ifstream ifs (string(name+".txt").c_str(), std::ifstream::in);
		string line;
		vector<string> H(3);
		for (int i=0;i<3;i++){
			ifs >> H[i];
		}
		getline(ifs, line);
		while(t--){
			getline(ifs, line);
			if(line[1]!='-'){
				stringstream iss(line);
				for (int i=0;i<3;i++){
					string next;
					iss >> next;
					if(pos==H[i] && next==cond){
						cout << line << endl;
					}
				}
			}
		}
		ifs.close();
	}
}
