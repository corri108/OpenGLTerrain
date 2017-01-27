#pragma once
class NewCamera
{
private:
	float forwardVector[3];
	float rightVector[3];
	float upVector[3];
	float position[3];
	float speed;
	float turnSpeed;
public:
	NewCamera(void);
	NewCamera(float fov, float ww, float wh, float n, float f);
	void UpdatePerspective(void);
	void UpdateView(void);
	void transformOrientation(void);
	void transformTranslation(void);
	void pointAt(float* targetVector);
	void speedUp(void);
	void slowDown(void);
	void forward(void);
	void left(void);
	void right(void);
	void backward(void);
	void pitchUp(void);
	void pitchDown(void);
	void yawLeft(void);
	void yawRight(void);

	float FOV, WINDOWWIDTH, WINDOWHEIGHT, cNEAR, cFAR = 0;
};