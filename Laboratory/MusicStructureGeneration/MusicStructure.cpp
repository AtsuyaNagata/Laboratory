#include "MusicStructure.h"
#include <iostream>
#include <string>
#include <fstream>
#include "MIDI.h"
#include <algorithm>
using namespace std;

//�K�v�ȃf�[�^�F�L�[�A���y�̃p�[�g�i�s�A��Ԃ̒����A�R�[�h�p�^�[��(�M�^�[�A�s�A�m�ȂǊy��ɑΉ�������\��)�A���Y���p�^�[���A�����f�B�p�^�[���A�x�[�X���C��
MusicStructure::MusicStructure(int key, Scale scale, vector<MusicStructure::Part> parts, vector<uint32_t> lengths, vector<RhythmType> rhythms, vector<ChordAlgorithm> chordAls, vector<MelodyAlgorithm>meloAls, vector<BaseAlgorithm> baseAls) :
	mMusicStruct(nullptr),
	mRhythmPattern(nullptr),
	mChords(nullptr),
	mMelody(nullptr),
	mMIDI(nullptr)
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
	mMelody(nullptr),
	mMIDI(nullptr)
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
	mMelody(nullptr),
	mMIDI(nullptr)
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
		mMelody->createDrum(mRhythmPattern);
		vector<Melody::Note> drumNotes = mMelody->getMelodyNotes();

		//�����ُ�ȃT�C�Y���͂Ȃ痎�Ƃ�
		if (drumPattern.size() != 7) {
			return;
		}
		for (int j = 0; j < drumNotes.size(); ++j) {
			Note note;
			note.length = drumNotes[j].length;
			note.num = drumNotes[j].num;
			note.startTime = drumNotes[j].startTime;
			note.velocity = 100;

			rs.notes.push_back(note);
		}

		//cout << "end of drum" << endl;

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
		mChords->create(parts[i].length / (256 * 4), parts[i].length, mMusicStruct->key, scale, Chords::Algorithm::Fashionable);
		//���������R�[�h��p���ċȂ̐݌v�}�̃f�[�^�𖄂߂�
		cs.chords = mChords->getChords();
		//cout << "end of chord" << endl;


		//==========�����f�B�𐶐�����==========
		MelodyStruct& ms = mMusicStruct->melodys[i];
		mRhythmPattern->createMelodyRhythm(parts[i].length, 128);		//�����f�B�p�̃��Y���p�^�[���𐶐��B8���������ŏ��P�ʂƂ��Ă�(�v�C��)
		mMelody->createMelody(mRhythmPattern, mChords);
		vector<Melody::Note> melodyNotes = mMelody->getMelodyNotes();
		for (int j = 0; j < melodyNotes.size(); ++j) {
			Note note;
			note.length = melodyNotes[j].length;
			note.num = melodyNotes[j].num;
			note.startTime = melodyNotes[j].startTime;
			note.velocity = 100;

			ms.notes.push_back(note);
		}
		//cout << "end of melody" << endl;

		//==========�x�[�X���C���𐶐�����==========
		BaseStruct& bs = mMusicStruct->bases[i];
		mRhythmPattern->createMelodyRhythm(parts[i].length, 128);						//�����f�B�p�̃��Y���p�^�[���𗬗p���Ă�B8���������ŏ��P�ʂƂ��Ă�(�v�C��)
		mMelody->createBase(mRhythmPattern, mChords, Melody::BaseAlgorithm::Melo);		//�x�[�X���C���𐶐�(���݂̓����f�B���ɍ��A���S���Y���̂ݓ��삵�Ă�)
		vector<Melody::Note> baseNote = mMelody->getMelodyNotes();
		for (int j = 0; j < baseNote.size(); ++j) {
			Note note;
			note.length = baseNote[j].length;
			note.num = baseNote[j].num;
			note.startTime = baseNote[j].startTime;
			note.velocity = 90;							//������Ə����߂̉��ɂ��Ă�

			bs.notes.push_back(note);
		}
		//cout << "end of base" << endl;

	}
}

//�ȍ\���Ő����������y����MIDI�t�@�C���ɏ������ޏ���
void MusicStructure::outputMIDI()
{
	//����MIDI�N���X�̃C���X�^���X�����݂��Ă��Ȃ�������
	if (mMIDI != nullptr) {
		delete mMIDI;
		mMIDI = nullptr;
	}

	mMIDI = new MIDI{};		//MIDI�\���𐶐����Ă���

	vector<Note> notes;
	string filename;
	//�e�p�[�g���Ƃ�MIDI�t�@�C���𐶐�����
	for (int i = 0; i < mMusicStruct->parts.size(); ++i) {
		//�n�߂Ƀh������MIDI�t�@�C���𐶐�����
		notes = mMusicStruct->rhythms[i].notes;
		filename = "./data/drum/drum_part" + to_string(i);
		filename += ".mid";
		writeMidiFile(filename.c_str(), notes);

		//���ɃR�[�h��MIDI�t�@�C���𐶐�����
		notes = chordToNote(mMusicStruct->chords[i].chords);			//�R�[�h����m�[�g�f�[�^���\�z���Ċi�[
		filename = "./data/chord/Chord_part" + to_string(i);
		filename += ".mid";
		writeMidiFile(filename.c_str(), notes);

		//����Ɏ��Ƀ����f�B��MIDI�t�@�C���𐶐�����
		notes = mMusicStruct->melodys[i].notes;
		filename = "./data/melody/melody_part" + to_string(i);
		filename += ".mid";
		writeMidiFile(filename.c_str(), notes);

		//�Ō�Ƀx�[�X��MIDI�t�@�C���𐶐�����
		notes = mMusicStruct->bases[i].notes;
		filename = "./data/base/base_part" + to_string(i);
		filename += ".mid";
		writeMidiFile(filename.c_str(), notes);
		
	}//mMIDI�̍\����MIDI�t�@�C���ɏ�������

	//�I�u���K�[�g�̏o��
	for (int i = 0; i < mMusicStruct->obbligatos.size(); ++i) {
		notes = mMusicStruct->obbligatos[i].notes;
		filename = "Obbligato" + to_string(i);
		filename += ".mid";
		writeMidiFile(filename.c_str(), notes);
	}

	return;
}

void MusicStructure::writeMidiFile(const char* filename, vector<Note> notes)
{
	//MIDI�\�����\�z����Ă��Ȃ�������ُ�Ȃ̂ŗ��Ƃ�
	if (mMIDI == nullptr) {
		return;
	}

	vector<MIDI::MIDINoteEvent> noteEvents;

	vector<Note> wNote;			//�������ݗp�̃f�[�^������̈�
	wNote = notes;				//�m�[�g�E�I���̗v�f���R�s�[

	//wNote�Ƀm�[�g�E�I�t�̃C�x���g�ƂȂ�v�f�����Ă����i���Ƀm�[�g�I���̏���wNote�ɓ����Ă�j
	Note noteBuff;
	int noteSize = wNote.size();
	for (int i = 0; i < noteSize; ++i) {
		//�m�[�g�I�t�̗v�f���i�[���Ă���
		noteBuff.startTime = notes[i].startTime + notes[i].length;		//�m�[�g�I�t�C�x���g�̊J�n���_�́u�Ή������m�[�g�I���C�x���g�̊J�n���_ + ���̒����v�ŕ\�����Ƃ��o����
		noteBuff.num = notes[i].num;									//�m�[�g�̒l�͈�v�����Ă���
		noteBuff.length = 0;											//�������͕K�v�Ȃ����𖾎����邽�߂�0���i�[���Ă�
		noteBuff.velocity = 0;											//�x���V�e�B��0�Ƃ��ē��͂��鎖�ŁA�m�[�g�I���C�x���g�Ńm�[�g�I�t��\���\�ƂȂ�

		wNote.push_back(noteBuff);
	}

	//�\�[�g�������Ԃŏ������邱�ƂŃm�[�g���m���ꕔ�d�Ȃ�����Ԃ͕\���\�����A�m�[�gin�m�[�g�̏�Ԃ͕\���Ȃ��Ȃ��Ă�B�����SMF�̎d�l�サ�傤���Ȃ�
	std::sort(wNote.begin(), wNote.end(), [](const Note& a, const Note& b) {
		return a.startTime < b.startTime;								//�m�[�g�̊J�n�n�_���������Ń\�[�g����
		});

	size_t time = 0;													//�f���^�^�C����\�����邽�߂Ƀ��[�v���̐i�񂾎��Ԓl���i�[����
	for (int i = 0; i < wNote.size(); ++i) {
		MIDI::MIDINoteEvent noteEvent;
		noteEvent.time = (wNote[i].startTime * 960 / 256) - time;		//���ΓI�Ȏ��Ԃ�����o���A�f���^�^�C���Ƃ���
		time += noteEvent.time;											//���ݎ��_���X�V����

		noteEvent.noteNum = wNote[i].num;								//���ԈȊO�̕K�v�Ȓl�����̂܂܊i�[����
		noteEvent.velocity = wNote[i].velocity;							//��ɓ���

		noteEvents.push_back(noteEvent);
	}
	mMIDI->createMIDI_oneTrack(noteEvents, wNote.size(), 120);			//1�g���b�N�����̗��\������̂ɍŒ���̗v�f���i�[����MIDI�I�u�W�F�N�g�𐶐�����mMIDI�ɓK��������
	mMIDI->MIDIWrite(filename);

	return;
}

//�R�[�h����m�[�g��𐶐�����B�A���y�W�I�Ȃǂ͍l�����A�V���v���ȃR�[�h�i�s�̃m�[�g�ɂȂ�B
vector<MusicStructure::Note> MusicStructure::chordToNote(vector<Chords::Chord> chords)
{
	uint32_t time = 0;			//���[�v���̌��ݎ��Ԃ�\��
	vector<Note> notes;			//�S�̓I�ȃm�[�g�f�[�^�̓��ꕨ�B����̊֐��ŕԂ�����
	Note note;					//�m�[�g�̔z��Ƀv�b�V���o�b�N���邽�߂̃o�b�t�@
	vector<Note> chordNotes;	//�R�[�h���\������m�[�g�Q��\��
	note.velocity = 100;		//���ʂ�100�ŌŒ�
	for (int i = 0; i < chords.size(); ++i) {
		//����R�[�h�̃m�[�g�Q�͏��������Ƃ�
		chordNotes.clear();
		//�ŏ��ɁA�����̃R�[�h���\������̂ɕK�v�Ȋ�{�����o�b�t�@�Ɋi�[����
		note.startTime = time;
		note.length = chords[i].length;
		note.num = chords[i].baseNoteNum + 0;			//��ꉹ�̃m�[�g�l
		chordNotes.push_back(note);						//��ꉹ�͋��ʂ̗v�f�Ȃ̂ŃR�[�h�̃m�[�g�Q�ɓ���Ă���

		//��������R�[�h���Ƃɕ\�����Ⴄ�v�f���i�[����
		//��{�̃R�[�h�\�����̊i�[
		if (chords[i].type == Chords::ChordType::Major) {
			//���W���[�R�[�h�̍\�����̃m�[�g�ԍ����i�[(�J�n���_�Ⓑ���͊�ꉹ�ƈꏏ)
			note.num = chords[i].baseNoteNum + 4;		//�R�[�h��2�ڂ̃m�[�g�l���i�[
			chordNotes.push_back(note);					//�R�[�h�̃m�[�g�Q�ɒǉ�
			note.num = chords[i].baseNoteNum + 7;		//�R�[�h��3�ڂ̃m�[�g�l���i�[
			chordNotes.push_back(note);					//�R�[�h�̃m�[�g�Q�ɒǉ�
		}
		else if (chords[i].type == Chords::ChordType::Minor) {
			//�}�C�i�[�R�[�h�̍\�����̃m�[�g�ԍ����i�[(�J�n���_�Ⓑ���͊�ꉹ�ƈꏏ)
			note.num = chords[i].baseNoteNum + 3;
			chordNotes.push_back(note);
			note.num = chords[i].baseNoteNum + 7;
			chordNotes.push_back(note);
		}
		else if (chords[i].type == Chords::ChordType::Aug) {
			//Aug�R�[�h�̍\�����̃m�[�g�ԍ����i�[(�J�n���_�Ⓑ���͊�ꉹ�ƈꏏ)
			note.num = chords[i].baseNoteNum + 4;
			chordNotes.push_back(note);
			note.num = chords[i].baseNoteNum + 8;
			chordNotes.push_back(note);
		}
		else if (chords[i].type == Chords::ChordType::mb5) {
			//mb5�R�[�h�̍\�����̃m�[�g�ԍ����i�[(�J�n���_�Ⓑ���͊�ꉹ�ƈꏏ)
			note.num = chords[i].baseNoteNum + 3;
			chordNotes.push_back(note);
			note.num = chords[i].baseNoteNum + 6;
			chordNotes.push_back(note);
		}
		else if (chords[i].type == Chords::ChordType::Sus2) {
			//sus2�R�[�h�̍\�����̃m�[�g�ԍ����i�[(�J�n���_�Ⓑ���͊�ꉹ�ƈꏏ)
			note.num = chords[i].baseNoteNum + 2;
			chordNotes.push_back(note);
			note.num = chords[i].baseNoteNum + 7;
			chordNotes.push_back(note);
		}
		else if (chords[i].type == Chords::ChordType::Sus4) {
			//sus4�R�[�h�̍\�����̃m�[�g�ԍ����i�[(�J�n���_�Ⓑ���͊�ꉹ�ƈꏏ)
			note.num = chords[i].baseNoteNum + 5;
			chordNotes.push_back(note);
			note.num = chords[i].baseNoteNum + 7;
			chordNotes.push_back(note);
		}

		//4���ڂ̃m�[�g�����݂���R�[�h��������A�R�[�h�̃m�[�g�Q��4���ڂ�����ɒǉ�����
		if (chords[i].plus == Chords::ChordPlus::Add9) {
			for (int j = 0; j < 7; ++j) {
				if (chords[i].baseIndex == mChords->getFloor()[j]) {
					uint8_t floorBuff = mChords->getFloor()[(j + 1) % 7] + 12 - mChords->getFloor()[j];		//���ΓI�Ɍ���2�x�̈ʒu�Ƀm�[�g��ǉ����邽�߂ɍ����v�Z����
					if (floorBuff > 12) {
						floorBuff -= 12;
					}
					note.num = chords[i].baseNoteNum + floorBuff;							//9�x����ꂽ������������̂ŁA�I�N�^�[�u����2�x������
					chordNotes.push_back(note);
					break;
				}
			}
		}
		else if (chords[i].plus == Chords::ChordPlus::Add11) {
			for (int j = 0; j < 7; ++j) {
				if (chords[i].baseIndex == mChords->getFloor()[j]) {
					uint8_t floorBuff = mChords->getFloor()[(j + 1) % 7] + 12 - mChords->getFloor()[j];		//���ΓI�Ɍ���4�x�̈ʒu�Ƀm�[�g��ǉ����邽�߂ɍ����v�Z����
					if (floorBuff > 12) {
						floorBuff -= 12;
					}
					note.num = chords[i].baseNoteNum + floorBuff;			//11�x����ꂽ������������̂ŁA�I�N�^�[�u����4�x������
					chordNotes.push_back(note);
					break;
				}
			}
		}
		else if (chords[i].plus == Chords::ChordPlus::Add13) {
			for (int j = 0; j < 7; ++j) {
				if (chords[i].baseIndex == mChords->getFloor()[j]) {
					uint8_t floorBuff = mChords->getFloor()[(j + 1) % 7] + 12 - mChords->getFloor()[j];		//���ΓI�Ɍ���6�x�̈ʒu�Ƀm�[�g��ǉ����邽�߂ɍ����v�Z����
					if (floorBuff > 12) {
						floorBuff -= 12;
					}
					note.num = chords[i].baseNoteNum + floorBuff;			//13�x����ꂽ������������̂ŁA�I�N�^�[�u����6�x������
					chordNotes.push_back(note);
					break;
				}
			}
		}
		else if (chords[i].plus == Chords::ChordPlus::Dim7) {
			if (chords[i].type == Chords::ChordType::mb5) {						//3�a�����}�C�i�[��t�@�C�u�łȂ����Dim�͖����������
				note.num = chords[i].baseNoteNum + 9;							//9������̉���ǉ�
				chordNotes.push_back(note);
			}
		}
		else if (chords[i].plus == Chords::ChordPlus::MajorSeventh) {
			note.num = chords[i].baseNoteNum + 11;								//��7�x�̉���ǉ�
			chordNotes.push_back(note);
		}
		else if (chords[i].plus == Chords::ChordPlus::Seventh) {
			note.num = chords[i].baseNoteNum + 10;								//�Z7�x�̉���ǉ�
			chordNotes.push_back(note);
		}
		else if (chords[i].plus == Chords::ChordPlus::Six) {
			note.num = chords[i].baseNoteNum + 9;								//��6�x�̉���ǉ�
			chordNotes.push_back(note);
		}

		//5�ڂ̃m�[�g�����݂���R�[�h��������A5���ڂ�ǉ�����
		else if (chords[i].tension == Chords::ChordTension::FlatNinth) {
			note.num = chords[i].baseNoteNum + 13;						//�Z9�x��ǉ�
			chordNotes.push_back(note);
		}
		else if (chords[i].tension == Chords::ChordTension::Ninth) {
			note.num = chords[i].baseNoteNum + 14;						//��9�x��ǉ�
			chordNotes.push_back(note);
		}
		else if (chords[i].tension == Chords::ChordTension::SharpNinth) {
			note.num = chords[i].baseNoteNum + 15;						//��9�x��ǉ�
			chordNotes.push_back(note);
		}
		else if (chords[i].tension == Chords::ChordTension::Eleventh) {
			note.num = chords[i].baseNoteNum + 17;						//���S11�x��ǉ�
			chordNotes.push_back(note);
		}
		else if (chords[i].tension == Chords::ChordTension::SharpEleventh) {
			note.num = chords[i].baseNoteNum + 18;						//��11�x��ǉ�
			chordNotes.push_back(note);
		}
		else if (chords[i].tension == Chords::ChordTension::FlatThirteenth) {
			note.num = chords[i].baseNoteNum + 20;						//�Z13�x��ǉ�
			chordNotes.push_back(note);
		}
		else if (chords[i].tension == Chords::ChordTension::FlatThirteenth) {
			note.num = chords[i].baseNoteNum + 21;						//��13�x��ǉ�
			chordNotes.push_back(note);
		}

		if (chords[i].onIndex < 12) {									//�I���R�[�h�����݂���Ȃ�(12�����̐�����������)
			note.num = chords[i].baseNoteNum - 12 + chords[i].onIndex;	//�I���R�[�h�̃m�[�g��ʏ�̈�I�N�^�[�u���̈ʒu�ɒǉ�����
			chordNotes.push_back(note);
		}
		if (chords[i].omitIndex < 12) {									//omit�R�[�h�Ȃ��(12�����̐��Ȃ��)
			//omit�ɏ����ꂽ�m�[�g��T���č폜����
			for (int j = 0; j < chordNotes.size(); ++j) {
				if (chordNotes[j].num == chords[i].omitIndex) {
					swap(chordNotes[j], chordNotes.back());
					chordNotes.pop_back();
				}
			}
		}

		//�R�[�h�̃m�[�g�Q�̊J�n���_�͑S�ē����Ȃ̂ŁA���ʃ\�[�g���s���K�v�͂Ȃ�

		//�m�[�g�z��ɍ���̃R�[�h�̃m�[�g�Q��ǉ�
		for (int j = 0; j < chordNotes.size(); ++j) {
			notes.push_back(chordNotes[j]);
		}
		
		//�Ō�Ɏ��Ԃ��X�V����
		time += chords[i].length;
	}

	return notes;
}

//--------------------��������ȍ\���̕`��A�o�͏���(�������炵���̂Ō��Ɉړ�����)--------------------
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
			cout << "�u" << notes[j].startTime << ", " << notes[j].length << ", " << (int)notes[j].num << "�v�@";
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
			cout << "�u" << notes[j].startTime << ", " << notes[j].length << ", " << (int)notes[j].num << "�v�@";
		}
		cout << endl << endl;
	}

	return;
}

//�}�[�N�_�E���ɋȍ\�����L�q���鏈��
void MusicStructure::outputMarkDown(const char* filename)
{
	ofstream ofs(filename);
	if (!ofs.is_open()) {
		cout << "MusicStruct�FoutputMarkDown error" << endl;
		return;
	}

	ofs << "# ���������Ȃ̍\��" << endl << endl;

	ofs << "## �Ȃ̃L�[�F";
	if (mMusicStruct->key % 12 == 0) {
		ofs << "C";
	}
	else if (mMusicStruct->key % 12 == 1) {
		ofs << "C#";
	}
	else if (mMusicStruct->key % 12 == 2) {
		ofs << "D";
	}
	else if (mMusicStruct->key % 12 == 3) {
		ofs << "D#";

	}
	else if (mMusicStruct->key % 12 == 4) {
		ofs << "E";
	}
	else if (mMusicStruct->key % 12 == 5) {
		ofs << "F";
	}
	else if (mMusicStruct->key % 12 == 6) {
		ofs << "F#";
	}
	else if (mMusicStruct->key % 12 == 7) {
		ofs << "G";
	}
	else if (mMusicStruct->key % 12 == 8) {
		ofs << "A��";
	}
	else if (mMusicStruct->key % 12 == 9) {
		ofs << "A";
	}
	else if (mMusicStruct->key % 12 == 10) {
		ofs << "B��";
	}
	else if (mMusicStruct->key % 12 == 11) {
		ofs << "B";
	}
	ofs << endl << endl;

	ofs << "## �Ȃ̃X�P�[���F";
	if (mMusicStruct->scale == Scale::Major) {
		ofs << "���W���[";
	}
	else if (mMusicStruct->scale == Scale::Minor) {
		ofs << "�}�C�i�[";
	}
	else if (mMusicStruct->scale == Scale::HarmonicMinor) {
		ofs << "�n�[���j�b�N�}�C�i�[";
	}
	else if (mMusicStruct->scale == Scale::MelodicMinor) {
		ofs << "�����f�B�b�N�}�C�i�[";
	}
	ofs << endl << "<br>" << endl << endl;

	//�e�p�[�g�ɂ��ĕ\�����鏈��
	for (int i = 0; i < mMusicStruct->parts.size(); ++i) {
		//==========�p�[�g�̏��̕`��==========
		ofs << "# �p�[�g" << i + 1 << "�̎�ށF";
		if (mMusicStruct->parts[i].type == Part::A) {
			ofs << "A����";
		}
		else if (mMusicStruct->parts[i].type == Part::B) {
			ofs << "B����";
		}
		else if (mMusicStruct->parts[i].type == Part::C) {
			ofs << "C����";
		}
		else if (mMusicStruct->parts[i].type == Part::Climax) {
			ofs << "�T�r";
		}
		else if (mMusicStruct->parts[i].type == Part::Intro) {
			ofs << "�C���g��";
		}
		else if (mMusicStruct->parts[i].type == Part::Outro) {
			ofs << "�A�E�g��";
		}
		else if (mMusicStruct->parts[i].type == Part::Interlude) {
			ofs << "�ԑt";
		}
		else if (mMusicStruct->parts[i].type == Part::Special) {
			ofs << "���ʂȃp�[�g";
		}
		ofs << endl << endl;

		ofs << "### �p�[�g�̊J�n���ԁF" << mMusicStruct->parts[i].startTimes << "�@�p�[�g�̒����F" << mMusicStruct->parts[i].length << endl << endl;

		//==========�h�����̃��Y���p�^�[���ɂ��Ă̕`��==========
		ofs << "### �h�����̃p�^�[���̎�ށF";
		if (mMusicStruct->rhythms[i].type == RhythmType::BasicDownBeat8) {
			ofs << "��{8�r�[�g";
		}
		else if (mMusicStruct->rhythms[i].type == RhythmType::BasicUpBeat8) {
			ofs << "��{8�r�[�g ���ł�";
		}
		else if (mMusicStruct->rhythms[i].type == RhythmType::HalfTimeShuffle) {
			ofs << "�n�[�t�^�C���V���b�t��";
		}
		else if (mMusicStruct->rhythms[i].type == RhythmType::ShuffleBeat) {
			ofs << "�V���b�t���r�[�g";
		}
		else if (mMusicStruct->rhythms[i].type == RhythmType::SlipBeat8) {
			ofs << "�X���b�v8�r�[�g";
		}
		ofs << endl << endl;

		//==========�R�[�h��ɂ��Ă̕`��==========
		ofs << "### �R�[�h�����A���S���Y���F";
		if (mMusicStruct->chords[i].type == ChordAlgorithm::Simple) {
			ofs << "�V���v��";
		}
		else if (mMusicStruct->chords[i].type == ChordAlgorithm::Fashionable) {
			ofs << "�I�V����";
		}
		else if (mMusicStruct->chords[i].type == ChordAlgorithm::Modulation) {
			ofs << "�]��";
		}
		ofs << endl << endl;
		vector<Chords::Chord> chords = mMusicStruct->chords[i].chords;
		ofs << "### �R�[�h��F";
		for (int j = 0; j < chords.size(); ++j) {
			//C�̂悤�ȃR�[�h�ɂ����ĊK�������̕`��
			if (chords[j].baseNoteNum % 12 == 0) {
				ofs << "C";
			}
			else if (chords[j].baseNoteNum % 12 == 1) {
				ofs << "C#";
			}
			else if (chords[j].baseNoteNum % 12 == 2) {
				ofs << "D";
			}
			else if (chords[j].baseNoteNum % 12 == 3) {
				ofs << "D#";

			}
			else if (chords[j].baseNoteNum % 12 == 4) {
				ofs << "E";
			}
			else if (chords[j].baseNoteNum % 12 == 5) {
				ofs << "F";
			}
			else if (chords[j].baseNoteNum % 12 == 6) {
				ofs << "F#";
			}
			else if (chords[j].baseNoteNum % 12 == 7) {
				ofs << "G";
			}
			else if (chords[j].baseNoteNum % 12 == 8) {
				ofs << "A��";
			}
			else if (chords[j].baseNoteNum % 12 == 9) {
				ofs << "A";
			}
			else if (chords[j].baseNoteNum % 12 == 10) {
				ofs << "B��";
			}
			else if (chords[j].baseNoteNum % 12 == 11) {
				ofs << "B";
			}

			//�a���̎�ނɂ��ċL�q����
			if (chords[j].type == Chords::ChordType::Major) {
				//�����L�q���Ȃ�
			}
			else if (chords[j].type == Chords::ChordType::Minor) {
				ofs << "m";
			}
			else if (chords[j].type == Chords::ChordType::Aug) {
				ofs << "aug";
			}
			//�}�C�i�[��͏�����������Ȃ̂�2�`�揈��������
			else if (chords[j].type == Chords::ChordType::mb5) {
				if (chords[j].plus != Chords::ChordPlus::Dim7) {
					ofs << "m";
				}
			}

			//4�ڂ̉��ɂ��Ă̕t�������L�q����
			if (chords[j].plus == Chords::ChordPlus::Seventh) {
				ofs << "7";
			}
			else if (chords[j].plus == Chords::ChordPlus::MajorSeventh) {
				ofs << "M7";
			}
			else if (chords[j].plus == Chords::ChordPlus::Six) {
				ofs << "6";
			}
			else if (chords[j].plus == Chords::ChordPlus::Add9) {
				ofs << "Add9";
			}
			else if (chords[j].plus == Chords::ChordPlus::Add11) {
				ofs << "Add11";
			}
			else if (chords[j].plus == Chords::ChordPlus::Add13) {
				ofs << "Add13";
			}
			else if (chords[j].plus == Chords::ChordPlus::Dim7) {
				ofs << "Dim7";
			}

			//3�a���̍\���ɑ����邪�A4�a���̏�񂪕K�v�ɂȂ镔���̕`��
			if (chords[j].type == Chords::ChordType::Sus2) {
				ofs << "sus2";
			}
			else if (chords[j].type == Chords::ChordType::Sus4) {
				ofs << "sus4";
			}
			else if (chords[j].type == Chords::ChordType::mb5) {
				if (chords[j].plus != Chords::ChordPlus::Dim7) {
					ofs << "-5";
				}
			}
			//5�ڂ̉��̕t�������L�q����
			if (chords[j].tension == Chords::ChordTension::FlatNinth) {
				ofs << "��9";
			}
			else if (chords[j].tension == Chords::ChordTension::Ninth) {
				ofs << "9";
			}
			else if (chords[j].tension == Chords::ChordTension::SharpNinth) {
				ofs << "#9";
			}
			else if (chords[j].tension == Chords::ChordTension::Eleventh) {
				ofs << "11";
			}
			else if (chords[j].tension == Chords::ChordTension::SharpEleventh) {
				ofs << "#11";
			}
			else if (chords[j].tension == Chords::ChordTension::FlatThirteenth) {
				ofs << "��13";
			}
			else if (chords[j].tension == Chords::ChordTension::Thirteenth) {
				ofs << "13";
			}

			//�I���R�[�h�̋L�q
			if (chords[j].onIndex != chords[j].baseIndex) {					//�m�[�g�̊�ꉹ�ƃI���R�[�h�̒l����v���Ă��Ȃ����̂݃I���R�[�h�̕`����s��
				ofs << "/";
				if (chords[j].onIndex == 0) {
					ofs << "C";
				}
				else if (chords[j].onIndex == 1) {
					ofs << "C#";
				}
				else if (chords[j].onIndex == 2) {
					ofs << "D";
				}
				else if (chords[j].onIndex == 3) {
					ofs << "D#";
				}
				else if (chords[j].onIndex == 4) {
					ofs << "E";
				}
				else if (chords[j].onIndex == 5) {
					ofs << "F";
				}
				else if (chords[j].onIndex == 6) {
					ofs << "F#";
				}
				else if (chords[j].onIndex == 7) {
					ofs << "G";
				}
				else if (chords[j].onIndex == 8) {
					ofs << "A��";
				}
				else if (chords[j].onIndex == 9) {
					ofs << "A";
				}
				else if (chords[j].onIndex == 10) {
					ofs << "B��";
				}
				else if (chords[j].onIndex == 11) {
					ofs << "B";
				}
			}
			//�I�~�b�g�R�[�h�͍��͍l���Ȃ�
			ofs << "�@";
		}
		ofs << endl << endl;

		//==========�����f�B�ɂ��Ă̕`��==========
		ofs << "### �����f�B�����A���S���Y���F";
		if (mMusicStruct->melodys[i].type == MelodyAlgorithm::Standard) {
			ofs << "Standard";
		}
		ofs << endl << endl;
		vector<Note> notes = mMusicStruct->melodys[i].notes;
		ofs << "�����f�B�̃m�[�g��F(�p�[�g���猩�đ��ΓI�ȊJ�n���ԁA�����A�m�[�g�̍���)<br>" << endl;
		for (int j = 0; j < notes.size(); ++j) {
			ofs << "�u" << notes[j].startTime << ", " << notes[j].length << ", ";
			//�m�[�g�ԍ��𕶎��ɕϊ����ďo��
			if (notes[j].num % 12 == 0) {
				ofs << "C";
			}
			else if (notes[j].num % 12 == 1) {
				ofs << "C#";
			}
			else if (notes[j].num % 12 == 2) {
				ofs << "D";
			}
			else if (notes[j].num % 12 == 3) {
				ofs << "D#";

			}
			else if (notes[j].num % 12 == 4) {
				ofs << "E";
			}
			else if (notes[j].num % 12 == 5) {
				ofs << "F";
			}
			else if (notes[j].num % 12 == 6) {
				ofs << "F#";
			}
			else if (notes[j].num % 12 == 7) {
				ofs << "G";
			}
			else if (notes[j].num % 12 == 8) {
				ofs << "A��";
			}
			else if (notes[j].num % 12 == 9) {
				ofs << "A";
			}
			else if (notes[j].num % 12 == 10) {
				ofs << "B��";
			}
			else if (notes[j].num % 12 == 11) {
				ofs << "B";
			}
			ofs << "�v�@";
		}
		ofs << endl << endl;

		//==========�x�[�X�ɂ��Ă̕`��==========
		ofs << "### �x�[�X�����A���S���Y���F";
		if (mMusicStruct->bases[i].type == BaseAlgorithm::Simple) {
			ofs << "Simple";
		}
		else if (mMusicStruct->bases[i].type == BaseAlgorithm::UpDown) {
			ofs << "Updown";
		}
		else if (mMusicStruct->bases[i].type == BaseAlgorithm::Melo) {
			ofs << "Melo";
		}
		ofs << endl << endl;
		notes = mMusicStruct->bases[i].notes;
		ofs << "�x�[�X�̃m�[�g��F(�p�[�g���猩�đ��ΓI�ȊJ�n���ԁA�����A�m�[�g�ԍ�)<br>" << endl;
		for (int j = 0; j < notes.size(); ++j) {
			ofs << "�u" << notes[j].startTime << ", " << notes[j].length << ", " << (int)notes[j].num << "�v�@";
		}
		ofs << endl << endl;
	}

	return;
}
