//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_WCONTEXTMENU_H
#define MK_WCONTEXTMENU_H

/* mk */
#include <Ui/mkUiForward.h>
#include <Ui/Widget/mkSheet.h>

namespace mk
{
	class MK_UI_EXPORT ContextMenu : public Sheet
	{
	public:
		ContextMenu(Trigger onClose);
		~ContextMenu();

		virtual void bind(Sheet& parent, size_t index);

		void clear();

		virtual void leftClick(MouseEvent& mouseEvent);
		virtual void rightClick(MouseEvent& mouseEvent);
		virtual void middleClick(MouseEvent& mouseEvent);

		static StyleType& cls() { static StyleType ty("ContextMenu", Sheet::cls()); return ty; }

	protected:
		Trigger m_onClose;
	};
}

#endif
