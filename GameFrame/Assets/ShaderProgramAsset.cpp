#include "ShaderProgramAsset.hpp"

#include <iostream>
#include <stdexcept>
#include <utility>

#include "../Utilize/CallAssert.hpp"

namespace HJUIK
{
	namespace Assets
	{
		namespace detail
		{
			auto ShaderFile::source() const -> std::string
			{
				return File->readText();
			}

			auto ShaderFile::registerUpdateHandler(std::function<void()> callback) -> void
			{
				HJUIK_ASSERT(!Handle.isValid(), "shader file is already in use");
				Handle = File->registerUpdateHandler(std::move(callback));
			}

			auto ShaderSource::source() const -> std::string
			{
				return Source;
			}
		} // namespace detail

		using GenericShader = std::variant<Graphics::VertexShader, Graphics::FragmentShader, Graphics::GeometryShader,
			Graphics::ComputeShader, Graphics::TessControlShader, Graphics::TessEvaluationShader>;

		static auto compileShader(Graphics::ShaderType type, const char* source) -> GenericShader
		{
			switch (type) {
			case Graphics::ShaderType::VERTEX:
				return Graphics::VertexShader::createAndCompile(source);
			case Graphics::ShaderType::FRAGMENT:
				return Graphics::FragmentShader::createAndCompile(source);
			case Graphics::ShaderType::GEOMETRY:
				return Graphics::GeometryShader::createAndCompile(source);
			case Graphics::ShaderType::COMPUTE:
				return Graphics::ComputeShader::createAndCompile(source);
			case Graphics::ShaderType::TESS_CONTROL:
				return Graphics::TessControlShader::createAndCompile(source);
			case Graphics::ShaderType::TESS_EVALUATION:
				return Graphics::TessEvaluationShader::createAndCompile(source);
			}

			HJUIK_ASSERT(false, "invalid shader type");
		}

		ShaderProgramAsset::ShaderProgramAsset(ShaderProgramAssetBuilder&& builder)
			: mShaders{std::move(builder.mShaders)}, mProgram{nullptr}, mInvalidated{true}
		{
			for (auto& [type, shader] : mShaders) {
				if (std::holds_alternative<detail::ShaderFile>(shader)) {
					// since `this` directly manages the ShaderFile,
					// it's safe to pass this directly here
					std::get<detail::ShaderFile>(shader).registerUpdateHandler([this]() { this->update(); });
				}
			}
			load();
		}

		ShaderProgramAsset::~ShaderProgramAsset() = default;

		auto ShaderProgramAsset::load() -> std::shared_ptr<Graphics::Program>
		{
			auto program = std::make_shared<Graphics::Program>();
			std::vector<GenericShader> shaders;
			for (const auto& [type, shader] : mShaders) {
				const auto source	= std::visit([](const auto& shader) { return shader.source(); }, shader);
				auto compiledShader = compileShader(type, source.c_str());
				std::visit([&](const auto& shader) { program->attachShader(shader); }, compiledShader);
				shaders.push_back(std::move(compiledShader));
			}

			if (!program->link()) {
				throw std::runtime_error("unable to link program: " + program->getInfoLog());
			}

			program->detachAllShaders();
			return program;
		}

		auto ShaderProgramAsset::get() -> ShaderProgramAssetValue
		{
			const auto wasInvalidated = mInvalidated.load(std::memory_order_relaxed);
			if (wasInvalidated) {
				try {
					mProgram = load();
				} catch (std::runtime_error& err) {
					// error compiling/linking shader program
					std::cout << "error while hot-reloading shader program: " << err.what() << '\n';
				}

				assetUpdated();
				mInvalidated.store(false, std::memory_order_relaxed);
			}
			return ShaderProgramAssetValue{
				mProgram,
				wasInvalidated,
			};
		}

		auto ShaderProgramAsset::update() -> void
		{
			mInvalidated = true;
		}

		auto ShaderProgramAssetBuilder::shaderSource(
			Graphics::ShaderType type, std::string source) && -> ShaderProgramAssetBuilder
		{
			mShaders.insert_or_assign(type, detail::ShaderSource{std::move(source)});
			return std::move(*this);
		}

		auto ShaderProgramAssetBuilder::shaderFile(
			Graphics::ShaderType type, Path path, bool watchFile) && -> ShaderProgramAssetBuilder
		{
			return std::move(*this).shaderFile(type, std::make_shared<FileAsset>(std::move(path), watchFile));
		}

		auto ShaderProgramAssetBuilder::shaderFile(
			Graphics::ShaderType type, std::shared_ptr<FileAsset> file) && -> ShaderProgramAssetBuilder
		{
			mShaders.insert_or_assign(type, detail::ShaderFile{std::move(file)});
			return std::move(*this);
		}

		auto ShaderProgramAssetBuilder::build() && -> std::shared_ptr<ShaderProgramAsset>
		{
			return std::make_shared<ShaderProgramAsset>(std::move(*this));
		}

	} // namespace Assets

} // namespace HJUIK
