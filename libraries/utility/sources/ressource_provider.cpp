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

	std::map<std::string, std::shared_ptr<graphic::Material>> RessourceProvider::getMaterials() const
	{
		return _materials;
	}

	std::map<std::string, std::shared_ptr<graphic::Texture>> RessourceProvider::getTextures() const
	{
		return _textures;
	}

	std::map<std::string, std::shared_ptr<graphic::Model>> RessourceProvider::getModels() const
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
		auto it = _fonts.find(path);

		if (it != _fonts.end()) {
			return it->second;
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
		auto font = std::make_shared<graphic::Font>(std::vector { *fontAsset });

		font->onNewTextureCreated =
			[this, fontAsset](std::string name,
							  std::shared_ptr<graphic::Texture> atlas) {
				_textures[name] = atlas;

				if (_materials.find(fontAsset->path()) != _materials.end()) {
					auto textMaterial =
						std::dynamic_pointer_cast<graphic::TextMaterial>(
							_materials[fontAsset->path()]);

					if (textMaterial) {
						textMaterial->addAtlas(name, atlas);
					} else {
						throw std::runtime_error(
							"Miss configured material for font: "
							+ fontAsset->path() + " is not a TextMaterial");
					}
				} else {
					auto textMaterial =
						std::make_shared<graphic::TextMaterial>();

					textMaterial->addAtlas(name, atlas);
					_materials[fontAsset->path()] = textMaterial;
				}
			};

		_fonts[fontAsset->path()] = font;
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

		auto font = std::make_shared<graphic::Font>(assets);

		font->onNewTextureCreated =
			[this, familyName](std::string name,
							   std::shared_ptr<graphic::Texture> atlas) {
				_textures[name] = atlas;

				if (_materials.find(familyName) != _materials.end()) {
					auto textMaterial =
						std::dynamic_pointer_cast<graphic::TextMaterial>(
							_materials[familyName]);

					if (textMaterial) {
						textMaterial->addAtlas(name, atlas);
					} else {
						throw std::runtime_error(
							"Miss configured material for font: " + name
							+ " is not a TextMaterial");
					}
				} else {
					auto textMaterial =
						std::make_shared<graphic::TextMaterial>();

					textMaterial->addAtlas(name, atlas);
					_materials[familyName] = textMaterial;
				}
			};

		_fonts[familyName] = font;
		return font;
	}

	std::shared_ptr<graphic::Material>
		RessourceProvider::loadMaterial(const std::string &path,
									   ShaderType shaderType,
									   SystemIO &systemInterface)
	{
		auto it = _materials.find(path);

		if (it != _materials.end()) {
			return it->second;
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
		std::string shaderName = (shaderType == ShaderType::TEXT_SHADER)
			? "text_shader"
			: "mesh_shader";
		std::vector<File> textureAssets = { *materialAsset };
		auto material = std::make_shared<graphic::Material>(*this, shaderName,
															textureAssets);

		// TODO: we should discuss about the format of the material file to be
		// able to load the textures from the material file and store them in
		// the material object
		_materials[materialAsset->path()] = material;
		return material;
	}

	std::shared_ptr<graphic::Texture>
		RessourceProvider::loadTexture(const std::string &path,
									  SystemIO &systemInterface)
	{
		auto it = _textures.find(path);

		if (it != _textures.end()) {
			return it->second;
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

		_textures[textureAsset->path()] =
			std::make_shared<graphic::Texture>(texWidth, texHeight);
		std::copy(pixels, pixels + (texWidth * texHeight * 4),
				  _textures[textureAsset->path()]->_pixels.data());

		return _textures[textureAsset->path()];
	}

	std::shared_ptr<graphic::Model> RessourceProvider::loadModel(const std::string &path,
																SystemIO &systemInterface)
	{
		auto it = _models.find(path);

		if (it != _models.end()) {
			return it->second;
		}

		auto modelAsset = systemInterface.add(path);

		if (!modelAsset) {
			throw std::runtime_error("Failed to load model asset: " + path);
		}

		_models[path] = loadModelFromAsset(modelAsset);

		return _models[path];
	}

	std::shared_ptr<graphic::Model> RessourceProvider::loadModelFromAsset(
		std::shared_ptr<utility::File> modelAsset)
	{
		auto it = _models.find(modelAsset->path());

		if (it != _models.end()) {
			return it->second;
		}

		auto model = std::make_shared<graphic::Model>(modelAsset);

		_models[modelAsset->path()] = model;
		return model;
	}

	std::shared_ptr<graphic::Model> RessourceProvider::loadModelFromAsset(
		std::shared_ptr<utility::File> modelAsset, graphic::Model::ModelType type)
	{
		auto it = _models.find(modelAsset->path());

		if (it != _models.end()) {
			return it->second;
		}

		auto model = std::make_shared<graphic::Model>(modelAsset, type);

		_models[modelAsset->path()] = model;
		return model;
	}

	std::shared_ptr<graphic::Model> RessourceProvider::loadObj(const std::string &path, SystemIO &systemInterface)
	{
		auto it = _models.find(path);

		if (it != _models.end()) {
			return it->second;
		}

		auto modelAsset = systemInterface.add(path);

		if (!modelAsset) {
			throw std::runtime_error("Failed to load model asset: " + path);
		}

		auto model = std::make_shared<graphic::Model>(modelAsset,
													   graphic::Model::ModelType::OBJ);

		_models[path] = model;
		return model;
	}

	std::shared_ptr<graphic::Model> RessourceProvider::loadObjFromAsset(std::shared_ptr<utility::File> modelAsset)
	{
		auto it = _models.find(modelAsset->path());

		if (it != _models.end()) {
			return it->second;
		}

		auto model = std::make_shared<graphic::Model>(modelAsset,
													   graphic::Model::ModelType::OBJ);

		_models[modelAsset->path()] = model;
		return model;
	}

}	 // namespace utility
