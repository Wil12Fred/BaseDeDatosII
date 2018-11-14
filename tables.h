vector<pair<int,int> > obtener_tabla(string name){
	vector<pair<int,int> > ES;
	std::ifstream ifs ("tables.txt", std::ifstream::in);
	string line;
	while(getline(ifs, line)){
		stringstream iss(line);
		string table_name;
		int cant_at;
		iss >> table_name;
		iss >> cant_at;
		if(table_name==name){
			string at;
			while(iss >> at){
				string tam;
				if(at=="1"){
					iss >> tam;
					ES.push_back(make_pair(1, stoi(tam)));
				} else {
					ES.push_back(make_pair(0, -1));
				}
			}
			break;
		}
	}
	ifs.close();
	return ES;
}

vector<string> obtener_cabecera(string tabla){
	vector<pair<int,int> > ES=obtener_tabla(tabla);
	vector<string> H;
	std::ifstream ifs (string(tabla+".txt").c_str(),std::ifstream::in);
	for (int i=0;i<ES.size();i++){
		string param;
		ifs >> param;
		H.push_back(param);
	}
	ifs.close();
	return H;
}

void clear_tables(){//R
	FILE * pFile;
	pFile = fopen ("tables.txt","w");
	fclose (pFile);
}

void add_table(string& line_table, string& name,vector<string>& headers){
	FILE * pFile;
	long lSize;
	fpos_t position;
	pFile = fopen ("tables.txt","rw");
	if(pFile==NULL){
		pFile = fopen ("tables.txt","w");
		fputs (line_table.c_str(),pFile);
		fclose (pFile);
	} else {
		fclose (pFile);
		pFile = fopen ("tables.txt","a");
		fputs (line_table.c_str(),pFile);
		fclose (pFile);
	}
	string name2=name+".txt";
	pFile = fopen (name2.c_str(),"w");
	string top;
	for (int i=0;i<headers.size();i++){
		top+=headers[i] + " ";
	}
	top+="\n 0";
	cout << top;
	fputs (top.c_str(),pFile);
	fclose (pFile);
}

//entero 0
//varchar 1 + tam
string crear_tabla (string& name, vector<string>& name_registers, vector<string>& types){
	string line_table;
	for (int i=0;i<name.size();i++){
		if(name[i]==' '){
			break;
		}  else {
			line_table+=name[i];
		}
	}
	line_table+=" ";
	line_table+=to_string(name_registers.size());
	for (int i=0;i<name_registers.size();i++){
		string type;
		if(types[i].size()<6){
			return "Error de syntaxis";
		}
		for (int j=0;j<6;j++){
			type.push_back(types[i][j]);
		}
		if(type=="entero" || type=="ENTERO"){
			line_table+=" 0";
		} else {
			if(types[i].size()<10){
				return "Error de syntaxis";
			}
			type.push_back(types[i][6]);
			type.push_back(types[i][7]);
			if(type=="varchar(" || type=="VARCHAR("){
				int begin=8;
				string number;
				for (int j=begin;j<types[i].size();j++){
					if(types[i][j]==')'){
						begin=j;
						break;
					} else {
						number.push_back(types[i][j]);
					}
				}
				if(types[i][begin]!=')'){
					return "Error de syntaxis";
				}
				line_table+= " 1 ";
				line_table+= number;
			} else {
				return "Error de syntaxis";
			}
		}
	}
	line_table+="\n";
	add_table(line_table, name, name_registers);
	return "Operación ejecutada exitosamente";
}

string crear_tabla (string& command, int begin=0){
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
		if(command[i]==' ' || command[i]=='('){
			begin=i;
			break;
		} else {
			name.push_back(command[i]);
		}
	}
	while(command[begin]==' '){
		begin++;
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
	return crear_tabla(name, name_registers, type_registers);
}
