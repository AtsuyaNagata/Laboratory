#ifndef OPENAL_H_2020_1_10_
#define OPENAL_H_2020_1_10_

/*"https://hatakenoko.hateblo.jp/entry/2018/05/24/220046"�ɂ���R�[�h*/
#define SAMPLINGRATE 44100
//�����Ń΂�錾
#define _USE_MATH_DEFINES

#include <al.h>
#include <alc.h>
#include <math.h>
#include <stdlib.h>

#pragma comment(lib, "OpenAL32.lib")

int playAL(void) {
	//OpenAL�̉������@���܂��Ȃ��I��
	ALCdevice* device = alcOpenDevice(NULL);					//�o�̓f�o�C�X���w��(�X�s�[�J�[�Ȃ�)������NULL��������f�t�H���g�̃X�s�[�J�[���o�͐�ɂȂ�
	ALCcontext* context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);								//�����̃\�[�X���甭������������ɂ܂Ƃ߂ďo�͂���錾

	//ALuint�́AOpenAL�̋K�i�ɍ��킹���f�[�^������肷��̂ɗp������ϐ�
	ALuint buffer;		//�o�b�t�@(�ۑ��̈�Ƃ����g���Ƃ��T���v��������̃r�b�g���Ƃ����Ǘ����Ă�)��錾
	ALuint source;		//�\�[�X(PLAYING�Ƃ�PAUSED�Ƃ����Ǘ����Ă�)��錾
	//����𐶐�
	alGenBuffers(1, &buffer);
	alGenSources(1, &source);

	//new���Z�q�ŉ���������z��𓮓I�m��
	signed short* wav_data = new signed short[SAMPLINGRATE];
	//������440Hz��sin�g��1�b�ԕ�����
	for (int i = 0; i < SAMPLINGRATE; i++) {
		wav_data[i] = 32767 * sin(2 * M_PI * i * 440 / SAMPLINGRATE);		//(2�� x i/SAMPLINGLATE) x 440�A32767��16bitwav�̍ő�T�C�Y
	}
	//�o�b�t�@�ɉ����f�[�^������
	//�������F�f�[�^������o�b�t�@�A�������F�f�[�^�t�H�[�}�b�g�i���m�����A�X�e���I�A8�A16�r�b�g�j�A��O�����F�����f�[�^�A��l�����F�����f�[�^�̃T�C�Y�A��܈����F�T���v�����O���[�g
	alBufferData(buffer, AL_FORMAT_MONO16, wav_data, SAMPLINGRATE * sizeof(signed short), SAMPLINGRATE);
	//�\�[�X�Ƀo�b�t�@��K�p
	alSourcei(source, AL_BUFFER, buffer);
	//���[�v�Đ���ON
	alSourcei(source, AL_LOOPING, AL_TRUE);
	//�\�[�X���Đ��I
	alSourcePlay(source);

	//�����ňꎞ��~
	system("PAUSE");

	//���I�m�ۂ�����������j��
	delete[] wav_data;
	// �o�b�t�@�̔j��
	alDeleteBuffers(1, &buffer);
	// �\�[�X�̔j��
	alDeleteSources(1, &source);

	//OpenAL�̌�n��
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(context);
	alcCloseDevice(device);
	return 0;
}

#endif // !OPENAL_H_2020_1_10
