//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_WTEXTBOX_H
#define MK_WTEXTBOX_H

/* mk */
#include <Ui/mkUiForward.h>
#include <Ui/Widget/mkTypeIn.h>

namespace mk
{
	class _I_ MK_UI_EXPORT Textbox : public TypeIn
	{
	public:
		Textbox(WValue& input);
		Textbox(string& text);

		static StyleType& cls() { static StyleType ty("Textbox", TypeIn::cls()); return ty; }

	protected:
		string m_string;
	};
}

#endif
