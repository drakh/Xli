#include <XliGraphics/Parameter.h>

namespace Xli
{
	Parameter* Parameter::GetListItem(int index)
	{
		switch (GetListType())
		{
		case Parameter::TypeFloat:
			{
				FloatParamList* s = (FloatParamList*)this;
				if (s->AnimatedValues.ContainsKey(index)) return Share(s->AnimatedValues.Get(index).Get());
				return new FloatConst(s->Values[index]);
			}

		case Parameter::TypeVector2:
			{
				Vector2ParamList* s = (Vector2ParamList*)this;
				if (s->AnimatedValues.ContainsKey(index)) return Share(s->AnimatedValues.Get(index).Get());
				return new Vector2Const(s->Values[index]);
			}

		case Parameter::TypeVector3:
			{
				Vector3ParamList* s = (Vector3ParamList*)this;
				if (s->AnimatedValues.ContainsKey(index)) return Share(s->AnimatedValues.Get(index).Get());
				return new Vector3Const(s->Values[index]);
			}

		case Parameter::TypeVector4:
			{
				Vector4ParamList* s = (Vector4ParamList*)this;
				if (s->AnimatedValues.ContainsKey(index)) return Share(s->AnimatedValues.Get(index).Get());
				return new Vector4Const(s->Values[index]);
			}

		case Parameter::TypeMatrix4:
			{
				Matrix4ParamList* s = (Matrix4ParamList*)this;
				if (s->AnimatedValues.ContainsKey(index)) return Share(s->AnimatedValues.Get(index).Get());
				return new Matrix4Const(s->Values[index]);
			}

		case Parameter::TypeString:
			{
				StringParamList* s = (StringParamList*)this;
				if (s->AnimatedValues.ContainsKey(index)) return Share(s->AnimatedValues.Get(index).Get());
				return new StringConst(s->Values[index]);
			}

		default:
			XLI_THROW("Unable to get item from unsupported list type: " + (String)(int)GetListType());
		}
	}
}
