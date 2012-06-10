#ifndef __XLI_GRAPHICS_PSG_READER_H__
#define __XLI_GRAPHICS_PSG_READER_H__

#include "Scene.h"
#include <Xli/BinaryReader.h>
#include <Xli/File.h>

namespace Xli
{
	namespace Psg
	{
		class Reader: public BinaryReader
		{
			Array<Managed<Parameter> > params;
			Array<Managed<Mesh> > meshes;
			Psg::Node* readNode();

		public:
			Reader(Stream* stream);

			template <typename T> void ReadKeyframedSpline(KeyframedSpline<double, T>* spline)
			{
				int count = ReadInt32();
				while (count-- > 0)
				{
					double t;
					T v;
					Read(t);
					Read(v);
					spline->AddKey(t, v);
				}
			}

			Parameter* ReadParameterIndex(Parameter::Type type);
			FloatParam* ReadFloatParamIndex() { return (FloatParam*)ReadParameterIndex(Parameter::TypeFloat); }
			Vector2Param* ReadVector2ParamIndex() { return (Vector2Param*)ReadParameterIndex(Parameter::TypeVector2); }
			Vector3Param* ReadVector3ParamIndex() { return (Vector3Param*)ReadParameterIndex(Parameter::TypeVector3); }
			Vector4Param* ReadVector4ParamIndex() { return (Vector4Param*)ReadParameterIndex(Parameter::TypeVector4); }
			Matrix4Param* ReadMatrix4ParamIndex() { return (Matrix4Param*)ReadParameterIndex(Parameter::TypeMatrix4); }

			virtual Parameter* ReadParameter(ParamType paramType);
			virtual Matrix4Param* ReadTransform();
			virtual Mesh* ReadMesh();

			virtual Node* ReadPsg();
			virtual Mesh* ReadPsm();
		};

		static inline Node* LoadPsg(Stream* f)
		{
			return Reader(f).ReadPsg();
		}

		static inline Node* LoadPsg(const String& fileName)
		{
			File f(fileName, FileModeWrite);
			return Reader(&f).ReadPsg();
		}

		static inline Mesh* LoadPsm(Stream* f)
		{
			return Reader(f).ReadPsm();
		}

		static inline Mesh* LoadPsm(const String& fileName)
		{
			File f(fileName, FileModeRead);
			return Reader(&f).ReadPsm();
		}
	}
}


#endif
