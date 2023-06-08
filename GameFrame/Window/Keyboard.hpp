#ifndef GAMEFRAME_WINDOW_KEYBOARD_HPP
#define GAMEFRAME_WINDOW_KEYBOARD_HPP

/*
	Define keyboard keys. Use SFML scancode in SFML/Window/Keyboard.hpp.
*/

#include <SFML/Window.hpp>

namespace HJUIK
{
	namespace Window
	{
		// Static class for querying real-time keyboard information
		class Keyboard
		{
		public:
			enum class Key : int {
				UNKNOWN = -1,  // Represents any scancode not present in this enum
				A		= 0,   // Keyboard a and A key
				B,			   // Keyboard b and B key
				C,			   // Keyboard c and C key
				D,			   // Keyboard d and D key
				E,			   // Keyboard e and E key
				F,			   // Keyboard f and F key
				G,			   // Keyboard g and G key
				H,			   // Keyboard h and H key
				I,			   // Keyboard i and I key
				J,			   // Keyboard j and J key
				K,			   // Keyboard k and K key
				L,			   // Keyboard l and L key
				M,			   // Keyboard m and M key
				N,			   // Keyboard n and N key
				O,			   // Keyboard o and O key
				P,			   // Keyboard p and P key
				Q,			   // Keyboard q and Q key
				R,			   // Keyboard r and R key
				S,			   // Keyboard s and S key
				T,			   // Keyboard t and T key
				U,			   // Keyboard u and U key
				V,			   // Keyboard v and V key
				W,			   // Keyboard w and W key
				X,			   // Keyboard x and X key
				Y,			   // Keyboard y and Y key
				Z,			   // Keyboard z and Z key
				NUM_1,		   // Keyboard 1 and ! key
				NUM_2,		   // Keyboard 2 and @ key
				NUM_3,		   // Keyboard 3 and # key
				NUM_4,		   // Keyboard 4 and $ key
				NUM_5,		   // Keyboard 5 and % key
				NUM_6,		   // Keyboard 6 and ^ key
				NUM_7,		   // Keyboard 7 and & key
				NUM_8,		   // Keyboard 8 and * key
				NUM_9,		   // Keyboard 9 and ) key
				NUM_0,		   // Keyboard 0 and ) key
				ENTER,		   // Keyboard Enter/Return key
				ESCAPE,		   // Keyboard Escape key
				BACKSPACE,	   // Keyboard Backspace key
				TAB,		   // Keyboard Tab key
				SPACE,		   // Keyboard Space key
				HYPHEN,		   // Keyboard - and _ key
				EQUAL,		   // Keyboard = and +
				LEFT_BRACKET,  // Keyboard [ and { key
				RIGHT_BRACKET, // Keyboard ] and } key
				// For US keyboards mapped to key 29 (Microsoft Keyboard Scan Code Specification)
				// For Non-US keyboards mapped to key 42 (Microsoft Keyboard Scan Code Specification)
				// Typical language mappings: Belg:��` FrCa:<>} Dan:*' Dutch:`� Fren:�* Ger:'# Ital:�� LatAm:[}` Nor:*@
				// Span:�} Swed:*' Swiss:$�} UK:~# Brazil:}]
				BACKSLASH,		 // Keyboard \ and | key OR various keys for Non-US keyboards
				SEMICOLON,		 // Keyboard ; and : key
				APOSTROPHE,		 // Keyboard ' and " key
				GRAVE,			 // Keyboard ` and ~ key
				COMMA,			 // Keyboard , and < key
				PERIOD,			 // Keyboard . and > key
				SLASH,			 // Keyboard / and ? key
				F1,				 // Keyboard F1 key
				F2,				 // Keyboard F2 key
				F3,				 // Keyboard F3 key
				F4,				 // Keyboard F4 key
				F5,				 // Keyboard F5 key
				F6,				 // Keyboard F6 key
				F7,				 // Keyboard F7 key
				F8,				 // Keyboard F8 key
				F9,				 // Keyboard F9 key
				F10,			 // Keyboard F10 key
				F11,			 // Keyboard F11 key
				F12,			 // Keyboard F12 key
				F13,			 // Keyboard F13 key
				F14,			 // Keyboard F14 key
				F15,			 // Keyboard F15 key
				F16,			 // Keyboard F16 key
				F17,			 // Keyboard F17 key
				F18,			 // Keyboard F18 key
				F19,			 // Keyboard F19 key
				F20,			 // Keyboard F20 key
				F21,			 // Keyboard F21 key
				F22,			 // Keyboard F22 key
				F23,			 // Keyboard F23 key
				F24,			 // Keyboard F24 key
				CAPS_LOCK,		 // Keyboard Caps %Lock key
				PRINT_SCREEN,	 // Keyboard Print Screen key
				SCROLL_LOCK,	 // Keyboard Scroll %Lock key
				PAUSE,			 // Keyboard Pause key
				INSERT,			 // Keyboard Insert key
				HOME,			 // Keyboard Home key
				PAGE_UP,		 // Keyboard Page Up key
				DELETE,			 // Keyboard Delete Forward key
				END,			 // Keyboard End key
				PAGE_DOWN,		 // Keyboard Page Down key
				RIGHT,			 // Keyboard Right Arrow key
				LEFT,			 // Keyboard Left Arrow key
				DOWN,			 // Keyboard Down Arrow key
				UP,				 // Keyboard Up Arrow key
				NUM_LOCK,		 // Keypad Num %Lock and Clear key
				NUMPAD_DIVIDE,	 // Keypad / key
				NUMPAD_MULTIPLY, // Keypad * key
				NUMPAD_MINUS,	 // Keypad - key
				NUMPAD_PLUS,	 // Keypad + key
				NUMPAD_EQUAL,	 // keypad = key
				NUMPAD_ENTER,	 // Keypad Enter/Return key
				NUMPAD_DECIMAL,	 // Keypad . and Delete key
				NUMPAD_1,		 // Keypad 1 and End key
				NUMPAD_2,		 // Keypad 2 and Down Arrow key
				NUMPAD_3,		 // Keypad 3 and Page Down key
				NUMPAD_4,		 // Keypad 4 and Left Arrow key
				NUMPAD_5,		 // Keypad 5 key
				NUMPAD_6,		 // Keypad 6 and Right Arrow key
				NUMPAD_7,		 // Keypad 7 and Home key
				NUMPAD_8,		 // Keypad 8 and Up Arrow key
				NUMPAD_9,		 // Keypad 9 and Page Up key
				NUMPAD_0,		 // Keypad 0 and Insert key
				// For US keyboards doesn't exist
				// For Non-US keyboards mapped to key 45 (Microsoft Keyboard Scan Code Specification)
				// Typical language mappings: Belg:<\> FrCa:��� Dan:<\> Dutch:]|[ Fren:<> Ger:<|> Ital:<> LatAm:<>
				// Nor:<> Span:<> Swed:<|> Swiss:<\> UK:\| Brazil: \|.
				NON_US_BACKSLASH,	  // Keyboard Non-US \ and | key
				APPLICATION,		  // Keyboard Application key
				EXECUTE,			  // Keyboard Execute key
				MODE_CHANGE,		  // Keyboard Mode Change key
				HELP,				  // Keyboard Help key
				MENU,				  // Keyboard Menu key
				SELECT,				  // Keyboard Select key
				REDO,				  // Keyboard Redo key
				UNDO,				  // Keyboard Undo key
				CUT,				  // Keyboard Cut key
				COPY,				  // Keyboard Copy key
				PASTE,				  // Keyboard Paste key
				VOLUME_MUTE,		  // Keyboard Volume Mute key
				VOLUME_UP,			  // Keyboard Volume Up key
				VOLUME_DOWN,		  // Keyboard Volume Down key
				MEDIA_PLAY_PAUSE,	  // Keyboard Media Play Pause key
				MEDIA_STOP,			  // Keyboard Media Stop key
				MEDIA_NEXT_TRACK,	  // Keyboard Media Next Track key
				MEDIA_PREVIOUS_TRACK, // Keyboard Media Previous Track key
				LEFT_CONTROL,		  // Keyboard Left Control key
				LEFT_SHIFT,			  // Keyboard Left Shift key
				LEFT_ALT,			  // Keyboard Left Alt key
				LEFT_SYSTEM,		  // Keyboard Left System key
				RIGHT_CONTROL,		  // Keyboard Right Control key
				RIGHT_SHIFT,		  // Keyboard Right Shift key
				RIGHT_ALT,			  // Keyboard Right Alt key
				RIGHT_SYSTEM,		  // Keyboard Right System key
				BACK,				  // Keyboard Back key
				FORWARD,			  // Keyboard Forward key
				REFRESH,			  // Keyboard Refresh key
				STOP,				  // Keyboard Stop key
				SEARCH,				  // Keyboard Search key
				FAVORITES,			  // Keyboard Favorites key
				HOME_PAGE,			  // Keyboard Home Page key
				LAUNCH_APPLICATION_1, // Keyboard Launch Application 1 key
				LAUNCH_APPLICATION_2, // Keyboard Launch Application 2 key
				LAUNCH_MAIL,		  // Keyboard Launch Mail key
				LAUNCH_MEDIA_SELECT,  // Keyboard Launch Media Select key

				KEY_COUNT // Keep last -- the total number of scancodes
			};

			// Get the name of the key
			static auto getName(Key key) -> std::string;
			// Return true if key is currently being pressed, and false otherwise
			static auto isPressed(Key key) -> bool;

			// Library specific functions
			static auto toSFMLKey(Key key) -> sf::Keyboard::Scancode;
			static auto fromSFMLKey(sf::Keyboard::Scancode key) -> Key;
		};
	} // namespace Window
} // namespace HJUIK

#endif