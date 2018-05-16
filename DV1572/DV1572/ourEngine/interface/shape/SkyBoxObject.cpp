#include "OurEngine.h"
#include "SkyBoxObject.h"

SkyBoxObject::SkyBoxObject()
{
	setVertexShader(DX::g_skyBoxVertexShader);
	setPixelShader(DX::g_skyBoxPixelShader);
	//setHullShader(DX::g_standardHullShader);
	//setDomainShader(DX::g_standardDomainShader);
}

std::string SkyBoxObject::toString() const
{
	return Shape::toString() + "\n";
}

void SkyBoxObject::Draw()
{
	DX::submitToInstance(this, DX::g_instanceGroupsSkyBox);
}
