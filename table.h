#include <vector>

using namespace std;

struct datum{
	int size;
	bool type;
	//0 entero
	//1 varchar
	bool incremental;
	bool base;
};

struct header_register{
	string name;
	datum type_datum;
}

struct header_table{
	string name;
	vector<header_registro> Registro;
};

struct table{
	header_table header;
	void read_table();
	void write_table();
	void write_register();
}
