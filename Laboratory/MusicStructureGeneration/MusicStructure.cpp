#include "MusicStructure.h"

//�K�v�ȃf�[�^�F�L�[�A���y�̃p�[�g�i�s�A��Ԃ̒����A�R�[�h�p�^�[��(�M�^�[�A�s�A�m�ȂǊy��ɑΉ�������\��)�A���Y���p�^�[���A�����f�B�p�^�[���A�x�[�X���C��
MusicStructure::MusicStructure(int key, vector<MusicStructure::Part> parts, vector<uint32_t> lengths, vector<Rhythm> rhythms, vector<ChordAlgorithm> chordAls, vector<MelodyAlgorithm>meloAls, vector<Base> bases):
	mMusicStruct(nullptr)
{
	prepare(key, parts, lengths, rhythms, chordAls, meloAls, bases);
}

//�Ȃ̐݌v�}���󂯎���ď���������R���X�g���N�^�B������g�����Ƃ͑����Ȃ�
MusicStructure::MusicStructure(const MusicStruct& mus) : mMusicStruct(nullptr) {
	mMusicStruct = new MusicStruct;
	mMusicStruct->key = mus.key;
	mMusicStruct->length = mus.length;
	mMusicStruct->part = mus.part;
	mMusicStruct->rhythm = mus.rhythm;
	mMusicStruct->chord = mus.chord;
	mMusicStruct->melody = mus.melody;
	mMusicStruct->base = mus.base;
	mMusicStruct->obbligatos = mus.obbligatos;
}

//�����Ȃ��̃R���X�g���N�^�B�Ȃ̐݌v�}���k���|�ŏ��������邾��
MusicStructure::MusicStructure() : mMusicStruct(nullptr) {}

//�f�X�g���N�^
MusicStructure::~MusicStructure() {
	delete mMusicStruct;
	mMusicStruct = nullptr;
}

void MusicStructure::prepare(int key, vector<MusicStructure::Part> parts, vector<uint32_t> lengths, vector<Rhythm> rhythms, vector<ChordAlgorithm> chordAls, vector<MelodyAlgorithm>meloAls, vector<Base> bases)
{
	//�v�f�����ɑ��݂��Ă鎞�A��x�S�č폜���Ă��܂�(�v�f���ςɎc���ăo�O�ɋC�t���ɂ����Ȃ�̂�h�����߂̎d�l)
	if (mMusicStruct != nullptr) {
		delete mMusicStruct;
		mMusicStruct = nullptr;
	}
	//�e�\�������p�[�g���ƈ�v���Ȃ��ُ�ȓ��͂�������
	if (parts.size() != lengths.size() || parts.size() != rhythms.size() || parts.size() != chordAls.size() || parts.size() != meloAls.size() || parts.size() != bases.size()) {
		return;			//�\�������O�ɗ��Ƃ����ƂŁA���̎Q�Ǝ���nullptr��Ԃ��悤�ɂ���
	}
	mMusicStruct = new MusicStruct;
	//�����l�̐ݒ�B��̓I�ȃm�[�g�̐������̏����͊e�N���X�ōs���̂ŁA�����ł͒��ۓI�Ȓl�����i�[���Ȃ��B
	mMusicStruct->key = key;
	uint32_t length = 0;
	for (int i = 0; i < parts.size(); ++i) {
		//�p�[�g�̍\���������̒l����\�z����(Obbligato�\���͓���ȕ��Ȃ̂ŁA�����Ŋi�[�����͍s��Ȃ�)
		PartStruct ps;
		ps.type = parts[i];
		ps.length = lengths[i];
		ps.startTimes = length;
		mMusicStruct->part.push_back(ps);		//�����������p�[�g�\�����v�b�V��(��������S�Ă̗v�f���i�[�\�ƂȂ��Ă���)

		RhythmStruct rs;
		rs.type = rhythms[i];
		rs.fill_in = false;
		mMusicStruct->rhythm.push_back(rs);		//�������������Y���\�����v�b�V��(��̓I�Ȓl�͊i�[����Ă��Ȃ�)

		ChordStruct cs;
		cs.type = chordAls[i];
		mMusicStruct->chord.push_back(cs);		//�����������R�[�h�\�����v�b�V��(��̓I�Ȓl�͊i�[����Ă��Ȃ�)

		MelodyStruct ms;
		ms.type = meloAls[i];
		mMusicStruct->melody.push_back(ms);		//���������������f�B�\�����v�b�V��(��̓I�Ȓl�͊i�[����Ă��Ȃ�)

		BaseStruct bs;
		bs.type = bases[i];
		mMusicStruct->base.push_back(bs);		//�����������x�[�X�\�����v�b�V��(��̓I�Ȓl�͊i�[����Ă��Ȃ�)

		length += lengths[i];					//���ݎQ�Ƃ����p�[�g�̍Ō���̈ʒu�̒l�ɂȂ�
	}
	mMusicStruct->length = length;
}

void MusicStructure::create()
{
	//�p�[�g���������l�ȏ������s��
	vector<PartStruct> part = mMusicStruct->part;
	for (int i = 0; i < part.size(); ++i) {
		//���Y���𐶐�����
		RhythmStruct &rs = mMusicStruct->rhythm[i];


		//�R�[�h�i�s�𐶐�����
		ChordStruct& cs = mMusicStruct->chord[i];


		//�����f�B�𐶐�����
		MelodyStruct& ms = mMusicStruct->melody[i];


		//�x�[�X���C���𐶐�����
		BaseStruct& bs = mMusicStruct->base[i];


	}
}