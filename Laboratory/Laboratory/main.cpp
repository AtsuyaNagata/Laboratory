/*#include <iostream>
#include <boost/format.hpp>
using namespace std;

int main()
{
	// %1%����Ԗڂ̃p�����[�^�A%2%����Ԗڂ̂ɒu�������܂��B
	cout <<
		boost::format("%1% %2%") % "hello" % 100
		<< endl;

	// printf�Ɠ���������OK�B
	// %05d:0���߂�5���̐��� %x:16�i�\�L %f:���������_�� %%:%���̂���
	cout <<
		boost::format("%05d %x %f %%") % 200 % 255 % 3.33
		<< endl;

	// ����ɁA%2%��%1%���t���Ɏg�����Ƃ�
	cout <<
		boost::format("%2% %1%") % "hello" % 100
		<< endl;

	// %N$...�ŏ��Ԏw��Əo�͌`���w��̍��킹�Z
	// %04x:0���߂�4����16�i���� %s:������
	cout <<
		boost::format("%2$04x %1$s") % "hello" % 254
		<< endl;

	return 0;
}*/

/*
//�֐��|�C���^
#include<iostream>
#include"functionPtr.h"
using namespace std;


int main()
{
	check_return_funcptr();

	return 0;
}
*/

/*�}���`�v���W�F�N�g
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

/*
//SQLmodoki
#include"SQLmodoki.h"
int main()
{
	SQLmodoki();
}
*/


#include "auto.h"
#include <iostream>
using namespace std;
int main()
{
	printType();
}
