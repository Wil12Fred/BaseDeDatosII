#include <map>

template<class T>
class nodoAVL{
  public:
    int dato;
    int altura;
    vector<int> value;
    nodoAVL<T>* nodo[2];
    nodoAVL<T>(int n){
      dato = n;
      nodo[0]=nodo[1]= 0;
      altura = 1;
    }
};
template<class T>
int balanceo(nodoAVL<T>* n){
    return altura(n->nodo[1])-altura(n->nodo[0]);
}
template<class T>
inline bool dec(nodoAVL<T>* n ,bool aux){
  return aux?balanceo(n->nodo[aux])<0:balanceo(n->nodo[aux])>0;
}
template<class T>
void actualizar(nodoAVL<T>* n){
  n->altura=max(altura(n->nodo[0]),altura(n->nodo[1]))+1;
}

template<class T>
int altura(nodoAVL<T>* n){
  return n?n->altura:0;
}

template<class T>
nodoAVL<T>* rotar(nodoAVL<T>* n,bool aux){
    nodoAVL<T>* q = n->nodo[aux];
    n->nodo[aux] = q->nodo[!aux];
    q->nodo[!aux] = n;
    actualizar(n);
    actualizar(q);
    return q;
}

template<class T>
nodoAVL<T>* fmin(nodoAVL<T>* n){
    return n->nodo[0]?fmin(n->nodo[0]):n;
}

template<class T>
nodoAVL<T>* find(nodoAVL<T>* n, int x){
	if(n==NULL){
		return NULL;
	}
	if(n->dato==x){
		return n;
	}
	if(x<n->dato){
		return find(n->nodo[0],x);
	}
	return find(n->nodo[1],x);
}


template<class T>
vector<int> get(nodoAVL<T>* n, int id){
	nodoAVL<T>* lines=find(n,id);
	if(lines!=NULL){
		return lines->value;
	}
}

template<class T>
nodoAVL<T>* loadAVL(string file){
	map<int, nodoAVL<T>*> V;
	FILE* pFile;
	pFile = fopen(string(file+".txt").c_str(), "r");
	int id;
	while(fscanf(pFile, "%d", &id)!=EOF){
		id++;
		int value, cant;
		fscanf(pFile, "%d %d", &value, &cant);
		if(id==1){
			V[id]=new nodoAVL<T>(value);
		} else {
			int dad=id/2;
			if(id==dad*2){
				V[dad]->nodo[0]=new nodoAVL<T>(value);
				V[id]=V[dad]->nodo[0];
			} else {
				V[dad]->nodo[1]=new nodoAVL<T>(value);
				V[id]=V[dad]->nodo[1];
			}
		}
		for (int i=0;i<cant;i++){
			int pos;
			fscanf(pFile, "%d", &pos);
			V[id]->value.push_back(pos);
		}
	}
	fclose(pFile);
	return V[1];
}
