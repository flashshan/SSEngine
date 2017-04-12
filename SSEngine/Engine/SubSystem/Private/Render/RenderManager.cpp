#include "SubSystem\Render\RenderManager.h"

#include "Core\String\HashedString.h"
#include "Core\Basic\FileIO.h"

GLib::Sprites::Sprite * CreateSprite(const char * i_pFilename)
{
	SLOW_ASSERT(i_pFilename != nullptr, ErrorType::ENullPointer);

	size_t sizeTextureFile = 0;

	// Load the source file (texture data)
	uint8 * pTextureFile = LoadFile(i_pFilename, sizeTextureFile);

	// Ask GLib to create a texture out of the data (assuming it was loaded successfully)
	GLib::Texture * pTexture = pTextureFile ? GLib::CreateTexture(pTextureFile, sizeTextureFile) : nullptr;

	// exit if something didn't work
	// probably need some debug logging in here!!!!
	if (pTextureFile)
		delete[] pTextureFile;

	if (pTexture == nullptr)
		return NULL;

	uint32 width = 0;
	uint32 height = 0;
	uint32 depth = 0;

	// Get the dimensions of the texture. We'll use this to determine how big it is on screen
	bool result = GLib::GetDimensions(pTexture, width, height, depth);
	ASSERT(result == true);
	ASSERT((width > 0) && (height > 0));

	// Define the sprite edges
	GLib::Sprites::SpriteEdges	Edges = { -float(width / 2.0f), float(height), float(width / 2.0f), 0.0f };
	GLib::Sprites::SpriteUVs	UVs = { { 0.0f, 0.0f },{ 1.0f, 0.0f },{ 0.0f, 1.0f },{ 1.0f, 1.0f } };
	GLib::RGBA							Color = { 255, 255, 255, 255 };

	// Create the sprite
	GLib::Sprites::Sprite * pSprite = GLib::Sprites::CreateSprite(Edges, 0.1f, Color, UVs);
	if (pSprite == nullptr)
	{
		GLib::Release(pTexture);
		return nullptr;
	}

	// Bind the texture to sprite
	GLib::Sprites::SetTexture(*pSprite, *pTexture);

	return pSprite;
}


RenderManager *RenderManager::globalInstance_ = nullptr;

RenderManager::~RenderManager()
{
	renderObjects_.clear();
	for (auto it = spriteResources_.begin();it != spriteResources_.end(); ++it)
	{
		GLib::Sprites::Release(it->second);
	}
	spriteResources_.clear();
}

void RenderManager::RenderUpdate()
{
	GLib::BeginRendering();
	GLib::Sprites::BeginRendering();
	
	for(auto it = renderObjects_.begin(); it != renderObjects_.end(); ++it)
	{
		if ((*it)->IsValid())
		{
			(*it)->DoRender();
		}
		else
		{
			renderObjects_.erase(it);
		}
	}

	GLib::Sprites::EndRendering();
	GLib::EndRendering();
}

WeakPtr<RenderObject> RenderManager::AddRenderObject(const WeakPtr<GameObject> &i_gameObject, const char *i_filePath, uint32 i_priority)
{
	GLib::Sprites::Sprite *tempSprite;

	HashedString filePathHash(i_filePath);

	// check if sprite exist, if not create one and store it in map
	auto it = spriteResources_.find(filePathHash);
	if (it == spriteResources_.end())
	{
		tempSprite = CreateSprite(i_filePath);
		ASSERT(tempSprite != nullptr);
		spriteResources_.insert(std::pair<HashedString, GLib::Sprites::Sprite*>(filePathHash, tempSprite));
	}
	else
	{
		tempSprite = it->second;
	}
	StrongPtr<RenderObject> newRenderObject = new TRACK_NEW RenderObject(i_gameObject, tempSprite, i_priority);
	ASSERT(newRenderObject);
	EnterCriticalSection(&criticalSection);
	renderObjects_.insert(newRenderObject);
	LeaveCriticalSection(&criticalSection);
	return WeakPtr<RenderObject>(newRenderObject);
}

// go through the set, seldomly used
void RenderManager::Remove(const WeakPtr<RenderObject> &i_renderObject)
{
	EnterCriticalSection(&criticalSection);
	for (auto && item : renderObjects_)
	{
		if (&(*item) == &(*i_renderObject))
		{
			renderObjects_.erase(item);
			break;
		}
	}
	LeaveCriticalSection(&criticalSection);
}
