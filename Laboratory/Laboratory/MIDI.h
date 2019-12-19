#ifndef MIDI_H_2019_11_21_
#define MIDI_H_2019_11_21_

#include"File.h"
#include<stdio.h>

bool isEvent(unsigned char byteData) {
	return (byteData == 0xff || byteData == 0xf0 || byteData == 0xf7 || byteData == 0xb0 || (byteData & 0xf0) == 0x90 || (byteData & 0xf0) == 0x80);
}

class MIDI : File{
public:
	MIDI(const char* filename) :
		File(filename),
		mTrack(0),
		mResolution(0),
		mEventLine(0)
	{
		const unsigned char* fileData = reinterpret_cast<unsigned char*>(mData);
		//ファイルの実体がなければここで落とす
		if (mSize == 0) {
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
			mTrack = 1;
		}
		//フォーマットが1の時
		if (fileData[8] == 0 && fileData[9] == 1) {
			//トラック数を獲得(2バイト)
			mTrack = (fileData[10] << 8) + fileData[11];
			//mTrackが0だと色々まずいので落とす
			if (mTrack == 0) {
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

		//トラック分のEventLineを用意する
		mEventLine = new EventLine[mTrack];
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
		int eventLinePoint = 0;
		for (int i = 14; i < mSize;) {
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
					else if (isEvent(fileData[j])) {
						if (fileData[j] == 0xff) {
							//FFから始まるデータはlenバイトでバイトの長さを指定しているので、その分のバイトを飛ばす
							unsigned char len = fileData[j + 2];
							j += 3 + len;
							count++;
						}
						else if (fileData[j] == 0xf0 || fileData[j] == 0xf7) {
							//0xf0分のデータバイトを飛ばす
							j += 1;
							//lenは可変長である事を考慮する
							int len = 0;
							for (int k = 0; k < 4; ++k) {
								len << 8;
								len += fileData[j + k] & 0x7f;
								if (fileData[j + k] & 0x80) {			//最上位ビットが1なら、次のビットも確認する
									//何もしない
								}
								else {
									//元々1バイト分のデータはある
									j += k + 1;
									break;
								}
							}
							//長さ分のバイトをぶち飛ばす
							j += len;
							count++;
						}
						else if ((fileData[j] & 0xf0) == 0x90 || (fileData[j] & 0xf0) == 0x80 || (fileData[j] & 0xf0) == 0xb0) {
							j += 3;
							count++;
						}
						else {
							printf("0x%x is not defined", fileData[j]);
							destroy();
							return;
						}
					}
					delta = !delta;
				}

				mEventLine[eventLinePoint].eventnum = count;
				mEventLine[eventLinePoint].event = new Event[count];

				//要素を具体的に格納していく（2週目）
				//最初はデルタタイムのデータ
				delta = true;
				int eventPoint = 0;
				for (int j = i; j < (trackSize + i);) {
					if (delta) {
						unsigned int time = 0;
						//最大4バイトだから実質O(1)
						for (int k = 0; k < 4; ++k) {
							time << 8;
							if (fileData[j + k] & 0x80) {
								time += fileData[j + k] & 0x7f;
							}
							else {
								//デルタタイム分のバイト数進める
								j += k + 1;
								break;
							}
						}
						mEventLine[eventLinePoint].event[eventPoint].time = time;
					}
					else if (isEvent(fileData[j])) {
						if (fileData[j] == 0xff) {
							//データのイベントタイプを判別する値
							unsigned char eventType = fileData[j + 1];
							//FFから始まるデータはlenバイトでバイトの長さを指定しているので、その分のバイトを飛ばす
							unsigned char len = fileData[j + 2];
							//長さのデータ格納
							mEventLine[eventLinePoint].event[eventPoint].datasize = len;
							if (len == 0) {
								//サイズが0ならデータを獲得しない
								mEventLine[eventLinePoint].event[eventPoint].data = nullptr;
							}
							else {
								//データの開始地点のポインタをいただく
								mEventLine[eventLinePoint].event[eventPoint].data = &fileData[j + 3];
							}
							//イベントタイプをそれぞれ格納する
							switch (eventType)
							{
							//case 0:
							//	mEventLine[eventLinePoint].event[eventPoint].type = EVENT::SEQUENCE_NUMBER; break;
							case 1:
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::COMMENT; break;
							case 2:
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::COPY_RIGHT; break;
							case 3:
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::SEQUENCE_TRACK_NAME; break;
							case 4:
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::INSTRUMENT_NAME; break;
							case 5:
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::LYRIC; break;
							//case 0x20:
							//	mEventLine[eventLinePoint].event[eventPoint].type = EVENT::CHANNEL_PREFIX; break;
							//csse 0x21:
							//	mEventLine[eventLinePoint].event[eventPoint].type = EVENT::PORT_SPECIFIED; break;
							case 0x2f:
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::END_OF_TRACK; break;
							case 0x51:
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::SET_TEMPO; break;
							case 0x58:
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::TIME_SIGNATURE; break;
							case 0x59:
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::KEY_SIGNATURE; break;

							default:
								break;
							}

							j += 3 + len;
						}
						else if (fileData[j] == 0xf0 || fileData[j] == 0xf7) {
							//f0(f7)識別子分のビットを飛ばす
							j += 1;
							//lenは可変長である事を考慮する
							int len = 0;
							for (int k = 0; k < 4; ++k) {
								len << 8;
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
							mEventLine[eventLinePoint].event[eventPoint].type = EVENT::SYSEX;
							//長さを入れてやる
							mEventLine[eventLinePoint].event[eventPoint].datasize = len;
							//長さを獲得してしまえば後はこっちのもの
							mEventLine[eventLinePoint].event[eventPoint].data = &fileData[j + len];
							//長さ分のバイトをぶち飛ばす
							j += len;
						}
						//MIDIイベントは、識別子にデータが入ってる特殊な形
						else if ((fileData[j] & 0xf0) == 0x90 || (fileData[j] & 0xf0) == 0x80 || (fileData[j] & 0xf0) == 0xb0) {
							mEventLine[eventLinePoint].event[eventPoint].datasize = 3;
							mEventLine[eventLinePoint].event[eventPoint].data = &fileData[j];
							//
							if ((fileData[j] & 0xf0) == 0x90) {
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::NOTE_ON;
							}
							else if ((fileData[j] & 0xf0) == 0x80) {
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::NOTE_OFF;
							}
							else if ((fileData[j] & 0xf0) == 0xb0) {
								mEventLine[eventLinePoint].event[eventPoint].type = EVENT::CONTROL;
							}
							j += 3;
						}
						else {
							int a = 0;
							printf("a = %d\n", a);
							printf("0x%x is not defined\n", fileData[j]);
							return;
						}
						eventPoint += 1;
					}
					delta = !delta;
				}
				i += trackSize;
				eventLinePoint += 1;
			}
			else {
				//MTrkが来ないのはおかしいので落とす
				destroy();
				return;
			}
		}

	}
	~MIDI() {
		destroy();
	}

	enum class EVENT : unsigned char {			
		//メタイベント				//FF｜イベントタイプ｜データ長｜データ
		//SEQUENCE_NUMBER,			//FF 00 02 シーケンス番号、フォーマット0と1
		COMMENT,					//FF 01 len
		COPY_RIGHT,					//FF 02 len
		SEQUENCE_TRACK_NAME,		//FF 03 len
		INSTRUMENT_NAME,			//FF 04 len
		LYRIC,						//FF 05 len  lyricは歌詞の事
		//CHANNEL_PREFIX,			//FF 20 01 cc MIDIイベントじゃないのに(SysEx or メタイベント)MIDIチャンネルを指定する際に用いる
		//PORT_SPECIFIED,			//FF 21 01 pp 出力ポートの指定	0～3がそれぞれポートの1～4に対応する(例外はあるが、データのみでは判別不可)
		END_OF_TRACK,				//FF 2F 00
		SET_TEMPO,					//FF 51 03
		TIME_SIGNATURE,				//FF 58 04 nn dd cc bb | 拍子 nn = 分子 dd = 分子 cc = メトロノーム間隔[四分音符間隔なら18H] bb = 四分音符当たりの三十二分音符の数
		KEY_SIGNATURE,				//FF 59 02 sf ml キー(調)を示す	sf = 正:シャープの数 or 負:フラットの数		ml = 0:長調(メジャー) 1:短調(マイナー)

		//システムエクスクルシーブメッセージを格納するイベント。個々の楽器メーカが独自に作成したメッセージを格納する。MIDIデバイスに何かしら設定するのに利用される
		SYSEX,						//F0 データ長 エクスクルシーブメッセージ F7
		F7_SYSEX,					//F7 データ長 エクスクルシーブメッセージ

		//MIDIイベント
		CONTROL,					//B0 コントローラ設定
		NOTE_ON,					//9n ノート・オン
		NOTE_OFF					//8n ノート・オフ
	};
	//Eventの構造
	typedef struct Event {
		EVENT type;					//何のイベントかを格納する
		size_t datasize;			//それぞれのイベントのデータサイズを格納する
		const unsigned char* data;	//イベントのデータを格納する
		unsigned int time;			//デルタタイムを格納する
	} Event;

	typedef struct EventLine {
		Event* event;
		size_t eventnum;
	} EventLine;

	EventLine* getEventLine(int n) {
		return &mEventLine[n];
	}

private:
	//要素を破壊する関数はクラス自身のみ呼び出せるものとする
	 void destroy() {
		 for (int i = 0; i < mTrack; ++i) {
			 delete[] mEventLine[i].event;
			 mEventLine[i].event = nullptr;
		 }
		 delete[] mEventLine;
		 mEventLine = nullptr;
	 }

	 short mTrack;		//トラックの数
	 short mResolution;	//分解能の値

	 EventLine* mEventLine;
};

#endif