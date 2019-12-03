/*
 *	event.hpp
 *	Event
 *	Constructors in header for cleanliness,
 *		even though it breaks standard.
 */

#ifndef __ENGINE_EVENT_HPP
#define __ENGINE_EVENT_HPP

namespace Engine
{
	/*class Event
	{
	public:
		typedef enum _EventType
		{
			FocusChange,
			Key,
			MouseButton,
			MousePosition,
			MouseScroll
		} EventType;

	protected:
		// TODO Instead of event type, I'm pretty sure I can dynamic cast these
		EventType eventType;

		explicit Event(EventType eventType)
			: eventType(eventType)
		{

		}

	public:
		virtual ~Event()
		{
			
		}

		EventType GetEventType() const
		{
			return eventType;
		}
	};

	class EventFocusChanged : public Event
	{
	public:
		bool		focused;

	public:
		EventFocusChanged(bool focused)
			: Event(FocusChange),
			  focused(focused)
		{

		}
	};

	class EventKey : public Event
	{
	public:
		int			keyCode;
		int			action;
		int			mods;

	public:
		EventKey(int keyCode, int action, int mods)
			: Event(Key),
			  keyCode(keyCode),
			  action(action),
			  mods(mods)
		{

		}
	};
	
	class EventMouseButton : public Event
	{
	public:
		int			button;
		int			action;
		int			x;
		int			y;

	public:
		EventMouseButton(int button, int action, int x, int y)
			: Event(MouseButton),
			  button(button),
			  action(action),
			  x(x),
			  y(y)
		{

		}
	};

	class EventMousePosition : public Event
	{
	public:
		int			x;
		int			y;

	public:
		EventMousePosition(int x, int y)
			: Event(MousePosition),
			  x(x),
			  y(y)
		{

		}
	};

	class EventMouseScroll : public Event
	{
	public:
		double		x;
		double		y;

	public:
		EventMouseScroll(double x, double y)
			: Event(MouseScroll),
			  x(x),
			  y(y)
		{

		}
	};*/

	class Event
	{
	public:
		typedef enum _EventType
		{
			FocusChange,
			Key,
			MouseButton,
			MousePosition,
			MouseScroll
		} EventType;

	public:
		EventType type;

		union
		{
			struct
			{
				bool	focused;
			} focusChanged;

			struct
			{
				int		keyCode;
				int		action;
				int		mods;
			} key;

			struct
			{
				int		button;
				int		action;
				int		x;
				int		y;
			} mouseButton;

			struct
			{
				int		x;
				int		y;
			} mousePosition;

			struct
			{
				double	x;
				double	y;
			} mouseScroll;
		};
	};
}

#endif
