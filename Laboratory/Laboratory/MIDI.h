#ifndef MIDI_H_2019_11_21_
#define MIDI_H_2019_11_21_

#include"File.h"
#include<stdio.h>

bool isEvent(unsigned char byteData) {
	return (byteData == 0xff || byteData == 0xf0 || byteData == 0xf7 || byteData == 0xb0 || (byteData & 0xf0) == 0x90 || (byteData & 0xf0) == 0x80);
}

class MIDI : File{
public:
	MIDI(const char* filename) :
		File(filename),
		mTrack(0),
		mResolution(0),
		mEventLine(0)
	{
		const unsigned char* fileData = reinterpret_cast<unsigned char*>(mData);
		//�t�@�C���̎��̂��Ȃ���΂����ŗ��Ƃ�
		if (mSize == 0) {
			return;
		}
		//�t�@�C���w�b�_�m�F(4�o�C�g)
		if (fileData[0] == 'M' && fileData[1] == 'T' && fileData[2] == 'h' && fileData[3] == 'd') {
			//�������Ȃ�
		}
		else {
			return;
		}
		//�w�b�_�T�C�Y��6�ɂȂ��Ă��邩���m�F����(�Ȃ���4�o�C�g)
		if (fileData[4] == 0 && fileData[5] == 0 && fileData[6] == 0 && fileData[7] == 0x06) {
			//�������Ȃ�
		}
		else {
			return;
		}

		//�t�H�[�}�b�g�m�F�i�t�H�[�}�b�g�ɂ�0�A1�A2������j(2�o�C�g)
		//�t�H�[�}�b�g0�͑S�Ẵ`�����l���̃f�[�^����̃g���b�N�ɂ܂Ƃ߂�����
		//�t�H�[�}�b�g1�͕����̃`�����l����p���ăf�[�^���܂Ƃ߂�����
		//�t�H�[�}�b�g2�̓V�[�P���X�p�^�[�������̂܂ܕۑ���������A�����炭���Ȃ蕡�G���Ǝv����(������)
		//�t�H�[�}�b�g��0�̎�
		if (fileData[8] == 0 && fileData[9] == 0) {
			//�g���b�N��(2�o�C�g)
			//�g���b�N����1�łȂ��ƃt�H�[�}�b�g���ƍ���Ȃ�
			if (fileData[10] != 0 || fileData[11] != 1) {
				return;
			}
			mTrack = 1;
		}
		//�t�H�[�}�b�g��1�̎�
		if (fileData[8] == 0 && fileData[9] == 1) {
			//�g���b�N�����l��(2�o�C�g)
			mTrack = (fileData[10] << 8) + fileData[11];
			//mTrack��0���ƐF�X�܂����̂ŗ��Ƃ�
			if (mTrack == 0) {
				return;
			}
		}

		//���ԒP�ʂ̊l��(����\�̂���)
		//�ŏ��̃r�b�g��0�Ȃ�΁A�����߂̉����ڂ̃f�[�^�Ƃ����������Ńg���b�N�`�����N�������
		//�ŏ��̃r�b�g��1�Ȃ�΁A�������b�Ƃ������f�[�^�Ƃ����������ŕۑ�����
		//���͑O��
		if (fileData[12] & 0b10000000) {
			//�������b�̂��
		}
		else {
			//�����߉����ڂ̂��
			mResolution = (fileData[12] << 8) + fileData[13];
		}

		//�g���b�N����EventLine��p�ӂ���
		mEventLine = new EventLine[mTrack];
		//MTrk[4D 54 72 6B]�̎��ʎq�ƃf�[�^�T�C�Y�̊l���̌�ɃC�x���g�����ׂ���

		//�f���^�^�C���ɂ���
		/*
			�f���^�^�C���͉ϒ��o�C�g�̕\�����Ȃ���Ă���A�ŏ�ʃr�b�g�����̃o�C�g�������œǂݍ��ނ��̔��ʎq�ƂȂ�B
			��F
			10�i���u1�v -> 00000001
			10�i���u128�v -> 10000001 00000000
			
			�܂�A1�o�C�g���Ƃ�1�r�b�g�̖��ʂ��o��
		*/

		//MIDI�C�x���g�ɂ���
		/*
			3�o�C�g�̃f�[�^�ŕ\������
			��F�u8�`�����l����C4(60 = 0x3C)���x���V�e�B�[��90�Ŗ炷�v => [0x98 0x3C 0x5A]
			�m�[�g�I�t[0x8n 0xkk 0xvv]�F�����~�߂�B�x���V�e�B�[�͌��Ղ��������������\��
			(n = �Ώۃ`�����l���i���o�[�@kk = �m�[�g�i���o�[�@vv = �x���V�e�B�[)
			�m�[�g�I��[0x9n 0xkk 0xvv]�F����炷�B�x���V�e�B�[�͌��Ղ�e������(���̋���)
			(n = �Ώۃ`�����l���i���o�[�@kk = �m�[�g�i���o�[�@vv = �x���V�e�B�[)
			�R���g���[���`�F���W[0xBn 0xcc 0xvv]�F�e��R���g���[���̐ݒ肪�o����
			(n = �Ώۃ`�����l���i���o�[�@cc = �R���g���[���i���o�[�@vv = �f�[�^)
		*/
		int eventLinePoint = 0;
		for (int i = 14; i < mSize;) {
			int eventNum = 0;
			if (fileData[i] == 'M' && fileData[i + 1] == 'T' && fileData[i + 2] == 'r' && fileData[i + 3] == 'k') {
				//MTrk�̕������̃o�C�g��i�߂�
				i += 4;
				//�g���b�N�̃f�[�^�̃T�C�Y
				size_t trackSize = (fileData[i] << 24) + (fileData[i + 1] << 16) + (fileData[i + 2] << 8) + fileData[i + 3];
				//�g���b�N�T�C�Y���̃o�C�g��i�߂�
				i += 4;
				//�C�x���g�̐����J�E���g����
				size_t count = 0;
				//�ŏ��̓f���^�^�C���̃f�[�^������̂�true
				bool delta = true;
				for (int j = i; j < (trackSize + i);) {
					if (delta) {
						//�ő�4�o�C�g���������O(1)
						for (int k = 0; k < 4; ++k) {
							if (fileData[j + k] & 0x80) {
								//�������Ȃ�
							}
							else {
								//�f���^�^�C�����̃o�C�g���i�߂�
								j += k + 1;
								break;
							}
						}
					}
					else if (isEvent(fileData[j])) {
						if (fileData[j] == 0xff) {
							//FF����n�܂�f�[�^��len�o�C�g�Ńo�C�g�̒������w�肵�Ă���̂ŁA���̕��̃o�C�g���΂�
							unsigned char len = fileData[j + 2];
							j += 3 + len;
							count++;
						}
						else if (fileData[j] == 0xf0 || fileData[j] == 0xf7) {
							//0xf0���̃f�[�^�o�C�g���΂�
							j += 1;
							//len�͉ϒ��ł��鎖���l������
							int len = 0;
							for (int k = 0; k < 4; ++k) {
								len << 8;
								len += fileData[j + k] & 0x7f;
								if (fileData[j + k] & 0x80) {			//�ŏ�ʃr�b�g��1�Ȃ�A���̃r�b�g���m�F����
									//�������Ȃ�
								}
								else {
									//���X1�o�C�g���̃f�[�^�͂���
									j += k + 1;
									break;
								}
							}
							//�������̃o�C�g���Ԃ���΂�
							j += len;
							count++;
						}
						else if ((fileData[j] & 0xf0) == 0x90 || (fileData[j] & 0xf0) == 0x80 || (fileData[j] & 0xf0) == 0xb0) {
							j += 3;
							count++;
						}
						else {
							printf("0x%x is not defined", fileData[j]);
							destroy();
							return;
						}
					}
					delta = !delta;
				}

				mEventLine[eventLinePoint].eventnum = count;
				mEventLine[eventLinePoint].event = new Event[count];

				//�v�f����̓I�Ɋi�[���Ă����i2�T�ځj
				//�ŏ��̓f���^�^�C���̃f�[�^
				delta = true;
				int eventPoint = 0;
				for (int j = i; j < (trackSize + i);) {
					if (delta) {
						unsigned int time = 0;
						//�ő�4�o�C�g���������O(1)
						for (int k = 0; k < 4; ++k) {
							time << 8;
							if (fileData[j + k] & 0x80) {
								time += fileData[j + k] & 0x7f;
							}
							else {
								//�f���^�^�C�����̃o�C�g���i�߂�
								j += k + 1;
								break;
							}
						}
						mEventLine[eventLinePoint].event[eventPoint].time = time;
					}
					else if (isEvent(fileData[j])) {
						if (fileData[j] == 0xff) {
							//�f�[�^�̃C�x���g�^�C�v�𔻕ʂ���l
							unsigned char eventType = fileData[j + 1];
							//FF����n�܂�f�[�^��len�o�C�g�Ńo�C�g�̒������w�肵�Ă���̂ŁA���̕��̃o�C�g���΂�
							unsigned char len = fileData[j + 2];
							//�����̃f�[�^�i�[
							mEventLine[eventLinePoint].event[eventPoint].datasize = len;
							if (len == 0) {
								//�T�C�Y��0�Ȃ�f�[�^���l�����Ȃ�
								mEventLine[eventLinePoint].event[eventPoint].data = nullptr;
							}
							else {
								//�f�[�^�̊J�n�n�_�̃|�C���^����������
								mEventLine[eventLinePoint].event[eventPoint].data = &fileData[j + 3];
							}
							//�C�x���g�^�C�v�����ꂼ��i�[����
							switch (eventType)
							{
							//case 0:
							//	mEventLine[eventLinePoint].event[eventPoint].type = EVENT::SEQUENCE_NUMBER; break;
							case 1:
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::COMMENT; break;
							case 2:
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::COPY_RIGHT; break;
							case 3:
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::SEQUENCE_TRACK_NAME; break;
							case 4:
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::INSTRUMENT_NAME; break;
							case 5:
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::LYRIC; break;
							//case 0x20:
							//	mEventLine[eventLinePoint].event[eventPoint].type = EVENT::CHANNEL_PREFIX; break;
							//csse 0x21:
							//	mEventLine[eventLinePoint].event[eventPoint].type = EVENT::PORT_SPECIFIED; break;
							case 0x2f:
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::END_OF_TRACK; break;
							case 0x51:
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::SET_TEMPO; break;
							case 0x58:
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::TIME_SIGNATURE; break;
							case 0x59:
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::KEY_SIGNATURE; break;

							default:
								break;
							}

							j += 3 + len;
						}
						else if (fileData[j] == 0xf0 || fileData[j] == 0xf7) {
							//f0(f7)���ʎq���̃r�b�g���΂�
							j += 1;
							//len�͉ϒ��ł��鎖���l������
							int len = 0;
							for (int k = 0; k < 4; ++k) {
								len << 8;
								len += fileData[j + k] & 0x7f;
								if (fileData[j + k] & 0x80) {			//�ŏ�ʃr�b�g��1�Ȃ�A���̃r�b�g���m�F����
									//�������Ȃ�
								}
								else {
									j += k + 1;
									break;
								}
							}
							//�C�x���g�̊i�[
							mEventLine[eventLinePoint].event[eventPoint].type = EVENT::SYSEX;
							//���������Ă��
							mEventLine[eventLinePoint].event[eventPoint].datasize = len;
							//�������l�����Ă��܂��Ό�͂������̂���
							mEventLine[eventLinePoint].event[eventPoint].data = &fileData[j + len];
							//�������̃o�C�g���Ԃ���΂�
							j += len;
						}
						//MIDI�C�x���g�́A���ʎq�Ƀf�[�^�������Ă����Ȍ`
						else if ((fileData[j] & 0xf0) == 0x90 || (fileData[j] & 0xf0) == 0x80 || (fileData[j] & 0xf0) == 0xb0) {
							mEventLine[eventLinePoint].event[eventPoint].datasize = 3;
							mEventLine[eventLinePoint].event[eventPoint].data = &fileData[j];
							//
							if ((fileData[j] & 0xf0) == 0x90) {
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::NOTE_ON;
							}
							else if ((fileData[j] & 0xf0) == 0x80) {
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::NOTE_OFF;
							}
							else if ((fileData[j] & 0xf0) == 0xb0) {
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::CONTROL;
							}
							j += 3;
						}
						else {
							int a = 0;
							printf("a = %d\n", a);
							printf("0x%x is not defined\n", fileData[j]);
							return;
						}
						eventPoint += 1;
					}
					delta = !delta;
				}
				i += trackSize;
				eventLinePoint += 1;
			}
			else {
				//MTrk�����Ȃ��̂͂��������̂ŗ��Ƃ�
				destroy();
				return;
			}
		}

	}
	~MIDI() {
		destroy();
	}

	enum class EVENT : unsigned char {			
		//���^�C�x���g				//FF�b�C�x���g�^�C�v�b�f�[�^���b�f�[�^
		//SEQUENCE_NUMBER,			//FF 00 02 �V�[�P���X�ԍ��A�t�H�[�}�b�g0��1
		COMMENT,					//FF 01 len
		COPY_RIGHT,					//FF 02 len
		SEQUENCE_TRACK_NAME,		//FF 03 len
		INSTRUMENT_NAME,			//FF 04 len
		LYRIC,						//FF 05 len  lyric�͉̎��̎�
		//CHANNEL_PREFIX,			//FF 20 01 cc MIDI�C�x���g����Ȃ��̂�(SysEx or ���^�C�x���g)MIDI�`�����l�����w�肷��ۂɗp����
		//PORT_SPECIFIED,			//FF 21 01 pp �o�̓|�[�g�̎w��	0�`3�����ꂼ��|�[�g��1�`4�ɑΉ�����(��O�͂��邪�A�f�[�^�݂̂ł͔��ʕs��)
		END_OF_TRACK,				//FF 2F 00
		SET_TEMPO,					//FF 51 03
		TIME_SIGNATURE,				//FF 58 04 nn dd cc bb | ���q nn = ���q dd = ���q cc = ���g���m�[���Ԋu[�l�������Ԋu�Ȃ�18H] bb = �l������������̎O�\�񕪉����̐�
		KEY_SIGNATURE,				//FF 59 02 sf ml �L�[(��)������	sf = ��:�V���[�v�̐� or ��:�t���b�g�̐�		ml = 0:����(���W���[) 1:�Z��(�}�C�i�[)

		//�V�X�e���G�N�X�N���V�[�u���b�Z�[�W���i�[����C�x���g�B�X�̊y�탁�[�J���Ǝ��ɍ쐬�������b�Z�[�W���i�[����BMIDI�f�o�C�X�ɉ�������ݒ肷��̂ɗ��p�����
		SYSEX,						//F0 �f�[�^�� �G�N�X�N���V�[�u���b�Z�[�W F7
		F7_SYSEX,					//F7 �f�[�^�� �G�N�X�N���V�[�u���b�Z�[�W

		//MIDI�C�x���g
		CONTROL,					//B0 �R���g���[���ݒ�
		NOTE_ON,					//9n �m�[�g�E�I��
		NOTE_OFF					//8n �m�[�g�E�I�t
	};
	//Event�̍\��
	typedef struct Event {
		EVENT type;					//���̃C�x���g�����i�[����
		size_t datasize;			//���ꂼ��̃C�x���g�̃f�[�^�T�C�Y���i�[����
		const unsigned char* data;	//�C�x���g�̃f�[�^���i�[����
		unsigned int time;			//�f���^�^�C�����i�[����
	} Event;

	typedef struct EventLine {
		Event* event;
		size_t eventnum;
	} EventLine;

	EventLine* getEventLine(int n) {
		return &mEventLine[n];
	}

private:
	//�v�f��j�󂷂�֐��̓N���X���g�̂݌Ăяo������̂Ƃ���
	 void destroy() {
		 for (int i = 0; i < mTrack; ++i) {
			 delete[] mEventLine[i].event;
			 mEventLine[i].event = nullptr;
		 }
		 delete[] mEventLine;
		 mEventLine = nullptr;
	 }

	 short mTrack;		//�g���b�N�̐�
	 short mResolution;	//����\�̒l

	 EventLine* mEventLine;
};

#endif