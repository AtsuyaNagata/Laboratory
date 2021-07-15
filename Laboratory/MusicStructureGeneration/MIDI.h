#ifndef MIDI_H_2019_12_19_
#define MIDI_H_2019_12_19_

#include"File.h"
#include<stdio.h>
#include <cstdint>
#include <vector>

class MIDI {
public:
	//引数ありならMIDIファイルの解析処理を回す
	MIDI(const char* filename);
	//引数がなければ空っぽのMIDIクラスを作る
	MIDI();
	//破壊処理
	~MIDI();

	enum class EVENT : unsigned char {
		//メタイベント				//FF｜イベントタイプ｜データ長｜データ
		//SEQUENCE_NUMBER,			//FF 00 02 シーケンス番号、フォーマット0と1(フォーマット0で固定するのでコメントアウト)
		COMMENT,					//FF 01 len data　テキストを書き込むためのイベント
		COPY_RIGHT,					//FF 02 len data　著作権が書いてある
		SEQUENCE_TRACK_NAME,		//FF 03 len data　シーケンス名(曲名)・トラック名
		INSTRUMENT_NAME,			//FF 04 len data　楽器名
		LYRIC,						//FF 05 len data　lyricは歌詞の事
		MARKER,						//FF 06 len data　リハーサル記号やセクション名のような、シーケンスのその時点の名称を記述する
		QUEUE_POINT,				//FF 07 len data　曲データ中、このイベントが挿入されている位置で、曲以外の進行を記述する。(画面のフェードアウトのような動画等に対する進行)
		PROGRAM_NAME,				//FF 08 len data　プログラムチェンジとバンクチェンジで表している音色名を記載する（General MIDIが制定される前の遺物）
		DEVICE_NAME,				//FF 09 len data　このイベントがあるトラックがどのデバイスに配置されるのかということを示す。このイベントは、1トラックにつきi度だけ、最初に呼ばれるべきもの
		CHANNEL_PREFIX,				//FF 20 01 cc MIDIイベントじゃないのに(SysEx or メタイベント)MIDIチャンネルを指定する際に用いる
		PORT_SPECIFIED,				//FF 21 01 pp 出力ポートの指定	0～3がそれぞれポートの1～4に対応する(例外はあるが、データのみでは判別不可)
		END_OF_TRACK,				//FF 2F 00　　トラックの最後であることを表す
		SET_TEMPO,					//FF 51 03 tt tt tt　tttttt(3byte)には、4分音符の長さをμ秒で表したものを格納する。BPM = 120の時、4分音符の長さは 60 x 10^6 / 120 = 500000[μs]
		TIME_SIGNATURE,				//FF 58 04 nn dd cc bb | 拍子 nn = 分子 dd = 分母(2^-ddで表される->dd=2の時、分母=4) cc = メトロノーム間隔[四分音符間隔なら18H] bb = 四分音符当たりの三十二分音符の数
		KEY_SIGNATURE,				//FF 59 02 sf ml キー(調)を示す	sf = 正:シャープの数 or 負:フラットの数		ml = 0:長調(メジャー) 1:短調(マイナー)
		//SPECIAL_META_EVENT,		//FF 7F len data　最初の1バイトはメーカIDで、その後に独自フォーマットのデータが続く(独自のフォーマットまで気を回すつもりはないのでコメントアウト)
		FF_NONE,					//何も見つからなかった時、これを入れる

		//システムエクスクルシーブメッセージを格納するイベント。個々の楽器メーカが独自に作成したメッセージを格納する。MIDIデバイスに何かしら設定するのに利用される
		SYSEX,						//F0 データ長 エクスクルシーブメッセージ F7
		F7_SYSEX,					//F7 データ長 エクスクルシーブメッセージ

		//MIDIイベント
		POLYPHONIC_KEY_PRESSURE,	//An kk vv nのチャンネルのkk番目のノートにベロシティvvのプレッシャー情報を与える
		CONTROL,					//Bn cc vv コントローラ設定。チャンネルnでコントローラナンバーccにvvの値を送る
		PROGRAM_CHANGE,				//Cn pp プログラムチェンジ。チャンネルnで、音色をppに変更する
		CHANNEL_PRESSURE,			//Dn vv チャンネルnに対してプレッシャー情報vvを送信する。
		PITCH_BENT,					//En mm ll ピッチベント。なぜかリトルエンディアンらしい
		NOTE_ON,					//9n nn vv ノート・オン nnはノート番号 vvはベロシティ
		NOTE_OFF					//8n nn vv ノート・オフ nnはノート番号 vvはベロシティ
	};

	//Eventの構造
	typedef struct Event {
		EVENT type;					//何のイベントかを格納する
		size_t datasize;			//それぞれのイベントのデータサイズを格納する
		const unsigned char* data;	//イベントのデータを格納する
		size_t time;				//デルタタイムを格納する（デルタタイムは前のイベントからの相対時間）
	} Event;
	typedef struct Track {
		Event* event;				//イベントの構造を配列で入れる
		size_t eventnum;			//イベント構造の配列のサイズを入れる
		size_t tracksize;			//midiファイルのそれぞれのトラックチャンクのヘッダ部分にあるトラックデータサイズを格納する場所
	} Track;

	//いつもの
	short getMaxTrackNum() {
		return mMaxTrackNum;
	}
	short getResolution() {
		return mResolution;
	}
	//あるトラックの情報を纏めて獲得！
	Track* getTrack(int n) {
		return &mTrack[n];
	}

	//書き込み用にMIDIイベント専用のデータ構造を宣言する
	typedef struct MIDINoteEvent {
		size_t time;			//960ごとに四分音符一つ分上がると考えてる
		uint8_t noteNum;		//音の高さ
		uint8_t velocity;		//音の大きさ
	}MIDINoteEvent, *MIDINoteEvents;
	//MIDIデータを構築する関数。一つのトラックで時間の分解能は960で固定。引数は入力したいMIDIイベント用構造の配列とその最大サイズ。
	int createMIDI_oneTrack(MIDINoteEvents midiEvents, int noteSize, int bmp);
	int createMIDI_oneTrack(std::vector<MIDINoteEvent> midiEvents, int noteSize, int bmp);

	//ここがMIDIファイル書き込みのメイン
	int MIDIWrite(const char* filename);

private:		//============privateの開始地点====================
	//要素を破壊する関数はクラス自身のみ呼び出せるものとする
	void destroy() {
		for (int i = 0; i < mMaxTrackNum; ++i) {
			delete[] mTrack[i].event;
			mTrack[i].event = nullptr;
		}
		delete[] mTrack; mTrack = nullptr;
		delete[] mMemoryMap; mMemoryMap = nullptr;
		delete mFile; mFile = nullptr;
		mMaxTrackNum = 0;
		mResolution = 0;
	}
	File* mFile;

	short mMaxTrackNum;		//トラックの数
	short mResolution;		//四分音符一つあたりの分解能の値が入る(480が入っていたらデルタタイム480で四分音符一つ分飛ぶ)

	uint8_t* mMemoryMap;	//書き込んだデータを格納しておく場所

	Track* mTrack;			//Trackの構造：

	//========================MIDI書き込みの処理で用いる処理群==============================
	//変数はリトルエンディアンで格納されているので、ビックエンディアンに直す関数が必要(動的な領域を取る処理なのでなるだけ呼び出し数は減らすこと)
	void reverceByte(void* data, unsigned char datasize);

	//デルタタイムの書き込み処理(4バイトが上限だと決まってるので、拡張性は意識しなくてもよいドン)
	void DeltaTimeWrite(FILE* midiFile, size_t time);

	//イベントに応じて書き込むバイナリを変える
	void EventWrite(FILE* midiFile, MIDI::Event& eve);
	//===================MIDI書き込みの処理で用いる処理群の終了地点===============================
};

#endif