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

	////////////////////
	// Public Methods //
	////////////////////

	std::shared_ptr<graphic::Font>
		RessourceProvider::loadFont(const std::string &path,
								   SystemIO &systemInterface)
	{
		auto it = _elementsIDs.find(path);

		if (it != _elementsIDs.end()) {
			if (_fonts.find(it->second) != _fonts.end()) {
				return _fonts[it->second];
			}
		}

		auto fontAsset = systemInterface.add(path);

		if (!fontAsset) {
			throw std::runtime_error("Failed to load font asset: " + path);
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

		font->onNewTextureCreated =
			[this, fontAsset](std::string name,
							   std::shared_ptr<graphic::Texture> atlas) {
				auto textureID = getNextID();

				_textures[textureID] = atlas;
				_elementsIDs[name] = textureID;

				auto familyIt = _elementsIDs.find(fontAsset->path());

				if (familyIt != _elementsIDs.end()) {
					auto materialIt = _materials.find(familyIt->second);

					if (materialIt != _materials.end()) {
						auto textMaterial =
							std::dynamic_pointer_cast<graphic::TextMaterial>(
								materialIt->second);

						if (textMaterial) {
							textMaterial->addAtlas(name, atlas);
						} else {
							throw std::runtime_error(
								"Miss configured material for font: " + name
								+ " is not a TextMaterial");
						}
					}
				} else {
					auto textMaterial =
						std::make_shared<graphic::TextMaterial>();

					textMaterial->addAtlas(name, atlas);

					_materials[familyIt->second] = textMaterial;
				}
			};

		auto id = getNextID();

		_fonts[id] = font;
		return _fonts[id];
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

		font->onNewTextureCreated =
			[this, familyName](std::string name,
							   std::shared_ptr<graphic::Texture> atlas) {
				auto textureID = getNextID();

				_textures[textureID] = atlas;
				_elementsIDs[name] = textureID;

				auto familyIt = _elementsIDs.find(familyName);

				if (familyIt != _elementsIDs.end()) {
					auto materialIt = _materials.find(familyIt->second);

					if (materialIt != _materials.end()) {
						auto textMaterial =
							std::dynamic_pointer_cast<graphic::TextMaterial>(
								materialIt->second);

						if (textMaterial) {
							textMaterial->addAtlas(name, atlas);
						} else {
							throw std::runtime_error(
								"Miss configured material for font: " + name
								+ " is not a TextMaterial");
						}
					}
				} else {
					auto textMaterial =
						std::make_shared<graphic::TextMaterial>();

					textMaterial->addAtlas(name, atlas);

					_materials[familyIt->second] = textMaterial;
				}
			};

		auto id = getNextID();

		_fonts[id] = font;
		return _fonts[id];
	}

	std::shared_ptr<graphic::Material>
		RessourceProvider::loadMaterial(const std::string &path,
									   ShaderType shaderType,
									   SystemIO &systemInterface)
	{
		auto it = _elementsIDs.find(path);

		if (it != _elementsIDs.end()) {
			if (_materials.find(it->second) != _materials.end()) {
				return _materials[it->second];
			}
		}

		auto materialAsset = systemInterface.add(path);

		if (!materialAsset) {
			throw std::runtime_error("Failed to load material asset: " + path);
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
			? "text_shader"
			: "mesh_shader";
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
		RessourceProvider::loadTexture(const std::string &path,
									  SystemIO &systemInterface)
	{
		auto it = _elementsIDs.find(path);

		if (it != _elementsIDs.end()) {
			if (_textures.find(it->second) != _textures.end()) {
				return _textures[it->second];
			}
		}

		auto textureAsset = systemInterface.add(path);

		if (!textureAsset) {
			throw std::runtime_error("Failed to load texture asset: " + path);
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
			throw std::runtime_error("Failed to load texture: "
									 + textureAsset->path());
		}

		auto id = getNextID();

		_textures[id] =
			std::make_shared<graphic::Texture>(texWidth, texHeight);
		std::copy(pixels, pixels + (texWidth * texHeight * 4),
				  _textures[id]->_pixels.data());

		return _textures[id];
	}

	std::shared_ptr<graphic::Model> RessourceProvider::loadModel(const std::string &path,
																SystemIO &systemInterface)
	{
		auto it = _elementsIDs.find(path);

		if (it != _elementsIDs.end()) {
			if (_models.find(it->second) != _models.end()) {
				return _models[it->second];
			}
		}

		auto modelAsset = systemInterface.add(path);

		if (!modelAsset) {
			throw std::runtime_error("Failed to load model asset: " + path);
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

	std::shared_ptr<graphic::Model> RessourceProvider::loadObj(const std::string &path, SystemIO &systemInterface)
	{
		auto it = _elementsIDs.find(path);

		if (it != _elementsIDs.end()) {
			if (_models.find(it->second) != _models.end()) {
				return _models[it->second];
			}
		}

		auto modelAsset = systemInterface.add(path);

		if (!modelAsset) {
			throw std::runtime_error("Failed to load model asset: " + path);
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

	///////////////////////
	// Protected Methods //
	///////////////////////

	uint32_t RessourceProvider::getNextID()
	{
		return _currentID++;
	}

}	 // namespace utility
