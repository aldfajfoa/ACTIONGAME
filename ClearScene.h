#pragma once
#include "Engine/GameObject.h"

class ClearScene:
	public GameObject
{
	int hImage_;
public:
	ClearScene(GameObject* parent);
	void Initialize() override {};
	void Update() override;
	void Draw() override;
	void Release() override;
};

