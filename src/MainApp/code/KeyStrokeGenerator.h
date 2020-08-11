#ifndef KEY_STROKE_GENERATOR
#define KEY_STROKE_GENERATOR


#include <map>
#include <QString>
using uint = unsigned int;

// file established using this data: https://msdn.microsoft.com/fr-fr/library/windows/desktop/dd375731(v=vs.85).aspx
// or search "virtual key code" on microsoft msdn
// but here, only the codes that are fully compatible with hardware scan codes are listed


class KeyStrokeGenerator
{
	public:
		KeyStrokeGenerator();
		KeyStrokeGenerator(const KeyStrokeGenerator &other) = delete;
		KeyStrokeGenerator(KeyStrokeGenerator &&other) = delete;
		KeyStrokeGenerator& operator=(const KeyStrokeGenerator &other) = delete;
		KeyStrokeGenerator& operator=(KeyStrokeGenerator &&other) = delete;
		~KeyStrokeGenerator() = default;
		
		bool generateKeyStroke(const QString &keyStr, bool bDown);
		bool generateKeyStroke(uint keycode, bool bDown);
		
		
	private:
		uint keystr2keycode(const QString &keyStr);
		
		std::map<QString,uint> m_keystr2keycode_data;
};


#endif

