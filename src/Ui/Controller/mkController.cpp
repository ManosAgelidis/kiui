//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <Ui/mkUiConfig.h>
#include <Ui/Controller/mkController.h>

#include <Ui/Widget/mkWidget.h>
#include <Ui/Widget/mkRootSheet.h>

namespace mk
{
	KeyInputFrame::KeyInputFrame()
		: InputWidget()
	{}

	void KeyInputFrame::keyUp(KeyEvent& keyEvent)
	{
		auto it = keyUpHandlers.find(keyEvent.code);
		if(it != keyUpHandlers.end())
		{
			(*it).second();
			keyEvent.consumed = true;
		}
	}

	void KeyInputFrame::keyDown(KeyEvent& keyEvent)
	{
		auto it = keyDownHandlers.find(keyEvent.code);
		if(it != keyDownHandlers.end())
		{
			(*it).second();
			keyEvent.consumed = true;
		}
	}

	Controller::Controller(ControlMode controlMode)
		: KeyInputFrame()
		, m_controlMode(controlMode)
		, m_inputWidget(nullptr)
	{}

	void Controller::takeControl(Widget& inputWidget)
	{
		m_inputWidget = &inputWidget;
		InputFrame::m_controlMode = m_controlMode;
		InputFrame::takeControl(inputWidget);
	}

	void Controller::yieldControl()
	{
		m_inputWidget = nullptr;
		InputFrame::yieldControl();
	}

}
