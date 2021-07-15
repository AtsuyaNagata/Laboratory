#include "Melody.h"
#include "Chords.h"
#include "RhythmPattern.h"
#include <iostream>
#include <algorithm>

Melody::Melody() {

}

void Melody::createMelody(const RhythmPattern* rhythmPattern, const Chords* chordString) {
	//�ŏ��ɍ폜���������Ă���
	mNotes.clear();

	//�����f�B�̏�Ԃ�ێ����邽�߂̍\��
	enum class MelodyState : unsigned char {
		Inner,			//����
		Embroidery,		//�h�J��
		Progress,		//�o�߉�
		Lean,			//�߉�
		None			//���̑�
	} melodyState = MelodyState::None;					//�O��̃����f�B�̏��

	RhythmPattern::Rhythm rhythm = rhythmPattern->getRhythmPattern()[0];	//����̎���ɂ����Ẵ��Y���\���𒊏o(�����f�B�̃��Y����0�Ԗڂ̃C���f�b�N�X�Ɉ�������݂���͂�)
	vector<Chords::Chord> chords = chordString->getChords();				//�R�[�h����l�����Ă���

	//�����A�R�[�h�̍��v���� < �����f�B���Y���̍ő�J�n�n�_��������ُ�l�Ȃ̂Ő��������ɗ��Ƃ�
	if (chordString->getLength() < rhythm.position[rhythm.position.size() - 1].startTime) {
		return;
	}

	uint32_t nowCreatePosition;								//���ݐ������悤�Ƃ��Ă���ʒu(�l����������̒����F256)
	uint32_t nowChordLength = chords[0].length;				//�����[�v�̃R�[�h�̒���(�����l�͈�ԍŏ��̃R�[�h�̒���)
	int nowChordIndex = 0;									//���ݐ������悤�Ƃ��Ă���m�[�g�̎Q�ƈʒu�ɑΉ�����R�[�h�̎�ނ��������߂̃C���f�b�N�X
	int index = 0;											//����̃��[�v�̃m�[�g�̃C���f�b�N�X�l������B
	int prevChordIndex;										//�O��̃��[�v�̃C���f�b�N�X�l��ۑ����Ă����o�b�t�@
	int baseNoteNum = chordString->getBaseNoteNum() + 24;	//Chords�N���X�̏��L�����ꉹ�̒l����2�I�N�^�[�u��̈ʒu�������f�B�����̊�ꉹ�Ƃ���
	const int* floor = chordString->getFloor();				//����g�p����K�����l�����Ă���(7�̔z��ŌŒ肳��Ă�)

	//�����f�B�̃��Y���p�^�[���𒊏o���鏈��
	if (rhythmPattern->getRhythmPattern().size() > 1 || rhythmPattern->getRhythmPattern()[0].type != RhythmPattern::RhythmType::Melody) {
		//���Y���p�^�[���N���X�̍\������v���Ȃ��̂ŃG���[�Ƃ��ĉ������������ɗ��Ƃ�
		return;
	}

	//�����f�B�̐����������C�����[�v
	for (int i = 0; i < rhythm.position.size(); ++i) {
		//==========�m�[�g�����������s�O�̏�������==========
		nowCreatePosition = rhythm.position[i].startTime;				//����̎���̂��߂�length���X�V���Ă���
		//�R�[�h�J�ڂ��s��ꂽ���ǂ����̔���
		for (int j = nowChordIndex; j < chords.size(); ++j) {
			//�����_�̃R�[�h�̒��������݂̐����ʒu�𒴂���܂ŏ������s��
			if (nowCreatePosition < nowChordLength) {					//��Ɂu���݂̃R�[�h�̍��v�� > ���݂̐����ʒu�v�̊֌W��ۂl�ɂ���
				break;
			}
			//���̃R�[�h�̒��������Z���Ă���
			nowChordIndex++;											//�R�[�h�̃C���f�b�N�X�����Z����(�R�[�h�̒����̏��͉��Z�݂̂ŎZ�o�����̂ŁA�C���f�b�N�X�̈����͂��������ōs���l�ɂ���)
			nowChordLength += chords[nowChordIndex].length;				//�X�V�����C���f�b�N�X�l�̃R�[�h�̒��������݂̃R�[�h���ɉ��Z����
		}

		Chords::Chord chord = chords[nowChordIndex];					//����̎���ɂ����ẴR�[�h�\���𒊏o

		Note note;														//�m�[�g�̒l���ꎞ�I�ɂ����Ă����o�b�t�@
		note.velocity = 100;											//���G�ȉ��ʒ����͍��͍l���Ȃ�
		note.startTime = rhythm.position[i].startTime;					//���Y���f�[�^����J�n�n�_�ƒ������i�[����
		note.length = rhythm.position[i].length;						//��ɓ���

		//�R�[�h�̊��̃C���f�b�N�X�l���l�����Ă���
		int baseIndex = -1;
		for (int j = 0; j < 7; ++j) {
			if (floor[j] == chord.baseIndex) {
				baseIndex = j;
				break;
			}
		}
		//�����K���Ɋ܂܂�Ȃ������R�[�h�̊�ꉹ�������ꍇ�A���̃R�[�h�̓�����K���ɑI��Ō��肷��(���A���S���Y���Ƃ��Ď������Ă�)
		if (baseIndex == -1) {
			int randBuff = (std::rand() - 1) * 3 / RAND_MAX;				//�ꎞ�I�ȃo�b�t�@
			index = (baseIndex + ((randBuff == 0) ? 0 : ((randBuff == 1) ? 2 : 4))) % 7;	//������I�����邽�߂̎�
			note.num = baseNoteNum + chord.baseIndex + floor[index];
			mNotes.push_back(note);
			continue;
		}

		//==========���C���̃����f�B��������==========
		int randState = (std::rand() - 1) * 100 / RAND_MAX;						//�����f�B�̏�ԑJ�ڂ����肷�邽�߂̗����l(0����99�܂ł̒l���o��)

		if (melodyState == MelodyState::Inner) {
			//20���̊m��
			if (randState < 20) {
				//�����F�R�[�h���\�����鉹
				melodyState = MelodyState::Inner;
				int randBuff = (std::rand() - 1) * 3 / RAND_MAX;				//�ꎞ�I�ȃo�b�t�@
				index = (baseIndex + ((randBuff == 0) ? 0 : ((randBuff == 1) ? 2 : 4))) % 7;	//������I�����邽�߂̎�
				note.num = baseNoteNum + floor[index];						//�m�[�g�l�̐ݒ�
				prevChordIndex = index;
			}
			//30���̊m��
			else if (randState < 50) {
				//�h�J���F���������̓����ŋ��܂�鉹�B�����̈ꉹ�ォ���������ꉹ���̉����g��
				melodyState = MelodyState::Embroidery;
				int randBuff = (std::rand() - 1) * 2 / RAND_MAX;		//�ꎞ�I�ȃo�b�t�@
				index = (prevChordIndex + ((randBuff == 0) ? -1 : 1)) % 7;		//�h�J����I�����邽�߂̎�
				if (index <= -1) { index = 1; }		//�C���f�b�N�X�����̕����ɍs���߂��Ă���߂�
				note.num = baseNoteNum + floor[index];	//�m�[�g�l�̐ݒ�
			}
			//30���̊m��
			else if (randState < 80) {
				//�o�߉��F�Ⴄ2�̓������Ȃ������g��
				melodyState = MelodyState::Progress;
				if ((prevChordIndex - baseIndex) % 7 == 0) {							//�}�C�i�X���ƈُ퓮�삷��̂�+7���Ă���
					index = (prevChordIndex + 1) % 7;
				}
				else if (((prevChordIndex - baseIndex) + 7) % 7 == 2) {				//�}�C�i�X���ƈُ퓮�삷��̂�+7���Ă���
					int randBuff = (std::rand() - 1) * 2 / RAND_MAX;				//0or1�̒l���i�[����
					index = (prevChordIndex + ((randBuff == 0) ? -1 : 1)) % 7;		//�o�߉���I�����邽�߂̎�
				}
				else {
					index = (prevChordIndex - 1) % 7;
				}
				if (index <= -1) { index = 6; }		//�C���f�b�N�X�����̕����ɍs���߂��Ă���߂�
				note.num = baseNoteNum + floor[index];	//�m�[�g�l�̐ݒ�
			}
			//20���̊m��
			else {
				//�߉��F�����̑O�ɒ��߂Ɉ�ォ������������̉����g��
				melodyState = MelodyState::Lean;
				int randBuff = (std::rand() - 1) * 3 / RAND_MAX;		//�ꎞ�I�ȃo�b�t�@
				//�����ȊO�̉��������I������
				switch (randBuff) {
				case 0:
					index = (baseIndex + 1) % 7;
					break;
				case 1:
					index = (baseIndex + 3) % 7;
					break;
				case 2:
					index = (baseIndex + 5) % 7;
					break;
					/*case 3:
						index = (baseIndex + 6) % 7;
						break;*/
				}
				note.num = baseNoteNum + floor[index];
				//���������ʂɕύX��������
			}
		}
		//�h�J���̑���
		else if (melodyState == MelodyState::Embroidery) {
			//90���̊m���œ�����I������
			if (randState < 90) {
				melodyState = MelodyState::Inner;
				index = prevChordIndex;			//�O��̓��������̂܂܎g�p����
				note.num = baseNoteNum + floor[index];
			}
			else {
				//��批�F�h�J��������ɖ߂����A�������邱��(����܂�o�Ȃ��悤�ɂ���)
				//�퉹�F�h�J����������ɖ߂����Ɏh�J�����������ɕʂ̉��֓W�J������(����܂�o�Ȃ��悤�ɂ���)
				//�|�����F�߉��̑���ɓ��������̓������^�C�łȂ���i����܂łȂ��悤�Ɂj
				//�v�̓e�L�g�[�ȉ�������
				melodyState = MelodyState::None;				//����`��Ԃɂ��Ă���
				index = (std::rand() - 1) * 7 / RAND_MAX;		//�ꎞ�I�ȃo�b�t�@
				note.num = baseNoteNum + floor[index];
			}
		}
		//�߉��̑���
		else if (melodyState == MelodyState::Lean) {
			//��΂ɓ���������
			melodyState = MelodyState::Inner;
			int octaveBuff = 0;
			//�K�������̂�����ԏ������l���������
			if (index == (baseIndex + 6) % 7) {
				index = baseIndex;
				//A�����ƃT�r�̎��A�I�N�^�[�u���グ��
				/*if (mMelody == Melody::A || mMelody == Melody::Climax) {
					octaveBuff = 12;
				}*/
			}
			//�K�������̂�����ԑ傫���l���������
			else if (index == (baseIndex + 5) % 7) {
				index = (baseIndex + 4) % 7;
			}
			else {
				//��ł����ł��ړ����Ă�����
				if ((std::rand() - 1) * 2 / RAND_MAX == 0) {
					index = (index - 1) % 7;
				}

			}
			if (index <= -1) { index = 6; octaveBuff = 0; }		//�C���f�b�N�X�����̕����ɍs���߂��Ă���߂�(�Ă΂�Ȃ������O��ł͂���)
			note.num = baseNoteNum + octaveBuff + floor[index];
			prevChordIndex = index;
		}
		//�o�߉��̑���
		else if (melodyState == MelodyState::Progress) {
			//��΂ɓ����ɑ����̓���������
			melodyState = MelodyState::Inner;
			index = (index + ((prevChordIndex - index > 0) ? -1 : 1)) % 7;
			if (index <= -1) { index = 6; }		//�C���f�b�N�X�����̕����ɍs���߂��Ă���߂�
			note.num = baseNoteNum + floor[index];
		}
		//�O�񂪖���`��Ԃ�������
		else if (melodyState == MelodyState::None) {
			//1/2�̊m���œ����A1/2�̊m���Ř߉�
			if ((std::rand() - 1) * 2 / RAND_MAX == 0) {
				//����
				melodyState = MelodyState::Inner;
				int randBuff = (std::rand() - 1) * 3 / RAND_MAX;		//�ꎞ�I�ȃo�b�t�@
				index = (baseIndex + ((randBuff == 0) ? 0 : ((randBuff == 1) ? 2 : 4))) % 7;	//������I�����邽�߂̎�
				note.num = baseNoteNum + floor[index];	//�m�[�g�l�̐ݒ�
				prevChordIndex = index;
			}
			else {
				//�߉�
				melodyState = MelodyState::Lean;
				int randBuff = (std::rand() - 1) * 3 / RAND_MAX;		//�ꎞ�I�ȃo�b�t�@
				//�����ȊO�̉��������I������4�Ԗڂ͉�����Ԏ��������̂ō폜
				switch (randBuff) {
				case 0:
					index = (baseIndex + 1) % 7;
					break;
				case 1:
					index = (baseIndex + 3) % 7;
					break;
				case 2:
					index = (baseIndex + 5) % 7;
					break;
					/*case 3:
						index = (baseIndex + 6) % 7;
						break;*/
				}
				note.num = baseNoteNum + floor[index];
			}
		}
		//�m�肵���m�[�g�̍�����Melody�̃m�[�g�Ƃ��ăv�b�V������
		mNotes.push_back(note);
		//============================================
	}

	return;
}

void Melody::createBase(const RhythmPattern* rhythmPattern, const Chords* chords, BaseAlgorithm baseAlgorithm) {
	//���݂͂��ꂾ��
	if (baseAlgorithm == BaseAlgorithm::Melo) {
		createBaseMelody(rhythmPattern, chords);
	}

	return;
}

//�Ƃ肠���������݂̂ŃV���v���ȃx�[�X���C������鎖���l����
void Melody::createBaseMelody(const RhythmPattern* rhythmPattern, const Chords* chordString) {
	//�ŏ��ɍ폜���������Ă���
	mNotes.clear();

	RhythmPattern::Rhythm rhythm = rhythmPattern->getRhythmPattern()[0];	//����̎���ɂ����Ẵ��Y���\���𒊏o(�����f�B�̃��Y����0�Ԗڂ̃C���f�b�N�X�Ɉ�������݂���͂�)
	vector<Chords::Chord> chords = chordString->getChords();				//�R�[�h����l�����Ă���

	//�����A�R�[�h�̍��v���� < �����f�B���Y���̍ő�J�n�n�_��������ُ�l�Ȃ̂Ő��������ɗ��Ƃ�
	if (chordString->getLength() < rhythm.position[rhythm.position.size() - 1].startTime) {
		return;
	}

	uint32_t nowCreatePosition;								//���ݐ������悤�Ƃ��Ă���ʒu(�l����������̒����F256)
	uint32_t nowChordLength = chords[0].length;				//�����[�v�̃R�[�h�̒���(�����l�͈�ԍŏ��̃R�[�h�̒���)
	int nowChordIndex = 0;									//���ݐ������悤�Ƃ��Ă���m�[�g�̎Q�ƈʒu�ɑΉ�����R�[�h�̎�ނ��������߂̃C���f�b�N�X
	int index = 0;											//����̃��[�v�̃m�[�g�̃C���f�b�N�X�l������B
	int prevChordIndex;										//�O��̃��[�v�̃C���f�b�N�X�l��ۑ����Ă����o�b�t�@
	int baseNoteNum = chordString->getBaseNoteNum() - 12;	//Chords�N���X�̏��L�����ꉹ�̒l����1�I�N�^�[�u���̈ʒu���x�[�X���C�������̊�ꉹ�Ƃ���
	const int* floor = chordString->getFloor();				//����g�p����K�����l�����Ă���(7�̔z��ŌŒ肳��Ă�)

	//�����f�B�̐����������C�����[�v
	for (int i = 0; i < rhythm.position.size(); ++i) {
		//==========�m�[�g�����������s�O�̏�������==========
		nowCreatePosition = rhythm.position[i].startTime;				//����̎���̂��߂�length���X�V���Ă���
		//�R�[�h�J�ڂ��s��ꂽ���ǂ����̔���
		for (int j = nowChordIndex; j < chords.size(); ++j) {
			//�����_�̃R�[�h�̒��������݂̐����ʒu�𒴂���܂ŏ������s��
			if (nowCreatePosition < nowChordLength) {					//��Ɂu���݂̃R�[�h�̍��v�� > ���݂̐����ʒu�v�̊֌W��ۂl�ɂ���
				break;
			}
			//���̃R�[�h�̒��������Z���Ă���
			nowChordIndex++;											//�R�[�h�̃C���f�b�N�X�����Z����(�R�[�h�̒����̏��͉��Z�݂̂ŎZ�o�����̂ŁA�C���f�b�N�X�̈����͂��������ōs���l�ɂ���)
			nowChordLength += chords[nowChordIndex].length;				//�X�V�����C���f�b�N�X�l�̃R�[�h�̒��������݂̃R�[�h���ɉ��Z����
		}

		Chords::Chord chord = chords[nowChordIndex];					//����̎���ɂ����ẴR�[�h�\���𒊏o

		Note note;														//�m�[�g�̒l���ꎞ�I�ɂ����Ă����o�b�t�@
		note.velocity = 100;			//���G�ȉ��ʒ����͍��͍l���Ȃ�
		note.startTime = rhythm.position[i].startTime;					//���Y���f�[�^����J�n�n�_�ƒ������i�[����
		note.length = rhythm.position[i].length;						//��ɓ���

		int baseIndex = - 1;								//�R�[�h�̊��̃m�[�g�l���l�����Ă���
		for (int j = 0; j < 7; ++j) {
			if (floor[j] == chord.baseIndex) {
				baseIndex = j;
				break;
			}
		}
		//�R�[�h�̊�ꉹ���K�����Ȃ����̉��A���S���Y��
		if (baseIndex == -1) {
			note.num = baseIndex + chord.baseIndex;
			mNotes.push_back(note); continue;
		}

		//==========���C���̃����f�B��������==========
		//80���̊m���őO��̃m�[�g�l���Q�Ƃ��Ēl���o��
		if ((std::rand() - 1) * 100 / RAND_MAX < 80) {
			//�������Ȃ�
		}
		//�c��̊m���őJ�ڂ��l����
		else {
			//�K���Ȋm���œ�����I������
			int randNum = (std::rand() - 1) * 100 / RAND_MAX;
			if (randNum < 30) {
				index = 2;
			}
			else if (randNum < 60) {
				index = 4;
			}
			else {
				index = 0;
			}
		}
		note.num = baseNoteNum + floor[(baseIndex + index) % 7];

		//�m�肵���m�[�g�̍�����Melody�̃m�[�g�Ƃ��ăv�b�V������
		
		mNotes.push_back(note);
	}

	return;
}

void Melody::createDrum(const RhythmPattern* rhythmPattern)
{
	//�ŏ��ɍ폜���������Ă���
	mNotes.clear();

	vector<RhythmPattern::Rhythm> rhythms = rhythmPattern->getRhythmPattern();
	//�n���ꂽ�f�[�^���h�����Z�b�g�̃T�C�Y�ƈ�v���Ă��Ȃ����
	if (!(rhythms.size() >= 7 && rhythmPattern->getRhythmPattern().size() <= 9)) {
		return;
	}
	for (int i = 0; i < rhythms.size(); ++i) {
		Note note;
		if (rhythms[i].type == RhythmPattern::RhythmType::BassDrum) {
			note.num = 36;					//GM�z��ɏ]���ăo�X�h�����̃m�[�g�ԍ���ݒ�
		}
		else if (rhythms[i].type == RhythmPattern::RhythmType::SnareDrum) {
			note.num = 40;					//GM�z��ɏ]���ăG���N�g���b�N�E�X�l�A�̃m�[�g�ԍ���ݒ�
		}
		else if (rhythms[i].type == RhythmPattern::RhythmType::FloorTom) {
			note.num = 43;					//GM�z��ɏ]���ăt���A�^���̃m�[�g�ԍ���ݒ�
		}
		else if (rhythms[i].type == RhythmPattern::RhythmType::Tom) {
			note.num = 50;					//GM�z��ɏ]���ăn�C�^���̃m�[�g�ԍ���ݒ�
		}
		else if (rhythms[i].type == RhythmPattern::RhythmType::CloseHiHatCymbal) {
			note.num = 42;					//GM�z��ɏ]���ăN���[�Y�n�C�n�b�g�̃m�[�g�ԍ���ݒ�
		}
		else if (rhythms[i].type == RhythmPattern::RhythmType::OpenHiHatCymbal) {
			note.num = 46;					//GM�z��ɏ]���ăI�[�v���n�C�n�b�g�̃m�[�g�ԍ���ݒ�
		}
		else if (rhythms[i].type == RhythmPattern::RhythmType::Cymbal) {
			note.num = 49;					//GM�z��ɏ]���ăN���b�V���V���o���̃m�[�g�ԍ���ݒ�
		}

		vector<RhythmPattern::Position> positions = rhythms[i].position;
		for (int j = 0; j < positions.size(); ++j) {
			note.startTime = positions[j].startTime;
			note.length = positions[j].length;
			note.velocity = 100;			//���G�ȉ��ʒ����͍��͍l���Ȃ�
			mNotes.push_back(note);
		}
	}
	//�Ō�Ƀ\�[�g�����ŏ����ɕ��ׂ�
	std::sort(mNotes.begin(),mNotes.end(), [](const Note& a, const Note& b) {
		return a.startTime < b.startTime;
		});
}