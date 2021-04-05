/*#include <iostream>
#include <boost/format.hpp>
using namespace std;

int main()
{
	// %1%が一番目のパラメータ、%2%が二番目のに置き換わります。
	cout <<
		boost::format("%1% %2%") % "hello" % 100
		<< endl;

	// printfと同じ書式もOK。
	// %05d:0埋めで5桁の整数 %x:16進表記 %f:浮動小数点数 %%:%そのもの
	cout <<
		boost::format("%05d %x %f %%") % 200 % 255 % 3.33
		<< endl;

	// さらに、%2%と%1%を逆順に使うことも
	cout <<
		boost::format("%2% %1%") % "hello" % 100
		<< endl;

	// %N$...で順番指定と出力形式指定の合わせ技
	// %04x:0埋めで4桁の16進整数 %s:文字列
	cout <<
		boost::format("%2$04x %1$s") % "hello" % 254
		<< endl;

	return 0;
}*/

/*
//関数ポインタ
#include<iostream>
#include"functionPtr.h"
using namespace std;


int main()
{
	check_return_funcptr();

	return 0;
}
*/

/*マルチプロジェクト
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

/*MIDI読み込み処理確認
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

//MIDI構造構築確認
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