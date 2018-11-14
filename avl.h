#include "nodo.h"

template<class T>
nodoAVL<T>* rmin(nodoAVL<T>* n){                       
  if( n->nodo[0]==0 )                          
    return n->nodo[1];                       
  n->nodo[0] = rmin(n->nodo[0]);               
  return balancear(n);                           
}

template<class T>
nodoAVL<T>* balancear(nodoAVL<T>* n){
    actualizar(n);
    if(abs(balanceo(n))!=2)
      return n;
    bool aux=(balanceo(n)==2);
    if (dec(n,aux)){
      n->nodo[aux]=rotar(n->nodo[aux],!aux);
    }
    return rotar(n,aux);
}

template<class T>
nodoAVL<T>* inserting(nodoAVL<T>* n, int dato){
    if( !n )
      return new nodoAVL<T>(dato);
    bool aux=!(dato<n->dato);
    n->nodo[aux] = inserting(n->nodo[aux],dato);
    return balancear(n);
}

template<class T>
nodoAVL<T>* removing(nodoAVL<T>* n, int dato){
    if( !n ) 
      return 0;
    if(n->dato==dato){
      nodoAVL<T>* pp=n;//duplico
      delete n;
      if( !pp->nodo[1] ) return pp->nodo[0];
      nodoAVL<T>* aux = fmin(pp->nodo[1]);//aux
      aux->nodo[1] = rmin(pp->nodo[1]);
      aux->nodo[0] = pp->nodo[0];
      return balancear(aux);
    }
    bool aux=!(dato<n->dato);
    n->nodo[aux] = removing(n->nodo[aux],dato);
    return balancear(n);
}

template<class T>
class AVL{
  public:
    nodoAVL<T>* m_head;
    AVL(){
      m_head=NULL;
    }
    void insert(int dato){
      m_head=inserting(m_head,dato);
    }
    void remove(int dato){
      m_head=removing(m_head,dato);
    }
    void set(int id, int value){
	nodoAVL<int>* f = find(m_head,id);
	if(!f){
		insert(id);
		f = find(m_head,id);
	}
	f->value.push_back(value);
    }
};

template<class T>
void dfs(nodoAVL<T>* n,int nivel=0){
  if(n){
    dfs(n->nodo[0],nivel+1);
    cout << n->dato << " "  << nivel<<  endl;
    dfs(n->nodo[1],nivel+1);
  }
}

template<class T>
void bfs(nodoAVL<T>* n, string indexFile){
	FILE* pFile;
	pFile = fopen(string(indexFile+".txt").c_str(), "w");
	vector<pair<nodoAVL<T>*,int> > BFS;
	if(n){
		BFS.push_back(make_pair(n,0));
	}
	int i=0;
	int ant=-1;
	while(i<BFS.size()){
		for (int ii=ant+1; ii<BFS[i].second; ii++){
			//cout << endl;
			fprintf(pFile, "\n");
		}
		//cout <<  BFS[i].first->dato << " ";
		fprintf(pFile, "%d %d %d ",BFS[i].second, BFS[i].first->dato, (int)BFS[i].first->value.size());
		for (int j=0;j<BFS[i].first->value.size();j++){
			//cout << BFS[i].first->value[j] << " ";
			fprintf(pFile, "%d ", BFS[i].first->value[j]);
		}
		//cout << endl;
		fprintf(pFile, "\n");
		int nlinea=BFS[i].second;
		if(BFS[i].first->nodo[0]){
			BFS.push_back(make_pair(BFS[i].first->nodo[0],(nlinea+1)*2-1));
		}
		if(BFS[i].first->nodo[1]){
			BFS.push_back(make_pair(BFS[i].first->nodo[1],(nlinea+1)*2));
		}
		ant=BFS[i].second;
		i++;
	}
	fclose(pFile);
}
