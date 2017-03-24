#include"SubSystem\RenderObject.h"

GLib::Sprites::Sprite * CreateSprite(const char * i_pFilename)
{
	SLOW_ASSERT(i_pFilename != nullptr, ErrorMessage::kNullPointer);

	size_t sizeTextureFile = 0;

	// Load the source file (texture data)
	void * pTextureFile = LoadFile(i_pFilename, sizeTextureFile);

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

void * LoadFile(const char * i_pFilename, size_t & o_sizeFile)
{
	SLOW_ASSERT(i_pFilename != NULL, ErrorMessage::kNullPointer);

	FILE * pFile = NULL;

	errno_t fopenError = fopen_s(&pFile, i_pFilename, "rb");
	if (fopenError != 0)
		return NULL;

	SLOW_ASSERT(pFile != NULL, ErrorMessage::kNullPointer);

	int32 FileIOError = fseek(pFile, 0, SEEK_END);
	SLOW_ASSERT(FileIOError == 0, ErrorMessage::kNonZeroValue);

	long FileSize = ftell(pFile);
	SLOW_ASSERT(FileSize >= 0, ErrorMessage::kNonPostiveValue);

	FileIOError = fseek(pFile, 0, SEEK_SET);
	SLOW_ASSERT(FileIOError == 0, ErrorMessage::kNonZeroValue);

	uint8 * pBuffer = new uint8[FileSize];
	SLOW_ASSERT(pBuffer, ErrorMessage::kNullPointer);

	size_t FileRead = fread(pBuffer, 1, FileSize, pFile);
	ASSERT(FileRead == FileSize);

	fclose(pFile);

	o_sizeFile = FileSize;

	return pBuffer;
}