#ifndef RHYTHM_PATTERN_H_2021_H_
#define RHYTHM_PATTERN_H_2021_H_
#include <cstdint>
#include <vector>
using namespace std;

//�����f�B��R�[�h�̃��Y���p�^�[���𐶐����Ă����N���X
class RhythmPattern
{
public:
	enum class DrumPattern : unsigned char {
		None,

		BasicDownBeat8,			//��{8�r�[�g
		BasicUpBeat8,			//��{8�r�[�g���ł�(�悭�T�r�ɗp������)
		SlipBeat8,				//�X���b�v8�r�[�g(�X�l�A��8���������ɂ������肷�郊�Y���p�^�[��)
		ShuffleBeat,			//�V���b�t���r�[�g(4���~3��3���~4�̂悤�ɍl���郊�Y���p�^�[���B2���ڂ�4���ڂɃo�b�N�r�[�g(�X�l�A�̋�)������)
		HalfTimeShuffle,		//�n�[�t�^�C���V���b�t���r�[�g(�`�^�` �`�^�` �^�^�` �`�^�` �̃��Y���p�^�[��)[�n�[�t�^�C���^�C����3���ڂɃo�b�N�r�[�g������Ӗ�����]
	};

	enum class RhythmType : unsigned char {
		None,

		Melody,
		BassDrum,
		CloseHiHatCymbal,
		OpenHiHatCymbal,
		SnareDrum,
		Tom,
		FloorTom,
		CrashCymbal,
		RideCymbal
	};

	//�����̃��Y���v�f�̈ʒu��\���\��(�����f�B��h�����̃p�^�[���ȂǂɎg���\��)
	typedef struct Position {
		uint32_t length;			//���̒���(256�Ŏl�����������\������)
		uint32_t startTime;			//���̊J�n�n�_(256�Ŏl���������)
	}Position;

	typedef struct Rhythm {
		RhythmType type;			//���Y���̎��
		vector<Position> position;		//���Y���̈ʒu�̏W��
	} Rhythm;

	//�R���X�g���N�^
	RhythmPattern();
	~RhythmPattern();

	
	void createMelodyRhythm(int size, int mostSmall);									//���݂͕����A���S���Y���ɂ���Đ������郁���f�B���Y���p�^�[�������삵�Ă�
	void createDrumRhythm(DrumPattern drumPattern, uint32_t length, bool fillIn);		//�h�����̃p�^�[���𐶐�����֐�

	vector<Rhythm> getRhythmPattern() const {
		return mRhythm;
	}

private:
	void createSplitRhythmPattern_melody(int size, int mostSmall);		//�������F�������鉹�̒����̃T�C�Y�A�������F��������p�^�[���̍ŏ�����

	vector<Rhythm> mRhythm;								//�eRhythm�\����length�����ɕ���ł���C���[�W�Ń��Y���p�^�[����\������
};

#endif