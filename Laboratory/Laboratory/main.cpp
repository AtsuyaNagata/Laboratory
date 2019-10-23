#include<iostream>
#include"functionPtr.h"
using namespace std;


int main()
{
	check_return_funcptr();

	return 0;
}

/*
int main()
{
	FILE* MultiProject;
	MultiProject = _popen("MultiProject.exe", "r");
	if (MultiProject == 0) {
		return 1;
	}
	char buff[256];
	while (fgets(buff, sizeof(buff), MultiProject) != NULL)
	{
		printf("%s", buff);
	}

	scanf_s("%s", buff);

	_pclose(MultiProject);

	system("MultiProject");

	return 0;
}
*/