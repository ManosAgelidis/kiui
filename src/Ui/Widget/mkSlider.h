//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#ifndef MK_WSLIDER_H
#define MK_WSLIDER_H

/* mk */
#include <Object/Util/mkStat.h>
#include <Object/String/mkStringConvert.h>
#include <Ui/mkUiForward.h>
#include <Ui/Frame/mkUibox.h>
#include <Ui/Widget/mkValue.h>
#include <Ui/Widget/mkWidget.h>
#include <Ui/Widget/mkButton.h>
#include <Ui/Widget/mkTypeIn.h>

namespace mk
{
	class MK_UI_EXPORT SliderKnob : public Button
	{
	public:
		SliderKnob(Dimension dim = DIM_X, StyleType& type = cls());

		float offset(float pos);

		void leftDragStart(MouseEvent& mouseEvent);
		void leftDrag(MouseEvent& mouseEvent);
		void leftDragEnd(MouseEvent& mouseEvent);

		static StyleType& cls() { static StyleType ty("SliderKnob", Button::cls()); return ty; }

	protected:
		Dimension m_dim;
		float m_startPos;
		float m_startOffset;
	};

	class MK_UI_EXPORT SpacerX : public Widget
	{
	public:
		SpacerX();

		static StyleType& cls() { static StyleType ty("SpacerX", Widget::cls()); return ty; }
	};

	class MK_UI_EXPORT SpacerY : public Widget
	{
	public:
		SpacerY();

		static StyleType& cls() { static StyleType ty("SpacerY", Widget::cls()); return ty; }
	};

	class MK_UI_EXPORT SliderKnobX : public SliderKnob
	{
	public:
		SliderKnobX();

		static StyleType& cls() { static StyleType ty("SliderKnobX", SliderKnob::cls()); return ty; }
	};

	class MK_UI_EXPORT SliderKnobY : public SliderKnob
	{
	public:
		SliderKnobY();

		static StyleType& cls() { static StyleType ty("SliderKnobY", SliderKnob::cls()); return ty; }
	};

	class MK_UI_EXPORT Slider : public Sheet
	{
	public:
		Slider(Dimension dim = DIM_X, const Trigger& onUpdated = nullptr, StyleType& type = cls());

		Widget& filler() { return m_filler; }
		SliderKnob& slider() { return m_button; }

		float val() { return m_val; }

		float length();

		void nextFrame(size_t tick, size_t delta);

		void updateMetrics(float min, float max, float val, float stepLength, float knobLength = 0.f);
		void resetMetrics(float min, float max, float val, float stepLength, float knobLength = 0.f);

		void offsetChange(float offset, bool ended);
		
		void updateKnob();

		virtual void sliderStep(float value, bool ended) { UNUSED(value); UNUSED(ended); m_onUpdated(*this); }

		static StyleType& cls() { static StyleType ty("Slider", Sheet::cls()); return ty; }

	protected:
		Dimension m_dim;
		Widget& m_filler;
		SliderKnob& m_button;

		float m_min;
		float m_max;
		float m_val;
		float m_stepLength;
		float m_knobLength;

		float m_numSteps;
		int m_step;

		Trigger m_onUpdated;
	};

	class MK_UI_EXPORT SliderX : public Slider
	{
	public:
		SliderX(const Trigger& onUpdated = Trigger());

		static StyleType& cls() { static StyleType ty("SliderX", Slider::cls()); return ty; }
	};

	class MK_UI_EXPORT SliderY : public Slider
	{
	public:
		SliderY(const Trigger& onUpdated = Trigger());

		static StyleType& cls() { static StyleType ty("SliderY", Slider::cls()); return ty; }
	};

	class MK_UI_EXPORT SliderDisplay : public Label
	{
	public:
		SliderDisplay(const string& label);

		static StyleType& cls() { static StyleType ty("SliderDisplay", Label::cls()); return ty; }
	};

	template <class T>
	class StatSlider : public WValue
	{
	public:
		StatSlider(Lref& lref, std::function<void(T)> callback = nullptr, Dimension dim = DIM_X)
			: WValue(lref, this->cls(), callback ? [callback](Lref& lref) { callback(lref->get<AutoStat<T>>()); } : OnUpdate())
			, m_stat(this->m_value->template ref<AutoStat<T>>())
			, m_dim(dim)
			, m_display(this->template makeappend<SliderDisplay>(toString(m_stat.value())))
			, m_slider(m_dim == DIM_X ? (Slider&) this->template makeappend<SliderX>(std::bind(&StatSlider::updateStat, this))
									: (Slider&) this->template makeappend<SliderY>(std::bind(&StatSlider::updateStat, this)))
		{
			this->updateSlider();
		}

		StatSlider(AutoStat<T> value, std::function<void(T)> callback = nullptr, Dimension dim = DIM_X)
			: WValue(lref(value), this->cls(), callback ? [callback](Lref& lref) { callback(lref->get<AutoStat<T>>()); } : OnUpdate())
			, m_stat(this->m_value->template ref<AutoStat<T>>())
			, m_dim(dim)
			, m_display(this->template makeappend<SliderDisplay>(toString(m_stat.value())))
			, m_slider(m_dim == DIM_X ? (Slider&) this->template makeappend<SliderX>(std::bind(&StatSlider::updateStat, this))
									: (Slider&) this->template makeappend<SliderY>(std::bind(&StatSlider::updateStat, this)))
		{
			this->updateSlider();
		}

		void updateSlider()
		{
			m_slider.resetMetrics(float(m_stat.min()), float(m_stat.max()), float(m_stat.value()), float(m_stat.step()));
			m_slider.updateKnob();
			m_display.setLabel(toString(m_stat.value()));
		}

		void updateStat()
		{
			m_stat.modify(T(m_slider.val()));
			m_display.setLabel(toString(m_stat.value()));
			this->triggerModify();
		}

		void updateValue(T val) { m_stat.modify(val); this->triggerUpdate(); }
		void modifyValue(T val) { m_stat.modify(val); this->triggerModify(); }

		void notifyUpdate() { this->updateSlider(); }
		void notifyModify() { this->updateSlider(); }

		static StyleType& cls() { static StyleType ty("StatSlider<" + typecls<T>().name() + ">", WValue::cls()); return ty; }

	protected:
		AutoStat<T>& m_stat;

		Dimension m_dim;

		Slider& m_slider;
		SliderDisplay& m_display;
	};

	template class _I_ MK_UI_EXPORT StatSlider<float>;
	template class _I_ MK_UI_EXPORT StatSlider<int>;
}

#endif
