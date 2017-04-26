#ifndef _Camara_
#define _Camara_

class Camara
{
public:
	D3DXVECTOR3 Position;
	D3DXVECTOR3 Target;
	D3DXVECTOR3 Up;

	D3DXMATRIX View;
	D3DXMATRIX Projection;

	D3DXVECTOR3 DefaultForward;
	D3DXVECTOR3 DefaultRight;
	D3DXVECTOR3 Forward;
	D3DXVECTOR3 Right;

	D3DXMATRIX RotationMatrix;
	D3DXMATRIX groundWorld;

	float Yaw;
	float Pitch;
	float distance;
	float Zoom;
	bool isTPS;

	Camara(D3DXVECTOR3 Position, D3DXVECTOR3 Target, D3DXVECTOR3 Up, int Width, int Height)
	{
		this->Position = Position;
		this->Target = Target;
		this->Up = Up;

		//Set the View matrix 
		D3DXMatrixLookAtLH(&View, &Position, &Target, &Up);

		//Set the Projection matrix
		D3DXMatrixPerspectiveFovLH(&Projection, 
			D3DXToRadian(45), (float)Width / Height, 
			0.01f, 1100.0f); //0.2f, 1000.0f);

		DefaultForward = D3DXVECTOR3(0.0f,0.0f,1.0f);
		DefaultRight = D3DXVECTOR3(1.0f,0.0f,0.0f);
		Forward = D3DXVECTOR3(0.0f,0.0f,1.0f);
		Right = D3DXVECTOR3(1.0f,0.0f,0.0f);

		Yaw = 0.0f;
		Pitch = 0.0f;
		distance = 10.0f;
		isTPS = false;
	}
	//Camara(D3DXVECTOR3 Position, D3DXVECTOR3 Target, D3DXVECTOR3 Up, int Width, int Height, float renderDistance)
	//{
	//	this->Position = Position;
	//	this->Target = Target;
	//	this->Up = Up;
	//	D3DXMatrixLookAtLH(&View, &Position, &Target, &Up);
	//	D3DXMatrixPerspectiveFovLH(&Projection, D3DXToRadian(45), (float)Width / Height, 1.0f, renderDistance);
	//	DefaultForward = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	//	DefaultRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	//	Forward = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	//	Right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	//	Yaw = 0.0f;
	//	Pitch = 0.0f;
	//	Zoom = 15.0f;
	//}

	void setDistance(float distance)
	{
		this->distance = distance;
	}

	D3DXVECTOR2 getXZ(){
		return D3DXVECTOR2(Position.x, Position.z);
	}

	void UpdateCamera(float moveBackForward, float moveLeftRight, float yaw, float pitch)
	{
		Yaw += yaw;
		Pitch += pitch;

		D3DXMatrixRotationYawPitchRoll(&RotationMatrix, Yaw, Pitch, 0);
		D3DXVec3TransformCoord(&Target, &DefaultForward, &RotationMatrix);
		D3DXVec3Normalize(&Target, &Target);

		D3DXMATRIX RotateYTempMatrix;
		D3DXMatrixRotationY(&RotateYTempMatrix, Yaw);

		D3DXVec3TransformCoord(&Right, &DefaultRight, &RotateYTempMatrix);
		D3DXVec3TransformCoord(&Up, &Up, &RotateYTempMatrix);
		D3DXVec3TransformCoord(&Forward, &DefaultForward, &RotateYTempMatrix);

		Position += moveLeftRight*Right;
		Position += moveBackForward*Forward;

		moveLeftRight = 0.0f;
		moveBackForward = 0.0f;

		Target = Position + Target;

		D3DXMatrixLookAtLH(&View, &Position, &Target, &Up);
	}

	void UpdateCameraTPS(float moveBackForward, float moveLeftRight, float yaw, float pitch)
	{
		Yaw += yaw;
		Pitch += pitch;
		isTPS = true;

		D3DXMatrixRotationYawPitchRoll(&RotationMatrix, Yaw, Pitch, 0);
		D3DXVec3TransformCoord(&Position, &DefaultForward, &RotationMatrix);
		D3DXVec3Normalize(&Position, &Position);

		D3DXMATRIX RotateYTempMatrix;
		D3DXMatrixRotationY(&RotateYTempMatrix, Yaw);

		D3DXVec3TransformCoord(&Right, &DefaultRight, &RotationMatrix);
		D3DXVec3TransformCoord(&Forward, &DefaultForward, &RotationMatrix);
		D3DXVec3Cross(&Up, &Forward, &Right);

		Target -= moveLeftRight*Right;
		Target -= moveBackForward*Forward;

		Position = (Position * distance) + Target;

		D3DXMatrixLookAtLH(&View, &Position, &Target, &Up);
	}

	void UpdateCameraFree(float moveBackForward, float moveLeftRight, float yaw, float pitch)
	{
		Yaw += yaw;
		Pitch += pitch;

		D3DXMatrixRotationYawPitchRoll(&RotationMatrix, Yaw, Pitch, 0);
		D3DXVec3TransformCoord(&Target, &DefaultForward, &RotationMatrix);
		D3DXVec3Normalize(&Target, &Target);

		D3DXVec3TransformCoord(&Right, &DefaultRight, &RotationMatrix);
		D3DXVec3TransformCoord(&Forward, &DefaultForward, &RotationMatrix);
		D3DXVec3Cross(&Up, &Forward, &Right);

		Position += moveLeftRight*Right;
		Position += moveBackForward*Forward;

		moveLeftRight = 0.0f;
		moveBackForward = 0.0f;

		Target = Position + Target;

		D3DXMatrixLookAtLH(&View, &Position, &Target, &Up);
	}

	void setSkyCamara(Camara *camara)
	{
		D3DXMatrixRotationYawPitchRoll(&RotationMatrix, camara->Yaw, camara->Pitch, 0);
		D3DXVec3TransformCoord(&Target, &DefaultForward, &RotationMatrix);
		D3DXVec3Normalize(&Target, &Target);

		Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			D3DXMatrixLookAtLH(&View, &Target, &Position, &Up);
	}

	void setSkyCamara2(Camara *camara)
	{
		D3DXMatrixRotationYawPitchRoll(&RotationMatrix, camara->Yaw, camara->Pitch, 0);
		D3DXVec3TransformCoord(&Target, &DefaultForward, &RotationMatrix);
		D3DXVec3Normalize(&Target, &Target);

		Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			D3DXMatrixLookAtLH(&View, &Position, &Target, &Up);
		
	}
};

#endif