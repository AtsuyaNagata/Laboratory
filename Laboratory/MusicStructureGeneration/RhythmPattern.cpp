#include "RhythmPattern.h"
#include <algorithm>
#include <ctime>

RhythmPattern::RhythmPattern():
	mRhythmPattern(0)
{
	std::srand(std::time(nullptr));						//乱数をリセット
}

RhythmPattern::~RhythmPattern() {
	mRhythmPattern.clear();
}

void RhythmPattern::create(int size, int mostSmall) {
	mRhythmPattern.clear();
	createSplitRhythmPattern(size, mostSmall);
}

//再帰的に生成
void RhythmPattern::createSplitRhythmPattern(int size, int mostSmall){
	Rhythm rhythm;
	if (size == mostSmall) {
		rhythm.length = size;
		rhythm.on = true;
		if ((rand() - 1) * 100 / RAND_MAX < 20) {		//一定確率で休符にしておく
			rhythm.on = false;
		}
		mRhythmPattern.push_back(rhythm);
		return;
	}
	//確率計算が妥当か要検証->そこそこ妥当
	else if ((rand() - 1) * 100 / RAND_MAX < 100 * mostSmall * mostSmall / (size * size)) {		//一定確率で再帰に入らずに終了させる。再帰が深いほど終了しやすくなるような確率にする
		rhythm.length = size;
		rhythm.on = true;
		if ((rand() - 1) * 100 / RAND_MAX < 20) {		//一定確率で休符にしておく
			rhythm.on = false;
		}
		mRhythmPattern.push_back(rhythm);
		return;
	}

	//半分に分割してそれぞれ実行！
	createSplitRhythmPattern(size / 2, mostSmall);
	createSplitRhythmPattern(size / 2, mostSmall);
}
