#ifndef DEEPLEARNING_SAMPLE_2021_3_15_
#define DEEPLEARNING_SAMPLE_2021_3_15_

#include<vector>
#include<algorithm>

//�p�[�Z�v�g������\������N���X
class Perceptron 
{
public:
	typedef struct Input 
	{
		double x;		//����(0 �` 1�̒l)
		double w;		//�d��
	} Input;

	//�����Ȃ��R���X�g���N�^���\�ߐ錾���Ă���
	Perceptron():
		mTheta(0),
		mBias(0),
		mY(0){}

	//��������R���X�g���N�^
	Perceptron(double theta, double bias, int inputNum, double* x, double* w):
		mTheta(theta),
		mBias(bias),
		mY(0)
	{
		//�f�[�^���R�s�[���Ă���
		for (int i = 0; i < inputNum; ++i) {
			mInputs[i].x = x[i];
			mInputs[i].w = w[i];
		}
	}

	//set�֐��Q
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
		mInputs = inputs;			//vector�̃R�s�[�R���X�g���N�^�Ăяo��
	}
	void setW(int index, double w) {
		if (index >= mInputs.size()) {
			printf("error : Perceptron Input W index error (index = %d) ", index);
			return;
		}
		mInputs[index].w = w;
	}

	//get�֐��Q
	double getY() {
		return mY;
	}

	//�����o��p�����V�O���C�h�֐��ɂ��y�l�̌v�Z
	void sigmoid() {
		double x = 0.0;
		for (int i = 0; i < mInputs.size(); ++i) {
			x += mInputs[i].w * mInputs[i].x;
		}
		mY = 1 / (1 + exp(-x));		//h(x)���V�O���C�h�֐��Ƃ����Ƃ��̌v�Z
	}

	//�����o��p����ReLU�֐��ɂ��y�l�̌v�Z
	void ReLU() {
		double x = 0.0;
		for (int i = 0; i < mInputs.size(); ++i) {
			x += mInputs[i].w * mInputs[i].x;
		}
		//���͂�0�ȏ�Ȃ���͂����̂܂܏o��
		if (x > 0) {
			mY = x;
		}
		else {
			mY = 0;
		}
	}

private:
	double mTheta;					//臒l��
	double mBias;					//�o�C�A�X�l
	double mY;						//�o�͒l
	std::vector<Input> mInputs;		//���͒l�Q(TODO�F�����������͂��󂯎��������͂��󂯎�����ۂɑ��a���Ă��܂���@�����肻���Ȃ̂ō폜����\��)
};

#define DELTA 0.000000001			//0�Ŋ�����\��������l�ɉ��Z����p

class NeuralNetwork
{
public:
	//���ԑw�̐��A���ԑw�̏c�̒����A���͂̐��A���͒l�A�o�͐��������Ƃ��Ċl������
	NeuralNetwork(int middleLayerNum,  int* middleLayerLen, int inputNum, double* inputData, int outputNum) :
		mMiddleLayerNum(middleLayerNum),
		mMiddleLayerLen(nullptr),
		mInputNum(inputNum),
		mInputData(nullptr),
		mOutputs(nullptr),
		mPerceptrons(nullptr)
	{
		//���ԑw���Ƃ̃p�[�Z�v�g���������Z�b�g����(��F��w�ڂ�3�A��w�ڂ�2�Ȃ�)
		for (int i = 0; i < middleLayerNum; ++i) {
			mMiddleLayerLen[i] = middleLayerLen[i];
		}
		mOutputs = new double[mMiddleLayerLen[mMiddleLayerNum - 1]];		//�Ō�̒��ԑw�̃p�[�Z�v�g�������Əo�͑w�͈�v����
		
		//���̓f�[�^���R�s�[����
		mInputData = new double[mInputNum];
		for (int i = 0; i < mInputNum; ++i) {
			mInputData[i] = inputData[i];
		}
		mPerceptrons = new Perceptron*[mMiddleLayerNum];					//���ԑw�̐������p�[�Z�v�g�����Q�𐶐�����
		for (int i = 0; i < mMiddleLayerNum; ++i) {
			mPerceptrons[i] = new Perceptron[mMiddleLayerLen[i]];			//���钆�ԑw�ɂ��w�萔�̃p�[�Z�v�g���������
			for (int j = 0; j < mMiddleLayerLen[i]; ++j) {					//�p�[�Z�v�g�����̒l�����������Ă���
				mPerceptrons[i][j].setTheta(0);								//�Ƃ̒l�̃Z�b�g���ǂ��낳��
				mPerceptrons[i][j].setBias(0);								//�o�C�A�X�̃Z�b�g
				if (i == 0) {												//�O�̑w�����͑w�̎��͓��ʂȏ������s��
					for (int k = 0; k < inputNum; ++k) {					//�O�̑w�̓��͒l�Əd�݂��i�[���Ă���
						mPerceptrons[i][j].setInput(k, inputData[k], 0);	//�f�t�H���g�̏d�݂�0�ɂ��Ă���
					}
				}
				else {
					//�O�̒��ԑw��getY��p���ăp�[�Z�v�g���������������Ă��邪�AY�̌v�Z���s���Ă��Ȃ����߈ُ�l�ɂȂ邱�Ƃɒ��ӂ��邱��
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

	//TODO�F�p�[�Z�v�g�����̒l���Z�b�g���郁���o�֐�

	//TODO�F�p�[�Z�v�g�����̍ŏI�n���猋�ʂ��\�t�g�}�b�N�X�֐��Ōv�Z���郁���o�֐�
	void softmax() {
		//�Ō�̒��ԑw�̏o�͂̑��a�ƍő�l���v�Z���Ă���
		double sumY = 0.0;
		double maxVal = 0.000001;				//0�ɂ���Ɗ��鎞�o�O�邩��0�ɂ͂��Ȃ�
		for (int i = 0; i < mMiddleLayerLen[mMiddleLayerNum - 1]; ++i) {	//�o�͑w�̏o�͐��͍Ō�̒��ԑw�̃p�[�Z�v�g�������ƈ�v����
			sumY += mPerceptrons[mMiddleLayerNum - 1][i].getY();			//�Ō�̒��ԑw�̏o�͂𑍘a����
			if (mPerceptrons[mMiddleLayerNum - 1][i].getY() > maxVal) {		//�ő�l���i�[����
				maxVal = mPerceptrons[mMiddleLayerNum - 1][i].getY();
			}
		}
		sumY /= maxVal;					//���a���v�Z�̂��߂ɍő�l�Ŋ����Ă���
		double expSumY = exp(sumY);		//�v�Z�ŗp����exp(sumY)�`����\�ߌv�Z���Ă���
		//TODO:�m���ŕ\�������悤�ȑ��a�����1�ɂȂ�o�͌Q���`�������͂��B�v�m�F
		for (int i = 0; i < mMiddleLayerLen[mMiddleLayerNum - 1]; ++i) {
			mOutputs[i] = exp(mPerceptrons[mMiddleLayerNum - 1][i].getY() / maxVal) / expSumY;
		}
	}

	//�p�[�Z�v�g�����̍ŏI�n���猋�ʂ��P���֐��Ōv�Z���郁���o�֐�
	void identityFunc() {
		for (int i = 0; i < mMiddleLayerLen[mMiddleLayerNum - 1]; ++i) {
			mOutputs[i] = mPerceptrons[mMiddleLayerNum - 1][i].getY();		//���ԑw�̍ŏI�n�_��Y�̒l�����̂܂܏o�͂ɂȂ�
		}
	}

	//�����֐��̌v�Z���s�������o�֐�
	//���a�덷�̌v�Z�B�����ɋ��t�f�[�^���󂯎��
	double meanSquaredError(double* t, int size) {
		//�T�C�Y���������G���[��f���Ă���
		if (size != mMiddleLayerLen[mMiddleLayerNum - 1]) {
			printf("Teacher data error : data size failed! tSize(%d), mSize(%d)", size, mMiddleLayerLen[mMiddleLayerNum - 1]);
			return 0.0;
		}
		double E = 0.0;
		for (int i = 0; i < mMiddleLayerLen[mMiddleLayerNum - 1]; ++i) {
			E += 0.5 * pow((mOutputs[i] * t[i]), 2);		// 1/2 * ��(yk -tk)^2
		}
		return E;
	}
	//�����G���g���s�[�덷�̌v�Z
	double crossEntropyError(double* t, int size) {
		//�T�C�Y���������G���[��f���Ă���
		if (size != mMiddleLayerLen[mMiddleLayerNum - 1]) {
			printf("Teacher data error : data size failed! tSize(%d), mSize(%d)", size, mMiddleLayerLen[mMiddleLayerNum - 1]);
			return 0.0;
		}
		double E = 0.0;
		for (int i = 0; i < mMiddleLayerLen[mMiddleLayerNum - 1]; ++i) {
			E -= log(mOutputs[i] + DELTA) * t[i];		// -��tk*log(yk)
		}
		return E;
	}


private:
	int mMiddleLayerNum;		//���ԑw�̐�
	int* mMiddleLayerLen;		//���ԑw�ɑ��݂���p�[�Z�v�g�����̐����Ǘ�����z��
	int mInputNum;				//���͑w�̓��͒l�̐�
	double* mInputData;			//���͒l�̋�̓I�Ȓl
	double* mOutputs;			//�o�͑w�ɏo�Ă������ʂ��i�[����z��
	Perceptron** mPerceptrons;	//�j���[�����l�b�g���[�N���̃p�[�Z�v�g������\�����郁���o
};

#endif
