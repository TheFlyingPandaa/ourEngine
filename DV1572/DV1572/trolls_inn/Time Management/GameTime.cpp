#include "../../OurEngine.h"
#include "OurEngine.h"
#include "GameTime.h"
#define PI 3.14159265358979323846f 
#define DEBUG 0 //DEBUG ON/OFF
#define sunY 30.0f
GameTime::GameTime()
{
	m_currentClockValue = 0.0f;
	m_currentTime = NOONTOEVENING;
	m_currentAngle = 90.0f;
	m_minutes = 0;
	m_seconds = 0.0f; 
	m_totalSeconds = 0; 
	m_divider = 255.0f; 
	m_sunCurrentFinalColor = DirectX::XMVECTOR{ 0,0,0 };
	m_sunStartInterpolate = DirectX::XMVECTOR{ 192.0f / m_divider,191.0f / m_divider, 173.0f / m_divider };
	m_sunRotationStart = m_sunFinalRotation;
	m_colorScaleFactor = 0.0f;
	m_sunAngle = 90.0f;
	
	m_colorScaleFactor = 0.0001f;
	m_rotationFactor = 0.0001f;
	
	m_sunTargetInterpolate = DirectX::XMVECTOR{ 192.0f / m_divider,191.0f / m_divider, 173.0f / m_divider };
	m_sunTargetInterpolate = DirectX::XMVECTOR{ 0,0,0 };
	


	m_sunRotationStart = DirectX::XMVECTOR{ 0,1,0 };
	m_sunRotationTarget = DirectX::XMVECTOR{ 0,0,0 };

	DirectX::XMFLOAT3 tempPos = { 0.0f,sunY,0.0f };
	m_sunPos = XMLoadFloat3(&tempPos);
	//DirectX::XMStoreFloat3(&tempPos, m_sunPos);
	m_sunAngle = 0.0f;
	
	
	m_createSunBuffer(); 

	m_sun.InitDirectional(
		DirectX::XMFLOAT4A(0, 100, 0, 0), 
		DirectX::XMFLOAT4A(-1, -1, -1, 0), 
		DirectX::XMFLOAT4A(1, 1, 1, 1), 420, 420);

	m_sun.CreatesShadows();
	m_vUp = XMVECTOR{ 0,1,0 }; 
}

GameTime::~GameTime()
{
}
#include "../../ourEngine/interface/Input.h"
void GameTime::updateCurrentTime(float refreshRate)
{
	using namespace DirectX;
	m_currentClockValue = refreshRate + Input::isKeyPressed('N');
	m_seconds += m_currentClockValue; 
	m_sun.CreatesShadows();

	switch (m_currentTime)
	{
		//06:00 -> 12:00
	case MORNINGTONOON:
		if (m_colorScaleFactor < 1.0f)
		{

			m_sunTargetInterpolate = DirectX::XMVECTOR{ 192.0f / m_divider,191.0f / m_divider, 173.0f / m_divider };
			m_colorScaleFactor = m_seconds / (6.0f * 60.0f);
			m_sunCurrentFinalColor = DirectX::XMVectorLerp(m_sunStartInterpolate, m_sunTargetInterpolate, m_colorScaleFactor);
			DirectX::XMStoreFloat4A(&m_fFinalColor, m_sunCurrentFinalColor);

			XMFLOAT3 targetPosition = { 16.0f, sunY, 16.0f };
			XMFLOAT3 startPosition = { 0, sunY, 0 };
			XMVECTOR result = XMVectorLerp(XMLoadFloat3(&startPosition), XMLoadFloat3(&targetPosition), m_colorScaleFactor);
			m_sunPos = result;
			XMStoreFloat3(&targetPosition, result);
		}
		else
		{
			m_seconds = 0;
			m_currentTime = NOONTOEVENING;
			m_sunStartInterpolate = m_sunCurrentFinalColor;
			XMFLOAT3 startNext(16, sunY, 16);
			m_sunPos = XMLoadFloat3(&startNext);
			m_colorScaleFactor = 0.0f;
		}
		break;

		//Fr�n 12:00 -> 17:00
	case NOONTOEVENING:
		if (m_colorScaleFactor < 1.0f)
		{

			m_sunTargetInterpolate = DirectX::XMVECTOR{ 230.0f / m_divider,120.0f / m_divider, 120.0f / m_divider };
			m_colorScaleFactor = m_seconds / (6.0f * 60.0f);
			m_sunCurrentFinalColor = DirectX::XMVectorLerp(m_sunStartInterpolate, m_sunTargetInterpolate, m_colorScaleFactor);
			DirectX::XMStoreFloat4A(&m_fFinalColor, m_sunCurrentFinalColor);

			XMFLOAT3 targetPosition = { 20, sunY, 20.0f };
			XMFLOAT3 startPosition = { 16, sunY, 16 };
			XMVECTOR result = XMVectorLerp(XMLoadFloat3(&startPosition), XMLoadFloat3(&targetPosition), m_colorScaleFactor);
			XMStoreFloat3(&targetPosition, result);
			m_sunPos = result;
		}
		else
		{
			m_seconds = 0;
			m_currentTime = EVENINGTONIGHT;
			m_sunStartInterpolate = m_sunCurrentFinalColor;
			XMFLOAT3 startNext(20, sunY, 20);
			m_sunPos = XMLoadFloat3(&startNext);
			m_colorScaleFactor = 0.0f;
		}
	break;
	//Fr�n 17:00 -> 21:00
case EVENINGTONIGHT:
	if (m_colorScaleFactor < 1.0f)
	{
		//std::cout << "Evening to Night" << std::endl;
		m_sunTargetInterpolate = DirectX::XMVECTOR{ 90.0f / m_divider,70.0f / m_divider, 190.0f / m_divider };
		m_colorScaleFactor = m_seconds / (6.0f * 60.0f);
		m_sunCurrentFinalColor = DirectX::XMVectorLerp(m_sunStartInterpolate, m_sunTargetInterpolate, m_colorScaleFactor);
		DirectX::XMStoreFloat4A(&m_fFinalColor, m_sunCurrentFinalColor);

		XMFLOAT3 targetPosition = { 32.0f, sunY, 32.0f };
		XMFLOAT3 startPosition = { 20, sunY, 20 };
		XMVECTOR result = XMVectorLerp(XMLoadFloat3(&startPosition), XMLoadFloat3(&targetPosition), m_colorScaleFactor);
		XMStoreFloat3(&targetPosition, result);
		m_sunPos = result;
		

	}
	else
	{

		m_seconds = 0;
		m_currentTime = MORNINGTONOON;
		m_sunStartInterpolate = DirectX::XMVECTOR{ 182.0f / m_divider,126.0f / m_divider,91.0f / m_divider };
		XMFLOAT3 startNext(0, sunY, 0);
		m_sunPos = XMLoadFloat3(&startNext);
		m_totalSeconds = 0;
		m_minutes = 0;
		m_colorScaleFactor = 0.0f;
	}
		break;
	}

	
}

TIMEOFDAY GameTime::getTimePeriod()
{
	return m_currentTime;
}

Light & GameTime::getSun()
{
	return m_sun;
}

void GameTime::m_createSunBuffer()
{
	HRESULT hr; 
	D3D11_BUFFER_DESC sunBdesc;
	sunBdesc.Usage = D3D11_USAGE_DYNAMIC;
	sunBdesc.ByteWidth = sizeof(DIRECTIONAL_LIGHT_BUFFER);
	sunBdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	sunBdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sunBdesc.MiscFlags = 0;
	sunBdesc.StructureByteStride = 0;

	hr = DX::g_device->CreateBuffer(&sunBdesc, nullptr, &m_pSunBuffer); 
}

void GameTime::m_cpyLightToGPU()
{
	
	m_sun.setColor(m_fFinalColor);
	DX::g_lightDir = m_sun.getColor();
	XMFLOAT3 sunPos;
	XMStoreFloat3(&sunPos, m_sunPos);
	DX::g_lightPos.x = sunPos.x;
	DX::g_lightPos.y = sunPos.y;
	DX::g_lightPos.z = sunPos.z;
		
	XMMATRIX rot = XMMatrixRotationZ(XMConvertToRadians(m_currentAngle));
		
	XMVECTOR newpos = XMVector3Transform(m_sunPos, rot);
	XMFLOAT4A sunPos4f;

	XMStoreFloat4A(&sunPos4f, newpos);
	sunPos4f.w = 1.0f;

	

	m_sunBuffer.pos = sunPos4f;

	m_sunBuffer.dir.x = -m_sunBuffer.pos.x;
	m_sunBuffer.dir.y = -m_sunBuffer.pos.y;
	m_sunBuffer.dir.z = -m_sunBuffer.pos.z;
	m_sunBuffer.dir.w = m_sunBuffer.pos.w;

	//std::cout << "\rAngle " << m_currentAngle << std::flush;
	m_sun.setPos(m_sunBuffer.pos);
	m_sun.setDir(m_sunBuffer.dir);
	/*std::cout << "Position (" << m_sunBuffer.pos.x << "," << m_sunBuffer.pos.y << "," << m_sunBuffer.pos.z << ")\n";
	std::cout << "Direciton (" << m_sunBuffer.dir.x << "," << m_sunBuffer.dir.y << "," << m_sunBuffer.dir.z << ")\n\n";*/

	m_sun.cpyDataDir(m_sunBuffer,m_pSunBuffer); 
}
