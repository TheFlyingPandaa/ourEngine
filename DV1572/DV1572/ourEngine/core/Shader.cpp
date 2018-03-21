#include "Shader.h"

Shader::Shader(ShaderInfo* shaderInfo, std::vector<D3D11_INPUT_ELEMENT_DESC> desc)
{
	
	_compileShaders(shaderInfo, desc);

}

void Shader::Bind()
{
	DX::g_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DX::g_deviceContext->IASetInputLayout(m_inputLayout.Get());

	DX::g_deviceContext->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	DX::g_deviceContext->HSSetShader(m_hullShader.Get(), nullptr, 0);
	DX::g_deviceContext->DSSetShader(m_domainShader.Get(), nullptr, 0);
	DX::g_deviceContext->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	DX::g_deviceContext->PSSetShader(m_pixelShader.Get(), nullptr, 0);

}

void Shader::_compileShaders(ShaderInfo * shaders, std::vector<D3D11_INPUT_ELEMENT_DESC> desc)
{
	ShaderInfo* entry = shaders;
	while (entry->type != NONE)
	{
		Microsoft::WRL::ComPtr<ID3DBlob> bufferPointer;
		Microsoft::WRL::ComPtr<ID3DBlob> error;
		HRESULT hr = D3DCompileFromFile(entry->source, nullptr, nullptr, "main", entry->pTarget, 0, 0, bufferPointer.ReleaseAndGetAddressOf(), error.ReleaseAndGetAddressOf());
		if (FAILED(hr))
		{
			printf((char*)error.Get()->GetBufferPointer());
			__debugbreak();
		}
		switch (entry->type)
		{
		case VERTEXSHADER:
			DX::g_device.Get()->CreateVertexShader(bufferPointer.Get()->GetBufferPointer(), bufferPointer.Get()->GetBufferSize(), nullptr, m_vertexShader.ReleaseAndGetAddressOf());

			DX::g_device.Get()->CreateInputLayout(desc.data(), desc.size(), bufferPointer.Get()->GetBufferPointer(), bufferPointer.Get()->GetBufferSize(), m_inputLayout.ReleaseAndGetAddressOf());
			break;
		case HULLSHADER:
			DX::g_device.Get()->CreateHullShader(bufferPointer.Get()->GetBufferPointer(), bufferPointer.Get()->GetBufferSize(), nullptr, m_hullShader.ReleaseAndGetAddressOf());
			break;
		case DOMAINSHADER:
			DX::g_device.Get()->CreateDomainShader(bufferPointer.Get()->GetBufferPointer(), bufferPointer.Get()->GetBufferSize(), nullptr, m_domainShader.ReleaseAndGetAddressOf());
			break;
		case GEOMETRYSHADER:
			DX::g_device.Get()->CreateGeometryShader(bufferPointer.Get()->GetBufferPointer(), bufferPointer.Get()->GetBufferSize(), nullptr, m_geometryShader.ReleaseAndGetAddressOf());
			break;
		case PIXELSHADER:
			DX::g_device.Get()->CreatePixelShader(bufferPointer.Get()->GetBufferPointer(), bufferPointer.Get()->GetBufferSize(), nullptr, m_pixelShader.ReleaseAndGetAddressOf());
			break;
		}
		entry++;
	}
}
