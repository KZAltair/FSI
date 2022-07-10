#pragma once

#include "Event.h"

namespace fsicore {

	class FSI_API OcctShowHideEvent : public Event
	{
	public:
		OcctShowHideEvent(bool flag)
			: flag(flag)
		{}

		inline bool GetFlag() const { return flag; }

		EVENT_CLASS_TYPE(AllObjectShowHide)
		EVENT_CLASS_CATEGORY(EventCategoryOcctWindow)
	private:
		bool flag;
	};

	class FSI_API OcctShowHideSignleObjectEvent : public Event
	{
	public:
		OcctShowHideSignleObjectEvent(bool flag)
			: flag(flag)
		{}

		inline bool GetFlag() const { return flag; }

		EVENT_CLASS_TYPE(SingleObjectShowHide)
		EVENT_CLASS_CATEGORY(EventCategoryOcctWindow)
	private:
		bool flag;
	};
}