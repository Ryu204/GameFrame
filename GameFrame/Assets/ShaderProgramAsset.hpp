#ifndef GAMEFRAME_ASSETS_SHADER_PROGRAM_ASSET_HPP
#define GAMEFRAME_ASSETS_SHADER_PROGRAM_ASSET_HPP

#include <filesystem>
#include <functional>
#include <map>
#include <mutex>
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

		struct ShaderProgramAssetInUse {
			std::shared_ptr<Graphics::Program> Program;
			std::unique_lock<std::mutex> MutexGuard;
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

			auto lock() -> ShaderProgramAssetInUse;

		private:
			auto update() -> void;
			auto load() -> std::shared_ptr<Graphics::Program>;

			std::map<Graphics::ShaderType, detail::Shader> mShaders;
			std::shared_ptr<Graphics::Program> mProgram;
			std::mutex mMutex;
			bool mInvalidated{true};
		};

		class ShaderProgramAssetBuilder
		{
		public:
			ShaderProgramAssetBuilder() = default;

			auto shaderSource(Graphics::ShaderType type, std::string source) && -> ShaderProgramAssetBuilder;
			auto shaderFile(Graphics::ShaderType type, Path path) && -> ShaderProgramAssetBuilder;
			auto shaderFile(Graphics::ShaderType type, std::shared_ptr<FileAsset> file) && -> ShaderProgramAssetBuilder;

			// NOTE: this invalidates the builder, so one can't use a builder twice
			auto build() && -> std::shared_ptr<ShaderProgramAsset>;

		private:
			std::map<Graphics::ShaderType, detail::Shader> mShaders;
			friend class ShaderProgramAsset;
		};
	} // namespace Assets

} // namespace HJUIK


#endif
