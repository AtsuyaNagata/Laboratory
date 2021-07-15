#ifndef CHORDS_H_2021_05_06_
#define CHORDS_H_2021_05_06_

#include <cstdint>
#include <vector>
using namespace std;

//�R�[�h��𐶐�����I�u�W�F�N�g�N���X
class Chords
{
public:
	enum class Algorithm : unsigned char {
		Simple,
		Fashionable,
		modulation
	};

	enum class Scale : unsigned char {
		None,

		Major,
		Minor,
		HarmonicMinor,
		MelodicMinor
	};

	//----------�R�[�h�̎�ނ̎��ʎq----------
	//��{�I��3�a���ɂ��R�[�h�̎��
	enum class ChordType : unsigned char {
		None,							//�u�R�[�h�Ȃ��v��\������(���݂͔������Ȃ��͂�)

		Major,							//���W���[�R�[�h��\��
		Minor,							//�}�C�i�[�R�[�h��\��
		Sus4,							//3�x�̉����グ�Ċ��S�l�x(P4)�ɂ���(��ꉹ����5��)
		Sus2,							//3�x�̉��������Ē���x(M2)�ɂ���(��ꉹ����2��)
		Aug,							//���W���[�R�[�h�̊��S5�x(P5)�𔼉��グ��(��ꉹ����8��)->�N���V�F(�ꉹ�����ς��Ă����i�s)�Ɏg��
		mb5,							//�}�C�i�[�t���b�g�t�@�C�u��\��(�}�C�i�[�R�[�h�̊��S5�x�𔼉���������)
	};

	//�R�[�h�̎�ނ̂����A4���ڂ�p���鎞�̕t������\��
	enum class ChordPlus : unsigned char {
		None,							//�t���Ȃ�

		Seventh,						//�Z�u���X(�Z7�x)
		MajorSeventh,					//���W���[�Z�u���X(��7�x)
		Six,							//��6�x�̉���t������(�}�C�i�[���ۂ����͋C��������)
		Add9,							//9�x(2�x)�̉���������
		Add11,							//11�x(4�x)�̉���������
		Add13,							//13�x(6�x)�̉���������
		Dim7,							//m��5�̂Ƃ��̂ݕt���\�A7th�𔼉�����������������
	};

	//5�ڂ̉���t�����鎞�̃p�^�[��
	enum class ChordTension : unsigned char {
		None,							//�t���Ȃ�

		Ninth,							//9th	(��9�x)
		FlatNinth,						//��9th	(�Z9�x)
		SharpNinth,						//#9th	(��9�x[�Z10�x])
		Eleventh,						//11th	(���S11�x)
		SharpEleventh,					//#11th	(��11�x)
		Thirteenth,						//13th	(��13�x)
		FlatThirteenth,					//��13th(�Z13�x)
	};
	//---------------------------------------

	//================7CM���_================
	/*
	//����\�������񋓎q
	enum class Bundle : unsigned char {

	};
	*/
	/*typedef struct SevenCM {
	* uint8_t simpleFrequencies;		//�ȈՓx����(0�`6��7�̉���\������)
	}SevenCM;
	*/

	typedef struct Chord {
		ChordType type;					//3�a���̃R�[�h�̎��
		ChordPlus plus;					//4�̉���p����R�[�h�̕t������\��
		ChordTension tension;			//5�̉���p����Ƃ��̕t������\��
		uint8_t baseNoteNum;			//���̉�(�I���R�[�h�͈�U�l�����A���̃R�[�h�̊�ꉹ���w�� ��F�L�[��C�̎���C/G -> 60 + 4 = 64�łȂ� 60 + 0 = 60[G�łȂ�C])
		uint8_t baseIndex;				//�L�[���牽�Ԗڂ����ƂȂ��Ă��邩��\��(�l�͈̔́F0�`11)
		uint8_t onIndex;				//�I���R�[�h (��ꉹ���Ⴄ�R�[�h ��:C/G �x�[�X���C������ɏ㏸�����������ȂǂɎg��) �̏ꍇ�Ɋ��ƂȂ鉹���w�肷�邽�߂̃C���f�b�N�X�l(�l�͈̔́F0�`11)
		uint8_t omitIndex;				//Omit�R�[�h (�R�[�h���̓���̉����ꉹ�������R�[�h) (0�`12 12�ȏ�̒l�͖����Ȓl��\�����Ƃɂ���)
		uint32_t length;				//�R�[�h�̒���(�l����������F256)
	}Chord;

	//�R���X�g���N�^
	Chords();
	//�f�X�g���N�^
	~Chords();

	//�R�[�h�𐶐�����֐�
	void create(int chordNum, uint32_t length, int key, Scale scale, Algorithm algorithm);			//��������R�[�h�̐��A��������R�[�h�̒���(256�Ŏl����������̒���)�A�L�[�A�X�P�[���������Ɏ��
	void create7CM(int barNum);										//7CM���_��p���ăR�[�h�𐶐�����A���S���Y��

	const vector<Chord> getChords() const {
		return mChords;
	}
	const int* getFloor() const {
		return mFloor;
	}
	const int getBaseNoteNum() const {
		return mBaseNoteNum;
	}
	const uint32_t getLength() const {
		return mLength;
	}
	

private:
	//�����̍ۂɕK�v�ȃ����o
	vector<Chord> mChords;				//�R�[�h���\������z��
	Scale mScale;
	int* mFloor;						//�K����\������z�񂪊i�[�����(�T�C�Y��7�ŌŒ�)
	int mBaseNoteNum;					//��������R�[�h�̊�ƂȂ�I�N�^�[�u��C�̍������i�[�����
	int mChordNum;						//��������R�[�h�̐����i�[����(mChords����������O�ɕK�v�Ȃ��̂Ƃ��Đ錾���ꂽ�����o)
	uint32_t mLength;					//�R�[�h��S�̂̒������i�[����
	Algorithm mAlgorithm;				//�R�[�h�̐����A���S���Y�������肷�邽�߂̗񋓎q

	//�R�[�h�̐����ɐ��ƕ��̌X����\���l��݂��Ă݂�Ƃǂ��Ȃ�H

	//�A���S���Y���ɗp����֗��Ȋ֐��Q
	Chord makeSimpleChord(int baseIndex);									//�R�[�h�̍\���ɃV���v���Ȓl���i�[���鏈��
	Chord makeChordFromPrev(Chord chord);									//�O�̃R�[�h���玟�̃R�[�h�𐶐�����֐�
	vector<Chord> changeChordFashionable(vector<Chord> chords);				//�R�[�h����������ȃR�[�h�i�s�ɕϊ����ĕԂ�(�����o�Ȃ̂ň����Ƃ���mChords���󂯎��K�v�͂Ȃ����A�ꉞ�̊g�������ӎ����Ĉ����Ƃ��Ď󂯎��\���ɂ��Ă�)
	ChordType specificChord(int is1, int is2, int is3);						//����3����R�[�h�̎�ނ���肷��֐�
	vector<Chord> shiftChordRight(vector<Chord> chords, int idx, int n);		//�w�肵���C���f�b�N�X�ȍ~�̗v�f��n���V�t�g�����v�f��Ԃ�
};

#endif
