#include <iostream>
#include "MusicStructure.h"
using namespace std;

int main()
{
	MusicStructure ms;

	//�ȍ\���̂ЂȌ`�𐬌`����
	int key = 0;
	MusicStructure::Scale scale = MusicStructure::Scale::Major;
	vector<MusicStructure::Part> parts;
	vector<uint32_t> lengths;
	vector<MusicStructure::RhythmType> rhythms;
	vector<MusicStructure::ChordAlgorithm> chordAls;
	vector<MusicStructure::MelodyAlgorithm> meloAls;
	vector<MusicStructure::BaseAlgorithm> baseAls;

	//4���߂�A�p�[�g�𐶐�
	parts.push_back(MusicStructure::Part::A);
	lengths.push_back(256 * 4 * 4);							//256�F�l��������A�u* 4�v�F�l������4�� = 1���߁A�u* 4�v�F1����4�� = 4����
	rhythms.push_back(MusicStructure::RhythmType::BasicDownBeat8);
	chordAls.push_back(MusicStructure::ChordAlgorithm::Simple);
	meloAls.push_back(MusicStructure::MelodyAlgorithm::Standard);
	baseAls.push_back(MusicStructure::BaseAlgorithm::Melo);

	//4���߂�B�p�[�g�𐶐�
	parts.push_back(MusicStructure::Part::B);
	lengths.push_back(256 * 4 * 4);
	rhythms.push_back(MusicStructure::RhythmType::BasicDownBeat8);
	chordAls.push_back(MusicStructure::ChordAlgorithm::Simple);
	meloAls.push_back(MusicStructure::MelodyAlgorithm::Standard);
	baseAls.push_back(MusicStructure::BaseAlgorithm::Melo);

	ms.prepare(key, scale, parts, lengths, rhythms, chordAls, meloAls, baseAls);
	ms.create();
	ms.printMusicStruct();
}