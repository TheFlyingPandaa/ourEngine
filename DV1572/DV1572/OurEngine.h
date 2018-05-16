#pragma once
// Standard Libraries
#include <chrono>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <stack>
#include <iostream>		// <-- 
#include <algorithm> 
#include <thread>
#include <mutex>
#include <future>
#include <unordered_map>
#include <deque> 
#include <queue>
#include <math.h> 
#include <memory>
#include <stdlib.h>


// Window header
#include <windows.h>

// Check Leaks
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

// DirectX - Related
#include <DirectXMath.h>
using namespace DirectX;
#include "ourEngine/core/WICTextureLoader/WICTextureLoader.h"
#include "ourEngine\core\Font\SpriteFont.h"
#include "ourEngine/core/Font/SpriteBatch.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

// OurEngine - Related

// Render, dont change
#include "ourEngine\core\RenderDefine.h"

// Structs
#include "ourEngine\Structs.h"

// Camera Header
#include "ourEngine\core\Camera\OrbitCamera.h"


// Shape Headers
#include "ourEngine\interface\shape\3DObject.h"
#include "ourEngine/interface/shape/Rectangle.h"
#include "ourEngine/interface/shape/Billboard.h"
#include "ourEngine/interface/shape/Material.h"
#include "ourEngine\interface\shape\Mesh.h"
#include "ourEngine/interface/shape/SkyBoxObject.h"
#include "ourEngine\interface\shape\Shape.h"

// Interface Headers
#include "ourEngine\interface\Window.h"
#include "ourEngine\interface\Input.h"
#include "ourEngine/interface/Text.h"

// Mesh Managers
#include "trolls_inn\Mesh Manager\MeshLoaderPlus.h"
#include "trolls_inn\Mesh Manager\MeshManager.h"

// InGame Console
#include "InGameConsole.h"

// Light Headers
#include "ourEngine/interface/light/Light.h"
#include "ourEngine/interface/light/PointLight.h"

// Core Headers
#include "ourEngine\core\FileReader.h"
#include "ourEngine/core/Dx.h"
#include "ourEngine/core/ObjLoader.h"
#include "ourEngine/core/Picking.h"
#include "ourEngine/core/ShaderCreator.h"

// Core - Extra Headers
#include "ourEngine\core\Audio\Audio.h"

#include "trolls_inn\Time Management\GameTime.h"

#ifdef __DEBUG
#pragma comment (lib, "ourEngine/core/Audio/AudioLibxRL.lib")
#pragma comment (lib, "ourEngine/core/Font/FontLibxRL.lib")
#else
#pragma comment (lib, "ourEngine/core/Audio/AudioLibxDB.lib")
#pragma comment (lib, "ourEngine/core/Font/FontLibxDB.lib")
#endif 

