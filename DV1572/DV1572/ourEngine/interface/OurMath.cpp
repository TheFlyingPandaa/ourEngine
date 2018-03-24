#include "OurMath.h"
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

		result.elements[3 + 0 * 4] = vector.x;
		result.elements[3 + 1 * 4] = vector.y;
		result.elements[3 + 2 * 4] = vector.z;

		return result;
	}

	float toRadians(float degrees)
	{
		return degrees * (PI / 180.0f);
	}

	float Length(Vec3 vector)
	{
		return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
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

