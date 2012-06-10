#ifndef __XLI_TRIGGER_H__
#define __XLI_TRIGGER_H__

namespace Xli
{
	class Trigger
	{
		bool b;

	public:
		Trigger()
		{
			b = false;
		}
		bool IsTriggered(bool t)
		{
			if (!b && t)
			{
				b = true;
				return true;
			}
			b = t;
			return false;
		}
	};
}

#endif
