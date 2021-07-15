#ifndef MIDI_H_2019_12_19_
#define MIDI_H_2019_12_19_

#include"File.h"
#include<stdio.h>
#include <cstdint>
#include <vector>

class MIDI {
public:
	//��������Ȃ�MIDI�t�@�C���̉�͏�������
	MIDI(const char* filename);
	//�������Ȃ���΋���ۂ�MIDI�N���X�����
	MIDI();
	//�j�󏈗�
	~MIDI();

	enum class EVENT : unsigned char {
		//���^�C�x���g				//FF�b�C�x���g�^�C�v�b�f�[�^���b�f�[�^
		//SEQUENCE_NUMBER,			//FF 00 02 �V�[�P���X�ԍ��A�t�H�[�}�b�g0��1(�t�H�[�}�b�g0�ŌŒ肷��̂ŃR�����g�A�E�g)
		COMMENT,					//FF 01 len data�@�e�L�X�g���������ނ��߂̃C�x���g
		COPY_RIGHT,					//FF 02 len data�@���쌠�������Ă���
		SEQUENCE_TRACK_NAME,		//FF 03 len data�@�V�[�P���X��(�Ȗ�)�E�g���b�N��
		INSTRUMENT_NAME,			//FF 04 len data�@�y�햼
		LYRIC,						//FF 05 len data�@lyric�͉̎��̎�
		MARKER,						//FF 06 len data�@���n�[�T���L����Z�N�V�������̂悤�ȁA�V�[�P���X�̂��̎��_�̖��̂��L�q����
		QUEUE_POINT,				//FF 07 len data�@�ȃf�[�^���A���̃C�x���g���}������Ă���ʒu�ŁA�ȈȊO�̐i�s���L�q����B(��ʂ̃t�F�[�h�A�E�g�̂悤�ȓ��擙�ɑ΂���i�s)
		PROGRAM_NAME,				//FF 08 len data�@�v���O�����`�F���W�ƃo���N�`�F���W�ŕ\���Ă��鉹�F�����L�ڂ���iGeneral MIDI�����肳���O�̈╨�j
		DEVICE_NAME,				//FF 09 len data�@���̃C�x���g������g���b�N���ǂ̃f�o�C�X�ɔz�u�����̂��Ƃ������Ƃ������B���̃C�x���g�́A1�g���b�N�ɂ�i�x�����A�ŏ��ɌĂ΂��ׂ�����
		CHANNEL_PREFIX,				//FF 20 01 cc MIDI�C�x���g����Ȃ��̂�(SysEx or ���^�C�x���g)MIDI�`�����l�����w�肷��ۂɗp����
		PORT_SPECIFIED,				//FF 21 01 pp �o�̓|�[�g�̎w��	0�`3�����ꂼ��|�[�g��1�`4�ɑΉ�����(��O�͂��邪�A�f�[�^�݂̂ł͔��ʕs��)
		END_OF_TRACK,				//FF 2F 00�@�@�g���b�N�̍Ō�ł��邱�Ƃ�\��
		SET_TEMPO,					//FF 51 03 tt tt tt�@tttttt(3byte)�ɂ́A4�������̒������ʕb�ŕ\�������̂��i�[����BBPM = 120�̎��A4�������̒����� 60 x 10^6 / 120 = 500000[��s]
		TIME_SIGNATURE,				//FF 58 04 nn dd cc bb | ���q nn = ���q dd = ����(2^-dd�ŕ\�����->dd=2�̎��A����=4) cc = ���g���m�[���Ԋu[�l�������Ԋu�Ȃ�18H] bb = �l������������̎O�\�񕪉����̐�
		KEY_SIGNATURE,				//FF 59 02 sf ml �L�[(��)������	sf = ��:�V���[�v�̐� or ��:�t���b�g�̐�		ml = 0:����(���W���[) 1:�Z��(�}�C�i�[)
		//SPECIAL_META_EVENT,		//FF 7F len data�@�ŏ���1�o�C�g�̓��[�JID�ŁA���̌�ɓƎ��t�H�[�}�b�g�̃f�[�^������(�Ǝ��̃t�H�[�}�b�g�܂ŋC���񂷂���͂Ȃ��̂ŃR�����g�A�E�g)
		FF_NONE,					//����������Ȃ��������A���������

		//�V�X�e���G�N�X�N���V�[�u���b�Z�[�W���i�[����C�x���g�B�X�̊y�탁�[�J���Ǝ��ɍ쐬�������b�Z�[�W���i�[����BMIDI�f�o�C�X�ɉ�������ݒ肷��̂ɗ��p�����
		SYSEX,						//F0 �f�[�^�� �G�N�X�N���V�[�u���b�Z�[�W F7
		F7_SYSEX,					//F7 �f�[�^�� �G�N�X�N���V�[�u���b�Z�[�W

		//MIDI�C�x���g
		POLYPHONIC_KEY_PRESSURE,	//An kk vv n�̃`�����l����kk�Ԗڂ̃m�[�g�Ƀx���V�e�Bvv�̃v���b�V���[����^����
		CONTROL,					//Bn cc vv �R���g���[���ݒ�B�`�����l��n�ŃR���g���[���i���o�[cc��vv�̒l�𑗂�
		PROGRAM_CHANGE,				//Cn pp �v���O�����`�F���W�B�`�����l��n�ŁA���F��pp�ɕύX����
		CHANNEL_PRESSURE,			//Dn vv �`�����l��n�ɑ΂��ăv���b�V���[���vv�𑗐M����B
		PITCH_BENT,					//En mm ll �s�b�`�x���g�B�Ȃ������g���G���f�B�A���炵��
		NOTE_ON,					//9n nn vv �m�[�g�E�I�� nn�̓m�[�g�ԍ� vv�̓x���V�e�B
		NOTE_OFF					//8n nn vv �m�[�g�E�I�t nn�̓m�[�g�ԍ� vv�̓x���V�e�B
	};

	//Event�̍\��
	typedef struct Event {
		EVENT type;					//���̃C�x���g�����i�[����
		size_t datasize;			//���ꂼ��̃C�x���g�̃f�[�^�T�C�Y���i�[����
		const unsigned char* data;	//�C�x���g�̃f�[�^���i�[����
		size_t time;				//�f���^�^�C�����i�[����i�f���^�^�C���͑O�̃C�x���g����̑��Ύ��ԁj
	} Event;
	typedef struct Track {
		Event* event;				//�C�x���g�̍\����z��œ����
		size_t eventnum;			//�C�x���g�\���̔z��̃T�C�Y������
		size_t tracksize;			//midi�t�@�C���̂��ꂼ��̃g���b�N�`�����N�̃w�b�_�����ɂ���g���b�N�f�[�^�T�C�Y���i�[����ꏊ
	} Track;

	//������
	short getMaxTrackNum() {
		return mMaxTrackNum;
	}
	short getResolution() {
		return mResolution;
	}
	//����g���b�N�̏���Z�߂Ċl���I
	Track* getTrack(int n) {
		return &mTrack[n];
	}

	//�������ݗp��MIDI�C�x���g��p�̃f�[�^�\����錾����
	typedef struct MIDINoteEvent {
		size_t time;			//960���ƂɎl����������オ��ƍl���Ă�
		uint8_t noteNum;		//���̍���
		uint8_t velocity;		//���̑傫��
	}MIDINoteEvent, *MIDINoteEvents;
	//MIDI�f�[�^���\�z����֐��B��̃g���b�N�Ŏ��Ԃ̕���\��960�ŌŒ�B�����͓��͂�����MIDI�C�x���g�p�\���̔z��Ƃ��̍ő�T�C�Y�B
	int createMIDI_oneTrack(MIDINoteEvents midiEvents, int noteSize, int bmp);
	int createMIDI_oneTrack(std::vector<MIDINoteEvent> midiEvents, int noteSize, int bmp);

	//������MIDI�t�@�C���������݂̃��C��
	int MIDIWrite(const char* filename);

private:		//============private�̊J�n�n�_====================
	//�v�f��j�󂷂�֐��̓N���X���g�̂݌Ăяo������̂Ƃ���
	void destroy() {
		for (int i = 0; i < mMaxTrackNum; ++i) {
			delete[] mTrack[i].event;
			mTrack[i].event = nullptr;
		}
		delete[] mTrack; mTrack = nullptr;
		delete[] mMemoryMap; mMemoryMap = nullptr;
		delete mFile; mFile = nullptr;
		mMaxTrackNum = 0;
		mResolution = 0;
	}
	File* mFile;

	short mMaxTrackNum;		//�g���b�N�̐�
	short mResolution;		//�l�������������̕���\�̒l������(480�������Ă�����f���^�^�C��480�Ŏl������������)

	uint8_t* mMemoryMap;	//�������񂾃f�[�^���i�[���Ă����ꏊ

	Track* mTrack;			//Track�̍\���F

	//========================MIDI�������݂̏����ŗp���鏈���Q==============================
	//�ϐ��̓��g���G���f�B�A���Ŋi�[����Ă���̂ŁA�r�b�N�G���f�B�A���ɒ����֐����K�v(���I�ȗ̈����鏈���Ȃ̂łȂ邾���Ăяo�����͌��炷����)
	void reverceByte(void* data, unsigned char datasize);

	//�f���^�^�C���̏������ݏ���(4�o�C�g��������ƌ��܂��Ă�̂ŁA�g�����͈ӎ����Ȃ��Ă��悢�h��)
	void DeltaTimeWrite(FILE* midiFile, size_t time);

	//�C�x���g�ɉ����ď������ރo�C�i����ς���
	void EventWrite(FILE* midiFile, MIDI::Event& eve);
	//===================MIDI�������݂̏����ŗp���鏈���Q�̏I���n�_===============================
};

#endif