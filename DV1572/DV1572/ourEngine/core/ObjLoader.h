#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "../Structs.h"
#include <vector>
#include <sstream>
#include "WICTextureLoader\WICTextureLoader.h"
#include <map>
#include "Dx.h"
#include "../interface/shape/Material.h"

namespace DX
{
	static std::vector<Material*> getMaterials(const std::string& path)
	{
		bool printSuccess = true;
		auto getMttlibPath = [&]() -> std::string
		{
			std::ifstream fptr;
			fptr.open(path);
			if (!fptr)
			{
				std::cout << "getMaterials(): Can not open obj file - " << path << std::endl;
				return "";
			}

			std::string currentLine = "";
			while (std::getline(fptr, currentLine))
			{
				if (currentLine[0] != '#' || currentLine != "")
				{
					std::istringstream stream(currentLine);
					std::string type;
					stream >> type;
					if (type == "mtllib")
					{
						std::string mttlibName;
						stream >> mttlibName;
						fptr.close();
						auto const pos = path.find_last_of('/');
						std::string newPath = path.substr(0, pos + 1);
						newPath += mttlibName;
						if(printSuccess)std::cout << "\nMaterial: Gathering materials from " << newPath << std::endl;
						return newPath;
					}
				}
			}
			fptr.close();
			return "";
		};
		
		std::string mttlibPath = getMttlibPath();
		std::string originPath = mttlibPath.substr(0, mttlibPath.find_last_of('/') + 1);

		std::ifstream fptr;
		fptr.open(mttlibPath);
		if (!fptr)
		{
			std::cout << "getMatrials() Can not open mttlib file - " << mttlibPath << std::endl;
			return std::vector<Material*>();
		}
		
		std::vector<Material*> materials;
		std::string currentLine = "";
		while (std::getline(fptr, currentLine))
		{
			if (currentLine[0] != '#' && currentLine != "")
			{
				std::istringstream stream(currentLine);
				std::string type;
				stream >> type;
				
				if (type == "newmtl")
				{
					std::string name = "";
					stream >> name;
					materials.push_back(new Material(name));
					if (printSuccess) std::cout << "newmtl " << name << std::endl;
				}
				else if (type == "Ns")
				{
					float specularComp = 0.0f;
					stream >> specularComp;
					materials.back()->setSpecularExponent(specularComp);
					if (printSuccess) std::cout << "\tNs " << specularComp << std::endl;
				}
				else if (type == "map_Kd")
				{
					std::string file = "";
					stream >> file;
					materials.back()->setDiffuseMap(originPath + file);
					if (printSuccess) std::cout << "\tmap_Kd " << file << std::endl;
				}
				else if (type == "map_Bump")
				{
					std::string file = "";
					stream >> file;
					materials.back()->setNormalMap(originPath + file);
					if (printSuccess) std::cout << "\tmap_Bump " << file << std::endl;
				}
				else if (type == "map_Ks")
				{
					std::string file = "";
					stream >> file;
					materials.back()->setHighlightMap(originPath + file);
					if (printSuccess) std::cout << "\tmap_Ks " << file << std::endl;
				}
			}
		}
		


		return materials;
	}

	static void loadOBJContinue(std::ifstream& fptr, std::vector<VERTEX>& tempvertices, std::vector<V>& vertices, std::vector<VN>& normals, std::vector<VT>& texture, std::string& mttlibName, bool inverted = false)
	{
		std::vector<F> face;
		std::string currentLine = "";
		bool readFaces = false;
		while (std::getline(fptr, currentLine))
		{
			if (currentLine[0] != '#')
			{
				std::istringstream stream(currentLine);
				std::string type;

				stream >> type;
			
				if (type == "v")
				{
					vertices.push_back(V());
					stream >> vertices.back().x >> vertices.back().y >> vertices.back().z;
				}
				else if (type == "vt")
				{
					texture.push_back(VT());
					float u = 0.0f;
					float v = 0.0f;

					stream >> u >> v;
					texture.back().u = u;
					texture.back().v = v;
				}
				else if (type == "vn")
				{
					normals.push_back(VN());

					stream >> normals.back().x >> normals.back().y >> normals.back().z;
				}
				else if (type == "usemtl")
				{
					stream >> mttlibName;
				}
				else if (type == "f")
				{
					readFaces = true;
					F fa[3];

					sscanf_s(currentLine.c_str(), "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
						&fa[0].vIndex, &fa[0].vtIndex, &fa[0].vnIndex,
						&fa[1].vIndex, &fa[1].vtIndex, &fa[1].vnIndex,
						&fa[2].vIndex, &fa[2].vtIndex, &fa[2].vnIndex
					);
					for (int i = 0; i < 3; i++)
					{
						face.push_back(fa[i]);
					}
				}
				else if(readFaces)
					break;
			}
		}

		for (size_t i = 0; i < face.size(); i++)
		{
			F f = face[i];
			V v = { vertices[f.vIndex - 1].x, vertices[f.vIndex - 1].y, vertices[f.vIndex - 1].z };
			VT vt = { texture[f.vtIndex - 1].u, texture[f.vtIndex - 1].v };
			VN vn = { normals[f.vnIndex - 1].x, normals[f.vnIndex - 1].y, normals[f.vnIndex - 1].z };
			VERTEX vertex = {
				inverted ? v.z : v.x , v.y, inverted ? v.x : v.z,
				vt.u, vt.v,
				vn.x, vn.y, vn.z
			};
			tempvertices.push_back(vertex);
		}
	}
	

	static void loadOBJ(const std::string & path, std::vector<VERTEX> &model)
	{
		std::ifstream fptr;
		fptr.open(path);
		if (!fptr)
		{
			std::cout << "Can not open " << path << " dickhead!\n";
		}
		else
		{
			std::vector<V> ver;
			std::vector<VN> nor;
			std::vector<VT> uv;
			std::vector<F> face;
			std::string currentLine = "";

			while(std::getline(fptr, currentLine))
			{
				if (currentLine[0] != '#' || currentLine != "")
				{
					std::istringstream stream(currentLine);
					std::string type;

					stream >> type;

					if (type == "v")
					{
						V v;
						sscanf_s(currentLine.c_str(), "%*s %f %f %f", &v.x, &v.y, &v.z);
						ver.push_back(v);
					}
					else if (type == "vt")
					{
						VT vt;
						sscanf_s(currentLine.c_str(), "%*s %f %f", &vt.u, &vt.v);
						uv.push_back(vt);
					}
					else if (type == "vn")
					{
						VN vn;
						sscanf_s(currentLine.c_str(), "%*s %f %f %f", &vn.x, &vn.y, &vn.z);
						nor.push_back(vn);
					}
					else if (type == "f")
					{
						F fa[3];
						
						sscanf_s(currentLine.c_str(), "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
							&fa[0].vIndex, &fa[0].vtIndex, &fa[0].vnIndex,
							&fa[1].vIndex, &fa[1].vtIndex, &fa[1].vnIndex,
							&fa[2].vIndex, &fa[2].vtIndex, &fa[2].vnIndex
						);
						for (int i = 0; i < 3; i++)
						{
							face.push_back(fa[i]);
						}
					}
				}
			}

			for (size_t i = 0; i < face.size(); i++)
			{
				F f = face[i];
				V v = ver[f.vIndex - 1];
				VT vt = uv[f.vtIndex - 1];
				VN vn = nor[f.vnIndex - 1];
				VERTEX vertex = {
					v.x, v.y, v.z,
					vt.u, vt.v,
					vn.x, vn.y, vn.z
				};
				model.push_back(vertex);
			}
		}
	}

	static void loadOBJInvert(const std::string & path, std::vector<VERTEX> &model)
	{
		std::ifstream fptr;
		fptr.open(path);
		if (!fptr)
		{
			std::cout << "Can not open " << path << " dickhead!\n";
		}
		else
		{
			std::vector<V> ver;
			std::vector<VN> nor;
			std::vector<VT> uv;
			std::vector<F> face;
			std::string currentLine = "";

			while (std::getline(fptr, currentLine))
			{
				if (currentLine[0] != '#' || currentLine != "")
				{
					std::istringstream stream(currentLine);
					std::string type;

					stream >> type;

					if (type == "v")
					{
						V v;
						sscanf_s(currentLine.c_str(), "%*s %f %f %f", &v.x, &v.y, &v.z);
						ver.push_back(v);
					}
					else if (type == "vt")
					{
						VT vt;
						sscanf_s(currentLine.c_str(), "%*s %f %f", &vt.u, &vt.v);
						uv.push_back(vt);
					}
					else if (type == "vn")
					{
						VN vn;
						sscanf_s(currentLine.c_str(), "%*s %f %f %f", &vn.x, &vn.y, &vn.z);
						nor.push_back(vn);
					}
					else if (type == "f")
					{
						F fa[3];

						sscanf_s(currentLine.c_str(), "%*s %d/%d/%d %d/%d/%d %d/%d/%d",
							&fa[0].vIndex, &fa[0].vtIndex, &fa[0].vnIndex,
							&fa[1].vIndex, &fa[1].vtIndex, &fa[1].vnIndex,
							&fa[2].vIndex, &fa[2].vtIndex, &fa[2].vnIndex
						);
						for (int i = 0; i < 3; i++)
						{
							face.push_back(fa[i]);
						}
					}
				}
			}

			for (size_t i = 0; i < face.size(); i++)
			{
				F f = face[i];
				V v = ver[f.vIndex - 1];
				VT vt = uv[f.vtIndex - 1];
				VN vn = nor[f.vnIndex - 1];
				VERTEX vertex = {
					v.z, v.y, v.x,
					vt.u, vt.v,
					vn.x, vn.y, vn.z
				};
				model.push_back(vertex);
			}
		}
	}

	static void loadTexture(const std::string & path, ID3D11Resource *& texture, ID3D11ShaderResourceView *& textureView)
	{
		std::wstring widestr = std::wstring(path.begin(), path.end());
		const wchar_t* widecstr = widestr.c_str();

		HRESULT hr = DirectX::CreateWICTextureFromFile(DX::g_device,DX::g_deviceContext, widecstr, &texture, &textureView);
	}

	static void CalculateTangents(std::vector<VERTEX> &model)
	{
		for (int i = 0; i < model.size(); i += 3)
		{
			int j = i + 1;
			int k = i + 2;

			DirectX::XMVECTOR v1 = DirectX::XMVectorSet(model[j].x, model[j].y, model[j].z, NULL);
			DirectX::XMVECTOR v2 = DirectX::XMVectorSet(model[k].x, model[k].y, model[k].z, NULL);
			DirectX::XMVECTOR v0 = DirectX::XMVectorSet(model[i].x, model[i].y, model[i].z, NULL);

			DirectX::XMVECTOR edge1 = DirectX::XMVectorSubtract(v1, v0);
			DirectX::XMVECTOR edge2 = DirectX::XMVectorSubtract(v2, v0);
			
			float deltaU1 = model[j].u - model[i].u;
			float deltaV1 = model[j].v - model[i].v;
			float deltaU2 = model[k].u - model[i].u;
			float deltaV2 = model[k].v - model[i].v;

			DirectX::XMFLOAT3 tangent;

			tangent.x = deltaV2 * DirectX::XMVectorGetX(edge1) - deltaV1 * DirectX::XMVectorGetX(edge2);
			tangent.y = deltaV2 * DirectX::XMVectorGetY(edge1) - deltaV1 * DirectX::XMVectorGetY(edge2);
			tangent.z = deltaV2 * DirectX::XMVectorGetZ(edge1) - deltaV1 * DirectX::XMVectorGetZ(edge2);

			DirectX::XMVECTOR vTangent;

			vTangent = DirectX::XMVector3Normalize(XMLoadFloat3(&tangent));

			for (int counter = 0; counter < 3; counter++)
			{
				int index = i + counter;
				model[index].tx = DirectX::XMVectorGetX(vTangent);
				model[index].ty = DirectX::XMVectorGetY(vTangent);
				model[index].tz = DirectX::XMVectorGetZ(vTangent);
			}
		}
	}

	static void indexVertices(const std::vector<VERTEX>& vertices, std::vector<unsigned int>& indices, std::vector<VERTEX>& outVertices)
	{
		struct PackedVertex
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT3 uv;
			DirectX::XMFLOAT3 normal;

			bool operator<(const PackedVertex& other) const
			{
				return memcmp((void*)this, (void*)&other, sizeof(PackedVertex)) > 0;
			}
		};

		//struct VERTEX
		//{
		//	float x, y, z;		//Position
		//	float u, v;			//Texel
		//	float nx, ny, nz;	//Normal
		//	float tx, ty, tz;	//Tangent
		//};
		std::map<PackedVertex, unsigned short> vertexToOutIndex;

		for (unsigned i = 0; i < vertices.size(); i++)
		{
			DirectX::XMFLOAT3 pos;
			pos.x = vertices[i].x;
			pos.y = vertices[i].y;
			pos.z = vertices[i].z;
			DirectX::XMFLOAT3 uv;
			uv.x = vertices[i].u;
			uv.y = vertices[i].v;
			DirectX::XMFLOAT3 normal;
			normal.x = vertices[i].nx;
			normal.y = vertices[i].ny;
			normal.z = vertices[i].nz;
			PackedVertex packed = { pos,uv,normal };

			std::map<PackedVertex, unsigned short>::iterator it = vertexToOutIndex.find(packed);

			if (it != vertexToOutIndex.end())
			{
				unsigned short index = it->second;
				indices.push_back(index);
			}
			else
			{
				outVertices.push_back(vertices[i]);
				unsigned short newIndex = (unsigned short)outVertices.size() - 1;
				indices.push_back(newIndex);
				vertexToOutIndex[packed] = newIndex;
			}
		}
	}
};