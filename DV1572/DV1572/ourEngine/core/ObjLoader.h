#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "../Structs.h"
#include <vector>
#include <sstream>

namespace DX
{
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
};