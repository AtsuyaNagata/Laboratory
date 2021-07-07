#include "Melody.h"
#include "Chords.h"
#include "RhythmPattern.h"

Melody::Melody() {

}

void Melody::create(RhythmPattern* rhythmPattern, Chords* chords, vector<uint32_t> chordLengths) {
	//�R�[�h�̐��ƃR�[�h�̒����̔z��̐�����v���Ă��Ȃ������炨�������̂ŗ��Ƃ�
	if (chords->getChords().size() != chordLengths.size()) {
		return;
	}
	//�����f�B�̏�Ԃ�ێ����邽�߂̍\��
	enum class MelodyState : unsigned char {
		Inner,			//����
		Embroidery,		//�h�J��
		Progress,		//�o�߉�
		Lean,			//�߉�
		None			//���̑�
	} melodyState = MelodyState::None;					//�O��̃����f�B�̏��

	int nowCreatePosition = 0;							//���ݐ������悤�Ƃ��Ă���m�[�g�̐����ʒu�̎��Ԃ̃|�W�V������\������
	int nowChordIndex = 0;								//���ݐ������悤�Ƃ��Ă���m�[�g�̎Q�ƈʒu�ɑΉ�����R�[�h�̎�ނ��������߂̃C���f�b�N�X
	int index = 0;										//����̃m�[�g�̃C���f�b�N�X�l������B
	int prevChordIndex;									//�O��̃��[�v�̃C���f�b�N�X�l��ۑ����Ă����o�b�t�@
	int baseNoteNum = chords->getBaseNoteNum() + 24;	//Chords�N���X�̏��L�����ꉹ�̒l����2�I�N�^�[�u��̈ʒu�������f�B�����̊�ꉹ�Ƃ���
	const int* floor = chords->getFloor();				//����g�p����K�����l�����Ă���(7�̔z��ŌŒ肳��Ă�)
	for (int i = 0; i < rhythmPattern->getRhythmPattern().size(); ++i) {
		//==========�m�[�g�����������s�O�̏�������==========
		RhythmPattern::Rhythm rhythm = rhythmPattern->getRhythmPattern()[i];		//����̎���ɂ����Ẵ��Y���\���𒊏o
		Chords::Chord chord = chords->getChords()[nowChordIndex];				//����̎���ɂ����ẴR�[�h�\���𒊏o
		uint8_t note;															//�m�[�g�̒l���ꎞ�I�ɂ����Ă����o�b�t�@
		int baseIndex = chord.baseIndex;
		//==========���C���̃����f�B��������==========
		if (rhythm.on == true) {													//����̃��Y���p�^�[�����x���̎��͔�΂�
			int randState = (std::rand() - 1) * 100 / RAND_MAX;						//�����f�B�̏�ԑJ�ڂ����肷�邽�߂̗����l(0����99�܂ł̒l���o��)

			if (melodyState == MelodyState::Inner) {
				//20���̊m��
				if (randState < 20) {
					//�����F�R�[�h���\�����鉹
					melodyState = MelodyState::Inner;
					int randBuff = (std::rand() - 1) * 3 / RAND_MAX;				//�ꎞ�I�ȃo�b�t�@
					index = (baseIndex + ((randBuff == 0) ? 0 : ((randBuff == 1) ? 2 : 4))) % 7;	//������I�����邽�߂̎�
					note = baseNoteNum + floor[index];						//�m�[�g�l�̐ݒ�
					prevChordIndex = index;
				}
				//30���̊m��
				else if (randState < 50) {
					//�h�J���F���������̓����ŋ��܂�鉹�B�����̈ꉹ�ォ���������ꉹ���̉����g��
					melodyState = MelodyState::Embroidery;
					int randBuff = (std::rand() - 1) * 2 / RAND_MAX;		//�ꎞ�I�ȃo�b�t�@
					index = (prevChordIndex + ((randBuff == 0) ? -1 : 1)) % 7;		//�h�J����I�����邽�߂̎�
					if (index <= -1) { index = 1; }		//�C���f�b�N�X�����̕����ɍs���߂��Ă���߂�
					note = baseNoteNum + floor[index];	//�m�[�g�l�̐ݒ�
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
					note = baseNoteNum + floor[index];	//�m�[�g�l�̐ݒ�
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
					note = baseNoteNum + floor[index];
					//���������ʂɕύX��������
				}
			}
			//�h�J���̑���
			else if (melodyState == MelodyState::Embroidery) {
				//90���̊m���œ�����I������
				if (randState < 90) {
					melodyState = MelodyState::Inner;
					index = prevChordIndex;			//�O��̓��������̂܂܎g�p����
					note = baseNoteNum + floor[index];
				}
				else {
					//��批�F�h�J��������ɖ߂����A�������邱��(����܂�o�Ȃ��悤�ɂ���)
					//�퉹�F�h�J����������ɖ߂����Ɏh�J�����������ɕʂ̉��֓W�J������(����܂�o�Ȃ��悤�ɂ���)
					//�|�����F�߉��̑���ɓ��������̓������^�C�łȂ���i����܂łȂ��悤�Ɂj
					//�v�̓e�L�g�[�ȉ�������
					melodyState = MelodyState::None;				//����`��Ԃɂ��Ă���
					index = (std::rand() - 1) * 7 / RAND_MAX;		//�ꎞ�I�ȃo�b�t�@
					note = baseNoteNum + floor[index];
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
				note = baseNoteNum + octaveBuff + floor[index];
				prevChordIndex = index;
			}
			//�o�߉��̑���
			else if (melodyState == MelodyState::Progress) {
				//��΂ɓ����ɑ����̓���������
				melodyState = MelodyState::Inner;
				index = (index + ((prevChordIndex - index > 0) ? -1 : 1)) % 7;
				if (index <= -1) { index = 6; }		//�C���f�b�N�X�����̕����ɍs���߂��Ă���߂�
				note = baseNoteNum + floor[index];
			}
			//�O�񂪖���`��Ԃ�������
			else if (melodyState == MelodyState::None) {
				//1/2�̊m���œ����A1/2�̊m���Ř߉�
				if ((std::rand() - 1) * 2 / RAND_MAX == 0) {
					//����
					melodyState = MelodyState::Inner;
					int randBuff = (std::rand() - 1) * 3 / RAND_MAX;		//�ꎞ�I�ȃo�b�t�@
					index = (baseIndex + ((randBuff == 0) ? 0 : ((randBuff == 1) ? 2 : 4))) % 7;	//������I�����邽�߂̎�
					note = baseNoteNum + floor[index];	//�m�[�g�l�̐ݒ�
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
					note = baseNoteNum + floor[index];
				}
			}
			//�m�肵���m�[�g�̍�����Melody�Ƃ��ăv�b�V������
			mMelody.push_back(note);
		}
		//============================================

		nowCreatePosition += rhythm.length;		//���̎���̂��߂�length�����Z���Ă���
		//�R�[�h�J�ڂ��s��ꂽ���ǂ����̔���
		if (nowCreatePosition > chordLengths[nowChordIndex]) {
			nowCreatePosition -= chordLengths[nowChordIndex];
			nowChordIndex++;
		}
	}
}