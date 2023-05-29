#include "Keyboard.hpp"

namespace HJUIK
{
	namespace Window
	{
        // Check real-time state of a key. True if it is being pressed
		bool Keyboard::isPressed(Keyboard::Key key)
		{
			return sf::Keyboard::isKeyPressed(toSFMLKey(key));
		}

        // Get descriptive name of buttons
        std::string Keyboard::getName(Keyboard::Key key)
        {
            static const std::vector<std::string> names =
            {
                // Unknown = -1, No unknown name, it is dealt with later
                "A",  //!< Keyboard a and A key
                "B",            //!< Keyboard b and B key
                "C",            //!< Keyboard c and C key
                "D",            //!< Keyboard d and D key
                "E",            //!< Keyboard e and E key
                "F",            //!< Keyboard f and F key
                "G",            //!< Keyboard g and G key
                "H",            //!< Keyboard h and H key
                "I",            //!< Keyboard i and I key
                "J",            //!< Keyboard j and J key
                "K",            //!< Keyboard k and K key
                "L",            //!< Keyboard l and L key
                "M",            //!< Keyboard m and M key
                "N",            //!< Keyboard n and N key
                "O",            //!< Keyboard o and O key
                "P",            //!< Keyboard p and P key
                "Q",            //!< Keyboard q and Q key
                "R",            //!< Keyboard r and R key
                "S",            //!< Keyboard s and S key
                "T",            //!< Keyboard t and T key
                "U",            //!< Keyboard u and U key
                "V",            //!< Keyboard v and V key
                "W",            //!< Keyboard w and W key
                "X",            //!< Keyboard x and X key
                "Y",            //!< Keyboard y and Y key
                "Z",            //!< Keyboard z and Z key
                "Num 1",         //!< Keyboard 1 and ! key
                "Num 2",         //!< Keyboard 2 and @ key
                "Num 3",         //!< Keyboard 3 and # key
                "Num 4",         //!< Keyboard 4 and $ key
                "Num 5",         //!< Keyboard 5 and % key
                "Num 6",         //!< Keyboard 6 and ^ key
                "Num 7",         //!< Keyboard 7 and & key
                "Num 8",         //!< Keyboard 8 and * key
                "Num 9",         //!< Keyboard 9 and ) key
                "Num 0",         //!< Keyboard 0 and ) key
                "Enter",        //!< Keyboard Enter/Return key
                "Escape",       //!< Keyboard Escape key
                "Backspace",    //!< Keyboard Backspace key
                "Tab",          //!< Keyboard Tab key
                "Space",        //!< Keyboard Space key
                "-",       //!< Keyboard - and _ key
                "=",        //!< Keyboard = and +
                "{",     //!< Keyboard [ and { key
                "}",     //!< Keyboard ] and } key
                // For US keyboards mapped to key 29 (Microsoft Keyboard Scan Code Specification)
                // For Non-US keyboards mapped to key 42 (Microsoft Keyboard Scan Code Specification)
                // Typical language mappings: Belg:£µ` FrCa:<>} Dan:*' Dutch:`´ Fren:µ* Ger:'# Ital:§ù LatAm:[}` Nor:*@ Span:ç} Swed:*' Swiss:$£} UK:~# Brazil:}]
                "\\",      //!< Keyboard \ and | key OR various keys for Non-US keyboards
                ";",      //!< Keyboard ; and : key
                "\'",     //!< Keyboard ' and " key
                "`",          //!< Keyboard ` and ~ key
                ",",          //!< Keyboard , and < key
                ".",         //!< Keyboard . and > key
                "/",          //!< Keyboard / and ? key
                "F1",             //!< Keyboard F1 key
                "F2",             //!< Keyboard F2 key
                "F3",             //!< Keyboard F3 key
                "F4",             //!< Keyboard F4 key
                "F5",             //!< Keyboard F5 key
                "F6",             //!< Keyboard F6 key
                "F7",             //!< Keyboard F7 key
                "F8",             //!< Keyboard F8 key
                "F9",             //!< Keyboard F9 key
                "F10",            //!< Keyboard F10 key
                "F11",            //!< Keyboard F11 key
                "F12",            //!< Keyboard F12 key
                "F13",            //!< Keyboard F13 key
                "F14",            //!< Keyboard F14 key
                "F15",            //!< Keyboard F15 key
                "F16",            //!< Keyboard F16 key
                "F17",            //!< Keyboard F17 key
                "F18",            //!< Keyboard F18 key
                "F19",            //!< Keyboard F19 key
                "F20",            //!< Keyboard F20 key
                "F21",            //!< Keyboard F21 key
                "F22",            //!< Keyboard F22 key
                "F23",            //!< Keyboard F23 key
                "F24",            //!< Keyboard F24 key
                "Caps Lock",       //!< Keyboard Caps %Lock key
                "Print Screen",    //!< Keyboard Print Screen key
                "Scroll Lock",     //!< Keyboard Scroll %Lock key
                "Pause",          //!< Keyboard Pause key
                "Insert",         //!< Keyboard Insert key
                "Home",           //!< Keyboard Home key
                "Page Up",         //!< Keyboard Page Up key
                "Delete",         //!< Keyboard Delete Forward key
                "End",            //!< Keyboard End key
                "Page Down",       //!< Keyboard Page Down key
                "Right",          //!< Keyboard Right Arrow key
                "Left",           //!< Keyboard Left Arrow key
                "Down",           //!< Keyboard Down Arrow key
                "Up",             //!< Keyboard Up Arrow key
                "Num Lock",        //!< Keypad Num %Lock and Clear key
                "Numpad /",   //!< Keypad / key
                "Numpad *", //!< Keypad * key
                "Numpad -",    //!< Keypad - key
                "Numpad +",     //!< Keypad + key
                "Numpad =",    //!< keypad = key
                "Numpad Enter",    //!< Keypad Enter/Return key
                "Numpad Decimal",  //!< Keypad . and Delete key
                "Numpad 1",        //!< Keypad 1 and End key
                "Numpad 2",        //!< Keypad 2 and Down Arrow key
                "Numpad 3",        //!< Keypad 3 and Page Down key
                "Numpad 4",        //!< Keypad 4 and Left Arrow key
                "Numpad 5",        //!< Keypad 5 key
                "Numpad 6",        //!< Keypad 6 and Right Arrow key
                "Numpad 7",        //!< Keypad 7 and Home key
                "Numpad 8",        //!< Keypad 8 and Up Arrow key
                "Numpad 9",        //!< Keypad 9 and Page Up key
                "Numpad 0",        //!< Keypad 0 and Insert key
                // For US keyboards doesn't exist
                // For Non-US keyboards mapped to key 45 (Microsoft Keyboard Scan Code Specification)
                // Typical language mappings: Belg:<\> FrCa:«°» Dan:<\> Dutch:]|[ Fren:<> Ger:<|> Ital:<> LatAm:<> Nor:<> Span:<> Swed:<|> Swiss:<\> UK:\| Brazil: \|.
                "Non-US \\",     //!< Keyboard Non-US \ and | key
                "Application",        //!< Keyboard Application key
                "Execute",            //!< Keyboard Execute key
                "Mode Change",         //!< Keyboard Mode Change key
                "Help",               //!< Keyboard Help key
                "Menu",               //!< Keyboard Menu key
                "Select",             //!< Keyboard Select key
                "Redo",               //!< Keyboard Redo key
                "Undo",               //!< Keyboard Undo key
                "Cut",                //!< Keyboard Cut key
                "Copy",               //!< Keyboard Copy key
                "Paste",              //!< Keyboard Paste key
                "Volume Mute",         //!< Keyboard Volume Mute key
                "Volume Up",           //!< Keyboard Volume Up key
                "Volume Down",         //!< Keyboard Volume Down key
                "Media Play Pause",     //!< Keyboard Media Play Pause key
                "Media Stop",          //!< Keyboard Media Stop key
                "Media Next Track",     //!< Keyboard Media Next Track key
                "Media Previous Track", //!< Keyboard Media Previous Track key
                "Left Control",           //!< Keyboard Left Control key
                "Left Shift",             //!< Keyboard Left Shift key
                "Left Alt",               //!< Keyboard Left Alt key
                "Left System",            //!< Keyboard Left System key
                "Right Control",           //!< Keyboard Right Control key
                "Right Shift",             //!< Keyboard Right Shift key
                "Right Alt",               //!< Keyboard Right Alt key
                "Right System",            //!< Keyboard Right System key
                "Back",               //!< Keyboard Back key
                "Forward",            //!< Keyboard Forward key
                "Refresh",            //!< Keyboard Refresh key
                "Stop",               //!< Keyboard Stop key
                "Search",             //!< Keyboard Search key
                "Favorites",          //!< Keyboard Favorites key
                "Home Page",           //!< Keyboard Home Page key
                "Launch Application 1", //!< Keyboard Launch Application 1 key
                "Launch Application 2", //!< Keyboard Launch Application 2 key
                "Launch Mail",         //!< Keyboard Launch Mail key
                "Launch Media Select",  //!< Keyboard Launch Media Select key
            };

            if (key == Key::Unknown || key == Key::ScancodeCount)
                return "Unknown";
            else
                return names[static_cast<int>(key)];
        }

        // The underlying enum is actually SFML key so we only cast
        sf::Keyboard::Scancode Keyboard::toSFMLKey(Keyboard::Key key)
        {
            return static_cast<sf::Keyboard::Scancode>(static_cast<int>(key));
        }

        // The underlying enum is actually SFML key so we only cast
        Keyboard::Key Keyboard::fromSFMLKey(sf::Keyboard::Scancode key)
        {
            return static_cast<Key>(static_cast<int>(key));
        }
	}
}