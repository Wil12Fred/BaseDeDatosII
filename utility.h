int get_MaxId(string& name, int jump=-1, int* size=NULL){
	cout << jump << endl;
	FILE * pFile;
	long lSize;
	pFile = fopen (string(name+".txt").c_str(),"rw");
	if(pFile==NULL){
		return -1;
	}
	char line[256];
	fgets(line, sizeof(line), pFile);
	fpos_t init;
	fgetpos(pFile, &init);
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	string number="";
	if(lSize>0){
		fseek ( pFile , lSize-1 , SEEK_SET );
		
		int c;
		while ( lSize-1>-1 && (c=char(fgetc(pFile)))!=' '){
			number.push_back(c);
			lSize--;
			fseek ( pFile , lSize-1 , SEEK_SET );
		}
		if(jump!=-1 && size){
			cout << lSize << endl;
			cout << init << endl;
			*size=(lSize-init)/(jump+1);
		}
	}
	fclose (pFile);
	reverse(number.begin(), number.end());
	return stoi(number);
}

void to_size(string& st,int size){
	for (int i=st.size();i<size;i++){
		st.push_back(' ');
	}
}
