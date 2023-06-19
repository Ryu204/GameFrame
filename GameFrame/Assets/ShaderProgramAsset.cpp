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
			: mShaders{std::move(builder.mShaders)}, mProgram{nullptr}
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
				const auto source		  = std::visit([](const auto& shader) { return shader.source(); }, shader);
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

		auto ShaderProgramAsset::lock() -> ShaderProgramAssetInUse
		{
			std::unique_lock lock{mMutex};
			const auto wasInvalidated = mInvalidated;
			if (mInvalidated) {
				try {
					mProgram = load();
					assetUpdated();
				} catch (std::runtime_error& err) {
					// error compiling/linking shader program
					std::cout << "error while hot-reloading shader program: " << err.what() << '\n';
				}

				mInvalidated = false;
			}
			return ShaderProgramAssetInUse{
				mProgram,
				std::move(lock),
				wasInvalidated,
			};
		}

		auto ShaderProgramAsset::update() -> void
		{
			// in a multithreaded context, this may be called while the
			// program is already in use
			// hence, one should use some kind of locking mechanism to
			// prevent this overriding behavior
			// also this will cause deadlock if we are in a singlethreaded
			// context (which i think is not used by efsw idk)
			const std::scoped_lock lock{mMutex};
			mInvalidated = true;
		}

		auto ShaderProgramAssetBuilder::shaderSource(Graphics::ShaderType type, std::string source)
			-> ShaderProgramAssetBuilder&
		{
			mShaders.insert_or_assign(type, detail::ShaderSource{std::move(source)});
			return *this;
		}

		auto ShaderProgramAssetBuilder::shaderFile(Graphics::ShaderType type, Path path) -> ShaderProgramAssetBuilder&
		{
			return shaderFile(type, std::make_shared<FileAsset>(std::move(path)));
		}

		auto ShaderProgramAssetBuilder::shaderFile(Graphics::ShaderType type, std::shared_ptr<FileAsset> file)
			-> ShaderProgramAssetBuilder&
		{
			mShaders.insert_or_assign(type, detail::ShaderFile{std::move(file)});
			return *this;
		}

	} // namespace Assets

} // namespace HJUIK
