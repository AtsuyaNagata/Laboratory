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


//SQLmodoki
/*#include"SQLmodoki.h"
int main()
{
	SQLmodoki();
}*/


/*
#include "auto.h"
#include <iostream>
using namespace std;
int main()
{
	printType();
}
*/

/*
#include <iostream>
using namespace std;

int main() {
	int num = 0;

	num += ++num + num++;
	printf("%d\n", num);
}
*/

/*
#include"enumSize.h"

int main()
{
	enumSize();
}
*/

#include"MIDI.h"

/*MIDI�ǂݍ��ݏ����m�F
int main()
{
	MIDI midi("forProgram.mid");

	printf("MaxTrackNum = %d\t Resolution = %d\n", midi.getMaxTrackNum(), midi.getResolution());

	MIDI::Track *evel = midi.getTrack(1);
	for (int i = 0; i < evel->eventnum; ++i) {
		//printf("%lx, ", (int)evel->event[i].data);
		for (int j = 0; j < evel->event[i].datasize; ++j) {
			printf("%x, ", evel->event[i].data[j]);
		}
		printf("\n");
	}
}*/

//MIDI�\���\�z�m�F
/*
int main(){
	MIDI midi{};

	MIDI::MIDINoteEvents noteEvent = new MIDI::MIDINoteEvent[10];
	for (int i = 0; i < 5; ++i) {
		noteEvent[2 * i].time = (2 * i) * 960;
		noteEvent[2 * i].noteNum = 0x3C + i;
		noteEvent[2 * i].velocity = 0x64;

		noteEvent[2 * i + 1].time = (2 * i + 1) * 960;
		noteEvent[2 * i + 1].noteNum = 0x3C + i;
		noteEvent[2 * i + 1].velocity = 0;
	}
	midi.createMIDI_oneTrack(noteEvent, 10);

	MIDI::Track* evel = midi.getTrack(0);
	for (int i = 0; i < evel->eventnum; ++i) {
		//printf("%lx, ", (int)evel->event[i].data);
		for (int j = 0; j < evel->event[i].datasize; ++j) {
			printf("%x, ", evel->event[i].data[j]);
		}
		printf("\n");
	}
}*/

/*
#include"OpenAL.h"

int main()
{
	playAL();

	return 0;
}
*/

/*
#include "lambda.h"
int main() {
	lambdaIndex();
}*/


#include"DeepL.h"
int main()
{

}