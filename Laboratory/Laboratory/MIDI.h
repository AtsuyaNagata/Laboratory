#ifndef MIDI_H_2019_12_19_
#define MIDI_H_2019_12_19_

#include"File.h"
#include<stdio.h>
#include <cstdint>

class MIDI {
public:
	MIDI(const char* filename) :
		mFile(nullptr),
		mMaxTrackNum(0),
		mResolution(0),
		mTrack(nullptr),
		mMemoryMap(nullptr)
	{
		mFile = new File(filename);
		const unsigned char* fileData = reinterpret_cast<const unsigned char*>(mFile->data());
		//ファイルの実体がなければここで落とす
		if (mFile->size() == 0) {
			return;
		}
		//ファイルヘッダ確認(4バイト)
		if (fileData[0] == 'M' && fileData[1] == 'T' && fileData[2] == 'h' && fileData[3] == 'd') {
			//何もしない
		}
		else {
			return;
		}
		//ヘッダサイズが6になっているかを確認する(なぜか4バイト)
		if (fileData[4] == 0 && fileData[5] == 0 && fileData[6] == 0 && fileData[7] == 0x06) {
			//何もしない
		}
		else {
			return;
		}

		//フォーマット確認（フォーマットには0、1、2がある）(2バイト)
		//フォーマット0は全てのチャンネルのデータを一つのトラックにまとめたもの
		//フォーマット1は複数のチャンネルを用いてデータをまとめたもの
		//フォーマット2はシーケンスパターンをそのまま保存する方式、おそらくかなり複雑だと思った(小並感)
		//フォーマットが0の時
		if (fileData[8] == 0 && fileData[9] == 0) {
			//トラック数(2バイト)
			//トラック数は1でないとフォーマット情報と合わない
			if (fileData[10] != 0 || fileData[11] != 1) {
				return;
			}
			mMaxTrackNum = 1;
		}
		//フォーマットが1の時
		if (fileData[8] == 0 && fileData[9] == 1) {
			//トラック数を獲得(2バイト)
			mMaxTrackNum = (fileData[10] << 8) + fileData[11];
			//mMaxTrackNumが0だと色々まずいので落とす
			if (mMaxTrackNum == 0) {
				return;
			}
		}

		//時間単位の獲得(分解能のこと)
		//最初のビットが0ならば、何小節の何拍目のデータといった方式でトラックチャンクが作られる
		//最初のビットが1ならば、何分何秒といったデータといった方式で保存する
		//大抵は前者
		if (fileData[12] & 0b10000000) {
			//何分何秒のやつ
		}
		else {
			//何小節何拍目のやつ
			mResolution = (fileData[12] << 8) + fileData[13];
		}

		//トラック分のTrackを用意する
		mTrack = new Track[mMaxTrackNum];
		//MTrk[4D 54 72 6B]の識別子とデータサイズの獲得の後にイベントが並べられる

		//デルタタイムについて
		/*
			デルタタイムは可変長バイトの表現がなされており、最上位ビットが次のバイトも数字で読み込むかの判別子となる。
			例：
			10進数「1」 -> 00000001
			10進数「128」 -> 10000001 00000000

			つまり、1バイトごとに1ビットの無駄が出る
		*/

		//MIDIイベントについて
		/*
			3バイトのデータで表現する
			例：「8チャンネルのC4(60 = 0x3C)をベロシティーを90で鳴らす」 => [0x98 0x3C 0x5A]
			ノートオフ[0x8n 0xkk 0xvv]：音を止める。ベロシティーは鍵盤から手を放す速さを表す
			(n = 対象チャンネルナンバー　kk = ノートナンバー　vv = ベロシティー)
			ノートオン[0x9n 0xkk 0xvv]：音を鳴らす。ベロシティーは鍵盤を弾く速さ(音の強さ)
			(n = 対象チャンネルナンバー　kk = ノートナンバー　vv = ベロシティー)
			コントロールチェンジ[0xBn 0xcc 0xvv]：各種コントローラの設定が出来る
			(n = 対象チャンネルナンバー　cc = コントロールナンバー　vv = データ)
		*/
		int TrackPoint = 0;
		for (int i = 14; i < mFile->size();) {
			int eventNum = 0;
			if (fileData[i] == 'M' && fileData[i + 1] == 'T' && fileData[i + 2] == 'r' && fileData[i + 3] == 'k') {
				//MTrkの文字分のバイトを進める
				i += 4;
				//トラックのデータのサイズ
				size_t trackSize = (fileData[i] << 24) + (fileData[i + 1] << 16) + (fileData[i + 2] << 8) + fileData[i + 3];
				//トラックサイズ分のバイトを進める
				i += 4;
				//イベントの数をカウントする
				size_t count = 0;
				//書略記述を認識するためのフラグ
				enum class MIDIEVENTFLAG { MIDIEVENT_3BYTES, MIDIEVENT_2BYTES, NONE } midiEventFlag = MIDIEVENTFLAG::NONE;
				//最初はデルタタイムのデータが来るのでtrue
				bool delta = true;
				for (int j = i; j < (trackSize + i);) {
					if (delta) {
						//最大4バイトだから実質O(1)
						for (int k = 0; k < 4; ++k) {
							if (fileData[j + k] & 0x80) {
								//何もしない
							}
							else {
								//デルタタイム分のバイト数進める
								j += k + 1;
								break;
							}
						}
					}
					else {	//イベント処理に関する処理
						if (fileData[j] == 0xff) {
							//FFから始まるデータはlenバイトでバイトの長さを指定しているので、その分のバイトを飛ばす
							unsigned char len = fileData[j + 2];
							j += 3 + len;
							count++;
							midiEventFlag = MIDIEVENTFLAG::NONE;
						}
						else if (fileData[j] == 0xf0 || fileData[j] == 0xf7) {
							//0xf0分のデータバイトを飛ばす
							j += 1;
							//lenは可変長である事を考慮する
							int k, len = 0;
							for (k = 0; k < 4; ++k) {
								len <<= 7;
								len += fileData[j + k] & 0x7f;
								if (fileData[j + k] & 0x80) {			//最上位ビットが1なら、次のビットも確認する
									//何もしない
								}
								else {
									break;
								}
							}
							//元々1バイト分のデータはある
							j += k + 1;
							//データの長さ分のバイトをぶち飛ばす
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
							if (fileData[i] & 0x80) {	//もし最上位ビットが1なら、MIDIイベントの省略形ですらない
								printf("0x%x is not defined", fileData[j]);
								destroy();
								return;
							}
							//省略的な記法の時の処理
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

				//要素を具体的に格納していく（2週目）
				//最初はデルタタイムのデータ
				delta = true;
				int eventPoint = 0;
				for (int j = i; j < (trackSize + i);) {
					if (delta) {
						unsigned int time = 0;
						int k;
						//最大4バイトだから実質O(1)
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
						//デルタタイム分のバイト数進める
						j += k + 1;
						mTrack[TrackPoint].event[eventPoint].time = time;
					}
					else {	//イベントとそれに基づくデータの格納処理
						if (fileData[j] == 0xff) {
							//データのイベントタイプを判別する値
							unsigned char eventType = fileData[j + 1];
							//FFから始まるデータはlenバイトでバイトの長さを指定しているので、その分のバイトを飛ばす
							unsigned char len = fileData[j + 2];
							//長さのデータ格納
							mTrack[TrackPoint].event[eventPoint].datasize = len;
							if (len == 0) {
								//サイズが0ならデータを獲得しない
								mTrack[TrackPoint].event[eventPoint].data = nullptr;
							}
							else {
								//データの開始地点のポインタをいただく
								mTrack[TrackPoint].event[eventPoint].data = &fileData[j + 3];
							}
							//イベントタイプをそれぞれ格納する
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
							//f0(f7)識別子分のビットを飛ばす
							j += 1;
							//lenは可変長である事を考慮する
							int len = 0;
							for (int k = 0; k < 4; ++k) {
								len <<= 7;
								len += fileData[j + k] & 0x7f;
								if (fileData[j + k] & 0x80) {			//最上位ビットが1なら、次のビットも確認する
									//何もしない
								}
								else {
									j += k + 1;
									break;
								}
							}
							//イベントの格納
							mTrack[TrackPoint].event[eventPoint].type = EVENT::SYSEX;
							//長さを入れてやる
							mTrack[TrackPoint].event[eventPoint].datasize = len;
							//長さを獲得してしまえば後はこっちのもの
							mTrack[TrackPoint].event[eventPoint].data = &fileData[j];
							//長さ分のバイトをぶち飛ばす
							j += len;
						}
						//MIDIイベントは、識別子にデータが入ってる特殊な形なのでイベント情報が格納されているバイトごとデータを獲得する
						//ノートオン・オフ処理
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
						//3バイト使うイベントをぶち込んでる
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
						//2バイト使うイベントをぶち込む
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
							if (fileData[i] & 0x80) {	//もし最上位ビットが1なら、MIDIイベントの省略形ですらないのでエラー
								printf("0x%x is not defined", fileData[j]);
								destroy();
								return;
							}
							//省略的な記法の時の処理
							int dataSizeBuff = 0;
							//0x8nの処理
							if (mTrack[TrackPoint].event[eventPoint - 1].type == EVENT::NOTE_OFF) {
								dataSizeBuff = 2;
								mTrack[TrackPoint].event[eventPoint].type = EVENT::NOTE_OFF;
							}
							//0x9nの処理
							else if (mTrack[TrackPoint].event[eventPoint - 1].type == EVENT::NOTE_ON) {
								dataSizeBuff = 2;
								mTrack[TrackPoint].event[eventPoint].type = EVENT::NOTE_ON;
							}
							//0xAnの処理
							else if (mTrack[TrackPoint].event[eventPoint - 1].type == EVENT::POLYPHONIC_KEY_PRESSURE) {
								dataSizeBuff = 2;
								mTrack[TrackPoint].event[eventPoint].type = EVENT::POLYPHONIC_KEY_PRESSURE;
							}
							//0xBnの処理
							else if (mTrack[TrackPoint].event[eventPoint - 1].type == EVENT::CONTROL) {
								dataSizeBuff = 2;
								mTrack[TrackPoint].event[eventPoint].type = EVENT::CONTROL;
							}
							//0xCnの処理
							else if (mTrack[TrackPoint].event[eventPoint - 1].type == EVENT::PROGRAM_CHANGE) {
								dataSizeBuff = 1;
								mTrack[TrackPoint].event[eventPoint].type = EVENT::PROGRAM_CHANGE;
							}
							//0xDnの処理
							else if (mTrack[TrackPoint].event[eventPoint - 1].type == EVENT::CHANNEL_PRESSURE) {
								dataSizeBuff = 1;
								mTrack[TrackPoint].event[eventPoint].type = EVENT::CHANNEL_PRESSURE;
							}
							//0xEnの処理
							else if (mTrack[TrackPoint].event[eventPoint - 1].type == EVENT::PITCH_BENT) {
								dataSizeBuff = 2;
								mTrack[TrackPoint].event[eventPoint].type = EVENT::PITCH_BENT;
							}
							else {
								//なんかヤバイ事が起こった時に発生する
								printf("some error has occurred\n");
								destroy();
								return;
							}
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
				//MTrkが来ないのはおかしいので落とす
				destroy();
				return;
			}
		}

	}

	//空っぽのMIDIクラスを作る
	MIDI() :
		mFile(nullptr),
		mMaxTrackNum(0),
		mResolution(0),
		mTrack(nullptr),
		mMemoryMap(nullptr)
	{}

	~MIDI() {
		destroy();
	}

	enum class EVENT : unsigned char {
		//メタイベント				//FF｜イベントタイプ｜データ長｜データ
		//SEQUENCE_NUMBER,			//FF 00 02 シーケンス番号、フォーマット0と1
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
		//SPECIAL_META_EVENT,		//FF 7F len data　最初の1バイトはメーカIDで、その後に独自フォーマットのデータが続く
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
		NOTE_ON,					//9n ノート・オン
		NOTE_OFF					//8n ノート・オフ
	};

	//Eventの構造
	typedef struct Event {
		EVENT type;					//何のイベントかを格納する
		size_t datasize;			//それぞれのイベントのデータサイズを格納する
		const unsigned char* data;	//イベントのデータを格納する
		size_t time;				//デルタタイムを格納する（デルタタイムは前のイベントからの相対時間）
	} Event;
	typedef struct Track {
		Event* event;
		size_t eventnum;
		size_t tracksize;
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
		size_t time;
		uint8_t noteNum;
		uint8_t velocity;
	}MIDINoteEvent, * MIDINoteEvents;
	//MIDIデータを構築する関数。引数は入力したいMIDIイベント用構造の配列とその最大サイズ
	int createMIDI_oneTrack(MIDINoteEvents midiEvents, int noteSize) {
		//下準備
		if (mMaxTrackNum != 0) { destroy(); }						//もし実体があるなら、全部破壊してから構築する
		mMaxTrackNum = 1;											//トラックは一つで固定する
		mResolution = 960;											//四分音符一つ分のタイムを960分割して考える様にする
		mTrack = new Track[1];										//一つ分のトラックの生成
		mTrack->eventnum = noteSize + 4;							//ノートイベントだけでなく、拍子分子等を示すイベント、BMPを示すイベント、トラック名のイベント、トラックの終端を示すイベントも考慮する
		mTrack->event = new Event[noteSize + 4];					//イベント枠を生成する
		mMemoryMap = new uint8_t[4 + 3 + 5 + 1 + (noteSize * 2)];	//獲得するデータサイズは、イベント0、イベント1、イベント3、0x90、2バイトづつのイベントサイズの和

		//データの格納処理
		mTrack->event[0].type = EVENT::TIME_SIGNATURE;					//拍子や分子、メトロノーム間隔を設定する
		mTrack->event[1].type = EVENT::SET_TEMPO;						//テンポを設定する
		mTrack->event[2].type = EVENT::SEQUENCE_TRACK_NAME;				//トラック名も設定する
		mTrack->event[0].datasize = 4;	mTrack->event[1].datasize = 3; mTrack->event[2].datasize = 5;			//それぞれのデータサイズを入れる	
		mTrack->event[0].time = mTrack->event[1].time = mTrack->event[2].time = 0;								//どれも設定なのでタイムは0

		//タイムシグネチャの値をメモリマップに仕込む(分子 = 4、 分母 = 4、 メトロノーム間隔 = 4分音符一つ分[24MIDIクロック]、 32分音符は4分音符を8分割した値 = 8)
		mMemoryMap[0] = 0x04; mMemoryMap[1] = 0x02; mMemoryMap[2] = 0x18; mMemoryMap[3] = 0x08;
		//テンポをセットしておく(0x07A120 = 500,000μsにセット = 120bmpになる)
		mMemoryMap[4] = 0x07; mMemoryMap[5] = 0xA1; mMemoryMap[6] = 0x20;
		//トラック名を定める
		mMemoryMap[7] = 'T'; mMemoryMap[8] = 'r'; mMemoryMap[9] = 'a';  mMemoryMap[10] = 'c'; mMemoryMap[11] = 'k';
		//データ位置をセットする
		mTrack->event[0].data = &mMemoryMap[0]; mTrack->event[1].data = &mMemoryMap[4]; mTrack->event[2].data = &mMemoryMap[7];

		if (midiEvents == nullptr) { return 1; }	//ノート格納処理を行う前にデータがないと以下の処理がバグるので、ここで落とす。「1」はデータ未定義のエラーとする

		//ここからノート格納処理(一行でtype datasize timeを書いてるので注意ね)
		mTrack->event[3].type = EVENT::NOTE_ON; mTrack->event[3].datasize = 3; mTrack->event[3].time = midiEvents[0].time;	//最初は省略不可能型なので特別に定義してやる
		mMemoryMap[12] = 0x90; mMemoryMap[13] = midiEvents[0].noteNum; mMemoryMap[14] = midiEvents[0].velocity;				//データをメモリマップに配置
		mTrack->event[3].data = &mMemoryMap[12];																			//データ位置を格納
		size_t trackdataSize = 24;		//Trackのバイトサイズを入れる。前述部分で24バイト分のデータバイトを使うことが分かってるので、初期値として格納させる(デルタタイム等を考慮すると15ではない)
		//midiEvents[0]のデルタタイムのサイズを加算する
		if (midiEvents[0].time & 0x0fe00000) { trackdataSize += 4; }
		else if (midiEvents[0].time & 0x001fc000) { trackdataSize += 3; }
		else if (midiEvents[0].time & 0x00003f80) { trackdataSize += 2; }
		else { trackdataSize += 1; }
		//省略可能なノートイベントを格納する
		int i;
		for (i = 1; i < noteSize; ++i) {
			//midiイベントについて、まずそのまま格納可能なものを入れる
			mTrack->event[i + 3].type = EVENT::NOTE_ON;
			mTrack->event[i + 3].time = midiEvents[i].time;
			mTrack->event[i + 3].datasize = 2;
			//メモリマップにデータを配置して、開始地点をevent構造に格納する
			mMemoryMap[(i * 2) + 13] = midiEvents[i].noteNum;
			mMemoryMap[(i * 2) + 1 + 13] = midiEvents[i].velocity;
			mTrack->event[i + 3].data = &mMemoryMap[(i * 2) + 13];
			//デルタタイムのバイト数を数える
			if (midiEvents[i].time & 0x0fe00000) { trackdataSize += 4; }
			else if (midiEvents[i].time & 0x001fc000) { trackdataSize += 3; }
			else if (midiEvents[i].time & 0x00003f80) { trackdataSize += 2; }
			else { trackdataSize += 1; }
			trackdataSize += 2;
		}
		//トラックの終了処理を最後に入れる
		mTrack->event[i + 3].type = EVENT::END_OF_TRACK;
		mTrack->event[i + 3].datasize = 0; mTrack->event[i + 3].data = nullptr;
		mTrack->tracksize = trackdataSize;
		return 0;
	}

private:
	//要素を破壊する関数はクラス自身のみ呼び出せるものとする
	void destroy() {
		for (int i = 0; i < mMaxTrackNum; ++i) {
			delete[] mTrack[i].event;
			mTrack[i].event = nullptr;
		}
		delete[] mTrack; mTrack = nullptr;
		delete[] mMemoryMap; mMemoryMap = nullptr;
		mMaxTrackNum = 0;
		mResolution = 0;
	}
	File* mFile;

	short mMaxTrackNum;		//トラックの数
	short mResolution;		//四分音符一つあたりの分解能の値が入る(480が入っていたらデルタタイム480で四分音符一つ分飛ぶ)

	uint8_t* mMemoryMap;	//書き込んだデータを格納しておく場所

	Track* mTrack;
};

#endif