vector<string> T;
vector<bool> E;

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

bool select(string name, string pos, string cond){
	string nameIndex;
	bool existQuery=false;
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
			existQuery=true;
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
		cout << "\'" << line_table << "\'" << endl;
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
			to_size(types, 7);
			line_table+=types;
			return stoi(types);
		} else {
			to_size(types, 30);
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
		to_size(increm, 7);
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
		to_size(aleat, 7);
		line_table+=aleat;
	}else if(type=="basico" || type=="BASICO"){
		string basic=name_register+to_string(maxId);
		to_size(basic, 30);
		line_table+=basic;
	} else if(types[0]>=0 && types[0]<=9){
		string number=to_string(stoi(types));
		to_size(number, 7);
		line_table+=number;
	} else {
		string basic=types;
		to_size(basic, 30);
		line_table+=basic;
	}
	return maxId;
}

string insertar (string& name, vector<string>& name_registers, vector<string>& types, int fori=1){
	int maxId= get_MaxId(name);
	string line_table;
	for (int ii=0;ii<fori;ii++){
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
	add_register(line_table, name, maxId);
	for (int i=0; i<name_registers.size(); i++){
		string nameIndex;
		if(existIndice(name, name_registers[i],nameIndex)){
			crear_indice(name, nameIndex, name_registers[i]);
		}
	}
	return "Operación ejecutada exitosamente";
}

string insertar_en_tabla (string& command, int begin=0, int ifor=1){
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
						if(spaces==i){
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
	while(getline(ifs, line)){
		if(line[0]!=' ' && line[1]!='-'){
			cout << line << endl;
		}
	}
	ifs.close();
}
