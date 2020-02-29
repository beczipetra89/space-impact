#pragma once
class LifeIcon : public GameObject
{
public:

	virtual ~LifeIcon() {  }

	virtual void Create() {}

	virtual void Init(int x, int y)
	{
		horizontalPosition = x;
		verticalPosition = y;
		GameObject::Init();
	}
};

