#pragma once
#include "Engine/GameObject.h"

class Timer :
    public GameObject
{
	float currentTime_;//�o�ߎ��Ԃ�ێ�����ϐ�
public:
	Timer(GameObject* parent);
	void Initialize() override {};
	void Update() override {};
	void Draw() override {};
	void Release() override {};

	void ResetTime(const float& time) { currentTime_ = time; }//�^�C�}�[�̃��Z�b�g
	bool TimeElapsed(const float& time);//�w�肵���b�����o������true��Ԃ�
	//�w�肵���b�����o������true��Ԃ�
	//����Ƀ��Z�b�g����Ȃ��^�C�}�[
	bool NoResetTimeElapsed();
};

