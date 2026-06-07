/*
** ETIB PROJECT, 2026
** utility
** File description:
** ressource_provider
*/

#include <utility/ressource_provider.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace utility
{

	RessourceProvider::RessourceProvider(SystemIO &systemInterface)
		: _systemInterface(systemInterface)
	{
	}

	/////////////
	// Getters //
	/////////////

	std::map<uint32_t, std::shared_ptr<graphic::Material>> RessourceProvider::getMaterials() const
	{
		return _materials;
	}

	std::map<uint32_t, std::shared_ptr<graphic::Texture>> RessourceProvider::getTextures() const
	{
		return _textures;
	}

	std::map<uint32_t, std::shared_ptr<graphic::Model>> RessourceProvider::getModels() const
	{
		return _models;
	}

	std::map<uint32_t, std::shared_ptr<graphic::Shader>> RessourceProvider::getShaders() const
	{
		return _shaders;
	}

	uint32_t RessourceProvider::getShaderID(const std::string &shaderName) const
	{
		for (const auto &[name, id]: _elementsIDs) {
			if (name.starts_with(shaderName)) {
				return id;
			}
		}
		return 0;  // Return 0 if the shader name is not found
	}

	uint32_t RessourceProvider::getMaterialID(const std::string &materialName) const
	{
		auto it = _elementsIDs.find(materialName);

		if (it != _elementsIDs.end()) {
			return it->second;
		}

		return 0;
	}

	////////////////////
	// Public Methods //
	////////////////////

	std::shared_ptr<graphic::Font>
		RessourceProvider::loadFont(const std::string &path)
	{
		auto it = _elementsIDs.find(path);

		if (it != _elementsIDs.end()) {
			if (_fonts.find(it->second) != _fonts.end()) {
				return _fonts[it->second];
			}
		}

		auto fontAsset = _systemInterface.add(path);

		if (!fontAsset) {
			std::cerr << "Failed to load font asset: " << path << std::endl;
			return nullptr;
		}

		auto font = loadFontFromAsset(fontAsset);

		return font;
	}

	std::shared_ptr<graphic::Font> RessourceProvider::loadFontFromAsset(
		std::shared_ptr<utility::File> fontAsset)
	{
		auto it = _elementsIDs.find(fontAsset->path());

		if (it != _elementsIDs.end()) {
			if (_fonts.find(it->second) != _fonts.end()) {
				return _fonts[it->second];
			}
		}

		auto font = std::make_shared<graphic::Font>(std::vector { *fontAsset });
		auto fontID = getNextID();
		auto materialKey = fontAsset->path() + "_material";

		_fonts[fontID] = font;
		_elementsIDs[fontAsset->path()] = fontID;

		auto textMaterial = std::make_shared<graphic::TextMaterial>();
		auto materialID = getNextID();

		_materials[materialID] = textMaterial;
		_elementsIDs[materialKey] = materialID;

		font->onNewTextureCreated =
			[this, materialKey](std::string name,
							   std::shared_ptr<graphic::Texture> atlas) {
				auto textureID = getNextID();

				_textures[textureID] = atlas;
				_elementsIDs[name] = textureID;

				auto materialKeyIt = _elementsIDs.find(materialKey);

				if (materialKeyIt == _elementsIDs.end()) {
					std::cerr << "Missing text material entry for font atlas: " << name
							  << std::endl;
					return;
				}

				auto materialIt = _materials.find(materialKeyIt->second);

				if (materialIt == _materials.end()) {
					std::cerr << "Missing text material for font atlas: " << name << std::endl;
					return;
				}

				auto atlasMaterial = std::dynamic_pointer_cast<graphic::TextMaterial>(
					materialIt->second);

				if (!atlasMaterial) {
					std::cerr << "Misconfigured material for font: " << name
							  << " is not a TextMaterial" << std::endl;
					return;
				}

				atlasMaterial->addAtlas(name, atlas);
			};

		return font;
	}

	std::shared_ptr<graphic::Font> RessourceProvider::loadFontFamilyFromAssets(
		const std::vector<std::shared_ptr<utility::File>> &fontAssets)
	{
		std::vector<File> assets;
		std::string familyName;

		if (!fontAssets.empty()) {
			familyName = fontAssets[0]->path();

			auto lastSlashPos = familyName.find_last_of("/\\");

			if (lastSlashPos != std::string::npos) {
				familyName = familyName.substr(lastSlashPos + 1);
			}

			auto dotPos = familyName.find_last_of(".");

			if (dotPos != std::string::npos) {
				familyName = familyName.substr(0, dotPos);
			}
		}

		for (const auto &asset: fontAssets) {
			assets.push_back(*asset);
		}

		auto it = _elementsIDs.find(familyName);

		if (it != _elementsIDs.end()) {
			if (_fonts.find(it->second) != _fonts.end()) {
				return _fonts[it->second];
			}
		}

		auto font = std::make_shared<graphic::Font>(assets);
		auto fontID = getNextID();
		auto materialKey = familyName + "_material";

		_fonts[fontID] = font;
		_elementsIDs[familyName] = fontID;

		auto textMaterial = std::make_shared<graphic::TextMaterial>();
		auto materialID = getNextID();

		_materials[materialID] = textMaterial;
		_elementsIDs[materialKey] = materialID;

		font->onNewTextureCreated =
			[this, materialKey](std::string name,
							   std::shared_ptr<graphic::Texture> atlas) {
				auto textureID = getNextID();

				_textures[textureID] = atlas;
				_elementsIDs[name] = textureID;

				auto materialKeyIt = _elementsIDs.find(materialKey);

				if (materialKeyIt == _elementsIDs.end()) {
					std::cerr << "Missing text material entry for font atlas: " << name
							  << std::endl;
					return;
				}

				auto materialIt = _materials.find(materialKeyIt->second);

				if (materialIt == _materials.end()) {
					std::cerr << "Missing text material for font atlas: " << name << std::endl;
					return;
				}

				auto atlasMaterial = std::dynamic_pointer_cast<graphic::TextMaterial>(
					materialIt->second);

				if (!atlasMaterial) {
					std::cerr << "Misconfigured material for font: " << name
							  << " is not a TextMaterial" << std::endl;
					return;
				}

				atlasMaterial->addAtlas(name, atlas);
			};

		return font;
	}

	std::shared_ptr<graphic::Material>
		RessourceProvider::loadMaterial(const std::string &path,
									   ShaderType shaderType)
	{
		auto it = _elementsIDs.find(path);

		if (it != _elementsIDs.end()) {
			if (_materials.find(it->second) != _materials.end()) {
				return _materials[it->second];
			}
		}

		auto materialAsset = _systemInterface.add(path);

		if (!materialAsset) {
			std::cerr << "Failed to load material asset: " << path << std::endl;
			return nullptr;
		}

		auto material = loadMaterialFromAsset(shaderType, materialAsset);

		return material;
	}

	std::shared_ptr<graphic::Material> RessourceProvider::loadMaterialFromAsset(
		ShaderType shaderType,
		std::shared_ptr<utility::File> materialAsset)
	{
		auto it = _elementsIDs.find(materialAsset->path());

		if (it != _elementsIDs.end()) {
			if (_materials.find(it->second) != _materials.end()) {
				return _materials[it->second];
			}
		}

		std::string shaderName = (shaderType == ShaderType::TEXT_SHADER)
			? "text"
			: "mesh";
		std::vector<File> textureAssets = { *materialAsset };
		auto material = std::make_shared<graphic::Material>(*this, shaderName,
															textureAssets);
		auto id = getNextID();

		// TODO: we should discuss about the format of the material file to be
		// able to load the textures from the material file and store them in
		// the material object
		_materials[id] = material;

		return _materials[id];
	}

	std::shared_ptr<graphic::Texture>
		RessourceProvider::loadTexture(const std::string &path)
	{
		auto it = _elementsIDs.find(path);

		if (it != _elementsIDs.end()) {
			if (_textures.find(it->second) != _textures.end()) {
				return _textures[it->second];
			}
		}

		auto textureAsset = _systemInterface.add(path);

		if (!textureAsset) {
			std::cerr << "Failed to load texture asset: " << path << std::endl;
			return nullptr;
		}

		auto texture = loadTextureFromAsset(textureAsset);

		return texture;
	}

	std::shared_ptr<graphic::Texture> RessourceProvider::loadTextureFromAsset(
		std::shared_ptr<utility::File> textureAsset)
	{
		auto it = _elementsIDs.find(textureAsset->path());

		if (it != _elementsIDs.end()) {
			if (_textures.find(it->second) != _textures.end()) {
				return _textures[it->second];
			}
		}

		int texWidth	= 0;
		int texHeight	= 0;
		int texChannels = 0;

		stbi_uc *pixels = stbi_load_from_memory(
			reinterpret_cast<const stbi_uc *>(textureAsset->content().c_str()),
			textureAsset->size(), &texWidth, &texHeight, &texChannels,
			STBI_rgb_alpha);

		if (!pixels) {
			std::cerr << "Failed to load texture: " << textureAsset->path() << std::endl;
			return nullptr;
		}

		auto id = getNextID();

		_textures[id] =
			std::make_shared<graphic::Texture>(texWidth, texHeight);
		std::copy(pixels, pixels + (texWidth * texHeight * 4),
				  _textures[id]->_pixels.data());

		return _textures[id];
	}

	std::shared_ptr<graphic::Model> RessourceProvider::loadModel(const std::string &path)
	{
		auto it = _elementsIDs.find(path);

		if (it != _elementsIDs.end()) {
			if (_models.find(it->second) != _models.end()) {
				return _models[it->second];
			}
		}

		auto modelAsset = _systemInterface.add(path);

		if (!modelAsset) {
			std::cerr << "Failed to load model asset: " << path << std::endl;
			return nullptr;
		}

		auto id = getNextID();

		_models[id] = loadModelFromAsset(modelAsset);

		return _models[id];
	}

	std::shared_ptr<graphic::Model> RessourceProvider::loadModelFromAsset(
		std::shared_ptr<utility::File> modelAsset)
	{
		auto it = _elementsIDs.find(modelAsset->path());

		if (it != _elementsIDs.end()) {
			if (_models.find(it->second) != _models.end()) {
				return _models[it->second];
			}
		}

		auto model = std::make_shared<graphic::Model>(modelAsset);
		auto id = getNextID();

		_models[id] = model;

		return _models[id];
	}

	std::shared_ptr<graphic::Model> RessourceProvider::loadModelFromAsset(
		std::shared_ptr<utility::File> modelAsset, graphic::Model::ModelType type)
	{
		auto it = _elementsIDs.find(modelAsset->path());

		if (it != _elementsIDs.end()) {
			if (_models.find(it->second) != _models.end()) {
				return _models[it->second];
			}
		}

		auto model = std::make_shared<graphic::Model>(modelAsset, type);
		auto id = getNextID();

		_models[id] = model;

		return _models[id];
	}

	std::shared_ptr<graphic::Model> RessourceProvider::loadObj(const std::string &path)
	{
		auto it = _elementsIDs.find(path);

		if (it != _elementsIDs.end()) {
			if (_models.find(it->second) != _models.end()) {
				return _models[it->second];
			}
		}

		auto modelAsset = _systemInterface.add(path);

		if (!modelAsset) {
			std::cerr << "Failed to load model asset: " << path << std::endl;
			return nullptr;
		}

		auto model = std::make_shared<graphic::Model>(modelAsset,
													   graphic::Model::ModelType::OBJ);
		auto id = getNextID();

		_models[id] = model;

		return _models[id];
	}

	std::shared_ptr<graphic::Model> RessourceProvider::loadObjFromAsset(std::shared_ptr<utility::File> modelAsset)
	{
		auto it = _elementsIDs.find(modelAsset->path());

		if (it != _elementsIDs.end()) {
			if (_models.find(it->second) != _models.end()) {
				return _models[it->second];
			}
		}

		auto model = std::make_shared<graphic::Model>(modelAsset,
													   graphic::Model::ModelType::OBJ);
		auto id = getNextID();

		_models[id] = model;

		return _models[id];
	}

	std::shared_ptr<graphic::Shader> RessourceProvider::loadShader(const std::string &vertexPath, const std::string &fragmentPath)
	{
		auto path = buildShaderPath(vertexPath, fragmentPath);
		auto it = _elementsIDs.find(path);

		if (it != _elementsIDs.end()) {
			if (_shaders.find(it->second) != _shaders.end()) {
				return _shaders[it->second];
			}
		}

		auto vertex = _systemInterface.add(vertexPath);
		auto fragment = _systemInterface.add(fragmentPath);

		if (!vertex) {
			std::cerr << "Failed to load shader asset: " << vertexPath << std::endl;
			return nullptr;
		}

		if (!fragment) {
			std::cerr << "Failed to load shader asset: " << fragmentPath << std::endl;
			return nullptr;
		}

		auto shader = std::make_shared<graphic::Shader>(vertex->content(), fragment->content());
		auto id = getNextID();

		_shaders[id] = shader;
		_elementsIDs[path] = id;

		return _shaders[id];
	}

	std::shared_ptr<graphic::Shader> RessourceProvider::loadShaderFromAssets(std::shared_ptr<utility::File> vertexAsset, std::shared_ptr<utility::File> fragmentAsset)
	{
		auto path = buildShaderPath(vertexAsset->path(), fragmentAsset->path());
		auto it = _elementsIDs.find(path);

		if (it != _elementsIDs.end()) {
			if (_shaders.find(it->second) != _shaders.end()) {
				return _shaders[it->second];
			}
		}

		auto shader = std::make_shared<graphic::Shader>(vertexAsset->content(), fragmentAsset->content());
		auto id = getNextID();

		_shaders[id] = shader;
		_elementsIDs[path] = id;

		return _shaders[id];
	}

	///////////////////////
	// Protected Methods //
	///////////////////////

	std::string RessourceProvider::buildShaderPath(const std::string &vertexPath, const std::string &fragmentPath) const
	{
		std::string pureVertexName = vertexPath.substr(vertexPath.find_last_of("/\\") + 1);
		std::string pureFragmentName = fragmentPath.substr(fragmentPath.find_last_of("/\\") + 1);
		std::string path = pureVertexName + "_with_" + pureFragmentName;

		return path;
	}

	uint32_t RessourceProvider::getNextID()
	{
		return _currentID++;
	}

}	 // namespace utility
