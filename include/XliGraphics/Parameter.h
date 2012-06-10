#ifndef __XLI_GRAPHICS_PARAMETER_H__
#define __XLI_GRAPHICS_PARAMETER_H__

#include <Xli/Managed.h>
#include <Xli/String.h>
#include <Xli/Vector4.h>
#include <Xli/Matrix4.h>
#include <Xli/Spline.h>
#include <Xli/List.h>
#include <Xli/HashMap.h>

namespace Xli
{
	enum ParamType
	{
		ParamTypeUnknown = 0,
		ParamTypeConstant = 1,
		ParamTypeTranslation = 2,
		ParamTypeRotation = 3,
		ParamTypeScaling = 4,
		ParamTypeSkew = 5,
		ParamTypeTransformNode = 6,
		ParamTypeList = 7,
		ParamTypeSpline = 8,
		ParamTypeAnimationSampled = 9,
	};

	class Parameter: public Object
	{
	public:
		enum Type
		{
			TypeUnknown = 0,
			TypeFloat = 1,
			TypeVector2 = 2,
			TypeVector3 = 3,
			TypeVector4 = 4,
			TypeMatrix4 = 5,
			TypeString = 6,
			TypeList = 7,
		};

		virtual Type GetType() = 0;
		virtual ParamType GetParamType() = 0;
		
		virtual Type GetListType() { return TypeUnknown; }
		virtual int GetListLength() { return 0; }
		
		virtual bool IsAnimated() { return false; }
		virtual int GetAnimatedKeyCount() { return -1; }

		/**
			Updates the value for this parameter.
			@return true if the value has changed or false if it is unchanged.
		*/
		virtual bool Update(double time) { return false; }

		/**
			Resets the dirtyflag. Should be called at end of frame.
		*/
		virtual void Validate() {}

		virtual double StartTime() { return 0.0; }
		virtual double EndTime() { return 0.0; }

		virtual int GetFloatCount() = 0;
		virtual void GetFloatValues(float* result) = 0;

		virtual String GetStringValue() = 0;

		/**
			Returns an item from a list parameter.
			Note: The returned pointer must be released by caller.
		*/
		Parameter* GetListItem(int index);
	};

	template <typename T, Parameter::Type TType> class ParameterT: public Parameter
	{
	public:
		virtual T GetValue() = 0;
		virtual Type GetType() { return TType; }

		virtual int GetFloatCount();
		virtual void GetFloatValues(float* result);

		virtual String GetStringValue();
	};

	template<> inline int ParameterT<float, Parameter::TypeFloat>::GetFloatCount() { return 1; }
	template<> inline int ParameterT<Vector2, Parameter::TypeVector2>::GetFloatCount() { return 2; }
	template<> inline int ParameterT<Vector3, Parameter::TypeVector3>::GetFloatCount() { return 3; }
	template<> inline int ParameterT<Vector4, Parameter::TypeVector4>::GetFloatCount() { return 4; }
	template<> inline int ParameterT<Matrix4, Parameter::TypeMatrix4>::GetFloatCount() { return 16; }
	template <typename T, Parameter::Type TType> int ParameterT<T, TType>::GetFloatCount() { return 0; }

	template<> inline void ParameterT<float, Parameter::TypeFloat>::GetFloatValues(float* result) { *(float*)result = GetValue(); }
	template<> inline void ParameterT<Vector2, Parameter::TypeVector2>::GetFloatValues(float* result) { *(Vector2*)result = GetValue(); }
	template<> inline void ParameterT<Vector3, Parameter::TypeVector3>::GetFloatValues(float* result) { *(Vector3*)result = GetValue(); }
	template<> inline void ParameterT<Vector4, Parameter::TypeVector4>::GetFloatValues(float* result) { *(Vector4*)result = GetValue(); }
	template<> inline void ParameterT<Matrix4, Parameter::TypeMatrix4>::GetFloatValues(float* result) { *(Matrix4*)result = GetValue(); }
	template <typename T, Parameter::Type TType> void ParameterT<T, TType>::GetFloatValues(float* result) { XLI_THROW("Parameter value is not float values"); }

	template<> inline String ParameterT<String, Parameter::TypeString>::GetStringValue() { return GetValue(); }
	template <typename T, Parameter::Type TType> String ParameterT<T, TType>::GetStringValue() { XLI_THROW("Parameter value is not a string value"); }

	typedef ParameterT<float, Parameter::TypeFloat> FloatParam;
	typedef ParameterT<Vector2, Parameter::TypeVector2> Vector2Param;
	typedef ParameterT<Vector3, Parameter::TypeVector3> Vector3Param;
	typedef ParameterT<Vector4, Parameter::TypeVector4> Vector4Param;
	typedef ParameterT<Matrix4, Parameter::TypeMatrix4> Matrix4Param;
	typedef ParameterT<String, Parameter::TypeString> StringParam;


	template <typename T, Parameter::Type TType> class Constant: public ParameterT<T, TType>
	{
	public:
		T Value;

		Constant() {}
		Constant(const T& value): Value(value) {}

		virtual T GetValue() { return Value; }
		virtual ParamType GetParamType() { return ParamTypeConstant; }
	};

	typedef Constant<float, Parameter::TypeFloat> FloatConst;
	typedef Constant<Vector2, Parameter::TypeVector2> Vector2Const;
	typedef Constant<Vector3, Parameter::TypeVector3> Vector3Const;
	typedef Constant<Vector4, Parameter::TypeVector4> Vector4Const;
	typedef Constant<Matrix4, Parameter::TypeMatrix4> Matrix4Const;
	typedef Constant<String, Parameter::TypeString> StringConst;



	class Translation: public Matrix4Param
	{
	public:
		Managed<Vector3Param> Value;

		Translation() {}
		Translation(Vector3Param* value): Value(value) {}

		virtual ParamType GetParamType()
		{
			return ParamTypeTranslation;
		}
		virtual Matrix4 GetValue()
		{
			return Matrix4::Translation(Value->GetValue());
		}
		virtual bool IsAnimated()
		{
			return Value->IsAnimated();
		}
		virtual bool Update(double time)
		{
			return Value->Update(time);
		}
		virtual void Validate()
		{
			Value->Validate();
		}
		virtual double StartTime()
		{
			return Value->StartTime();
		}
		virtual double EndTime()
		{
			return Value->EndTime();
		}
	};

	class Rotation: public Matrix4Param
	{
	public:
		Managed<Vector3Param> Axis;
		Managed<FloatParam> Angle;

		Rotation() {}
		Rotation(Vector3Param* axis, FloatParam* angle): Axis(axis), Angle(angle) {}

		virtual ParamType GetParamType()
		{
			return ParamTypeRotation;
		}
		virtual Matrix4 GetValue()
		{
			return Matrix4::Rotation(Axis->GetValue(), Angle->GetValue());
		}
		virtual bool IsAnimated()
		{
			return Axis->IsAnimated() || Angle->IsAnimated();
		}
		virtual bool Update(double time)
		{
			bool axisDirty = Axis->Update(time);
			bool angleDirty = Angle->Update(time);
			return axisDirty || angleDirty;
		}
		virtual void Validate()
		{
			Axis->Validate();
			Angle->Validate();
		}
		virtual double StartTime()
		{
			return Min(Axis->StartTime(), Angle->StartTime());
		}
		virtual double EndTime()
		{
			return Max(Axis->EndTime(), Angle->EndTime());
		}
	};

	class Scaling: public Matrix4Param
	{
	public:
		Managed<Vector3Param> Value;

		Scaling() {}
		Scaling(Vector3Param* value): Value(value) {}

		virtual ParamType GetParamType()
		{
			return ParamTypeScaling;
		}
		virtual Matrix4 GetValue()
		{
			return Matrix4::Scaling(Value->GetValue());
		}
		virtual bool IsAnimated()
		{
			return Value->IsAnimated();
		}
		virtual bool Update(double time)
		{
			return Value->Update(time);
		}
		virtual void Validate()
		{
			Value->Validate();
		}
		virtual double StartTime()
		{
			return Value->StartTime();
		}
		virtual double EndTime()
		{
			return Value->EndTime();
		}
	};

	class TransformNode: public Matrix4Param
	{
	public:
		Managed<Matrix4Param> Parent;
		Managed<Matrix4Param> Local;
		Matrix4 Value;
		double Time;
		bool Dirty;

		TransformNode(Matrix4Param* parent, Matrix4Param* local)
		{
			this->Parent = parent;
			this->Local = local;
			Value = parent->GetValue() * local->GetValue();
			Time = 0.0;
			Dirty = false;
		}
		virtual ParamType GetParamType()
		{
			return ParamTypeTransformNode;
		}
		virtual Matrix4 GetValue()
		{
			return Value;
		}
		virtual bool IsAnimated()
		{
			return Parent->IsAnimated() || Local->IsAnimated();
		}
		virtual bool Update(double time)
		{
			if (Time != time)
			{
				Time = time;
				bool parentDirty = Parent->Update(time);
				bool localDirty = Local->Update(time);

				if (parentDirty || localDirty)
				{
					Value = Parent->GetValue() * Local->GetValue();
					Dirty = true;
				}
			}

			return Dirty;
		}
		virtual void Validate()
		{
			Dirty = false;
		}
		virtual double StartTime()
		{
			return Min(Parent->StartTime(), Local->StartTime());
		}
		virtual double EndTime()
		{
			return Max(Parent->EndTime(), Local->EndTime());
		}
	};



	enum SplineType
	{
		SplineTypeUnknown = 0,
		SplineTypeLinear = 1,
		SplineTypeBezier = 2,
		SplineTypeNurbs = 3,
	};

	template <typename T, Parameter::Type TType> class SplineParameter: public ParameterT<T, TType>
	{
	public:
		virtual ParamType GetParamType()
		{
			return ParamTypeSpline;
		}

		virtual SplineType GetSplineType() = 0;
		virtual Spline<double, T>* GetSpline() = 0;


		T Value;
		double Time;
		bool Dirty;

		SplineParameter(Valid<Spline<double, T> > spline)
		{
			Value = spline->First();
			Time = spline->Start();
			Dirty = false;
		}

		virtual T GetValue() { return Value; }
		virtual bool IsAnimated() { return true; }
		virtual double StartTime() { return GetSpline()->Start(); }
		virtual double EndTime() { return GetSpline()->End(); }

		virtual bool Update(double time)
		{
			if (time != Time)
			{
				Value = GetSpline()->Sample(time);
				Time = time;
				Dirty = true;
			}
			return Dirty;
		}
		virtual void Validate()
		{
			Dirty = false;
		}
	};

	typedef SplineParameter<float, Parameter::TypeFloat> FloatSpline;
	typedef SplineParameter<Vector2, Parameter::TypeVector2> Vector2Spline;
	typedef SplineParameter<Vector3, Parameter::TypeVector3> Vector3Spline;
	typedef SplineParameter<Vector4, Parameter::TypeVector4> Vector4Spline;
	typedef SplineParameter<Matrix4, Parameter::TypeMatrix4> Matrix4Spline;

	template <typename T, Parameter::Type TType> class LinearSplineParameter: public SplineParameter<T, TType>
	{
	public:
		Local<LinearSpline<double, T> > Data;
		virtual bool IsAnimated() { return Data->GetKeyCount() > 1; }
		virtual SplineType GetSplineType() { return SplineTypeLinear; }
		virtual Spline<double, T>* GetSpline() { return Data; }
		virtual int GetAnimatedKeyCount() { return Data->GetKeyCount(); }
		LinearSplineParameter(LinearSpline<double, T>* data): SplineParameter<T, TType>(data), Data(data) {}
	};

	typedef LinearSplineParameter<float, Parameter::TypeFloat> FloatLinearSpline;
	typedef LinearSplineParameter<Vector2, Parameter::TypeVector2> Vector2LinearSpline;
	typedef LinearSplineParameter<Vector3, Parameter::TypeVector3> Vector3LinearSpline;
	typedef LinearSplineParameter<Vector4, Parameter::TypeVector4> Vector4LinearSpline;
	typedef LinearSplineParameter<Matrix4, Parameter::TypeMatrix4> Matrix4LinearSpline;

	template <typename T, Parameter::Type TType> class SampledAnimation: public ParameterT<T, TType>
	{
	public:
		Array<T> Values;
		double Start;
		double Fps;
		int Index;

		virtual ParamType GetParamType()
		{
			return ParamTypeAnimationSampled;
		}

		SampledAnimation()
		{
			Index = 0;
		}
		virtual T GetValue()
		{
			return Values[Index];
		}
		virtual bool IsAnimated()
		{
			return Values.Length() > 1;
		}
		virtual bool Update(double time)
		{
			Index = Clamp((int)((time - Start) * Fps), 0, Values.Length() - 1);
			return true;
		}
		virtual double StartTime()
		{
			return Start;
		}
		virtual double EndTime()
		{
			return Start + (double)(Values.Length()) / Fps;
		}
		virtual int GetAnimatedKeyCount()
		{
			return Values.Length();
		}
	};

	typedef SampledAnimation<float, Parameter::TypeFloat> FloatSampledAnim;
	typedef SampledAnimation<Vector2, Parameter::TypeVector2> Vector2SampledAnim;
	typedef SampledAnimation<Vector3, Parameter::TypeVector3> Vector3SampledAnim;
	typedef SampledAnimation<Vector4, Parameter::TypeVector4> Vector4SampledAnim;
	typedef SampledAnimation<Matrix4, Parameter::TypeMatrix4> Matrix4SampledAnim;

	template <typename T, Parameter::Type TType> class ListParameter: public ParameterT<List<T>, Parameter::TypeList>
	{
	public:
		List<T> Values;
		HashMap<int, Managed<ParameterT<T, TType> > > AnimatedValues;

		void AddValue(const T& value)
		{
			Values.Add(value);
		}
		void AddValue(Managed<ParameterT<T, TType> > value)
		{
			int i = Values.Add(value->GetValue());
			if (value->IsAnimated()) AnimatedValues.Add(i, value);
		}

		virtual ParamType GetParamType()
		{
			return ParamTypeList;
		}
		virtual Parameter::Type GetListType()
		{
			return TType;
		}
		virtual int GetListLength()
		{
			return Values.Length();
		}
		virtual List<T> GetValue()
		{
			return Values;
		}
		virtual bool IsAnimated()
		{
			return AnimatedValues.Count() > 0;
		}
		virtual bool Update(double time)
		{
			bool dirty = false;
			for (int it = AnimatedValues.Begin(); it != AnimatedValues.End(); it = AnimatedValues.Next(it))
			{
				int i = AnimatedValues.GetKey(it);
				ParameterT<T, TType>* p = AnimatedValues.GetValue(it).Get();
				if (p->Update(time))
				{
					Values[i] = p->GetValue();
					dirty = true;
				}
			}
			return dirty;
		}
		virtual void Validate()
		{
			for (int it = AnimatedValues.Begin(); it != AnimatedValues.End(); it = AnimatedValues.Next(it))
			{
				ParameterT<T, TType>* p = AnimatedValues.GetValue(it).Get();
				p->Validate();
			}
		}
		virtual double StartTime()
		{
			double startTime = DoubleMax;

			for (int it = AnimatedValues.Begin(); it != AnimatedValues.End(); it = AnimatedValues.Next(it))
			{
				startTime = Min(startTime, AnimatedValues.GetValue(it)->StartTime());
			}

			return startTime;
		}
		virtual double EndTime()
		{
			double endTime = DoubleMin;

			for (int it = AnimatedValues.Begin(); it != AnimatedValues.End(); it = AnimatedValues.Next(it))
			{
				endTime = Max(endTime, AnimatedValues.GetValue(it)->EndTime());
			}

			return endTime;
		}
	};

	typedef ListParameter<float, Parameter::TypeFloat> FloatParamList;
	typedef ListParameter<Vector2, Parameter::TypeVector2> Vector2ParamList;
	typedef ListParameter<Vector3, Parameter::TypeVector3> Vector3ParamList;
	typedef ListParameter<Vector4, Parameter::TypeVector4> Vector4ParamList;
	typedef ListParameter<Matrix4, Parameter::TypeMatrix4> Matrix4ParamList;
	typedef ListParameter<String, Parameter::TypeString> StringParamList;
}

#endif
