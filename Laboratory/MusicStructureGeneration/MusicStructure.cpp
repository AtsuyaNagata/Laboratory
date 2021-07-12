#include "MusicStructure.h"
#include <iostream>
using namespace std;

//�K�v�ȃf�[�^�F�L�[�A���y�̃p�[�g�i�s�A��Ԃ̒����A�R�[�h�p�^�[��(�M�^�[�A�s�A�m�ȂǊy��ɑΉ�������\��)�A���Y���p�^�[���A�����f�B�p�^�[���A�x�[�X���C��
MusicStructure::MusicStructure(int key, Scale scale, vector<MusicStructure::Part> parts, vector<uint32_t> lengths, vector<RhythmType> rhythms, vector<ChordAlgorithm> chordAls, vector<MelodyAlgorithm>meloAls, vector<BaseAlgorithm> baseAls) :
	mMusicStruct(nullptr),
	mRhythmPattern(nullptr),
	mChords(nullptr),
	mMelody(nullptr)
{
	mRhythmPattern = new RhythmPattern;
	mChords = new Chords;
	mMelody = new Melody;
	prepare(key, scale, parts, lengths, rhythms, chordAls, meloAls, baseAls);
}

//�Ȃ̐݌v�}���󂯎���ď���������R���X�g���N�^�B������g�����Ƃ͑����Ȃ�
MusicStructure::MusicStructure(const MusicStruct& mus) :
	mMusicStruct(nullptr),
	mRhythmPattern(nullptr),
	mChords(nullptr),
	mMelody(nullptr)
{
	mRhythmPattern = new RhythmPattern;
	mChords = new Chords;
	mMelody = new Melody;

	mMusicStruct = new MusicStruct;
	mMusicStruct->key = mus.key;
	mMusicStruct->scale = mus.scale;
	mMusicStruct->length = mus.length;
	mMusicStruct->parts = mus.parts;
	mMusicStruct->rhythms = mus.rhythms;
	mMusicStruct->chords = mus.chords;
	mMusicStruct->melodys = mus.melodys;
	mMusicStruct->bases = mus.bases;
	mMusicStruct->obbligatos = mus.obbligatos;
}

//�����Ȃ��̃R���X�g���N�^
MusicStructure::MusicStructure() :
	mMusicStruct(nullptr),
	mRhythmPattern(nullptr),
	mChords(nullptr),
	mMelody(nullptr)
{
	mRhythmPattern = new RhythmPattern;
	mChords = new Chords;
	mMelody = new Melody;
}

//�f�X�g���N�^
MusicStructure::~MusicStructure() {
	delete mMusicStruct;
	mMusicStruct = nullptr;
}

//�\�������������邽�߂̏���
void MusicStructure::prepare(int key, Scale scale, vector<MusicStructure::Part> parts, vector<uint32_t> lengths, vector<RhythmType> rhythms, vector<ChordAlgorithm> chordAls, vector<MelodyAlgorithm>meloAls, vector<BaseAlgorithm> baseAls)
{
	//�v�f�����ɑ��݂��Ă鎞�A��x�S�č폜���Ă��܂�(�v�f���ςɎc���ăo�O�ɋC�t���ɂ����Ȃ�̂�h�����߂̎d�l)
	if (mMusicStruct != nullptr) {
		delete mMusicStruct;
		mMusicStruct = nullptr;
	}
	//�e�\�������p�[�g���ƈ�v���Ȃ��ُ�ȓ��͂�������
	if (parts.size() != lengths.size() || parts.size() != rhythms.size() || parts.size() != chordAls.size() || parts.size() != meloAls.size() || parts.size() != baseAls.size()) {
		return;			//�\�������O�ɗ��Ƃ����ƂŁA���̎Q�Ǝ���nullptr��Ԃ��悤�ɂ���
	}
	mMusicStruct = new MusicStruct;

	//�����l�̐ݒ�B��̓I�ȃm�[�g�̐������̏����͊e�I�u�W�F�N�g�N���X�ōs���̂ŁA�����邽�߂ɂ����ł͒��ۓI�Ȓl�����i�[���Ȃ��B
	mMusicStruct->key = key;
	mMusicStruct->scale = scale;

	uint32_t length = 0;
	for (int i = 0; i < parts.size(); ++i) {
		//�p�[�g�̍\���������̒l����\�z����(Obbligato�\���͓���ȕ��Ȃ̂ŁA�����Ŋi�[�����͍s��Ȃ�)
		PartStruct ps;
		ps.type = parts[i];
		ps.length = lengths[i];
		ps.startTimes = length;
		mMusicStruct->parts.push_back(ps);			//�����������p�[�g�\�����v�b�V��(��������S�Ă̗v�f���i�[�\�ƂȂ��Ă���)

		RhythmStruct rs;
		rs.type = rhythms[i];
		rs.fill_in = false;
		mMusicStruct->rhythms.push_back(rs);		//�������������Y���\�����v�b�V��(��̓I�Ȓl�͊i�[����Ă��Ȃ�)

		ChordStruct cs;
		cs.type = chordAls[i];
		mMusicStruct->chords.push_back(cs);			//�����������R�[�h�\�����v�b�V��(��̓I�Ȓl�͊i�[����Ă��Ȃ�)

		MelodyStruct ms;
		ms.type = meloAls[i];
		mMusicStruct->melodys.push_back(ms);		//���������������f�B�\�����v�b�V��(��̓I�Ȓl�͊i�[����Ă��Ȃ�)

		BaseStruct bs;
		bs.type = baseAls[i];
		mMusicStruct->bases.push_back(bs);			//�����������x�[�X�\�����v�b�V��(��̓I�Ȓl�͊i�[����Ă��Ȃ�)

		length += lengths[i];						//���ݎQ�Ƃ����p�[�g�̍Ō���̈ʒu�̒l�ɂȂ�
	}
	mMusicStruct->length = length;
}

void MusicStructure::create()
{
	//�p�[�g���������l�ȏ������s��
	vector<PartStruct> parts = mMusicStruct->parts;
	for (int i = 0; i < parts.size(); ++i) {
		//==========�h�����̃��Y���p�^�[���𐶐�����==========
		RhythmStruct& rs = mMusicStruct->rhythms[i];
		rs.type = RhythmType::BasicDownBeat8;
		rs.fill_in = false;
		//�����̓h�����p�^�[���^�C�v�A��������h�����p�^�[���̒����A�t�B���C�������邩�̃t���O
		mRhythmPattern->createDrumRhythm(RhythmPattern::DrumPattern::BasicDownBeat8, parts[i].length, rs.fill_in);		//�܂���{�I��8�r�[�g�����������Ă��Ȃ�
		vector<RhythmPattern::Rhythm> drumPattern = mRhythmPattern->getRhythmPattern();

		//�����ُ�ȃT�C�Y���͂Ȃ痎�Ƃ�
		if (drumPattern.size() != 8) {
			return;
		}

		for (int j = 0; j < drumPattern.size(); ++j) {
			DrumPattern dp;
			//�h�����p�^�[���z��̃C���f�b�N�X�l���Ƃɕ��ޕ������鏈��
			if (j == 0) {
				dp.type = DrumType::BassDrum;
			}
			else if (j == 1) {
				dp.type = DrumType::SnareDrum;
			}
			else if (j == 2) {
				dp.type = DrumType::CloseHiHatCymbal;
			}
			else if (j == 3) {
				dp.type = DrumType::OpenHiHatCymbal;
			}
			else if (j == 4) {
				dp.type = DrumType::Tom;
			}
			else if (j == 5) {
				dp.type = DrumType::FloorTom;
			}
			else if (j == 6) {
				dp.type = DrumType::CrashCymbal;
			}
			else if (j == 7) {
				dp.type = DrumType::RideCymbal;
			}
			for (int k = 0; k < drumPattern[j].position.size(); ++k) {
				dp.startTimes.push_back(drumPattern[j].position[k].startTime);
			}
			rs.drumPatterns.push_back(dp);
		}

		//==========�R�[�h�i�s�𐶐�����==========
		ChordStruct& cs = mMusicStruct->chords[i];
		Chords::Scale scale = Chords::Scale::None;
		if (mMusicStruct->scale == Scale::Major) {
			scale = Chords::Scale::Major;
		}
		else if (mMusicStruct->scale == Scale::Minor) {
			scale = Chords::Scale::Minor;
		}
		else if (mMusicStruct->scale == Scale::HarmonicMinor) {
			scale = Chords::Scale::HarmonicMinor;
		}
		else if (mMusicStruct->scale == Scale::MelodicMinor) {
			scale = Chords::Scale::MelodicMinor;
		}
		//�R�[�h�̐����֐����Ăяo��
		mChords->create(parts[i].length / (256 * 4), parts[i].length, mMusicStruct->key, scale);
		//���������R�[�h��p���ċȂ̐݌v�}�̃f�[�^�𖄂߂�
		cs.chords = mChords->getChords();

		//==========�����f�B�𐶐�����==========
		//TODO�F�G���[���o�Ă�
		MelodyStruct& ms = mMusicStruct->melodys[i];
		mRhythmPattern->createMelodyRhythm(parts[i].length, 128);		//�����f�B�p�̃��Y���p�^�[���𐶐��B8���������ŏ��P�ʂƂ��Ă�(�v�C��)
		mMelody->createMelody(mRhythmPattern, mChords);
		vector<Melody::Note> melodyNotes = mMelody->getMelodyNotes();
		for (int j = 0; j < melodyNotes.size(); ++j) {
			Note note;
			note.length = melodyNotes[i].length;
			note.num = melodyNotes[i].num;
			note.startTime = melodyNotes[i].startTime;

			ms.notes.push_back(note);
		}

		//==========�x�[�X���C���𐶐�����==========
		BaseStruct& bs = mMusicStruct->bases[i];
		mRhythmPattern->createMelodyRhythm(parts[i].length, 128);						//�����f�B�p�̃��Y���p�^�[���𗬗p���Ă�B8���������ŏ��P�ʂƂ��Ă�(�v�C��)
		mMelody->createBase(mRhythmPattern, mChords, Melody::BaseAlgorithm::Melo);		//�x�[�X���C���𐶐�(���݂̓����f�B���ɍ��A���S���Y���̂ݓ��삵�Ă�)
		vector<Melody::Note> baseNote = mMelody->getMelodyNotes();
		for (int j = 0; j < baseNote.size(); ++j) {
			Note note;
			note.length = melodyNotes[i].length;
			note.num = melodyNotes[i].num;
			note.startTime = melodyNotes[i].startTime;

			bs.notes.push_back(note);
		}
	}
}

void MusicStructure::printMusicStruct()
{
	cout << "�Ȃ̃L�[�F" << mMusicStruct->key << endl;
	cout << "�Ȃ̃X�P�[���F";
	if (mMusicStruct->scale == Scale::Major) {
		cout << "���W���[";
	}
	else if (mMusicStruct->scale == Scale::Minor) {
		cout << "�}�C�i�[";
	}
	else if (mMusicStruct->scale == Scale::HarmonicMinor) {
		cout << "�n�[���j�b�N�}�C�i�[";
	}
	else if (mMusicStruct->scale == Scale::MelodicMinor) {
		cout << "�����f�B�b�N�}�C�i�[";
	}
	cout << endl << endl;

	//�e�p�[�g�ɂ��ĕ\�����鏈��
	for (int i = 0; i < mMusicStruct->parts.size(); ++i) {
		//==========�p�[�g�̏��̕`��==========
		cout << "�p�[�g" << i << "�̎�ށF";
		if (mMusicStruct->parts[i].type == Part::A) {
			cout << "A";
		}
		else if (mMusicStruct->parts[i].type == Part::B) {
			cout << "B";
		}
		else if (mMusicStruct->parts[i].type == Part::C) {
			cout << "C";
		}
		else if (mMusicStruct->parts[i].type == Part::Climax) {
			cout << "Climax";
		}
		else if (mMusicStruct->parts[i].type == Part::Intro) {
			cout << "Intro";
		}
		else if (mMusicStruct->parts[i].type == Part::Outro) {
			cout << "Outro";
		}
		else if (mMusicStruct->parts[i].type == Part::Interlude) {
			cout << "Interlude";
		}
		else if (mMusicStruct->parts[i].type == Part::Special) {
			cout << "Special";
		}
		cout << endl;
		cout << "�p�[�g�̊J�n���ԁF" << mMusicStruct->parts[i].startTimes << "�@�p�[�g�̒����F" << mMusicStruct->parts[i].length << endl << endl;

		//==========�h�����̃��Y���p�^�[���ɂ��Ă̕`��==========
		cout << "�h�����̃p�^�[���̎�ށF";
		if (mMusicStruct->rhythms[i].type == RhythmType::BasicDownBeat8) {
			cout << "��{8�r�[�g";
		}
		else if (mMusicStruct->rhythms[i].type == RhythmType::BasicUpBeat8) {
			cout << "��{8�r�[�g ���ł�";
		}
		else if (mMusicStruct->rhythms[i].type == RhythmType::HalfTimeShuffle) {
			cout << "�n�[�t�^�C���V���b�t��";
		}
		else if (mMusicStruct->rhythms[i].type == RhythmType::ShuffleBeat) {
			cout << "�V���b�t���r�[�g";
		}
		else if (mMusicStruct->rhythms[i].type == RhythmType::SlipBeat8) {
			cout << "�X���b�v8�r�[�g";
		}
		cout << endl << endl;

		//==========�R�[�h��ɂ��Ă̕`��==========
		cout << "�R�[�h�����A���S���Y���F";
		if (mMusicStruct->chords[i].type == ChordAlgorithm::Simple) {
			cout << "�V���v��";
		}
		else if (mMusicStruct->chords[i].type == ChordAlgorithm::Fashionable) {
			cout << "�I�V����";
		}
		else if (mMusicStruct->chords[i].type == ChordAlgorithm::Modulation) {
			cout << "�]��";
		}
		vector<Chords::Chord> chords = mMusicStruct->chords[i].chords;
		cout << "�@�R�[�h��F";
		for (int j = 0; j < chords.size(); ++j) {
			//C�̂悤�ȃR�[�h�ɂ����ĊK�������̕`��
			if (chords[j].baseNoteNum % 12 == 0) {
				cout << "C";
			}
			else if (chords[j].baseNoteNum % 12 == 1) {
				cout << "C#";
			}
			else if (chords[j].baseNoteNum % 12 == 2) {
				cout << "D";
			}
			else if (chords[j].baseNoteNum % 12 == 3) {
				cout << "D#";

			}
			else if (chords[j].baseNoteNum % 12 == 4) {
				cout << "E";
			}
			else if (chords[j].baseNoteNum % 12 == 5) {
				cout << "F";
			}
			else if (chords[j].baseNoteNum % 12 == 6) {
				cout << "F#";
			}
			else if (chords[j].baseNoteNum % 12 == 7) {
				cout << "G";
			}
			else if (chords[j].baseNoteNum % 12 == 8) {
				cout << "A��";
			}
			else if (chords[j].baseNoteNum % 12 == 9) {
				cout << "A";
			}
			else if (chords[j].baseNoteNum % 12 == 10) {
				cout << "B��";
			}
			else if (chords[j].baseNoteNum % 12 == 11) {
				cout << "B";
			}

			//�a���̎�ނɂ��ċL�q����
			if (chords[j].type == Chords::ChordType::Major) {
				//�����L�q���Ȃ�
			}
			else if (chords[j].type == Chords::ChordType::Minor) {
				cout << "m";
			}
			else if (chords[j].type == Chords::ChordType::Aug) {
				cout << "aug";
			}
			//�}�C�i�[��͏�����������Ȃ̂�2�`�揈��������
			else if (chords[j].type == Chords::ChordType::mb5) {
				if (chords[j].plus != Chords::ChordPlus::Dim7) {
					cout << "m";
				}
			}

			//4�ڂ̉��ɂ��Ă̕t�������L�q����
			if (chords[j].plus == Chords::ChordPlus::Seventh) {
				cout << "7";
			}
			else if (chords[j].plus == Chords::ChordPlus::MajorSeventh) {
				cout << "M7";
			}
			else if (chords[j].plus == Chords::ChordPlus::Six) {
				cout << "6";
			}
			else if (chords[j].plus == Chords::ChordPlus::Add9) {
				cout << "Add9";
			}
			else if (chords[j].plus == Chords::ChordPlus::Add11) {
				cout << "Add11";
			}
			else if (chords[j].plus == Chords::ChordPlus::Add13) {
				cout << "Add13";
			}
			else if (chords[j].plus == Chords::ChordPlus::Dim7) {
				cout << "Dim7";
			}

			//3�a���̍\���ɑ����邪�A4�a���̏�񂪕K�v�ɂȂ镔���̕`��
			if (chords[j].type == Chords::ChordType::Sus2) {
				cout << "sus2";
			}
			else if (chords[j].type == Chords::ChordType::Sus4) {
				cout << "sus4";
			}
			else if (chords[j].type == Chords::ChordType::mb5) {
				if (chords[j].plus != Chords::ChordPlus::Dim7) {
					cout << "-5";
				}
			}
			//5�ڂ̉��̕t�������L�q����
			if (chords[j].tension == Chords::ChordTension::FlatNinth) {
				cout << "��9";
			}
			else if (chords[j].tension == Chords::ChordTension::Ninth) {
				cout << "9";
			}
			else if (chords[j].tension == Chords::ChordTension::SharpNinth) {
				cout << "#9";
			}
			else if (chords[j].tension == Chords::ChordTension::Eleventh) {
				cout << "11";
			}
			else if (chords[j].tension == Chords::ChordTension::SharpEleventh) {
				cout << "#11";
			}
			else if (chords[j].tension == Chords::ChordTension::FlatThirteenth) {
				cout << "��13";
			}
			else if (chords[j].tension == Chords::ChordTension::Thirteenth) {
				cout << "13";
			}

			//�I���R�[�h�̋L�q
			if (chords[j].onIndex != chords[j].baseIndex) {					//�m�[�g�̊�ꉹ�ƃI���R�[�h�̒l����v���Ă��Ȃ����̂݃I���R�[�h�̕`����s��
				cout << "/";
				if (chords[j].onIndex == 0) {
					cout << "C";
				}
				else if (chords[j].onIndex == 1) {
					cout << "C#";
				}
				else if (chords[j].onIndex == 2) {
					cout << "D";
				}
				else if (chords[j].onIndex == 3) {
					cout << "D#";
				}
				else if (chords[j].onIndex == 4) {
					cout << "E";
				}
				else if (chords[j].onIndex == 5) {
					cout << "F";
				}
				else if (chords[j].onIndex == 6) {
					cout << "F#";
				}
				else if (chords[j].onIndex == 7) {
					cout << "G";
				}
				else if (chords[j].onIndex == 8) {
					cout << "A��";
				}
				else if (chords[j].onIndex == 9) {
					cout << "A";
				}
				else if (chords[j].onIndex == 10) {
					cout << "B��";
				}
				else if (chords[j].onIndex == 11) {
					cout << "B";
				}
			}
			//�I�~�b�g�R�[�h�͍��͍l���Ȃ�
			cout << "�@";
		}
		cout << endl << endl;

		//==========�����f�B�ɂ��Ă̕`��==========
		cout << "�����f�B�����A���S���Y���F";
		if (mMusicStruct->melodys[i].type == MelodyAlgorithm::Standard) {
			cout << "Standard";
		}
		cout << endl;
		vector<Note> notes = mMusicStruct->melodys[i].notes;
		cout << "�����f�B�̃m�[�g��F(�p�[�g���猩�đ��ΓI�ȊJ�n���ԁA�����A�m�[�g�ԍ�)" << endl;
		for (int j = 0; j < notes.size(); ++j) {
			cout << notes[j].startTime << ", " << notes[i].length << ", " << notes[i].num << "�@";
		}
		cout << endl << endl;

		//==========�x�[�X�ɂ��Ă̕`��==========
		cout << "�x�[�X�����A���S���Y���F";
		if (mMusicStruct->bases[i].type == BaseAlgorithm::Simple) {
			cout << "Simple";
		}
		else if (mMusicStruct->bases[i].type == BaseAlgorithm::UpDown) {
			cout << "Updown";
		}
		else if (mMusicStruct->bases[i].type == BaseAlgorithm::Melo) {
			cout << "Melo";
		}
		cout << endl;
		notes = mMusicStruct->bases[i].notes;
		cout << "�x�[�X�̃m�[�g��F(�p�[�g���猩�đ��ΓI�ȊJ�n���ԁA�����A�m�[�g�ԍ�)" << endl;
		for (int j = 0; j < notes.size(); ++j) {
			cout << notes[j].startTime << ", " << notes[i].length << ", " << notes[i].num << "�@";
		}
		cout << endl << endl;
	}
}