#ifndef GAMEFRAME_WINDOW_SFMLWINDOW_HPP
#define GAMEFRAME_WINDOW_SFMLWINDOW_HPP

/*
	The derived window class with SFML implementation
*/

#include <memory>

#include "../Graphics/IOpenGLContext.hpp"
#include "../Utilize/GLMTypedef.hpp"
#include "IWindow.hpp"
#include "SFML/Window.hpp"

namespace HJUIK
{
	namespace Window
	{
		// A SFML window capable of rendering and receiving input
		class SFMLWindow : public IWindow
		{
		public:
			// Window construction parameters
			struct Settings {
				Vector2u Size;
				std::string Title;
				WindowType Style;
				unsigned int OpenGLMajorVersion = 3;
				unsigned int OpenGLMinorVersion = 3;
			};

			SFMLWindow(Vector2u size, std::string title, WindowType style = WindowType::DEFAULT);
			explicit SFMLWindow(const Settings& settings);
			// Check if the window is still opened and available
			auto isOpen() const -> bool override;
			// Get size of the window in pixels
			auto getSize() const -> Vector2u override;
			// Close the window
			auto close() -> void override;
			// This function polls the event from the queue and returns false if
			// nothing is found
			auto pollEvent(Event& event) -> bool override;
			// Registers key pressed event only once after it is pressed if set
			// to false, otherwise registers the event every frame it is held
			auto setKeyRepeatable(bool repeatable) -> void override;
			// Try to match the framerate to FPS value, note that this function
			// is not precise
			auto limitFrameRate(unsigned int FPS) -> void override;
			// Get the OpenGL context that corresponds to this window
			auto getOpenGLContext() -> Graphics::IOpenGLContext& override;

		private:
			// Helper functions
			static auto createWindow(const Settings& settings) -> std::unique_ptr<sf::Window>;
			class SFMLOpenGLContext : public Graphics::IOpenGLContext
			{
			public:
				explicit SFMLOpenGLContext(sf::Window& window);
				~SFMLOpenGLContext() override;
				SFMLOpenGLContext(const SFMLOpenGLContext&)					   = delete;
				SFMLOpenGLContext(SFMLOpenGLContext&&)						   = delete;
				auto operator=(const SFMLOpenGLContext&) -> SFMLOpenGLContext& = delete;
				auto operator=(SFMLOpenGLContext&&) -> SFMLOpenGLContext&	   = delete;

				auto display() -> void override;

			private:
				sf::Window& mWindow;
			};

			std::unique_ptr<sf::Window> mWindow;
			SFMLOpenGLContext mOpenGLContext;
		};
	} // namespace Window
} // namespace HJUIK

#endif
