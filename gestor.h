#include <ctime>

int ifor=1;

unsigned t0, t1;

string ejecutar(string& command){
	//cout << ifor << endl;
	int n=ifor;
	string operation="";
	t0=clock();
	for (int ii=0;ii<n;ii++){
		int begin=0;
		while(command[begin]==' '){
			begin++;
		}
		for (int i=begin;i<command.size();i++){
			if(command[i]==' '){
				begin=i;
				break;
			} else {
				operation.push_back(command[i]);
			}
		}
		if(command[begin]==' ' || command[begin]==';'){
			if (operation=="CREAR_TABLA"){
				crear_tabla(command, begin);
			} else if (operation=="INSERTAR_EN"){
				cout << n << endl;
				insertar_en_tabla(command, begin, n);
				n=1;
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
					//cout << new_value << endl;
					actualizar(tabla,cond,comp,cond1,new_value);
			} else if (operation=="SELECCIONAR_EN"){
				stringstream iss(command);
				string oper,tabla,cond,comp;
				iss >> oper >> tabla >> cond;
				if (cond=="*"){
					select(tabla);
				} else {
					iss >> comp;
					if(comp[0]=='\''){
						int ibeg=-1;
						int iend=-1;
						for (int jj=0;jj<command.size();jj++){
							if(ibeg==-1){
								if(command[jj]=='\''){
									ibeg=jj;
								}
							} else {
								if(command[jj]=='\''){
									iend=jj;
								}
							}
						}
						comp=string(command,ibeg+1, iend-ibeg-1);
						//cout << comp << endl;
					}
					select(tabla,cond,comp);
				}
			} else if (operation=="ESCANEAR_EN"){
				stringstream iss(command);
				string oper,tabla,cond,comp;
				iss >> oper >> tabla >> cond;
				if (cond=="*"){
					select(tabla);
				} else {
					iss >> comp;
					if(comp[0]=='\''){
						int ibeg=-1;
						int iend=-1;
						for (int jj=0;jj<command.size();jj++){
							if(ibeg==-1){
								if(command[jj]=='\''){
									ibeg=jj;
								}
							} else {
								if(command[jj]=='\''){
									iend=jj;
								}
							}
						}
						comp=string(command,ibeg+1, iend-ibeg-1);
						//cout << comp << endl;
					}
					select(tabla,cond,comp, true);
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
					crear_indice(tabla, nameindex, cond);
				}
			}
		} else {
			break;//return "Error de syntaxis";
		}
	}
	t1=clock();
	double time = (double(t1-t0)/CLOCKS_PER_SEC);
	cout << "Execution Time: " << time << endl;
	if(operation!="FOR")
		ifor=1;
	return "";
}
