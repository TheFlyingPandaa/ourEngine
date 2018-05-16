#pragma once
// Standard Libraries
#include <vector>
#include <sstream>
#include <string>
#include <iostream>		// <-- 
#include <stack>
#include <chrono>

// Check Leaks
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

// DirectX - Related
#include <DirectXMath.h>
#include "ourEngine/core/WICTextureLoader/WICTextureLoader.h"
#include "ourEngine/core/WICTextureLoader/WICTextureLoader12.h"
#include "ourEngine\core\Font\SpriteFont.h"
#include "ourEngine/core/Font/SpriteBatch.h"

// OurEngine - Related

// Structs
#include "ourEngine\Structs.h"

// Interface Headers
#include "ourEngine\interface\Window.h"
#include "ourEngine\interface\Input.h"
#include "ourEngine/interface/Text.h"

// Shape Headers
#include "ourEngine\interface\shape\3DObject.h"
#include "ourEngine/interface/shape/Rectangle.h"
#include "ourEngine/interface/shape/Billboard.h"
#include "ourEngine/interface/shape/Material.h"
#include "ourEngine\interface\shape\Mesh.h"
#include "ourEngine/interface/shape/SkyBoxObject.h"
#include "ourEngine\interface\shape\Shape.h"

// Mesh Managers
#include "trolls_inn\Mesh Manager\MeshLoaderPlus.h"
#include "trolls_inn\Mesh Manager\MeshManager.h"

// InGame Console
#include "InGameConsole.h"

// Camera Header
#include "ourEngine\core\Camera\OrbitCamera.h"

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

#ifdef NDEBUG
#pragma comment (lib, "ourEngine/core/Audio/AudioLibxRL.lib")
#pragma comment (lib, "ourEngine/core/Font/FontLibxRL.lib")
#else
#pragma comment (lib, "ourEngine/core/Audio/AudioLibxDB.lib")
#pragma comment (lib, "ourEngine/core/Font/FontLibxDB.lib")
#endif 

