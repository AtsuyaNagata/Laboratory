#ifndef MATRIX_H_2021_3_15_
#define MATRIX_H_2021_3_15_

#include<vector>
using namespace std;

template<class T> class Matrix
{
public:
	//TODO:コンストラクタの検証
	Matrix(int rowSize, int lineSize) :
		mRowSize(rowSize),
		mLineSize(lineSize),
		mMatrix(rowSize, vector<T>(lineSize, 0))		//0で埋める
	{
		/*
		mRowSize = rowSize;
		mLineSize = lineSize;
		mMatrix.resize(rowSize);
		//マトリックスのサイズを獲得しておく
		for (int i = 0; i < rowSize; ++i) {
			mMatrix[i].resize(mLineSize);
		}
		*/
	}

	//TODO:コピーコンストラクタの作成

	//指定位置のパラメータをセットするメンバ関数
	void setParameter(int row, int line, T param) {
		mMatrix[row][line] = param;
	}

	//行列の掛け算はデフォルトで数学的な考え方と一致させておく
	Matrix operator * (Matrix &matrix) {
		//掛け算後は行列のサイズが変動することを想定する
		Matrix buff(mRowSize, matrix.mLineSize);		//全ての要素はコンストラクタ内で0の初期化がなされる
		if (mLineSize != matrix.mRowSize) {				//掛けられる側の行のサイズと掛ける側の列のサイズは一致させる必要がある
			printf("error : matrix size lineSize = %d, rowSize = %d", mLineSize, matrix.mRowSize);
			return buff;
		}
		//各要素ごとの行列の計算
		for (int i = 0; i < mRowSize; ++i) {
			for (int j = 0; j < mLineSize; ++j) {
				for (int k = 0; k < mLineSize; ++k) {
					buff.mMatrix[i][j] += mMatrix[i][k] * matrix.mMatrix[k][j];
				}
			}
		}

		return buff;
	}

	vector<vector<T>> getMatrix() {
		return mMatrix;
	}

private:
	int mRowSize;					//列のサイズ
	int mLineSize;					//行のサイズ
	vector<vector<T>> mMatrix;		//行列の内容
};


#endif