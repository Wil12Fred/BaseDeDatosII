int ifor=1;
string ejecutar(string& command){
	//cout << ifor << endl;
	int n=ifor;
	for (int ii=0;ii<n;ii++){
		int begin=0;
		while(command[begin]==' '){
			begin++;
		}
		string operation;
		for (int i=begin;i<command.size();i++){
			if(command[i]==' '){
				begin=i;
				break;
			} else {
				operation.push_back(command[i]);
			}
		}
		cout << "\'" << operation << "\'\n";
		if(command[begin]==' ' || command[begin]==';'){
			if (operation=="CREAR_TABLA"){
				crear_tabla(command, begin);
			} else if (operation=="INSERTAR_EN"){
				insertar_en_tabla(command, begin);
			} else if (operation=="ELIMINAR_EN"){
				stringstream iss(command);
				string oper,tabla,cond,comp;
				iss >> oper >> tabla >> cond;
				iss >> comp;
				eliminar(tabla,cond,comp);
			
			} else if (operation=="ACTUALIZAR_EN"){
				stringstream iss(command);
				string oper,tabla,cond,comp,cond1,new_value;
				iss >> oper >> tabla >> cond;
					iss >> comp;
					iss >> cond1 >> new_value;
					cout << new_value << endl;
					actualizar(tabla,cond,comp,cond1,new_value);
			} else if (operation=="SELECCIONAR_EN"){
				stringstream iss(command);
				string oper,tabla,cond,comp;
				iss >> oper >> tabla >> cond;
				if (cond=="*"){
					select(tabla);
				} else {
					iss >> comp;
					select(tabla,cond,comp);
				}
			} else if (operation=="BORRAR_TABLAS"){
				clear_tables();
			} else if (operation=="FOR"){
				stringstream iss(command);
				string oper;
				int cant;
				iss >> oper >> cant;
				ifor=cant;
			} else if (operation=="CREAR_INDICE_EN"){
				int op=-1,cl=-1;
				for (int i=0; i<command.size(); i++){
					if(command[i]=='('){
						command[i]=' ';
						op=i;
					} else if(command[i]==')'){
						command[i]=' ';
						cl=i;
					}
				}
				if(cl!=-1 && op!=-1 && op<cl){
					stringstream iss(command);
					string oper,tabla,cond,nameindex;
					iss >> oper >> tabla >> nameindex >> cond;
					int tipo=obtener_tipo(tabla, cond);
					if(tipo==0){//si existe
						AVL<int> A;
						int t=get_MaxId(tabla);
						vector<string> H=obtener_cabecera(tabla);
						//std::ifstream ifs (string(tabla+".txt").c_str(), std::ifstream::in);
						FILE *pFile;
						pFile = fopen(string(tabla+".txt").c_str(), "r");
						string line;
						fseek( pFile, 0, SEEK_SET);
						char c;
						while((c=char(fgetc(pFile)))!='\n'){
							line.push_back(c);
						}
						while(t--){
							line="";
							int bposition=ftell (pFile);
							while((c=char(fgetc(pFile)))!='\n'){
								line.push_back(c);
							}
							if(line[1]!='-'){
								stringstream iss(line);
								for (int i=0;i<3;i++){
									string next;
									iss >> next;
									if(H[i]==cond){
										A.set(stoi(next),bposition);
									}
								}
							}
						}
						fclose(pFile);
						//ifs.close();
						bfs(A.m_head, nameindex);
						//A.m_head=loadAVL<int>("index");
					}
				}
			}
		} else {
			break;//return "Error de syntaxis";
		}
	}
	if (n>1){
		ifor=1;
	}
	return "";
}
