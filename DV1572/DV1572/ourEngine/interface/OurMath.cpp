#include "OurMath.h"
using namespace DirectX;
namespace OurMath
{
	float PI = 3.14159264f;


	Vec2::Vec2()
	{
		x = 0;
		y = 0;
	}

	Vec2::Vec2(float value)
	{

		x = value;
		y = value;
	}

	Vec2::Vec2(float xVal, float yVal)
	{
		x = xVal;
		y = yVal;
	}

	Vec2 & OurMath::Vec2::operator-=(const Vec2 & otherVec)
	{
		x -= otherVec.x;
		y -= otherVec.y;
		return Vec2(x, y);
	}

	bool Vec2::operator!=(const Vec2 & otherVec)
	{
		return x != otherVec.x || y != otherVec.y;
	}

	Vec2 Vec2::operator=(const Vec2 & otherVec)
	{
		x = otherVec.x;
		y = otherVec.y;
		return Vec2(x,y);
	}

	bool Vec2::operator==(const Vec2 & otherVec)
	{
		return x == otherVec.x && y == otherVec.y;
	}

	Vec2 & Vec2::operator-(const Vec2 & otherVec)
	{
		return Vec2(x - otherVec.x, y - otherVec.y);
	}

	Vec3::Vec3()
	{
		x = y = z = 0;
	}

	Vec3::Vec3(float xVal, float yVal, float zVal)
	{
		x = xVal;
		y = yVal;
		z = zVal;
	}

	Vec3::Vec3(const DirectX::XMFLOAT3 & vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}

	Vec3 Vec3::operator+=(const Vec3 & vec)
	{
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return Vec3(x, y, z);
	}

	Vec3 Vec3::operator-=(const Vec3 & vec)
	{
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return Vec3(x, y, z);
	}

	Vec3 Vec3::operator=(const DirectX::XMFLOAT4 & vector)
	{
		x = vector.x;
		y = vector.y;
		z = vector.z;
		return Vec3(x,y,z);
	}

	Vec3 & Vec3::Rotate(float angle, Vec3 Axis)
	{
		XMFLOAT4 dVec(x,y,z,1.0f);
		XMFLOAT3 rotationAxis(Axis.x, Axis.y, Axis.z);

		XMVECTOR ourVec = XMLoadFloat4(&dVec);
		XMVECTOR dAxis = XMLoadFloat3(&rotationAxis);
		XMMATRIX rotationMatrix = XMMatrixRotationAxis(dAxis, angle);
		XMVECTOR result = XMVector4Transform(ourVec, rotationMatrix);
		XMStoreFloat4(&dVec, result);
		*this = dVec;
		return *this;
	}

	Vec3 & Vec3::RotateX(float angle)
	{
		XMFLOAT4 vec(x, y, z, 1.0f);
		XMVECTOR dVec = XMLoadFloat4(&vec);
		XMMATRIX mat = XMMatrixRotationX(angle);
		dVec = XMVector4Transform(dVec, mat);
		XMStoreFloat4(&vec, dVec);
		*this = vec;
		return *this;
	}

	Vec3 & Vec3::RotateY(float angle)
	{
		XMFLOAT4 vec(x, y, z, 1.0f);
		XMVECTOR dVec = XMLoadFloat4(&vec);
		XMMATRIX mat = XMMatrixRotationY(angle);
		dVec = XMVector4Transform(dVec, mat);
		XMStoreFloat4(&vec, dVec);
		*this = vec;
		return *this;
	}

	Vec3 Vec3::toDegrees() const
	{
		return Vec3(x,y,z) * (180.0f / OurMath::PI);
	}

	Vec4::Vec4()
	{
		x = y = z = w = 0;
	}

	Vec4::Vec4(float xVal, float yVal, float zVal, float wVal)
	{
		x = xVal;
		y = yVal;
		z = zVal;
		w = wVal;
	}

	DirectX::XMVECTOR Vec4::toXMVECTOR() const
	{
		DirectX::XMVECTOR vector = DirectX::XMVectorSet(0, 0, 0, 0);
		DirectX::XMVectorSetY(vector, x);
		DirectX::XMVectorSetY(vector, y);
		DirectX::XMVectorSetZ(vector, z);
		DirectX::XMVectorSetW(vector, w);
		return vector;
	}

	Mat4::Mat4()
	{
		for (int i = 0; i < 4*4; i++)
				elements[i] = 0.0f;

		elements[0 + 0 * 4] = 1.0f;
		elements[1 + 1 * 4] = 1.0f;
		elements[2 + 2 * 4] = 1.0f;
		elements[3 + 3 * 4] = 1.0f;
	}

	Mat4::Mat4(float value)
	{
		for (int i = 0; i < 4 * 4; i++)
			elements[i] = 0.0f;

		elements[0 + 0 * 4] = value;
		elements[1 + 1 * 4] = value;
		elements[2 + 2 * 4] = value;
		elements[3 + 3 * 4] = value;
	}

	Mat4::Mat4(DirectX::XMFLOAT4X4 mat)
	{
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				elements[i + j * 4] = mat.m[i][j];

	}

	Mat4 & Mat4::Multiply(const Mat4 & other)
	{
		float data[16];
		for (int row = 0; row < 4; row++)
		{
			for (int col = 0; col < 4; col++)
			{
				float sum = 0.0f;
				for (int e = 0; e < 4; e++)
				{
					sum += elements[e + row * 4] * other.elements[col + e * 4];
				}
				data[col + row * 4] = sum;
			}
		}
		for (int i = 0; i < 4 * 4; i++)
			elements[i] = data[i];
		return *this;
	}

	Mat4 Mat4::operator*(const Mat4 & right)
	{
		return Multiply(right);
	}

	DirectX::XMFLOAT4X4 Mat4::ToXMFLOAT4X4() const
	{
		XMFLOAT4X4 xmMat;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				xmMat.m[i][j] = elements[i + j * 4];
		return xmMat;
	}

	Mat4 Mat4::Rotate(float angle, const Vec3 & axis)
	{
		Mat4 result(1.0f);

		float r = toRadians(angle);
		float c = cos(r);
		float s = sin(r);
		float omc = 1.0f - c;

		float x = axis.x;
		float y = axis.y;
		float z = axis.z;

		result.elements[0 + 0 * 4] = x * x * omc + c;
		result.elements[0 + 1 * 4] = y * x * omc + z * s;
		result.elements[0 + 2 * 4] = x * z * omc - y * s;

		result.elements[1 + 0 * 4] = x * y * omc - z * s;
		result.elements[1 + 1 * 4] = y * y * omc + c;
		result.elements[1 + 2 * 4] = y * z * omc + x * s;

		result.elements[2 + 0 * 4] = x * z * omc + y * s;
		result.elements[2 + 1 * 4] = y * z * omc - x * s;
		result.elements[2 + 2 * 4] = z * z * omc + c;
		return result;
	}

	Mat4 Mat4::Rotate(const Quaternion & quat)
	{
		Mat4 result;

		float qx, qy, qz, qw, qx2, qy2, qz2, qxqx2, qyqy2, qzqz2, qxqy2, qyqz2, qzqw2, qxqz2, qyqw2, qxqw2;
		qx = quat.x;
		qy = quat.y;
		qz = quat.z;
		qw = quat.w;
		qx2 = (qx + qx);
		qy2 = (qy + qy);
		qz2 = (qz + qz);
		qxqx2 = (qx * qx2);
		qxqy2 = (qx * qy2);
		qxqz2 = (qx * qz2);
		qxqw2 = (qw * qx2);
		qyqy2 = (qy * qy2);
		qyqz2 = (qy * qz2);
		qyqw2 = (qw * qy2);
		qzqz2 = (qz * qz2);
		qzqw2 = (qw * qz2);

		result.rows[0] = Vec4(((1.0f - qyqy2) - qzqz2), (qxqy2 - qzqw2), (qxqz2 + qyqw2), 0.0f);
		result.rows[1] = Vec4((qxqy2 + qzqw2), ((1.0f - qxqx2) - qzqz2), (qyqz2 - qxqw2), 0.0f);
		result.rows[2] = Vec4((qxqz2 - qyqw2), (qyqz2 + qxqw2), ((1.0f - qxqx2) - qyqy2), 0.0f);
		return result;
	}

	Mat4 Mat4::Translate(const Vec3 & vector)
	{
		Mat4 result(1.0f);
		
		XMMATRIX translation = XMMatrixTranslation(vector.x, vector.y, vector.z);
		XMFLOAT4X4 xmFloat;
		XMStoreFloat4x4(&xmFloat, translation);

		return Mat4(xmFloat);
	}

	std::ostream & operator<<(std::ostream& stream, Vec3 vector)
	{
		stream << "Vec3:(" << vector.x << "," << vector.y << "," << vector.z << ")";
		return stream;
	}

	float toRadians(float degrees)
	{
		return degrees * (PI / 180.0f);
	}

	float Length(Vec3 vector)
	{
		return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
	}

	Vec3 Normalize(const Vec3 & vec3)
	{
		DirectX::XMFLOAT3 vec;
		vec.x = vec3.x;
		vec.y = vec3.y;
		vec.z = vec3.z;

		DirectX::XMVECTOR notNormalized = DirectX::XMLoadFloat3(&vec);
		DirectX::XMVECTOR answer = DirectX::XMVector3Normalize(notNormalized);
		DirectX::XMStoreFloat3(&vec, answer);

		return Vec3(vec);
	}

	std::ostream & operator<<(std::ostream & stream, Mat4 mat)
	{
		stream << mat.rows[0].x << " " << mat.rows[1].x << " " << mat.rows[2].x << " " << mat.rows[3].x << "\n";
		stream << mat.rows[0].y << " " << mat.rows[1].y << " " << mat.rows[2].y << " " << mat.rows[3].y << "\n";
		stream << mat.rows[0].z << " " << mat.rows[1].z << " " << mat.rows[2].z << " " << mat.rows[3].z << "\n";
		stream << mat.rows[0].w << " " << mat.rows[1].w << " " << mat.rows[2].w << " " << mat.rows[3].w<< "\n";

		return stream;
	}

	Mat4 LookAtLH(Vec3 eye, Vec3 focusPosition, Vec3 up)
	{
		XMFLOAT3 xmEye(eye.x, eye.y, eye.z);
		XMFLOAT3 xmFocus(focusPosition.x, focusPosition.y, focusPosition.z);
		XMFLOAT3 xmUp(up.x, up.y, up.z);

		XMVECTOR xmvEye = XMLoadFloat3(&xmEye);
		XMVECTOR xmvFocus = XMLoadFloat3(&xmFocus);
		XMVECTOR xmvUp = XMLoadFloat3(&xmUp);

		XMMATRIX lookAtMat4 = XMMatrixLookAtLH(xmvEye, xmvFocus, xmvUp);
		XMFLOAT4X4 xmf44Matrix;
		XMStoreFloat4x4(&xmf44Matrix, lookAtMat4);
		return Mat4(xmf44Matrix);
	}

	Mat4 Transpose(const Mat4 & mat)
	{
		XMFLOAT4X4 xmFloat4x4 = mat.ToXMFLOAT4X4();
		Mat4 matrix;
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				matrix.elements[i + j * 4] = mat.elements[j + i * 4];
		return matrix;
	}

	Quaternion Quaternion::Conjugate() const
	{
		return Quaternion(-x, -y, -z, w);
	}

	Quaternion Quaternion::operator+(const Quaternion & quat) const
	{
		return Quaternion(x + quat.x, y + quat.y, z + quat.z, w + quat.w);
	}

}

