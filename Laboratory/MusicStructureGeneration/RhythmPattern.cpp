#include "RhythmPattern.h"
#include <algorithm>
#include <ctime>

RhythmPattern::RhythmPattern():
	mRhythmPattern(0)
{
	std::srand(std::time(nullptr));						//���������Z�b�g
}

RhythmPattern::~RhythmPattern() {
	mRhythmPattern.clear();
}

void RhythmPattern::create(int size, int mostSmall) {
	mRhythmPattern.clear();
	createSplitRhythmPattern(size, mostSmall);
}

//�ċA�I�ɐ���
void RhythmPattern::createSplitRhythmPattern(int size, int mostSmall){
	Rhythm rhythm;
	if (size == mostSmall) {
		rhythm.length = size;
		rhythm.on = true;
		if ((rand() - 1) * 100 / RAND_MAX < 20) {		//���m���ŋx���ɂ��Ă���
			rhythm.on = false;
		}
		mRhythmPattern.push_back(rhythm);
		return;
	}
	//�m���v�Z���Ó����v����->���������Ó�
	else if ((rand() - 1) * 100 / RAND_MAX < 100 * mostSmall * mostSmall / (size * size)) {		//���m���ōċA�ɓ��炸�ɏI��������B�ċA���[���قǏI�����₷���Ȃ�悤�Ȋm���ɂ���
		rhythm.length = size;
		rhythm.on = true;
		if ((rand() - 1) * 100 / RAND_MAX < 20) {		//���m���ŋx���ɂ��Ă���
			rhythm.on = false;
		}
		mRhythmPattern.push_back(rhythm);
		return;
	}

	//�����ɕ������Ă��ꂼ����s�I
	createSplitRhythmPattern(size / 2, mostSmall);
	createSplitRhythmPattern(size / 2, mostSmall);
}
