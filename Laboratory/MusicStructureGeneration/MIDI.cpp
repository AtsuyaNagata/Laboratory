#include "MIDI.h"

//MIDI�t�@�C����ǂݎ��{�̂̏����������Ă�
MIDI::MIDI(const char* filename) :
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
					//�m�[�g�I���E�I�t�C�x���g�B���̃C�x���g����ɏd�v�����Ă�̂ŁA������Ղ����邽�ߕʂɏ����Ă�
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
						//-----------�ȗ��I�ȋL�@�̎��̏���-----------
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
						//�ȗ��L�@�̏����̋��ʕ���
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

MIDI::MIDI() :
	mFile(nullptr),
	mMaxTrackNum(0),
	mResolution(0),
	mTrack(nullptr),
	mMemoryMap(nullptr)
{}

MIDI::~MIDI(){
	destroy();
}

int MIDI::createMIDI_oneTrack(MIDINoteEvents midiEvents, int noteSize, int bmp) {
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
	//�e���|���Z�b�g���Ă���(0x07A120 = 500000��s = 120bmp�ɂȂ鎖���l������)
	int setBMP = 500000 * 120 / bmp;
	mMemoryMap[4] = ((0x00ff0000 & setBMP) >> 16); mMemoryMap[5] = ((0x0000ff00 & setBMP) >> 8); mMemoryMap[6] = (0x000000ff & setBMP);
	//�g���b�N�����߂�
	mMemoryMap[7] = 'T'; mMemoryMap[8] = 'r'; mMemoryMap[9] = 'a';  mMemoryMap[10] = 'c'; mMemoryMap[11] = 'k';
	//�f�[�^�ʒu���������}�b�v�ƑΉ��t����
	mTrack->event[0].data = &mMemoryMap[0]; mTrack->event[1].data = &mMemoryMap[4]; mTrack->event[2].data = &mMemoryMap[7];

	if (midiEvents == nullptr) { return 1; }	//�m�[�g�i�[�������s���O�Ƀf�[�^���Ȃ��ƈȉ��̏������o�O��̂ŁA�����ŗ��Ƃ��B�u1�v�̓f�[�^����`�̃G���[�Ƃ���

	//-----------��������m�[�g�i�[����(��s��type datasize time�������Ă�̂Œ��ӂ�)-----------
	mTrack->event[3].type = EVENT::NOTE_ON; mTrack->event[3].datasize = 3; mTrack->event[3].time = midiEvents[0].time;	//�ŏ��͏ȗ��s�\�^�Ȃ̂œ��ʂɒ�`���Ă��
	mMemoryMap[12] = 0x90; mMemoryMap[13] = midiEvents[0].noteNum; mMemoryMap[14] = midiEvents[0].velocity;				//�f�[�^���������}�b�v�ɔz�u
	mTrack->event[3].data = &mMemoryMap[12];																			//�f�[�^�ʒu���i�[
	size_t trackdataSize = 31;		//Track�̃o�C�g�T�C�Y������B�O�q������24�o�C�g���̃f�[�^�o�C�g���g�����Ƃ��������Ă�̂ŁA�����l�Ƃ��Ċi�[������(�f���^�^�C�������l�������15�ł͂Ȃ�)
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
	mTrack->event[i + 3].datasize = 0; mTrack->event[i + 3].data = nullptr; mTrack->event[i + 3].time = 0;
	mTrack->tracksize = trackdataSize;
	return 0;
}

int MIDI::createMIDI_oneTrack(std::vector<MIDINoteEvent> midiEvents, int noteSize, int bmp) {
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
	//�e���|���Z�b�g���Ă���(0x07A120 = 500000��s = 120bmp�ɂȂ鎖���l������)
	int setBMP = 500000 * 120 / bmp;
	mMemoryMap[4] = ((0x00ff0000 & setBMP) >> 16); mMemoryMap[5] = ((0x0000ff00 & setBMP) >> 8); mMemoryMap[6] = (0x000000ff & setBMP);
	//�g���b�N�����߂�
	mMemoryMap[7] = 'T'; mMemoryMap[8] = 'r'; mMemoryMap[9] = 'a';  mMemoryMap[10] = 'c'; mMemoryMap[11] = 'k';
	//�f�[�^�ʒu���������}�b�v�ƑΉ��t����
	mTrack->event[0].data = &mMemoryMap[0]; mTrack->event[1].data = &mMemoryMap[4]; mTrack->event[2].data = &mMemoryMap[7];

	if (midiEvents.size() == 0) { return 1; }	//�m�[�g�i�[�������s���O�Ƀf�[�^���Ȃ��ƈȉ��̏������o�O��̂ŁA�����ŗ��Ƃ��B�u1�v�̓f�[�^����`�̃G���[�Ƃ���

	//-----------��������m�[�g�i�[����(��s��type datasize time�������Ă�̂Œ��ӂ�)-----------
	mTrack->event[3].type = EVENT::NOTE_ON; mTrack->event[3].datasize = 3; mTrack->event[3].time = midiEvents[0].time;	//�ŏ��͏ȗ��s�\�^�Ȃ̂œ��ʂɒ�`���Ă��
	mMemoryMap[12] = 0x90; mMemoryMap[13] = midiEvents[0].noteNum; mMemoryMap[14] = midiEvents[0].velocity;				//�f�[�^���������}�b�v�ɔz�u(�C�x���g��񂲂�)
	mTrack->event[3].data = &mMemoryMap[12];																			//�f�[�^�ʒu���i�[
	size_t trackdataSize = 31;		//Track�̃o�C�g�T�C�Y������B�O�q������24�o�C�g���̃f�[�^�o�C�g���g�����Ƃ��������Ă�̂ŁA�����l�Ƃ��Ċi�[������(�f���^�^�C�������l�������15�ł͂Ȃ�)
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
	mTrack->event[i + 3].datasize = 0; mTrack->event[i + 3].data = nullptr; mTrack->event[i + 3].time = 0;
	mTrack->tracksize = trackdataSize;
	return 0;
}

//-----------��������MIDI�t�@�C���������݂̋�̓I�ȏ���-----------
int MIDI::MIDIWrite(const char* filename) {
	FILE* midiFile = fopen(filename, "wb");
	if (midiFile == NULL) {
		fprintf(stderr, "%s�̓ǂݍ��݂Ɏ��s���܂����B\n", filename);
		return 1;
	}
	unsigned short sbuff = 0;
	unsigned int ibuff = 0;
	fwrite("MThd", 1, 4, midiFile);						//�`�����N�^�C�v
	ibuff = 6;											//�w�b�_�̃f�[�^�T�C�Y�ł���6��������
	reverceByte((void*)&ibuff, 4);
	fwrite(&ibuff, 4, 1, midiFile);						//�w�b�_�̃f�[�^�T�C�Y
	if (mMaxTrackNum > 1) {								//�g���b�N�̐���1�ȏ�Ȃ�
		sbuff = 1;
		reverceByte((void*)&sbuff, 2);
		fwrite(&sbuff, 1, 2, midiFile);					//�t�H�[�}�b�g1
	}
	else if (mMaxTrackNum == 1) {						//�g���b�N�̐���1�Ȃ�
		sbuff = 0;
		reverceByte((void*)&sbuff, 2);					//�o�C�g���t�]������
		fwrite(&sbuff, 1, 2, midiFile);					//�t�H�[�}�b�g0
	}
	short maxTrackNum = mMaxTrackNum;					//�g���b�N�������邩���l��
	reverceByte((void*)&maxTrackNum, 2);
	fwrite(&maxTrackNum, 1, 2, midiFile);				//�g���b�N�����t�@�C���ɏ�������
	short resolution = mResolution;						//����\���l��
	reverceByte((void*)&resolution, 2);
	fwrite(&resolution, 1, 2, midiFile);				//����\����������

	for (int i = 0; i < mMaxTrackNum; ++i) {			//�g���b�N�̐��������[�v����
		fwrite("MTrk", 1, 4, midiFile);					//�g���b�N�`�����N�̎n�܂�̎��ʎq���L�q
		MIDI::Track* track = &mTrack[i];				//�����̃g���b�N�̃g���b�N�\����MIDI�N���X����l��
		size_t uibuff = track->tracksize;
		reverceByte((void*)&uibuff, 4);
		fwrite(&uibuff, 1, 4, midiFile);				//�g���b�N�`�����N�̃T�C�Y���L�q
		for (int j = 0; j < track->eventnum; ++j) {		//�g���b�N���ɑ��݂���C�x���g���̃��[�v����
			if (track->event[j].type != MIDI::EVENT::FF_NONE) {							//�����C�x���g���Ȃ��̂Ȃ�X���[����[(�����܃M���O)
				DeltaTimeWrite(midiFile, track->event[j].time);							//�f���^�^�C������������
				EventWrite(midiFile, track->event[j]);									//�C�x���g�����ʂ���v�f���������ފ֐�
				fwrite(track->event[j].data, 1, track->event[j].datasize, midiFile);	//�f�[�^�{�̂���������
			}
		}
	}
	fclose(midiFile);
	return 0;
}

//MIDI�t�@�C�����������ލۂɕK�v�Ȋ֐��F�r�b�N�G���f�B�A���������݂Ȃ̂ŏ������ރo�C�g�𔽓]������
void MIDI::reverceByte(void* data, unsigned char datasize) {
	unsigned char* reverceData = new unsigned char[datasize];
	for (int i = 0; i < datasize; ++i) {
		reverceData[i] = reinterpret_cast<char*>(data)[datasize - i - 1];
	}
	for (int i = 0; i < datasize; ++i) {
		reinterpret_cast<char*>(data)[i] = reverceData[i];
	}
	delete[] reverceData;
}

//�f���^�^�C���̏������ݏ���(4�o�C�g��������ƌ��܂��Ă�̂ŁA�g�����͈ӎ����Ȃ��Ă��悢)
void MIDI::DeltaTimeWrite(FILE* midiFile, size_t time) {
	unsigned char cbuff = 0;
	if (time >= (1 << 23)) {		//1�o�C�g�ɂ�7�r�b�g�����g���Ȃ������l�����A���̏�ōő�T�C�Y�ł���4�o�C�g�����l������
		fwrite(&(cbuff = (((time & 0x0fe00000) | 0x10000000) >> 24)), 1, 1, midiFile);		//�u1111 1110 0000 0000 0000 0000 0000�v(�S28�r�b�g)�̃}�X�N���l����
	}
	if (time >= (1 << 15)) {
		fwrite(&(cbuff = (((time & 0x001fc000) | 0x00200000) >> 14)), 1, 1, midiFile);		//�u0000 0001 1111 1100 0000 0000 0000 �v
	}
	if (time >= (1 << 7)) {
		fwrite(&(cbuff = (((time & 0x00003f80) | 0x00004000) >> 7)), 1, 1, midiFile);		//�u0000 0000 0000 0011 1111 1000 0000�v
	}
	fwrite(&(cbuff = (time & 0x0000007f)), 1, 1, midiFile);									//�u0000 0000 0000 0000 0000 0111 1111�v
}

//�C�x���g�ɉ����ď������ރo�C�i����ς���(�m�[�g�C�x���g�͏ȗ��L�@�ɑΉ����邽�߂ɁA�������}�b�v�ɂ��̂܂܋L�q����Ă���)
void MIDI::EventWrite(FILE* midiFile, MIDI::Event& eve) {
	unsigned short metaEventBuff = 0;
	unsigned char cbuff = 0;
	switch (eve.type) {
	case MIDI::EVENT::COMMENT:
		metaEventBuff = (0xff << 8) | 0x01;	break;
	case MIDI::EVENT::COPY_RIGHT:
		metaEventBuff = (0xff << 8) | 0x02;	break;
	case MIDI::EVENT::SEQUENCE_TRACK_NAME:
		metaEventBuff = (0xff << 8) | 0x03;	break;
	case MIDI::EVENT::INSTRUMENT_NAME:
		metaEventBuff = (0xff << 8) | 0x04;	break;
	case MIDI::EVENT::LYRIC:
		metaEventBuff = (0xff << 8) | 0x05;	break;
	case MIDI::EVENT::MARKER:
		metaEventBuff = (0xff << 8) | 0x06;	break;
	case MIDI::EVENT::QUEUE_POINT:
		metaEventBuff = (0xff << 8) | 0x07;	break;
	case MIDI::EVENT::PROGRAM_NAME:
		metaEventBuff = (0xff << 8) | 0x08;	break;
	case MIDI::EVENT::DEVICE_NAME:
		metaEventBuff = (0xff << 8) | 0x09;	break;
	case MIDI::EVENT::CHANNEL_PREFIX:
		metaEventBuff = (0xff << 8) | 0x20;	break;
	case MIDI::EVENT::PORT_SPECIFIED:
		metaEventBuff = (0xff << 8) | 0x21;	break;
	case MIDI::EVENT::END_OF_TRACK:
		metaEventBuff = (0xff << 8) | 0x2f;	break;
	case MIDI::EVENT::SET_TEMPO:
		metaEventBuff = (0xff << 8) | 0x51;	break;
	case MIDI::EVENT::TIME_SIGNATURE:
		metaEventBuff = (0xff << 8) | 0x58;	break;
	case MIDI::EVENT::KEY_SIGNATURE:
		metaEventBuff = (0xff << 8) | 0x59;	break;

	default:
		if (eve.type == MIDI::EVENT::SYSEX || eve.type == MIDI::EVENT::F7_SYSEX) {
			if (eve.type == MIDI::EVENT::SYSEX) {
				fwrite(&(cbuff = 0xf0), 1, 1, midiFile);
			}
			else {
				fwrite(&(cbuff = 0xf7), 1, 1, midiFile);
			}
			size_t len = eve.datasize;
			if (len >= (1 << 23)) {		//1�o�C�g�ɂ�7�r�b�g�����g���Ȃ������l�����A���̏�ōő�T�C�Y�ł���4�o�C�g�����l������
				fwrite(&(cbuff = (((len & 0x0fe00000) | 0x10000000) >> 24)), 1, 1, midiFile);		//�u1111 1110 0000 0000 0000 0000 0000�v(�S28�r�b�g)�̃}�X�N���l����
			}
			if (len >= (1 << 15)) {
				fwrite(&(cbuff = (((len & 0x001fc000) | 0x00200000) >> 14)), 1, 1, midiFile);		//�u0000 0001 1111 1100 0000 0000 0000 �v
			}
			if (len >= (1 << 7)) {
				fwrite(&(cbuff = (((len & 0x00003f80) | 0x00004000) >> 7)), 1, 1, midiFile);		//�u0000 0000 0000 0011 1111 1000 0000�v
			}
			fwrite(&(cbuff = (len & 0x0000007f)), 1, 1, midiFile);									//�u0000 0000 0000 0000 0000 0111 1111�v
		}
		return;
	}
	reverceByte((void*)&metaEventBuff, 2);
	fwrite(&metaEventBuff, 1, 2, midiFile);
	fwrite(&(cbuff = (unsigned char)eve.datasize), 1, 1, midiFile);		//�f�[�^�̒�������������
}