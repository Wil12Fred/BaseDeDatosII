void crear_indice(string tabla, string nameindex, string cond){
	int tipo=obtener_tipo(tabla, cond);
	if(tipo==0){//si existe y es entero
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
		pFile = fopen("indixes.txt", "rw");
		if(pFile==NULL){
			pFile=fopen("indexes.txt", "w");
			fputs (string(tabla+" "+cond+" "+nameindex+"\n").c_str(), pFile);
			fclose(pFile);
		} else {
			fclose (pFile);
			pFile = fopen("indexes.txt", "a");
			fputs (string(tabla+" "+cond+" "+nameindex+"\n").c_str(), pFile);
			fclose(pFile);
		}
		//A.m_head=loadAVL<int>("index");
	}
}
