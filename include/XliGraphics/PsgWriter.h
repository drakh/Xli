#ifndef __CRUNCH_PSG_WRITER_H__
#define __CRUNCH_PSG_WRITER_H__

#include "Scene.h"
#include <Xli/BinaryWriter.h>
#include <Xli/File.h>

namespace Xli
{
	namespace Psg
	{
		class Writer: public BinaryWriter
		{
			HashMap<Parameter*, int> params;
			int paramCount;

			HashMap<Mesh*, int> meshes;
			int meshCount;

			void writeParameters(Psg::Node* node);
			void writeMeshes(Psg::Node* node);
			void writeNodes(Psg::Node* node);

		public:
			int AddWrittenParam(Parameter* p);
			bool HaveWrittenParam(Parameter* p);
			int GetWrittenParamIndex(Parameter* p);
			void WriteParamIndex(Parameter* p);

			Writer(Stream* stream);

			virtual void WriteParam(FloatParam* param);
			virtual void WriteParam(Vector2Param* param);
			virtual void WriteParam(Vector3Param* param);
			virtual void WriteParam(Vector4Param* param);
			virtual void WriteParam(Matrix4Param* param);
			virtual void WriteParam(StringParam* param);

			virtual void WriteParameter(Parameter* param);
			virtual void WriteMesh(Mesh* mesh);
			
			virtual void WritePsg(Node* root);
			virtual void WritePsm(Mesh* mesh);
		};

		static inline void SavePsg(Stream* f, Psg::Node* scene)
		{
			Writer w(f);
			w.WritePsg(scene);
		}

		static inline void SavePsg(const String& fileName, Psg::Node* scene)
		{
			File f(fileName, FileModeWrite);
			Writer w(&f);
			w.WritePsg(scene);
		}

		static inline void SavePsm(Stream* f, Mesh* mesh)
		{
			Writer w(f);
			w.WritePsm(mesh);
		}

		static inline void SavePsm(const String& fileName, Mesh* mesh)
		{
			File f(fileName, FileModeWrite);
			Writer w(&f);
			w.WritePsm(mesh);
		}
	}
}


#endif
