#ifndef GAMEFRAME_ASSETS_SHADER_PROGRAM_ASSET_HPP
#define GAMEFRAME_ASSETS_SHADER_PROGRAM_ASSET_HPP

#include <atomic>
#include <filesystem>
#include <functional>
#include <map>
#include <string>
#include <variant>

#include "../Graphics/Program.hpp"
#include "../Graphics/Shader.hpp"
#include "Asset.hpp"
#include "FileAsset.hpp"

namespace HJUIK
{
	namespace Assets
	{
		namespace detail
		{
			struct ShaderFile {
				std::shared_ptr<FileAsset> File;
				UpdateHandle Handle;

				auto source() const -> std::string;
				auto registerUpdateHandler(std::function<void()> callback) -> void;
			};

			struct ShaderSource {
				std::string Source;

				auto source() const -> std::string;
			};

			using Shader = std::variant<ShaderFile, ShaderSource>;
		} // namespace detail

		class ShaderProgramAssetBuilder;

		struct ShaderProgramAssetValue {
			// A shared_ptr to the current program.
			// The user can reuse this program as much as they want
			// (provided that they kept a reference to this), but they
			// would miss out on the newly hot-reloaded program
			// Hence, it's advisable for one not to put this into any
			// long-term storage.
			std::shared_ptr<Graphics::Program> Program;
			// If this is true, the `Program` has been recreated since
			// the last call to `ShaderProgramAsset::get()`
			// Also, `registerUpdateHandler` can also be used to achieve
			// somewhat similar functionalities.
			bool Invalidated;
		};

		class ShaderProgramAsset : public Asset
		{
		public:
			explicit ShaderProgramAsset(ShaderProgramAssetBuilder&& builder);
			~ShaderProgramAsset() override;
			ShaderProgramAsset(const ShaderProgramAsset&)					 = delete;
			ShaderProgramAsset(ShaderProgramAsset&&)						 = delete;
			auto operator=(const ShaderProgramAsset&) -> ShaderProgramAsset& = delete;
			auto operator=(ShaderProgramAsset&&) -> ShaderProgramAsset&		 = delete;

			// get current shader program
			auto get() -> ShaderProgramAssetValue;

		private:
			auto update() -> void;
			auto load() -> std::shared_ptr<Graphics::Program>;

			std::map<Graphics::ShaderType, detail::Shader> mShaders;
			std::shared_ptr<Graphics::Program> mProgram;
			std::atomic_bool mInvalidated;
			friend class ShaderProgramAssetBuilder;
		};

		// rvalue-based builder: one builder can only `build` one `ShaderProgramAsset`
		// (motivated by implementations of the builder pattern in Rust libraries)
		// this will make the `build` process a little bit more efficient
		class ShaderProgramAssetBuilder
		{
		public:
			ShaderProgramAssetBuilder() = default;

			// supports both shader source and shader files (with optional hot-reloading for shader files)
			auto shaderSource(Graphics::ShaderType type, std::string source) && -> ShaderProgramAssetBuilder;
			auto shaderFile(
				Graphics::ShaderType type, Path path, bool watchFile = true) && -> ShaderProgramAssetBuilder;
			auto shaderFile(Graphics::ShaderType type, std::shared_ptr<FileAsset> file) && -> ShaderProgramAssetBuilder;

			auto build() && -> std::shared_ptr<ShaderProgramAsset>;

		private:
			std::map<Graphics::ShaderType, detail::Shader> mShaders;
			friend class ShaderProgramAsset;
		};
	} // namespace Assets

} // namespace HJUIK


#endif
