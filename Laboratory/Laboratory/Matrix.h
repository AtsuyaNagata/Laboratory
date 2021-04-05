#ifndef MATRIX_H_2021_3_15_
#define MATRIX_H_2021_3_15_

#include<vector>
using namespace std;

template<class T> class Matrix
{
public:
	//TODO:�R���X�g���N�^�̌���
	Matrix(int rowSize, int lineSize) :
		mRowSize(rowSize),
		mLineSize(lineSize),
		mMatrix(rowSize, vector<T>(lineSize, 0))		//0�Ŗ��߂�
	{
		/*
		mRowSize = rowSize;
		mLineSize = lineSize;
		mMatrix.resize(rowSize);
		//�}�g���b�N�X�̃T�C�Y���l�����Ă���
		for (int i = 0; i < rowSize; ++i) {
			mMatrix[i].resize(mLineSize);
		}
		*/
	}

	//TODO:�R�s�[�R���X�g���N�^�̍쐬

	//�w��ʒu�̃p�����[�^���Z�b�g���郁���o�֐�
	void setParameter(int row, int line, T param) {
		mMatrix[row][line] = param;
	}

	//�s��̊|���Z�̓f�t�H���g�Ő��w�I�ȍl�����ƈ�v�����Ă���
	Matrix operator * (Matrix &matrix) {
		//�|���Z��͍s��̃T�C�Y���ϓ����邱�Ƃ�z�肷��
		Matrix buff(mRowSize, matrix.mLineSize);		//�S�Ă̗v�f�̓R���X�g���N�^����0�̏��������Ȃ����
		if (mLineSize != matrix.mRowSize) {				//�|�����鑤�̍s�̃T�C�Y�Ɗ|���鑤�̗�̃T�C�Y�͈�v������K�v������
			printf("error : matrix size lineSize = %d, rowSize = %d", mLineSize, matrix.mRowSize);
			return buff;
		}
		//�e�v�f���Ƃ̍s��̌v�Z
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
	int mRowSize;					//��̃T�C�Y
	int mLineSize;					//�s�̃T�C�Y
	vector<vector<T>> mMatrix;		//�s��̓��e
};


#endif