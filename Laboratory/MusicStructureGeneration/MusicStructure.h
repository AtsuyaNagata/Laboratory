#ifndef MUSIC_STRUCTURE_H_2021_07_06_
#define MUSIC_STRUCTURE_H_2021_07_06_
#include <vector>
#include <cstdint>
#include "Chords.h"
#include "RhythmPattern.h"
#include "Melody.h"
using namespace std;

//���y�\������舵���N���X�B�����������܂�
class MusicStructure
{
public:
	enum class Scale : unsigned char {
		Major,
		Minor,
		HarmonicMinor,
		MelodicMinor
	};

	//�Ȃ̃p�[�g�̎�ނ�\�����邽�߂̗񋓎q
	enum class Part : unsigned char {
		None,					//���݂��Ȃ����Ƃ�\��(�����l�p)

		Intro,					//�C���g��
		Interlude,				//�ԑt�̂���
		Outro,					//�A�E�g��
		A,						//A�p�[�g
		B,						//B�p�[�g
		Climax,					//�T�r
		C,						//C�p�[�g

		Special					//None�ł͂Ȃ�(���݂͂���)���A���ނɊ܂܂�Ȃ��p�[�g��\��
	};

	//���Y���p�^�[���̎�ނ�\�����邽�߂̗�
	enum class RhythmType : unsigned char {
		None,

		BasicDownBeat8,			//��{8�r�[�g
		BasicUpBeat8,			//��{8�r�[�g���ł�(�悭�T�r�ɗp������)
		SlipBeat8,				//�X���b�v8�r�[�g(�X�l�A��8���������ɂ������肷�郊�Y���p�^�[��)
		ShuffleBeat,			//�V���b�t���r�[�g(4���~3��3���~4�̂悤�ɍl���郊�Y���p�^�[���B2���ڂ�4���ڂɃo�b�N�r�[�g(�X�l�A�̋�)������)
		HalfTimeShuffle,		//�n�[�t�^�C���V���b�t���r�[�g(�`�^�` �`�^�` �^�^�` �`�^�` �̃��Y���p�^�[��)[�n�[�t�^�C���^�C����3���ڂɃo�b�N�r�[�g������Ӗ�����]
	};

	//�R�[�h�����A���S���Y���̎�ނ̗�
	enum class ChordAlgorithm : unsigned char {
		None,
		
		Simple,					//�V���v���Ȑ���
		Fashionable,			//�I�V�����ɐ���
		Modulation				//�]�����s���悤�ɐ���
	};

	//�����f�B�����A���S���Y���̎�ނ̗�
	enum class MelodyAlgorithm : unsigned char {
		None,

		Standard				//��{�I�Ȑ����A���S���Y��
	};

	//�I�u���K�[�h�̎�ނ̗񋓁B�����f�B�������������邽�߂̃����f�B�A�X�ȃp�[�g�̂���
	enum class Obbligato : unsigned char{
		None,

		Fake,					//�����f�B�E���C���̕��͋C��ۂ��Ȃ���鑕��
		FillIn,					//�����f�B�̌p�ڂɏo�����鑦���I�ȃ���(�h�����ő�̍ς�)�������Ƃ��Ă΂��
		Harmony,				//�����f�B�E���C���Ƀn���郁��
		Riff					//���`�[�t���X�ɒZ�������悤�ȗv�f�A�I�u���K�[�h�Ƃ��Ď����Ă���
	};
	
	//�h�����Ɋ܂܂��y��̎�ނ̗�
	enum class DrumType : unsigned char {
		None,

		BassDrum,				//�C���f�b�N�X�l�F0
		CloseHiHatCymbal,		//�C���f�b�N�X�l�F1
		OpenHiHatCymbal,		//�C���f�b�N�X�l�F2
		SnareDrum,				//�C���f�b�N�X�l�F3
		Tom,					//�C���f�b�N�X�l�F4
		FloorTom,				//�C���f�b�N�X�l�F5
		CrashCymbal,			//�C���f�b�N�X�l�F6
		RideCymbal,				//�C���f�b�N�X�l�F7

		Other					//����ȉ���������ۂɗp����(���݂͍l���Ȃ�)
	};

	//�y��̎�ނ̗�
	enum class Instrument : unsigned char {
		None,

		Piano,
		Guiter,
		Synthesizer,
		AudioMaterial			//�����f�ނƂ������Ӗ�����
	};

	enum class BaseAlgorithm : unsigned char {
		None,

		Simple,					//�R�[�h�̊�ꉹ���Ђ����痬������
		UpDown,					//�R�[�h�̊�ꉹ����I�N�^�[�u��̉��ƌ��݂ɖ炷
		Melo					//�����f�B���ɑJ�ڂ���x�[�X���C�������
	};


	typedef struct Note {
		uint8_t num;			//�m�[�g�̔ԍ��̔z��
		uint32_t length;		//�m�[�g�̒����̔z��(noteNums�ƃT�C�Y�͈�v����)
		uint32_t startTime;			//�m�[�g�̊J�n���_�̔z��
	}Note;

	//�Ȃ̃p�[�g��\������\��
	typedef struct PartStruct {
		Part type;							//�p�[�g�̎�ނ��i�[����
		uint32_t startTimes;				//�J�n�n�_�̎��Ԃ��i�[����(4����������̒�����256�ɂ���)
		uint32_t length;					//�p�[�g�̒���
	} PartStruct;

	//����h�����y��ɂ��Ẵ��Y���p�^�[�����i�[����\��
	typedef struct DrumPattern {
		DrumType type;						//�炷�h�����̎��
		vector<uint32_t> startTimes;		//�e���̊J�n�n�_���i�[����
		//�e����32�������̒����̉��Ƃ��ďo�͂��邱�Ƃɂ���
	};

	//����p�[�g�̃��Y���p�^�[����\������\��
	typedef struct RhythmStruct {
		RhythmType type;					//���Y���̎��
		bool fill_in;						//�Ō�Ƀt�B���C���������Ă邩�ǂ���
		vector<DrumPattern> drumPatterns;	//�e�h�����y��̃��Y���p�^�[�����i�[����\��
	}RhythmStruct;

	//����p�[�g�̃R�[�h��ɂ��Ă̏��
	typedef struct ChordStruct {
		ChordAlgorithm type;				//�R�[�h�����A���S���Y���̎��
		vector<Chords::Chord> chords;		//�R�[�h��
	} ChordStruct;

	//����p�[�g�̃����f�B���C���ɂ��Ă̏��
	typedef struct MelodyStruct {
		MelodyAlgorithm type;				//�����f�B�����A���S���Y���̎��
		vector<Note> notes;					//�m�[�g���
	}MelodyStruct;

	typedef struct BaseStruct {
		BaseAlgorithm type;							//�x�[�X���C���̎��
		vector<Note> notes;					//�m�[�g���
	};

	//�I�u���K�[�g�̍\��
	typedef struct ObbligatoStruct {
		Obbligato type;						//�I�u���K�[�h�̎�ނ�\��
		Instrument instrument;				//�y��̎��
		uint32_t startTime;					//�J�n�n�_
		vector<Note> notes;					//�m�[�g���
	}ObbligatoStruct;

	//�Ȃ̍\���������݌v�}�̂悤�ȍ\��
	typedef struct MusicStruct {
		int key;							//�ȑS�̂̃L�[(0�`11�ŕ\�������B0��C)
		Scale scale;						//�X�P�[����\������
		uint32_t length;					//�ȑS�̂̒���
		vector<PartStruct> parts;			//�p�[�g���̗���i�[(�\���������Ɏ��ԏ��Ń\�[�g����Ă邱�Ƃ��O��ƂȂ��Ă�)
		vector<RhythmStruct> rhythms;		//���Y�����̗�(�p�[�g�ɑΉ����ĕ���)
		vector<ChordStruct> chords;			//�R�[�h���̗�(�p�[�g�ɑΉ����ĕ���)
		vector<MelodyStruct> melodys;		//�����f�B�\���̔z��(�p�[�g�ɑΉ����ĕ���)
		vector<BaseStruct> bases;			//�x�[�X�\���̔z��(�p�[�g�ɑΉ����ĕ���)
		vector<ObbligatoStruct> obbligatos;	//�I�u���K�[�h�̗�(�p�[�g�Ɉˑ����Ȃ��A�����C��)
	};

	//�K�v�ȃf�[�^�F�L�[�A���y�̃p�[�g�i�s�A��Ԃ̒����A�R�[�h�p�^�[��(�M�^�[�A�s�A�m�ȂǊy��ɑΉ�������\��)�A���Y���p�^�[���A�����f�B�p�^�[���A�x�[�X���C��
	MusicStructure(int key, Scale scale, vector<MusicStructure::Part> parts, vector<uint32_t> lengths, vector<RhythmType> rhythms, vector<ChordAlgorithm> chordAls, vector<MelodyAlgorithm>meloAls, vector<BaseAlgorithm> baseAls);
	//�݌v�}���󂯎���Ă�������̂܂܎g���ď���������R���X�g���N�^�B�����g��Ȃ�
	MusicStructure(const MusicStruct& mus);
	//��̗v�f��錾���邽�߂̃R���X�g���N�^�B�������s���ۂɂ�prepare�����o�֐���ʂŌĂяo���K�v������
	MusicStructure();
	
	~MusicStructure();						//�f�X�g���N�^

	//��������\�����\�z���鏈�����s��
	void prepare(int key, Scale scale, vector<MusicStructure::Part> parts, vector<uint32_t> lengths, vector<RhythmType> rhythms, vector<ChordAlgorithm> chordAls, vector<MelodyAlgorithm>meloAls, vector<BaseAlgorithm> baseAls);

	void create();							//�Ȃ𐧍삷��(���Y���A�R�[�h�A�����f�B�A�x�[�X)
	void addObbligato();					//���݂̋Ȃ��Q�Ƃ��Ȃ���A�w�肳�ꂽ�I�u���K�[�g�̒ǉ����s������(����������\��)

	void printMusicStruct();				//�ȍ\����\������֐�

private:
	MusicStruct* mMusicStruct;				//�Ȃ̐݌v�}
	RhythmPattern* mRhythmPattern;			//���Y���p�^�[���𐶐��A�Ǘ�����I�u�W�F�N�g
	Chords* mChords;						//�R�[�h��𐶐��A�Ǘ�����I�u�W�F�N�g
	Melody* mMelody;						//�����f�B�𐶐��A�Ǘ�����I�u�W�F�N�g
};

#endif