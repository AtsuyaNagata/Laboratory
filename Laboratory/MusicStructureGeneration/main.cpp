#include <iostream>
#include "MusicStructure.h"
using namespace std;

int main()
{
	MusicStructure ms;

	//曲構造のひな形を成形する
	int key = 0;
	MusicStructure::Scale scale = MusicStructure::Scale::Major;
	vector<MusicStructure::Part> parts;
	vector<uint32_t> lengths;
	vector<MusicStructure::RhythmType> rhythms;
	vector<MusicStructure::ChordAlgorithm> chordAls;
	vector<MusicStructure::MelodyAlgorithm> meloAls;
	vector<MusicStructure::BaseAlgorithm> baseAls;

	//4小節のAパートを生成
	parts.push_back(MusicStructure::Part::A);
	lengths.push_back(256 * 4 * 4);							//256：四分音符一つ、「* 4」：四分音符4つ = 1小節、「* 4」：1小節4つ = 4小節
	rhythms.push_back(MusicStructure::RhythmType::BasicDownBeat8);
	chordAls.push_back(MusicStructure::ChordAlgorithm::Simple);
	meloAls.push_back(MusicStructure::MelodyAlgorithm::Standard);
	baseAls.push_back(MusicStructure::BaseAlgorithm::Melo);

	//4小節のBパートを生成
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