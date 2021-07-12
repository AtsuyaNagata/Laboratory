#include "Chords.h"
#include <algorithm>
#include <ctime>

Chords::Chords()
	: mFloor(nullptr),
	mBaseNoteNum(36)		//���K�̊J�n�n�_���h�̉��ɐݒ肷��
{
	mFloor = new int[7];
	std::srand(std::time(nullptr));						//���������Z�b�g
}

Chords::~Chords()
{
	mChords.clear();
	delete[] mFloor;
}

//���y���_����R�[�h�𐶐�����A���S���Y��
void Chords::create(int chordNum, uint32_t length, int key, Scale scale)
{
	//����������
	mChords.clear();

	//���͂Ŏ󂯎�������ߐ����Œᐔ�ȉ��Ȃ�A�Ȃɂ��������Ƃ�
	if (chordNum < 1) {
		return;
	}

	mLength = length;
	mChordNum = chordNum;

	//�X�P�[�����烁�C���Ŏg�p����7����ݒ肷�鏈��
	for (int i = 0; i < 7; ++i) { 
		mFloor[i] = 0;
	}			//�������̏����������Ă���
	if (scale == Scale::Major) {
		//�g�p����C���f�b�N�X��[�S�S���S�S�S��]�Ȃ̂ŁA0�A2�A4�A5�A7�A9�A11�ԂƂȂ�
		mFloor[0] = 0; mFloor[1] = 2; mFloor[2] = 4; mFloor[3] = 5; mFloor[4] = 7; mFloor[5] = 9; mFloor[6] = 11;
	}
	//�}�C�i�[�X�P�[���̎�
	else if (scale == Scale::Minor) {
		//�g�p����C���f�b�N�X��[�S���S�S���S�S]�Ȃ̂ŁA0�A2�A3�A5�A7�A8�A10�ԂƂȂ�
		mFloor[0] = 0; mFloor[1] = 2; mFloor[2] = 3; mFloor[3] = 5; mFloor[4] = 7; mFloor[5] = 8; mFloor[6] = 10;
	}
	//�n�[���j�b�N�E�}�C�i�[�X�P�[���̎�
	else if (scale == Scale::HarmonicMinor) {
		//�g�p����C���f�b�N�X�́A0�A2�A3�A5�A7�A8�A11�ԂƂȂ�
		mFloor[0] = 0; mFloor[1] = 2; mFloor[2] = 3; mFloor[3] = 5; mFloor[4] = 7; mFloor[5] = 9; mFloor[6] = 11;
	}
	//�����f�B�b�N�}�C�i�[�X�P�[���̎�
	else if (scale == Scale::MelodicMinor) {
		//�g�p����C���f�b�N�X��[�S�S���S�S�S��]�Ȃ̂ŁA0�A2�A4�A5�A7�A9�A11�ԂƂȂ�
		mFloor[0] = 0; mFloor[1] = 2; mFloor[2] = 3; mFloor[3] = 5; mFloor[4] = 7; mFloor[5] = 9; mFloor[6] = 11;
	}
	//�L�[�̍������l������
	for (int i = 0; i < 7; ++i) {
		mFloor[i] = (mFloor[i] + key)/* % 12*/;		//�I�N�^�[�u�𒴂���\�����l������
	}

	Chord chord;										//�R�[�h���i�[����\���̃C���X�^���X��
	//�R�[�h�̊J�n�n�_�͌ʂɏ���
	//�X�ƇZ����J�n����R�[�h�͂���܂�Ȃ�
	//�T����ԑ���
	//���ɇW������
	//����Ɏ��ɇY�x�������A�U�A�V�̓��A
	int randNum = (rand() - 1) * 100 / RAND_MAX;		//�����̒l�͈̔͂�0�`100
	//�n�܂�̃R�[�h�𐶐����鏈��
	if (randNum < 30) {									//��������R�[�h���T�̎��̏���
		chord = makeSimpleChord(0);						//�x�[�X�̃C���f�b�N�X�l�������ɗ^����
	}
	else if (randNum < 60) {							//��������R�[�h���W�̎��̏���
		chord = makeSimpleChord(3);
	}
	else if (randNum < 90) {							//��������R�[�h���Y�̎��̏���
		chord = makeSimpleChord(5);
	}
	else if (randNum < 95) {							//��������R�[�h���Y�̎��̏���

		chord = makeSimpleChord(1);
	}
	else{												//��������R�[�h���V�̎��̏���
		chord = makeSimpleChord(2);
	}
	//�ŏ��̃R�[�h���R�[�h��Ƀv�b�V������
	mChords.push_back(chord);

	for(int i = 0; i < chordNum - 1; ++i) {				//�J�n���_�Ɉ�ǉ������R�[�h���������Đ������[�v���s��
		chord = makeChordFromPrev(chord);				//�O��̃R�[�h�̒l���玟�̃R�[�h�𐶐�����
		mChords.push_back(chord);						//���������R�[�h���R�[�h��̍Ō���ɒǉ�
	}
}

//7CM���_�ŃR�[�h�𐶐�����A���S���Y��
void Chords::create7CM(int barNum)
{

}

Chords::Chord Chords::makeSimpleChord(int baseIndex)
{
	Chord chord;										//�R�[�h�ɂ��Ă̏����i�[����\��
	//�ُ�ȓ��͂�������ُ�l�ŕԂ�
	if (baseIndex < 0 || baseIndex >= 7) {
		chord.baseNoteNum = -1;
		chord.baseIndex = -1;
		chord.omitIndex = -1;
		chord.onIndex = -1;
		return chord;
	}

	//is��Internal sound(����)�̗�
	int is1 = mFloor[baseIndex % 7];
	int is2 = mFloor[(baseIndex + 2) % 7];
	int is3 = mFloor[(baseIndex + 4) % 7];
	//����������2�Ԗڂ̓�����1�ԉ��̓�����菬������΁A�I�N�^�[�u����グ��
	if (is1 > is2) {
		is2 += 12;
	}
	//����������3�Ԗڂ̓�����2�ԉ��̓�����菬������΁A�I�N�^�[�u����グ��
	if (is2 > is3) {
		is3 += 12;
	}

	//3�a���̉��ɕK�v�ȃp�����[�^��ݒ肷��
	chord.baseNoteNum = mBaseNoteNum + is1;
	chord.baseIndex = mFloor[baseIndex];
	chord.onIndex = mFloor[baseIndex];					//baseIndex�ƒl����v���Ă�����onIndex�͍l���Ȃ��Ƃ�����
	chord.omitIndex = 12;								//12��Omit�𖳌�������l
	chord.type = specificChord(is1, is2, is3);			//��������R�[�h�̃^�C�v�𔻕ʂ���
	chord.plus = ChordPlus::None;						//�t�����͈�U��������Ȃ�
	chord.tension = ChordTension::None;					//��ɓ���

	chord.length = mLength / mChordNum;					//�����͋ϓ��ɔz������`�Ō��肵�Ă���

	return chord;
}

Chords::Chord Chords::makeChordFromPrev(Chord chord) {
	Chord createChord;									//��������R�[�h���i�[�����ꏊ
	int randNum = (rand() - 1) * 100 / RAND_MAX;
	//TODO:�O��̃R�[�h���h�~�i���g���[�V�������������̏���
	
	//�O�̃R�[�h���T���������̏���
	if (chord.baseIndex == mFloor[0]) {
		//�W�ɐi�s
		if (randNum < 50) {
			createChord = makeSimpleChord(3);
		}
		//�X�ɐi�s
		else if (randNum < 80) {
			createChord = makeSimpleChord(4);
		}
		else if (randNum < 90) {
			createChord = makeSimpleChord(1);
		}
		//�V(�Vb)�ɐi�s
		else {
			createChord = makeSimpleChord(2);
			//TODO:���W���[�R�[�h�̎��A����ɇV��ɐi�s����p�^�[�����l������
		}
	}
	//�O�̃R�[�h���U���������̏���
	else if (chord.baseIndex == mFloor[1]) {
		//�V�ɐi�s
		if (randNum < 20) {
			createChord = makeSimpleChord(2);
		}
		//�X�ɐi�s
		else {
			createChord = makeSimpleChord(4);
		}
	}
	//�O�̃R�[�h���V���������̏���
	else if (chord.baseIndex == mFloor[2]) {
		//�Y�ɐi�s
		if (randNum < 60) {
			createChord = makeSimpleChord(5);
		}
		//�W�ɐi�s
		else {
			createChord = makeSimpleChord(3);
		}
	}
	//�O�̃R�[�h���W���������̏���
	else if (chord.baseIndex == mFloor[3]) {
		//�X�ɐi�s
		if (randNum < 50) {
			createChord = makeSimpleChord(4);
		}
		//�T�ɐi�s
		else {
			createChord = makeSimpleChord(0);
		}
	}
	//�O�̃R�[�h���X���������̏���
	else if (chord.baseIndex == mFloor[4]) {
		//�T�ɐi�s
		if (randNum < 80) {
			createChord = makeSimpleChord(0);
		}
		//�V�ɐi�s
		else {
			createChord = makeSimpleChord(2);
			if (randNum < 90) {
				createChord.type = ChordType::Major;
			}
			else {
				createChord.type = ChordType::Minor;
			}
		}
	}
	//�O�̃R�[�h���Y���������̏���
	else if (chord.baseIndex == mFloor[5]) {
		//�W�ɐi�s
		if (randNum < 30) {
			createChord = makeSimpleChord(3);
		}
		//�X�ɐi�s
		else if (randNum < 50) {
			createChord = makeSimpleChord(4);
		}
		//�U�ɐi�s
		else if (randNum < 70) {
			createChord = makeSimpleChord(1);
		}
		//�V�ɐi�s
		else {
			createChord = makeSimpleChord(2);
		}
	}
	//�O�̃R�[�h���Z���������̏���
	else if (chord.baseIndex == mFloor[6]) {
		createChord = makeSimpleChord((rand() - 1) * 7 / RAND_MAX);		//�Z�̐i�s�͂悭�킩��񂩂痐���Ō��߂Ă�
	}
	else {
		//�o�O�Ȃ̂łȂɂ����Ȃ��ł���
	}

	return createChord;
}

//�R�[�h�����������肷�鏈��
Chords::ChordType Chords::specificChord(int is1, int is2, int is3)		//is��Internal sound�̗�
{
	ChordType type = ChordType::None;
	if (is2 - is1 == 5) {					//��ꉹ�Ƃ��̎��̓�����5�x���Ȃ�
		if (is3 - is2 == 2) {				//�����2�ڂ�3�ڂ̓�����2�x���Ȃ�
			type == ChordType::Sus4;		//sus4�̃R�[�h�Ŋm�肷��
		}
	}
	else if (is2 - is1 == 4) {				//��ꉹ�Ƃ��̎��̓�����4�x���Ȃ�
		if (is3 - is2 == 3) {				//�����2�ڂ�3�ڂ̓�����3�x���Ȃ�
			type = ChordType::Major;		//���W���[�R�[�h�Ŋm�肷��
		}
		else if (is3 - is2 == 4) {			//2�ڂ�3�ڂ̓�����4�x���Ȃ�
			type = ChordType::Aug;			//Aug�R�[�h�Ŋm�肷��
		}
	}
	else if (is2 - is1 == 3) {
		if (is3 - is2 == 4) {
			type = ChordType::Minor;		//�}�C�i�[�R�[�h
		}
		if (is3 - is2 == 3) {
			type = ChordType::mb5;			//�}�C�i�[��t�@�C�u
		}
	}
	else if (is2 - is1 == 2) {
		if (is3 - is2 == 5) {
			type = ChordType::Sus2;			//sus2�̃R�[�h
		}
	}

	return type;
}