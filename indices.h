map<pair<string, int> , nodoAVL<int>* > Indices;

//Verifica si existe indice en la tabla basada en col                         
bool existIndice(string tabla, string col, string& nameIndex, string cond="-1", int* last=0){//R
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
                        } else if(co==2){
                                if(cond!="-1" && stoi(cond)/1000000!=stoi(el)){             
                                        break;
                                }
                        } else if(co==3){
                                nameIndex=el;//+cond;                         
                        } else {
				if(last){
					*last=stoi(el);
				}
			}
                        co++;
                }

        }
        if(nameIndex==""){
                return false;
        }
        return true;
}

map<string, int> Last;

//int last=0;

void crear_indice(string tabla, string nameindex, string cond, bool force=false){//R
	cout << tabla << nameindex << cond << endl;
	int jump;
	int prow;
	int tipo=obtener_tipo(tabla, cond, &jump, &prow);
	if(tipo==0){//si existe y es entero
		map<int, AVL<int>* > B;
		int size;
		int t=get_MaxId(tabla, jump, &size);
		vector<string> H=obtener_cabecera(tabla);
		int h_pos=-1;
		for (int i=0;i<H.size();i++){
			if(H[i]==cond){
				h_pos=i;
			}
		}
		//std::ifstream ifs (string(tabla+".txt").c_str(), std::ifstream::in);
		FILE *pFile;
		pFile = fopen(string(tabla+".txt").c_str(), "r");
		//string line;
		//fseek( pFile, 0, SEEK_SET);
		char c;
		while((c=char(fgetc(pFile)))!='\n'){
			//line.push_back(c);
		}
		char line[256];
		int past3=size;
		int past2=0;
		int last;
		if(Last.find(nameindex)!=Last.end()){
			last=Last[nameindex];
		} else {
			last=0;
		}
		if(force){
			last=0;
		}
		while(past2<last){
			//int bposition=ftell (pFile);
			fgets(line,sizeof(line),pFile);
			past2++;
			size--;
		}
		last=past3;
		Last[nameindex]=last;
		while(size--){
			int bposition=ftell (pFile);
			/*while((c=char(fgetc(pFile)))!=EOF && c!='\n'){
				line.push_back(c);
			}*/
			fgets(line,sizeof(line),pFile);
			if(line[0]==' '){
				break;
			}
			if(line[1]=='-'){
				continue;
			}
			int value = stoi(string(line,prow,tamEntero));
			string nameIndexTemp;
			int base = (value/1000000);
			if(B.find(base)==B.end()){
 				B[base] = new AVL<int>();
				if(!force && existIndice(tabla, cond, nameIndexTemp, to_string(value))){
					cout << "temp: " << nameIndexTemp << endl; 
					cout << "base: " << base << endl;
					(*B[base]).m_head=loadAVL<int>(nameIndexTemp+to_string(base));
					cout << "pase\n";
				}
			}
			(*B[base]).set(stoi(string(line,prow,tamEntero)),bposition);
			/*stringstream iss(line);
			for (int i=0;i<3;i++){
				string next;
				iss >> next;
				if(H[i]==cond){
					B.set(stoi(next),bposition);
				}
			}*/
		}
		fclose(pFile);
		//ifs.close();
		for (auto it=B.begin(); it!=B.end(); it++){
			bfs((*(it->second)).m_head, nameindex+to_string(it->first));
		}
		pFile = fopen("indexes.txt", "rw");
		if(pFile==NULL){
			pFile=fopen("indexes.txt", "w");
			for (auto it=B.begin(); it!=B.end(); it++){
				fputs (string(tabla+" "+cond+" "+ to_string(it->first) +  " " + nameindex +"\n").c_str(), pFile);
			}
			fclose(pFile);
		} else {
			fclose (pFile);
			pFile = fopen("indexes.txt", "a");
			for (auto it=B.begin(); it!=B.end(); it++){
				fputs (string(tabla+" "+cond+ " " + to_string(it->first)+" "+nameindex + " " +to_string(Last[nameindex]) + "\n").c_str(), pFile);
				if(Indices.find(make_pair(nameindex, it->first))!=Indices.end()){
					Indices.erase(make_pair(nameindex, it->first));
				}
			}
			fclose(pFile);
		}
		//A.m_head=loadAVL<int>("index");
	}
}
