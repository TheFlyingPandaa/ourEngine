#pragma once
#include <DirectXMath.h>
#include <iostream>

namespace OurMath
{
	extern float PI;
	// VECTORS
	struct Vec2
	{
		DirectX::XMFLOAT2 vec;

		float x;
		float y;
		Vec2();
		
		Vec2(float value);
		Vec2(float xVal, float yVal);

		Vec2& operator-=(const Vec2& otherVec);
		bool operator!= (const Vec2& otherVec);
		Vec2 operator=(const Vec2& otherVec);
		bool operator==(const Vec2& otherVec);
		Vec2& operator-(const Vec2& otherVec);

	};
	struct Vec3
	{
		float x;
		float y;
		float z;
		Vec3();
		Vec3(float xVal, float yVal, float zVal);
		Vec3(const DirectX::XMFLOAT3& vec);
		Vec3 Negative() const
		{
			return Vec3(-x, -y, -z);
		}
		
		Vec3 operator*(float value) const
		{
			return Vec3(x * value, y * value, z * value);
		}
		Vec3 operator-(const Vec3& vec) const
		{
			return Vec3(x - vec.x, y - vec.y, z - vec.z);
		}
		
		Vec3 operator+(const Vec3& vec) const
		{
			return Vec3(
				x + vec.x,
				y + vec.y,
				z + vec.z);
		}
		Vec3 operator+=(const Vec3& vec);
		Vec3 operator-=(const Vec3& vec);
		Vec3 operator=(const DirectX::XMFLOAT4& vector);

		DirectX::XMFLOAT3 toXMFLOAT3()
		{
			DirectX::XMFLOAT3 returnVec;
			returnVec.x = x;
			returnVec.y = y;
			returnVec.z = z;
			return returnVec;
		}

		Vec3& Rotate(float angle, Vec3 Axis);
		Vec3& RotateX(float angle);
		Vec3& RotateY(float angle);

		Vec3 toDegrees() const;

		static Vec3 ZAxis()
		{
			return Vec3(0, 0, 1);
		}
		static Vec3 XAxis()
		{
			return Vec3(1, 0, 0);
		}
		static Vec3 YAxis()
		{
			return Vec3(0, 1, 0);
		}

		friend std::ostream& operator<<(std::ostream& stream, Vec3 vector);
	};
	Vec3 Normalize(const Vec3& vec3);

	struct Vec4
	{
		float x, y, z, w;
		Vec4();
		Vec4(float xVal, float yVal, float zVal, float wVal);
		
		DirectX::XMVECTOR toXMVECTOR() const;
	};

	// QUARTERNION
	struct Quaternion
	{
		float x, y, z, w;
		Quaternion(float xVal, float yVal, float zVal, float wVal)
		{
			x = xVal;
			y = yVal;
			z = zVal;
			w = wVal;
		}
		
		static Vec3 Rotate(const Quaternion& quat, const Vec3& vec)
		{
			float tmpX, tmpY, tmpZ, tmpW;
			tmpX = (((quat.w * vec.x) + (quat.y * vec.z)) - (quat.z * vec.y));
			tmpY = (((quat.w * vec.y) + (quat.z * vec.x)) - (quat.x * vec.z));
			tmpZ = (((quat.w * vec.z) + (quat.x * vec.y)) - (quat.y * vec.x));
			tmpW = (((quat.x * vec.x) + (quat.y * vec.y)) + (quat.z * vec.z));
			return Vec3(
				((((tmpW * quat.x) + (tmpX * quat.w)) - (tmpY * quat.z)) + (tmpZ * quat.y)),
				((((tmpW * quat.y) + (tmpY * quat.w)) - (tmpZ * quat.x)) + (tmpX * quat.z)),
				((((tmpW * quat.z) + (tmpZ * quat.w)) - (tmpX * quat.y)) + (tmpY * quat.x))
			);
		}
		static Quaternion RotationY(float radians)
		{
			float angle = radians * 0.5f;
			return Quaternion(0.0f, (float)sin(angle), 0.0f, (float)cos(angle));
		}
		static Quaternion RotationX(float radians)
		{
			float angle = radians * 0.5f;
			return Quaternion((float)sin(angle), 0.0f, 0.0f, (float)cos(angle));
		}
		
		Quaternion Conjugate() const;
		Quaternion Normalize(const Quaternion& quat) const
		{
			float lenSqr, lenInv;
			lenSqr = Norm(quat);
			lenInv = 1.0f / (float)sqrt(lenSqr);
			return quat * lenInv;
		}

		float Norm(const Quaternion& quaternion) const
		{
			float result;
			result = (quaternion.x * quaternion.x);
			result = (result + (quaternion.y * quaternion.y));
			result = (result + (quaternion.z * quaternion.z));
			result = (result + (quaternion.w * quaternion.w));
			return result;
		}

		Vec3 ToEulerAngles() const
		{
			double xx = (double)x;
			double yy = (double)y;
			double zz = (double)z;
			double ww = (double)w;
			return Vec3((float)atan2(2 * xx * ww - 2 * yy * zz, 1 - 2 * xx * xx - 2 * zz * zz),
				(float)atan2(2 * yy * ww - 2 * xx * zz, 1 - 2 * yy * yy - 2 * zz * zz),
				(float)asin(2 * xx * yy + 2 * zz * ww));
		}

		Quaternion operator*(float scalar) const
		{
			return Quaternion(x * scalar, y * scalar, z * scalar, w*scalar);
		}
		Quaternion operator*(const Quaternion& quat) const
		{
			return Normalize(Quaternion(
				(((w * quat.x) + (x * quat.w)) + (y * quat.z)) - (z * quat.y),
				(((w * quat.y) + (y * quat.w)) + (z * quat.x)) - (x * quat.z),
				(((w * quat.z) + (z * quat.w)) + (x * quat.y)) - (y * quat.x),
				(((w * quat.w) - (x * quat.x)) - (y * quat.y)) - (z * quat.z)
			));
		}
		Quaternion operator+(const Quaternion& quat) const;
	};
	// Matrices
	struct Mat4
	{
		// Acces to the same data
		union
		{
			float elements[4*4];
			Vec4 rows[4];
		};

		Mat4();
		Mat4(float value);
		Mat4(DirectX::XMFLOAT4X4 mat);

		Mat4& Multiply(const Mat4& other);

		Mat4 operator*(const Mat4& right);


		DirectX::XMFLOAT4X4 ToXMFLOAT4X4() const;

		static Mat4 Rotate(float angle, const Vec3& axis);
		static Mat4 Rotate(const Quaternion& quat);
		static Mat4 Translate(const Vec3& vector);

		friend std::ostream& operator<<(std::ostream& stream, Mat4 mat);
	};
	Mat4 LookAtLH(Vec3 eye, Vec3 focusPosition, Vec3 up);
	Mat4 Transpose(const Mat4& mat);

	float toRadians(float degrees);
	float Length(Vec3 vector);


	


}


