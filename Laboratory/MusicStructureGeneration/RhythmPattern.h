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

		Melody,					//�����f�B�p
		BassDrum,				//�o�X�h������GeneralMIDI�ԍ��F35->Acoustic Bass Drum, 36->Bass Drum 1
		SnareDrum,				//�X�l�A�h������GeneralMIDI�ԍ��F37->SideStick, 38->AcousticSnare, 39->HandCrap, 40->Electric Snare
		FloorTom,				//�t���A�^����GeneralMIDI�ԍ��F41->Low Floor Tom, 43->High Floor Tom
		Tom,					//�^����GeneralMIDI�ԍ��F45->Low Tom, 47->Low Mid Tom, 48->High Mid Tom, 50->High Tom
		CloseHiHatCymbal,		//�N���[�Y�n�C�n�b�g��GeneralMIDI�ԍ��F42->Closed Hi-Hat, ?44->Pedal Hi-Hat
		OpenHiHatCymbal,		//�I�[�v���n�C�n�b�g��GeneralMIDI�ԍ��F46->Open Hi-Hat
		Cymbal,					//�V���o����GeneralMIDI�ԍ��F49->Crash Cymbal 1, 51->Ride Cymbal 1, 52->Chinese Cymbal, 53->Ride Bell, 55->Splash Cymbal

		//�x����GeneralMIDI�ԍ��F54->Tambourine, 56->Cowbell, 57->Crash Cymbal 2, 58->Vibraslap, 59->Ride Cymbal 2
		//�p�[�J�b�V��������GeneralMIDI�ԍ��F60�`81
	};

	//�����̃��Y���v�f�̈ʒu��\���\��(�����f�B��h�����̃p�^�[���ȂǂɎg���\��)
	typedef struct Position {
		uint32_t length;			//���̒���(256�Ŏl�����������\������)
		uint32_t startTime;			//���̊J�n�n�_(256�Ŏl���������)
	}Position;

	typedef struct Rhythm {
		RhythmType type;				//���Y���̎��
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