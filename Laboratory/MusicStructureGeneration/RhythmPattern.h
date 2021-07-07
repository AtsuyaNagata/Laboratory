#ifndef RHYTHM_PATTERN_H_2021_H_
#define RHYTHM_PATTERN_H_2021_H_
#include <cstdint>
#include <vector>
using namespace std;

//�����f�B��R�[�h�̃��Y���p�^�[���𐶐����Ă����N���X
class RhythmPattern
{
public:
	//�����̃��Y���v�f��\���\��(�����f�B��h�����̃p�^�[���ȂǂɎg���\��)
	typedef struct Rhythm {
		uint32_t length;			//���̒���(256�Ŏl�����������\������)
		uint32_t startTime;			//���̊J�n�n�_(256�Ŏl���������)
		bool on;					//�x�����������̔���
	} Rhythm;

	//�R���X�g���N�^
	RhythmPattern();
	~RhythmPattern();

	//�����A���S���Y���ɂ���Đ������郊�Y���p�^�[��
	void create(int size, int mostSmall);
	void createSplitRhythmPattern(int size, int mostSmall);		//�������F�������鉹�̒����̃T�C�Y�A�������F��������p�^�[���̍ŏ�����

	vector<Rhythm> getRhythmPattern() {
		return mRhythmPattern;
	}

private:
	vector<Rhythm> mRhythmPattern;								//�eRhythm�\����length�����ɕ���ł���C���[�W�Ń��Y���p�^�[����\������
};

#endif