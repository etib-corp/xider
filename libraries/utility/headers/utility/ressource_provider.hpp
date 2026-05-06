#pragma once

#include <utility/system_io/file.hpp>
#include <utility/system_io/system_io.hpp>

#include <utility/graphic/text/font.hpp>
#include <utility/graphic/text/text_material.hpp>
#include <utility/graphic/material.hpp>
#include <utility/graphic/texture.hpp>
#include <utility/graphic/model.hpp>
#include <utility/graphic/shader.hpp>

namespace utility
{
	/**
	 * @brief The RessourceProvider class is responsible for managing and loading
	 * various resources such as fonts, materials, and textures in a graphics
	 * application.
	 *
	 * The RessourceProvider provides methods to load resources from file paths
	 * or from asset objects, and it maintains internal maps to store loaded
	 * resources for efficient retrieval. It supports loading fonts, materials,
	 * and textures, and it can handle different shader types for materials.
	 */
	class RessourceProvider
	{
		public:
		/**
		 * @brief Enumeration for different shader types that can be associated
		 * with materials.
		 *
		 * This enum defines the types of shaders that can be used when loading
		 * materials. It currently includes `TEXT_SHADER` for text rendering and
		 * `MESH_SHADER` for mesh rendering, allowing for different visual
		 * effects based on the shader type used in the material.
		 */
		enum class ShaderType { TEXT_SHADER, MESH_SHADER };

		/**
		 * @brief Constructs a RessourceProvider object.
		 */
		RessourceProvider() = default;

		/**
		 * @brief Destructs the RessourceProvider object.
		 */
		~RessourceProvider() = default;

		/**
		 * @brief Retrieves a map of loaded materials.
		 *
		 * @return A map where the keys are material IDs (uint32_t) and the values
		 * are shared pointers to the corresponding Material objects.
		 */
		[[nodiscard]] std::map<uint32_t, std::shared_ptr<graphic::Material>> getMaterials() const;

		/**
		 * @brief Retrieves a map of loaded textures.
		 *
		 * @return A map where the keys are texture IDs (uint32_t) and the values
		 * are shared pointers to the corresponding Texture objects.
		 */
		[[nodiscard]] std::map<uint32_t, std::shared_ptr<graphic::Texture>> getTextures() const;

		/**
		 * @brief Retrieves a map of loaded models.
		 *
		 * @return A map where the keys are model IDs (uint32_t) and the values
		 * are shared pointers to the corresponding Model objects.
		 */
		[[nodiscard]] std::map<uint32_t, std::shared_ptr<graphic::Model>> getModels() const;

		/**
		 * @brief Retrieves a map of loaded shaders.
		 *
		 * @return A map where the keys are shader IDs (uint32_t) and the values
		 * are shared pointers to the corresponding Shader objects.
		 */
		[[nodiscard]] std::map<uint32_t, std::shared_ptr<graphic::Shader>> getShaders() const;

		/**
		 * @brief Loads a font resource from a specified file path.
		 *
		 * @param path The file path to the font resource to be loaded.
		 * @param systemInterface A reference to the SystemIO instance used to
		 * load the font asset.
		 *
		 * @return A shared pointer to the loaded Font object.
		 */
		std::shared_ptr<graphic::Font> loadFont(const std::string &path,
												SystemIO &systemInterface);

		/**
		 * @brief Loads a font resource from a specified asset.
		 *
		 * @param fontAsset A shared pointer to the File object containing
		 * the font data to be loaded.
		 *
		 * @return A shared pointer to the loaded Font object.
		 */
		std::shared_ptr<graphic::Font>
			loadFontFromAsset(std::shared_ptr<utility::File> fontAsset);

		// TODO: add loadFontsFromDirectory method to load all fonts from a
		// directory But needs to be implemented in the SystemIO first to
		// load all assets from a directory

		// TODO: add loadFontFamily method to load all fonts from a directory
		// But needs to be implemented in the SystemIO first to load all
		// assets from a directory

		/**
		 * @brief Loads a font family resource from a vector of font assets.
		 *
		 * @param fontAssets A vector of shared pointers to File objects
		 * containing the font data for the font family to be loaded.
		 *
		 * @return A shared pointer to the loaded Font object representing the
		 * font family.
		 */
		std::shared_ptr<graphic::Font> loadFontFamilyFromAssets(
			const std::vector<std::shared_ptr<utility::File>> &fontAssets);

		/**
		 * @brief Loads a material resource from a specified file path.
		 *
		 * @param path The file path to the material resource to be loaded.
		 * @param shaderType The type of shader to be associated with the loaded
		 * material.
		 * @param systemInterface A reference to the SystemIO instance used to
		 * load the material asset.
		 *
		 * @return A shared pointer to the loaded Material object.
		 */
		std::shared_ptr<graphic::Material>
			loadMaterial(const std::string &path, ShaderType shaderType,
						 SystemIO &systemInterface);

		/**
		 * @brief Loads a material resource from a specified asset.
		 *
		 * @param shaderType The type of shader to be associated with the loaded
		 * material.
		 * @param materialAsset A shared pointer to the File object
		 * containing the material data to be loaded.
		 *
		 * @return A shared pointer to the loaded Material object.
		 */
		std::shared_ptr<graphic::Material> loadMaterialFromAsset(
			ShaderType shaderType,
			std::shared_ptr<utility::File> materialAsset);

		/**
		 * @brief Loads a texture resource from a specified file path.
		 *
		 * @param path The file path to the texture resource to be loaded.
		 * @param systemInterface A reference to the SystemIO instance used to
		 * load the texture asset
		 *
		 * @return A shared pointer to the loaded Texture object.
		 */
		std::shared_ptr<graphic::Texture>
			loadTexture(const std::string &path, SystemIO &systemInterface);

		/**
		 * @brief Loads a texture resource from a specified asset.
		 *
		 * @param textureAsset A shared pointer to the File object
		 * containing the texture data to be loaded.
		 *
		 * @return A shared pointer to the loaded Texture object.
		 */
		std::shared_ptr<graphic::Texture> loadTextureFromAsset(
			std::shared_ptr<utility::File> textureAsset);

		// TODO: add loadTexturesFromDirectory method to load all textures from
		// a directory But needs to be implemented in the SystemIO first to
		// load all assets from a directory

		/**
		 * @brief Loads a model resource from a specified file path.
		 *
		 * @param path The file path to the model resource to be loaded.
		 * @param systemInterface A reference to the SystemIO instance used to
		 * load the model asset.
		 *
		 * @return A shared pointer to the loaded Model object.
		 */
		std::shared_ptr<graphic::Model> loadModel(const std::string &path,
												SystemIO &systemInterface);

		/**
		 * @brief Loads a model resource from a specified asset.
		 *
		 * @param modelAsset A shared pointer to the File object containing the model data to be loaded.
		 *
		 * @return A shared pointer to the loaded Model object.
		 */
		std::shared_ptr<graphic::Model> loadModelFromAsset(std::shared_ptr<utility::File> modelAsset);

		/**
		 * @brief Loads a model resource from a specified file path and model type.
		 *
		 * @param path The file path to the model resource to be loaded.
		 * @param type The type of the model (e.g., OBJ, FBX, GLTF) to be loaded.
		 * @param systemInterface A reference to the SystemIO instance
		 *
		 * @return A shared pointer to the loaded Model object.
		 */
		std::shared_ptr<graphic::Model> loadModelFromAsset(std::shared_ptr<utility::File> modelAsset, graphic::Model::ModelType type);

		/**
		 * @brief Loads an OBJ model resource from a specified file path.
		 *
		 * @param path The file path to the OBJ model resource to be loaded.
		 * @param systemInterface A reference to the SystemIO instance used to load the model asset
		 *
		 * @return A shared pointer to the loaded Model object representing the OBJ model.
		 */
		std::shared_ptr<graphic::Model> loadObj(const std::string &path, SystemIO &systemInterface);

		/**
		 * @brief Loads an OBJ model resource from a specified asset.
		 *
		 * @param modelAsset A shared pointer to the File object containing the OBJ model data to be loaded.
		 *
		 * @return A shared pointer to the loaded Model object representing the OBJ model.
		 */
		std::shared_ptr<graphic::Model> loadObjFromAsset(std::shared_ptr<utility::File> modelAsset);

		/**
		 * @brief Loads a shader resource from specified vertex and fragment shader file paths.
		 *
		 * @param vertexPath The file path to the vertex shader resource to be loaded.
		 * @param fragmentPath The file path to the fragment shader resource to be loaded.
		 * @param systemInterface A reference to the SystemIO instance used to load the shader assets
		 *
		 * @return A shared pointer to the loaded Shader object.
		 */
		std::shared_ptr<graphic::Shader> loadShader(const std::string &vertexPath, const std::string &fragmentPath, SystemIO &systemInterface);

		/**
		 * @brief Loads a shader resource from specified vertex and fragment shader assets.
		 *
		 * @param vertexAsset A shared pointer to the File object containing the vertex shader data to be loaded.
		 * @param fragmentAsset A shared pointer to the File object containing the fragment shader data to be loaded.
		 *
		 * @return A shared pointer to the loaded Shader object.
		 */
		std::shared_ptr<graphic::Shader> loadShaderFromAssets(std::shared_ptr<utility::File> vertexAsset, std::shared_ptr<utility::File> fragmentAsset);

		protected:
		/**
		 * @brief Builds a unique shader name based on the vertex and fragment shader file paths.
		 *
		 * This method constructs a unique name for a shader by combining the vertex and fragment shader file paths.
		 * The resulting name can be used as a key in the internal map to store and retrieve shader resources efficiently.
		 * The method ensures that shaders with the same vertex and fragment paths will have the same name,
		 * allowing for proper caching and reuse of shader resources.
		 */
		const std::string &buildShaderPath(const std::string &vertexPath, const std::string &fragmentPath) const;

		/**
		 * @brief Retrieves the next unique ID for a resource.
		 *
		 * This method increments the internal `nextID` counter and returns the next unique ID for a resource.
		 * It is used internally to assign unique IDs to loaded resources such as fonts, materials,
		 * textures, and models. The returned ID can be used as a key in the internal maps to store and retrieve resources efficiently.
		 */
		uint32_t getNextID();

		/**
		 * @brief Internal counter for generating unique IDs for resources
		 *
		 * @note Do not modify this variable directly.
		 * Use the getNextID() method to retrieve the next unique ID for a resource.
		 */
		uint32_t _currentID = 0;

		/**
		 * @brief Internal maps to store loaded fonts for efficient retrieval.
		 */
		std::map<uint32_t, std::shared_ptr<graphic::Font>> _fonts;

		/**
		 * @brief Internal map to store loaded materials for efficient
		 * retrieval.
		 */
		std::map<uint32_t, std::shared_ptr<graphic::Material>> _materials;

		/**
		 * @brief Internal map to store loaded textures for efficient retrieval.
		 */
		std::map<uint32_t, std::shared_ptr<graphic::Texture>> _textures;

		/**
		 * @brief Internal map to store loaded models for efficient retrieval.
		 */
		std::map<uint32_t, std::shared_ptr<graphic::Model>> _models;

		/**
		 * @brief Internal map to store loaded shaders for efficient retrieval.
		 */
		std::map<uint32_t, std::shared_ptr<graphic::Shader>> _shaders;

		/**
		 * @brief Internal map to store resource IDs for efficient lookup based on file paths.
		 */
		std::unordered_map<std::string, uint32_t> _elementsIDs;

	};
}	 // namespace utility
