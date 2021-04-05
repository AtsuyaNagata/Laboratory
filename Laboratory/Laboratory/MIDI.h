#ifndef MIDI_H_2019_12_19_
#define MIDI_H_2019_12_19_

#include"File.h"
#include<stdio.h>
#include <cstdint>

class MIDI {
public:
	MIDI(const char* filename) :
		mFile(nullptr),
		mMaxTrackNum(0),
		mResolution(0),
		mTrack(nullptr),
		mMemoryMap(nullptr)
	{
		mFile = new File(filename);
		const unsigned char* fileData = reinterpret_cast<const unsigned char*>(mFile->data());
		//�t�@�C���̎��̂��Ȃ���΂����ŗ��Ƃ�
		if (mFile->size() == 0) {
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
			mMaxTrackNum = 1;
		}
		//�t�H�[�}�b�g��1�̎�
		if (fileData[8] == 0 && fileData[9] == 1) {
			//�g���b�N�����l��(2�o�C�g)
			mMaxTrackNum = (fileData[10] << 8) + fileData[11];
			//mMaxTrackNum��0���ƐF�X�܂����̂ŗ��Ƃ�
			if (mMaxTrackNum == 0) {
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

		//�g���b�N����Track��p�ӂ���
		mTrack = new Track[mMaxTrackNum];
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
		int TrackPoint = 0;
		for (int i = 14; i < mFile->size();) {
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
				//�����L�q��F�����邽�߂̃t���O
				enum class MIDIEVENTFLAG { MIDIEVENT_3BYTES, MIDIEVENT_2BYTES, NONE } midiEventFlag = MIDIEVENTFLAG::NONE;
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
					else {	//�C�x���g�����Ɋւ��鏈��
						if (fileData[j] == 0xff) {
							//FF����n�܂�f�[�^��len�o�C�g�Ńo�C�g�̒������w�肵�Ă���̂ŁA���̕��̃o�C�g���΂�
							unsigned char len = fileData[j + 2];
							j += 3 + len;
							count++;
							midiEventFlag = MIDIEVENTFLAG::NONE;
						}
						else if (fileData[j] == 0xf0 || fileData[j] == 0xf7) {
							//0xf0���̃f�[�^�o�C�g���΂�
							j += 1;
							//len�͉ϒ��ł��鎖���l������
							int k, len = 0;
							for (k = 0; k < 4; ++k) {
								len <<= 7;
								len += fileData[j + k] & 0x7f;
								if (fileData[j + k] & 0x80) {			//�ŏ�ʃr�b�g��1�Ȃ�A���̃r�b�g���m�F����
									//�������Ȃ�
								}
								else {
									break;
								}
							}
							//���X1�o�C�g���̃f�[�^�͂���
							j += k + 1;
							//�f�[�^�̒������̃o�C�g���Ԃ���΂�
							j += len;
							count++;
							midiEventFlag = MIDIEVENTFLAG::NONE;
						}
						else if ((fileData[j] & 0xf0) == 0x90 || (fileData[j] & 0xf0) == 0x80 || (fileData[j] & 0xf0) == 0xA0 || (fileData[j] & 0xf0) == 0xB0 || (fileData[j] & 0xf0) == 0xE0) {
							j += 3;
							count++;
							midiEventFlag = MIDIEVENTFLAG::MIDIEVENT_3BYTES;
						}
						else if ((fileData[j] & 0xf0) == 0xC0 || (fileData[j] & 0xf0) == 0xD0) {
							j += 2;
							count++;
							midiEventFlag = MIDIEVENTFLAG::MIDIEVENT_2BYTES;
						}
						else {
							if (fileData[i] & 0x80) {	//�����ŏ�ʃr�b�g��1�Ȃ�AMIDI�C�x���g�̏ȗ��`�ł���Ȃ�
								printf("0x%x is not defined", fileData[j]);
								destroy();
								return;
							}
							//�ȗ��I�ȋL�@�̎��̏���
							if (midiEventFlag == MIDIEVENTFLAG::MIDIEVENT_3BYTES) {
								j += 2;
								count++;
							}
							else if (midiEventFlag == MIDIEVENTFLAG::MIDIEVENT_2BYTES) {
								j += 1;
								count++;
							}
							else {
								printf("some error has occurred\n");
								destroy();
								return;
							}
						}
					}
					delta = !delta;
				}

				mTrack[TrackPoint].tracksize = trackSize;
				mTrack[TrackPoint].eventnum = count;
				mTrack[TrackPoint].event = new Event[count];

				//�v�f����̓I�Ɋi�[���Ă����i2�T�ځj
				//�ŏ��̓f���^�^�C���̃f�[�^
				delta = true;
				int eventPoint = 0;
				for (int j = i; j < (trackSize + i);) {
					if (delta) {
						unsigned int time = 0;
						int k;
						//�ő�4�o�C�g���������O(1)
						for (k = 0; k < 4; ++k) {
							time <<= 7;
							time += fileData[j + k] & 0x7f;
							if (fileData[j + k] & 0x80) {
								continue;
							}
							else {
								break;
							}
						}
						//�f���^�^�C�����̃o�C�g���i�߂�
						j += k + 1;
						mTrack[TrackPoint].event[eventPoint].time = time;
					}
					else {	//�C�x���g�Ƃ���Ɋ�Â��f�[�^�̊i�[����
						if (fileData[j] == 0xff) {
							//�f�[�^�̃C�x���g�^�C�v�𔻕ʂ���l
							unsigned char eventType = fileData[j + 1];
							//FF����n�܂�f�[�^��len�o�C�g�Ńo�C�g�̒������w�肵�Ă���̂ŁA���̕��̃o�C�g���΂�
							unsigned char len = fileData[j + 2];
							//�����̃f�[�^�i�[
							mTrack[TrackPoint].event[eventPoint].datasize = len;
							if (len == 0) {
								//�T�C�Y��0�Ȃ�f�[�^���l�����Ȃ�
								mTrack[TrackPoint].event[eventPoint].data = nullptr;
							}
							else {
								//�f�[�^�̊J�n�n�_�̃|�C���^����������
								mTrack[TrackPoint].event[eventPoint].data = &fileData[j + 3];
							}
							//�C�x���g�^�C�v�����ꂼ��i�[����
							switch (eventType)
							{
								//case 0:
								//	mTrack[TrackPoint].event[eventPoint].type = EVENT::SEQUENCE_NUMBER; break;
							case 1:
								mTrack[TrackPoint].event[eventPoint].type = EVENT::COMMENT; break;
							case 2:
								mTrack[TrackPoint].event[eventPoint].type = EVENT::COPY_RIGHT; break;
							case 3:
								mTrack[TrackPoint].event[eventPoint].type = EVENT::SEQUENCE_TRACK_NAME; break;
							case 4:
								mTrack[TrackPoint].event[eventPoint].type = EVENT::INSTRUMENT_NAME; break;
							case 5:
								mTrack[TrackPoint].event[eventPoint].type = EVENT::LYRIC; break;
							case 6:
								mTrack[TrackPoint].event[eventPoint].type = EVENT::MARKER; break;
							case 7:
								mTrack[TrackPoint].event[eventPoint].type = EVENT::QUEUE_POINT; break;
							case 8:
								mTrack[TrackPoint].event[eventPoint].type = EVENT::PROGRAM_NAME; break;
							case 9:
								mTrack[TrackPoint].event[eventPoint].type = EVENT::DEVICE_NAME; break;
							case 0x20:
								mTrack[TrackPoint].event[eventPoint].type = EVENT::CHANNEL_PREFIX; break;
							case 0x21:
								mTrack[TrackPoint].event[eventPoint].type = EVENT::PORT_SPECIFIED; break;
							case 0x2f:
								mTrack[TrackPoint].event[eventPoint].type = EVENT::END_OF_TRACK; break;
							case 0x51:
								mTrack[TrackPoint].event[eventPoint].type = EVENT::SET_TEMPO; break;
							case 0x58:
								mTrack[TrackPoint].event[eventPoint].type = EVENT::TIME_SIGNATURE; break;
							case 0x59:
								mTrack[TrackPoint].event[eventPoint].type = EVENT::KEY_SIGNATURE; break;
								//case 0x7f:
								//	mTrack[TrackPoint].event[eventPoint].type = EVENT::SPECIAL_META_EVENT; break;

							default:
								mTrack[TrackPoint].event[eventPoint].type = EVENT::FF_NONE; break;
							}

							j += 3 + len;
						}
						else if (fileData[j] == 0xf0 || fileData[j] == 0xf7) {
							//f0(f7)���ʎq���̃r�b�g���΂�
							j += 1;
							//len�͉ϒ��ł��鎖���l������
							int len = 0;
							for (int k = 0; k < 4; ++k) {
								len <<= 7;
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
							mTrack[TrackPoint].event[eventPoint].type = EVENT::SYSEX;
							//���������Ă��
							mTrack[TrackPoint].event[eventPoint].datasize = len;
							//�������l�����Ă��܂��Ό�͂������̂���
							mTrack[TrackPoint].event[eventPoint].data = &fileData[j];
							//�������̃o�C�g���Ԃ���΂�
							j += len;
						}
						//MIDI�C�x���g�́A���ʎq�Ƀf�[�^�������Ă����Ȍ`�Ȃ̂ŃC�x���g��񂪊i�[����Ă���o�C�g���ƃf�[�^���l������
						//�m�[�g�I���E�I�t����
						else if ((fileData[j] & 0xf0) == 0x90 || (fileData[j] & 0xf0) == 0x80) {
							mTrack[TrackPoint].event[eventPoint].datasize = 3;
							mTrack[TrackPoint].event[eventPoint].data = &fileData[j];
							if ((fileData[j] & 0xf0) == 0x90) {
								mTrack[TrackPoint].event[eventPoint].type = EVENT::NOTE_ON;
							}
							else if ((fileData[j] & 0xf0) == 0x80) {
								mTrack[TrackPoint].event[eventPoint].type = EVENT::NOTE_OFF;
							}
							j += 3;
						}
						//3�o�C�g�g���C�x���g���Ԃ�����ł�
						else if ((fileData[j] & 0xf0) == 0xA0 || (fileData[j] & 0xf0) == 0xB0 || (fileData[j] & 0xf0) == 0xE0) {
							mTrack[TrackPoint].event[eventPoint].datasize = 3;
							mTrack[TrackPoint].event[eventPoint].data = &fileData[j];
							if ((fileData[j] & 0xf0) == 0xA0) {
								mTrack[TrackPoint].event[eventPoint].type = EVENT::POLYPHONIC_KEY_PRESSURE;
							}
							else if ((fileData[j] & 0xf0) == 0xB0) {
								mTrack[TrackPoint].event[eventPoint].type = EVENT::CONTROL;
							}
							else if ((fileData[j] & 0xf0) == 0xE0) {
								mTrack[TrackPoint].event[eventPoint].type = EVENT::PITCH_BENT;
							}
							j += 3;
						}
						//2�o�C�g�g���C�x���g���Ԃ�����
						else if ((fileData[j] & 0xf0) == 0xC0 || (fileData[j] & 0xf0) == 0xD0) {
							mTrack[TrackPoint].event[eventPoint].datasize = 2;
							mTrack[TrackPoint].event[eventPoint].data = &fileData[j];
							if ((fileData[j] & 0xf0) == 0xC0) {
								mTrack[TrackPoint].event[eventPoint].type = EVENT::PROGRAM_CHANGE;
							}
							else if ((fileData[j] & 0xf0) == 0xD0) {
								mTrack[TrackPoint].event[eventPoint].type = EVENT::CHANNEL_PRESSURE;
							}
							j += 2;
						}
						else {
							if (fileData[i] & 0x80) {	//�����ŏ�ʃr�b�g��1�Ȃ�AMIDI�C�x���g�̏ȗ��`�ł���Ȃ��̂ŃG���[
								printf("0x%x is not defined", fileData[j]);
								destroy();
								return;
							}
							//�ȗ��I�ȋL�@�̎��̏���
							int dataSizeBuff = 0;
							//0x8n�̏���
							if (mTrack[TrackPoint].event[eventPoint - 1].type == EVENT::NOTE_OFF) {
								dataSizeBuff = 2;
								mTrack[TrackPoint].event[eventPoint].type = EVENT::NOTE_OFF;
							}
							//0x9n�̏���
							else if (mTrack[TrackPoint].event[eventPoint - 1].type == EVENT::NOTE_ON) {
								dataSizeBuff = 2;
								mTrack[TrackPoint].event[eventPoint].type = EVENT::NOTE_ON;
							}
							//0xAn�̏���
							else if (mTrack[TrackPoint].event[eventPoint - 1].type == EVENT::POLYPHONIC_KEY_PRESSURE) {
								dataSizeBuff = 2;
								mTrack[TrackPoint].event[eventPoint].type = EVENT::POLYPHONIC_KEY_PRESSURE;
							}
							//0xBn�̏���
							else if (mTrack[TrackPoint].event[eventPoint - 1].type == EVENT::CONTROL) {
								dataSizeBuff = 2;
								mTrack[TrackPoint].event[eventPoint].type = EVENT::CONTROL;
							}
							//0xCn�̏���
							else if (mTrack[TrackPoint].event[eventPoint - 1].type == EVENT::PROGRAM_CHANGE) {
								dataSizeBuff = 1;
								mTrack[TrackPoint].event[eventPoint].type = EVENT::PROGRAM_CHANGE;
							}
							//0xDn�̏���
							else if (mTrack[TrackPoint].event[eventPoint - 1].type == EVENT::CHANNEL_PRESSURE) {
								dataSizeBuff = 1;
								mTrack[TrackPoint].event[eventPoint].type = EVENT::CHANNEL_PRESSURE;
							}
							//0xEn�̏���
							else if (mTrack[TrackPoint].event[eventPoint - 1].type == EVENT::PITCH_BENT) {
								dataSizeBuff = 2;
								mTrack[TrackPoint].event[eventPoint].type = EVENT::PITCH_BENT;
							}
							else {
								//�Ȃ񂩃��o�C�����N���������ɔ�������
								printf("some error has occurred\n");
								destroy();
								return;
							}
							mTrack[TrackPoint].event[eventPoint].datasize = dataSizeBuff;
							mTrack[TrackPoint].event[eventPoint].data = &fileData[j];
							j += dataSizeBuff;
						}
						eventPoint += 1;
					}
					delta = !delta;
				}
				i += trackSize;
				TrackPoint += 1;
			}
			else {
				//MTrk�����Ȃ��̂͂��������̂ŗ��Ƃ�
				destroy();
				return;
			}
		}

	}

	//����ۂ�MIDI�N���X�����
	MIDI() :
		mFile(nullptr),
		mMaxTrackNum(0),
		mResolution(0),
		mTrack(nullptr),
		mMemoryMap(nullptr)
	{}

	~MIDI() {
		destroy();
	}

	enum class EVENT : unsigned char {
		//���^�C�x���g				//FF�b�C�x���g�^�C�v�b�f�[�^���b�f�[�^
		//SEQUENCE_NUMBER,			//FF 00 02 �V�[�P���X�ԍ��A�t�H�[�}�b�g0��1
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
		//SPECIAL_META_EVENT,		//FF 7F len data�@�ŏ���1�o�C�g�̓��[�JID�ŁA���̌�ɓƎ��t�H�[�}�b�g�̃f�[�^������
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
		NOTE_ON,					//9n �m�[�g�E�I��
		NOTE_OFF					//8n �m�[�g�E�I�t
	};

	//Event�̍\��
	typedef struct Event {
		EVENT type;					//���̃C�x���g�����i�[����
		size_t datasize;			//���ꂼ��̃C�x���g�̃f�[�^�T�C�Y���i�[����
		const unsigned char* data;	//�C�x���g�̃f�[�^���i�[����
		size_t time;				//�f���^�^�C�����i�[����i�f���^�^�C���͑O�̃C�x���g����̑��Ύ��ԁj
	} Event;
	typedef struct Track {
		Event* event;
		size_t eventnum;
		size_t tracksize;
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
		size_t time;
		uint8_t noteNum;
		uint8_t velocity;
	}MIDINoteEvent, * MIDINoteEvents;
	//MIDI�f�[�^���\�z����֐��B�����͓��͂�����MIDI�C�x���g�p�\���̔z��Ƃ��̍ő�T�C�Y
	int createMIDI_oneTrack(MIDINoteEvents midiEvents, int noteSize) {
		//������
		if (mMaxTrackNum != 0) { destroy(); }						//�������̂�����Ȃ�A�S���j�󂵂Ă���\�z����
		mMaxTrackNum = 1;											//�g���b�N�͈�ŌŒ肷��
		mResolution = 960;											//�l����������̃^�C����960�������čl����l�ɂ���
		mTrack = new Track[1];										//����̃g���b�N�̐���
		mTrack->eventnum = noteSize + 4;							//�m�[�g�C�x���g�����łȂ��A���q���q���������C�x���g�ABMP�������C�x���g�A�g���b�N���̃C�x���g�A�g���b�N�̏I�[�������C�x���g���l������
		mTrack->event = new Event[noteSize + 4];					//�C�x���g�g�𐶐�����
		mMemoryMap = new uint8_t[4 + 3 + 5 + 1 + (noteSize * 2)];	//�l������f�[�^�T�C�Y�́A�C�x���g0�A�C�x���g1�A�C�x���g3�A0x90�A2�o�C�g�Â̃C�x���g�T�C�Y�̘a

		//�f�[�^�̊i�[����
		mTrack->event[0].type = EVENT::TIME_SIGNATURE;					//���q�╪�q�A���g���m�[���Ԋu��ݒ肷��
		mTrack->event[1].type = EVENT::SET_TEMPO;						//�e���|��ݒ肷��
		mTrack->event[2].type = EVENT::SEQUENCE_TRACK_NAME;				//�g���b�N�����ݒ肷��
		mTrack->event[0].datasize = 4;	mTrack->event[1].datasize = 3; mTrack->event[2].datasize = 5;			//���ꂼ��̃f�[�^�T�C�Y������	
		mTrack->event[0].time = mTrack->event[1].time = mTrack->event[2].time = 0;								//�ǂ���ݒ�Ȃ̂Ń^�C����0

		//�^�C���V�O�l�`���̒l���������}�b�v�Ɏd����(���q = 4�A ���� = 4�A ���g���m�[���Ԋu = 4���������[24MIDI�N���b�N]�A 32��������4��������8���������l = 8)
		mMemoryMap[0] = 0x04; mMemoryMap[1] = 0x02; mMemoryMap[2] = 0x18; mMemoryMap[3] = 0x08;
		//�e���|���Z�b�g���Ă���(0x07A120 = 500,000��s�ɃZ�b�g = 120bmp�ɂȂ�)
		mMemoryMap[4] = 0x07; mMemoryMap[5] = 0xA1; mMemoryMap[6] = 0x20;
		//�g���b�N�����߂�
		mMemoryMap[7] = 'T'; mMemoryMap[8] = 'r'; mMemoryMap[9] = 'a';  mMemoryMap[10] = 'c'; mMemoryMap[11] = 'k';
		//�f�[�^�ʒu���Z�b�g����
		mTrack->event[0].data = &mMemoryMap[0]; mTrack->event[1].data = &mMemoryMap[4]; mTrack->event[2].data = &mMemoryMap[7];

		if (midiEvents == nullptr) { return 1; }	//�m�[�g�i�[�������s���O�Ƀf�[�^���Ȃ��ƈȉ��̏������o�O��̂ŁA�����ŗ��Ƃ��B�u1�v�̓f�[�^����`�̃G���[�Ƃ���

		//��������m�[�g�i�[����(��s��type datasize time�������Ă�̂Œ��ӂ�)
		mTrack->event[3].type = EVENT::NOTE_ON; mTrack->event[3].datasize = 3; mTrack->event[3].time = midiEvents[0].time;	//�ŏ��͏ȗ��s�\�^�Ȃ̂œ��ʂɒ�`���Ă��
		mMemoryMap[12] = 0x90; mMemoryMap[13] = midiEvents[0].noteNum; mMemoryMap[14] = midiEvents[0].velocity;				//�f�[�^���������}�b�v�ɔz�u
		mTrack->event[3].data = &mMemoryMap[12];																			//�f�[�^�ʒu���i�[
		size_t trackdataSize = 24;		//Track�̃o�C�g�T�C�Y������B�O�q������24�o�C�g���̃f�[�^�o�C�g���g�����Ƃ��������Ă�̂ŁA�����l�Ƃ��Ċi�[������(�f���^�^�C�������l�������15�ł͂Ȃ�)
		//midiEvents[0]�̃f���^�^�C���̃T�C�Y�����Z����
		if (midiEvents[0].time & 0x0fe00000) { trackdataSize += 4; }
		else if (midiEvents[0].time & 0x001fc000) { trackdataSize += 3; }
		else if (midiEvents[0].time & 0x00003f80) { trackdataSize += 2; }
		else { trackdataSize += 1; }
		//�ȗ��\�ȃm�[�g�C�x���g���i�[����
		int i;
		for (i = 1; i < noteSize; ++i) {
			//midi�C�x���g�ɂ��āA�܂����̂܂܊i�[�\�Ȃ��̂�����
			mTrack->event[i + 3].type = EVENT::NOTE_ON;
			mTrack->event[i + 3].time = midiEvents[i].time;
			mTrack->event[i + 3].datasize = 2;
			//�������}�b�v�Ƀf�[�^��z�u���āA�J�n�n�_��event�\���Ɋi�[����
			mMemoryMap[(i * 2) + 13] = midiEvents[i].noteNum;
			mMemoryMap[(i * 2) + 1 + 13] = midiEvents[i].velocity;
			mTrack->event[i + 3].data = &mMemoryMap[(i * 2) + 13];
			//�f���^�^�C���̃o�C�g���𐔂���
			if (midiEvents[i].time & 0x0fe00000) { trackdataSize += 4; }
			else if (midiEvents[i].time & 0x001fc000) { trackdataSize += 3; }
			else if (midiEvents[i].time & 0x00003f80) { trackdataSize += 2; }
			else { trackdataSize += 1; }
			trackdataSize += 2;
		}
		//�g���b�N�̏I���������Ō�ɓ����
		mTrack->event[i + 3].type = EVENT::END_OF_TRACK;
		mTrack->event[i + 3].datasize = 0; mTrack->event[i + 3].data = nullptr;
		mTrack->tracksize = trackdataSize;
		return 0;
	}

private:
	//�v�f��j�󂷂�֐��̓N���X���g�̂݌Ăяo������̂Ƃ���
	void destroy() {
		for (int i = 0; i < mMaxTrackNum; ++i) {
			delete[] mTrack[i].event;
			mTrack[i].event = nullptr;
		}
		delete[] mTrack; mTrack = nullptr;
		delete[] mMemoryMap; mMemoryMap = nullptr;
		mMaxTrackNum = 0;
		mResolution = 0;
	}
	File* mFile;

	short mMaxTrackNum;		//�g���b�N�̐�
	short mResolution;		//�l�������������̕���\�̒l������(480�������Ă�����f���^�^�C��480�Ŏl������������)

	uint8_t* mMemoryMap;	//�������񂾃f�[�^���i�[���Ă����ꏊ

	Track* mTrack;
};

#endif