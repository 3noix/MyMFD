#ifndef REMOTE_CONTROLLER
#define REMOTE_CONTROLLER


#include <QString>
#include <array>
using uint = unsigned int;


class RemoteController
{
	public:
		RemoteController(const QString &name);
		RemoteController(const RemoteController &other) = delete;
		RemoteController(RemoteController &&other) = delete;
		RemoteController& operator=(const RemoteController &other) = delete;
		RemoteController& operator=(RemoteController &&other) = delete;
		~RemoteController() = default;
		
		QString url() const;
		
		uint buttonsCount() const;
		bool getButton(uint button) const;
		bool setButton(uint button, bool bPressed);
		
		uint axesCount() const;
		float getAxis(uint axis) const;
		bool setAxis(uint axis, float value);
		
		uint povsCount() const;
		float getPov(uint pov) const;
		bool setPov(uint pov, float value);
		
		
	private:
		friend class UjpsHttpServer;
		
		QString m_name;
		
		uint m_nbButtons;
		uint m_nbAxes;
		uint m_nbPovs;
		std::array<bool,128> m_buttons;
		std::array<float,8> m_axes;
		std::array<float,4> m_povs;
};


#endif

