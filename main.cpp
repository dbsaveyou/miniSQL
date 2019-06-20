#include<iostream>
#include"Interpreter.h"
#include"API.h"
using namespace std;
int main()
{
	string tmp;
	Interpreter inter;
	cout << "miniSQL>>";
	while (true)
	{
		getline(cin, tmp, ';');	
		while (!tmp.empty() && (tmp[0] == 0x20 || tmp[0] == 0x9 || tmp[0] == 0x0D || tmp[0] == 0x0A))
			tmp.erase(0, 1);
		if (!tmp.empty() && tmp.compare("quit") == 0)
			return 0;
		tmp += ';';
		inter.set_cmd(tmp);
		inter.Interpreter_cmd();
		cout << "miniSQL>>";
	}
}

