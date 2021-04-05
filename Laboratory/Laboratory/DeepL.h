#ifndef DEEPLEARNING_SAMPLE_2021_3_15_
#define DEEPLEARNING_SAMPLE_2021_3_15_

#include<vector>
#include<algorithm>

//パーセプトロンを表現するクラス
class Perceptron 
{
public:
	typedef struct Input 
	{
		double x;		//入力(0 ～ 1の値)
		double w;		//重み
	} Input;

	//引数なしコンストラクタも予め宣言しておく
	Perceptron():
		mTheta(0),
		mBias(0),
		mY(0){}

	//引数ありコンストラクタ
	Perceptron(double theta, double bias, int inputNum, double* x, double* w):
		mTheta(theta),
		mBias(bias),
		mY(0)
	{
		//データをコピーしていく
		for (int i = 0; i < inputNum; ++i) {
			mInputs[i].x = x[i];
			mInputs[i].w = w[i];
		}
	}

	//set関数群
	void setTheta(double theta) {
		mTheta = theta;
	}
	void setBias(double bias) {
		mBias = bias;
	}
	void setInput(int index, double x, double w) {
		if (index >= mInputs.size()) {
			printf("error : Perceptron Input index error (index = %d) ", index);
			return;
		}
		mInputs[index].x = x;
		mInputs[index].w = w;
	}
	void setInput(std::vector<Input> inputs) {
		mInputs = inputs;			//vectorのコピーコンストラクタ呼び出し
	}
	void setW(int index, double w) {
		if (index >= mInputs.size()) {
			printf("error : Perceptron Input W index error (index = %d) ", index);
			return;
		}
		mInputs[index].w = w;
	}

	//get関数群
	double getY() {
		return mY;
	}

	//メンバを用いたシグモイド関数によるy値の計算
	void sigmoid() {
		double x = 0.0;
		for (int i = 0; i < mInputs.size(); ++i) {
			x += mInputs[i].w * mInputs[i].x;
		}
		mY = 1 / (1 + exp(-x));		//h(x)をシグモイド関数としたときの計算
	}

	//メンバを用いたReLU関数によるy値の計算
	void ReLU() {
		double x = 0.0;
		for (int i = 0; i < mInputs.size(); ++i) {
			x += mInputs[i].w * mInputs[i].x;
		}
		//入力が0以上なら入力をそのまま出力
		if (x > 0) {
			mY = x;
		}
		else {
			mY = 0;
		}
	}

private:
	double mTheta;					//閾値θ
	double mBias;					//バイアス値
	double mY;						//出力値
	std::vector<Input> mInputs;		//入力値群(TODO：いちいち入力を受け取るよりも入力を受け取った際に総和してしまう手法がありそうなので削除する予定)
};

#define DELTA 0.000000001			//0で割られる可能性がある値に加算する用

class NeuralNetwork
{
public:
	//中間層の数、中間層の縦の長さ、入力の数、入力値、出力数を引数として獲得する
	NeuralNetwork(int middleLayerNum,  int* middleLayerLen, int inputNum, double* inputData, int outputNum) :
		mMiddleLayerNum(middleLayerNum),
		mMiddleLayerLen(nullptr),
		mInputNum(inputNum),
		mInputData(nullptr),
		mOutputs(nullptr),
		mPerceptrons(nullptr)
	{
		//中間層ごとのパーセプトロン数をセットする(例：一層目は3個、二層目は2個など)
		for (int i = 0; i < middleLayerNum; ++i) {
			mMiddleLayerLen[i] = middleLayerLen[i];
		}
		mOutputs = new double[mMiddleLayerLen[mMiddleLayerNum - 1]];		//最後の中間層のパーセプトロン数と出力層は一致する
		
		//入力データをコピーする
		mInputData = new double[mInputNum];
		for (int i = 0; i < mInputNum; ++i) {
			mInputData[i] = inputData[i];
		}
		mPerceptrons = new Perceptron*[mMiddleLayerNum];					//中間層の数だけパーセプトロン群を生成する
		for (int i = 0; i < mMiddleLayerNum; ++i) {
			mPerceptrons[i] = new Perceptron[mMiddleLayerLen[i]];			//ある中間層につき指定数のパーセプトロンを作る
			for (int j = 0; j < mMiddleLayerLen[i]; ++j) {					//パーセプトロンの値を初期化しておく
				mPerceptrons[i][j].setTheta(0);								//θの値のセットしどころさん
				mPerceptrons[i][j].setBias(0);								//バイアスのセット
				if (i == 0) {												//前の層が入力層の時は特別な処理を行う
					for (int k = 0; k < inputNum; ++k) {					//前の層の入力値と重みを格納していく
						mPerceptrons[i][j].setInput(k, inputData[k], 0);	//デフォルトの重みは0にしておく
					}
				}
				else {
					//前の中間層のgetYを用いてパーセプトロンを初期化しているが、Yの計算を行っていないため異常値になることに注意すること
					for (int k = 0; k < mMiddleLayerLen[i]; ++k) {
						mPerceptrons[i][j].setInput(k, mPerceptrons[k][j].getY(), 0);
					}
				}
			}
		}
	}

	~NeuralNetwork() {
		delete[] mInputData; mInputData = nullptr;
		for (int i = 0; i < mMiddleLayerNum; ++i) {
			delete[] mPerceptrons[i];
		}
	}

	//TODO：パーセプトロンの値をセットするメンバ関数

	//TODO：パーセプトロンの最終地から結果をソフトマックス関数で計算するメンバ関数
	void softmax() {
		//最後の中間層の出力の総和と最大値を計算しておく
		double sumY = 0.0;
		double maxVal = 0.000001;				//0にすると割る時バグるから0にはしない
		for (int i = 0; i < mMiddleLayerLen[mMiddleLayerNum - 1]; ++i) {	//出力層の出力数は最後の中間層のパーセプトロン数と一致する
			sumY += mPerceptrons[mMiddleLayerNum - 1][i].getY();			//最後の中間層の出力を総和する
			if (mPerceptrons[mMiddleLayerNum - 1][i].getY() > maxVal) {		//最大値を格納する
				maxVal = mPerceptrons[mMiddleLayerNum - 1][i].getY();
			}
		}
		sumY /= maxVal;					//総和も計算のために最大値で割っておく
		double expSumY = exp(sumY);		//計算で用いるexp(sumY)形式を予め計算しておく
		//TODO:確率で表現されるような総和すると1になる出力群が形成されるはず。要確認
		for (int i = 0; i < mMiddleLayerLen[mMiddleLayerNum - 1]; ++i) {
			mOutputs[i] = exp(mPerceptrons[mMiddleLayerNum - 1][i].getY() / maxVal) / expSumY;
		}
	}

	//パーセプトロンの最終地から結果を恒等関数で計算するメンバ関数
	void identityFunc() {
		for (int i = 0; i < mMiddleLayerLen[mMiddleLayerNum - 1]; ++i) {
			mOutputs[i] = mPerceptrons[mMiddleLayerNum - 1][i].getY();		//中間層の最終地点のYの値がそのまま出力になる
		}
	}

	//損失関数の計算を行うメンバ関数
	//二乗和誤差の計算。引数に教師データを受け取る
	double meanSquaredError(double* t, int size) {
		//サイズが違ったらエラーを吐いておく
		if (size != mMiddleLayerLen[mMiddleLayerNum - 1]) {
			printf("Teacher data error : data size failed! tSize(%d), mSize(%d)", size, mMiddleLayerLen[mMiddleLayerNum - 1]);
			return 0.0;
		}
		double E = 0.0;
		for (int i = 0; i < mMiddleLayerLen[mMiddleLayerNum - 1]; ++i) {
			E += 0.5 * pow((mOutputs[i] * t[i]), 2);		// 1/2 * Σ(yk -tk)^2
		}
		return E;
	}
	//交差エントロピー誤差の計算
	double crossEntropyError(double* t, int size) {
		//サイズが違ったらエラーを吐いておく
		if (size != mMiddleLayerLen[mMiddleLayerNum - 1]) {
			printf("Teacher data error : data size failed! tSize(%d), mSize(%d)", size, mMiddleLayerLen[mMiddleLayerNum - 1]);
			return 0.0;
		}
		double E = 0.0;
		for (int i = 0; i < mMiddleLayerLen[mMiddleLayerNum - 1]; ++i) {
			E -= log(mOutputs[i] + DELTA) * t[i];		// -Σtk*log(yk)
		}
		return E;
	}


private:
	int mMiddleLayerNum;		//中間層の数
	int* mMiddleLayerLen;		//中間層に存在するパーセプトロンの数を管理する配列
	int mInputNum;				//入力層の入力値の数
	double* mInputData;			//入力値の具体的な値
	double* mOutputs;			//出力層に出てきた結果を格納する配列
	Perceptron** mPerceptrons;	//ニューラルネットワーク内のパーセプトロンを表現するメンバ
};

#endif
