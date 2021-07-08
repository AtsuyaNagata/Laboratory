#include "RhythmPattern.h"
#include <algorithm>
#include <ctime>

RhythmPattern::RhythmPattern():
	mRhythm(0)
{
	std::srand(std::time(nullptr));						//���������Z�b�g
}

RhythmPattern::~RhythmPattern() {
	for (int i = 0; i < mRhythm.size(); ++i) {
		mRhythm[i].position.clear();
	}
}

//�����f�B�̃��Y���p�^�[���̐����Ȃ̂ō�郊�Y���͈����
void RhythmPattern::createMelodyRhythm(int size, int mostSmall) {
	//���łɂ���v�f���폜
	for (int i = 0; i < mRhythm.size(); ++i) {
		mRhythm[i].position.clear();
	}
	mRhythm[0].type = RhythmType::Melody;					//�����f�B�̃��Y���ł��邱�Ƃ������Ă���
	createSplitRhythmPattern_melody(size, mostSmall);
	vector<Position>& positions = mRhythm[0].position;
	for (int i = 0; i < positions.size(); ++i) {
		//�K���ɍ폜�������s���āA�x�������
		if ((rand() - 1) * 100 / RAND_MAX < 20) {
			//�Ō���̗v�f�ƌ������ă|�b�v����(�������̕�����������)
			std::swap(positions[i], positions.back());
			positions.pop_back();
		}
	}
	//�J�n���_�̔�r�Ń��Y���p�^�[���̃\�[�g���s��
	std::sort(positions.begin(), positions.end(), [](const Position& a, const Position& b) {
		return a.startTime < b.startTime;
	});
}

//�ċA�I�Ƀ��Y���𐶐����鏈��
void RhythmPattern::createSplitRhythmPattern_melody(int size, int mostSmall){
	Position position;
	if (size == mostSmall) {
		const vector<Position>& rPosition = mRhythm[0].position;			//��O�̈ʒu���
		position.length = size;
		position.startTime = rPosition[rPosition.size() - 1].startTime + rPosition[rPosition.size() - 1].length;
		mRhythm[0].position.push_back(position);
		return;
	}
	//�m���v�Z���Ó����v����->���������Ó�
	else if ((rand() - 1) * 100 / RAND_MAX < 100 * mostSmall * mostSmall / (size * size)) {		//���m���ōċA�ɓ��炸�ɏI��������B�ċA���[���قǏI�����₷���Ȃ�悤�Ȋm���ɂ���
		const vector<Position>& rPosition = mRhythm[0].position;			//��O�̈ʒu���
		position.length = size;
		position.startTime = rPosition[rPosition.size() - 1].startTime + rPosition[rPosition.size() - 1].length;
		mRhythm[0].position.push_back(position);
		return;
	}

	//�����ɕ������Ă��ꂼ����s�I
	createSplitRhythmPattern_melody(size / 2, mostSmall);
	createSplitRhythmPattern_melody(size / 2, mostSmall);
}

void RhythmPattern::createDrumRhythm(DrumPattern drumPattern, uint32_t length, bool fillIn) {
	//���łɂ���v�f���폜
	for (int i = 0; i < mRhythm.size(); ++i) {
		mRhythm[i].position.clear();
	}
	int quarterNoteNum = length / 256;			//�������l�����������ŕ\��

	if (drumPattern == DrumPattern::None) {
		return;
	}

	//�h�����Z�b�g��\������l�ɁA���Y���p�^�[���̔z��ɑΉ�����y�������
	Rhythm rhythm;
	//�C���f�b�N�X0�F�o�X�h����
	rhythm.type = RhythmType::BassDrum;
	mRhythm.push_back(rhythm);
	//�C���f�b�N�X1�F�X�l�A�h����
	rhythm.type = RhythmType::SnareDrum;
	mRhythm.push_back(rhythm);
	//�C���f�b�N�X2�F�N���[�Y�h�n�C�n�b�g
	rhythm.type = RhythmType::CloseHiHatCymbal;
	mRhythm.push_back(rhythm);
	//�C���f�b�N�X3�F�I�[�v���n�C�n�b�g
	rhythm.type = RhythmType::OpenHiHatCymbal;
	mRhythm.push_back(rhythm);
	//�C���f�b�N�X4�F�^��
	rhythm.type = RhythmType::Tom;
	mRhythm.push_back(rhythm);
	//�C���f�b�N�X5�F�t���A�^��
	rhythm.type = RhythmType::FloorTom;
	mRhythm.push_back(rhythm);
	//�C���f�b�N�X6�F�N���b�V���V���o��
	rhythm.type = RhythmType::CrashCymbal;
	mRhythm.push_back(rhythm);
	//�C���f�b�N�X7�F���C�h�V���o��
	rhythm.type = RhythmType::RideCymbal;
	mRhythm.push_back(rhythm);
	Position position;								//�ꎞ�I�Ɉʒu����ێ����邽�߂̃o�b�t�@

	if (drumPattern == DrumPattern::BasicDownBeat8) {

		//8�r�[�g�̊�{�p�^�[�����\�z����
		for (int i = 0; i < quarterNoteNum * 2; ++i) {		//�����������Ƃɏ������s��
			//��������4�����ƂɃo�X�h�����̉���ݒu����
			if (i % 4 == 0) {
				position.startTime = i * 128;
				position.length = 256 / 8;					//������32�������ŌŒ�
				mRhythm[0].position.push_back(position);	//�ʒu����ǉ�
			}
			//��������4�����ƂɃX�l�A�h�����̉���ݒ肷��(�o�X�h���Ƃ�2���������Y����)
			if (i % 4 == 2) {
				position.startTime = i * 128;
				position.length = 256 / 8;					//������32�������ŌŒ�
				mRhythm[1].position.push_back(position);	//�ʒu����ǉ�
			}
			//�����[�v�N���[�Y�h�n�C�n�b�g������
			position.startTime = i * 128;
			position.length = 256 / 8;						//������32�������ŌŒ�
			mRhythm[2].position.push_back(position);		//�ʒu����ǉ�
		}
		//----------��������o���G�[�V������݂���----------
		//�o�X�h�����̃o���G�[�V����
		for (int i = 0; i < quarterNoteNum * 2; ++i){
			int randNum = (rand() - 1) * 100 / RAND_MAX;
			//8������2�ڂ̈ʒu�Ƀo�X�h�����̉���ݒu���邩�ǂ���
			if (i % 8 == 1) {
				if (randNum < 20) {
					position.startTime = i * 128;
					position.length = 256 / 8;
					mRhythm[0].position.push_back(position);
				}
			}
			//8������4�ڂ̈ʒu�Ƀo�X�h�����̉���ݒu���邩�ǂ���
			if (i % 8 == 3) {
				if (randNum < 20) {
					position.startTime = i * 128;
					position.length = 256 / 8;
					mRhythm[0].position.push_back(position);
					//�X�ɔ����̊m����8������5�Ԗڂ̃o�X�h�����폜���A�����6�Ԗڂ�8�������Ƀo�X�h��������
					if (randNum < 10) {
						for (int i = 0; i < mRhythm[0].position.size(); ++i) {
							if (mRhythm[0].position[i].startTime == (i + 1) * 128) {
								//�Ō���̗v�f�ƌ������ă|�b�v����(�������̕�����������)
								std::swap(mRhythm[0].position[i], mRhythm[0].position.back());
								mRhythm[0].position.pop_back();
							}
							position.startTime = (i + 2) * 128;
							position.length = 256 / 8;
							mRhythm[0].position.push_back(position);
						}
					}
				}
			}
		}

		//TODO�F�X�l�A

		//TODO�F�N���b�V���V���o��

		//�Ō�ɓZ�߂ă\�[�g�������s��
		for (int i = 0; i < mRhythm.size(); ++i) {
			std::sort(mRhythm[i].position.begin(), mRhythm[i].position.end(), [](const Position& a, const Position& b) {
				return a.startTime < b.startTime;
			});
		}
	}
	else if (drumPattern == DrumPattern::BasicUpBeat8) {

	}
	else if (drumPattern == DrumPattern::SlipBeat8) {

	}
	else if (drumPattern == DrumPattern::ShuffleBeat) {

	}
	else if (drumPattern == DrumPattern::HalfTimeShuffle) {

	}
}