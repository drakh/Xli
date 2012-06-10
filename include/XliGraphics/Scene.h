#ifndef __XLI_GRAPHICS_PSG_SCENE_H__
#define __XLI_GRAPHICS_PSG_SCENE_H__

#include "Mesh.h"
#include "Parameter.h"
#include "GeometrySpline.h"

namespace Xli
{
	namespace Psg
	{
		struct Drawable
		{
			HashMap<String, Managed<Parameter> > Params;
			Managed<Mesh> DrawMesh;

			bool HaveParameter(const String& name, Parameter::Type type)
			{
				Managed<Parameter> p;
				if (Params.Get(name, p))
				{
					return p->GetType() == type;
				}
				return false;
			}

			bool HaveParameterList(const String& name, Parameter::Type type)
			{
				Managed<Parameter> p;
				if (Params.Get(name, p))
				{
					return p->GetType() == Parameter::TypeList && p->GetListType() == type;
				}
				return false;
			}

			template <typename T> T* GetParameter(const String& name, Parameter::Type type)
			{
				Parameter* p = Params.Get(name);
				if (p->GetType() != type) XLI_THROW("Parameter has wrong type: " + name);
				return (T*)p;
			}

			template <typename T> T* GetParameterList(const String& name, Parameter::Type type)
			{
				Parameter* p = Params.Get(name);
				if (p->GetType() != Parameter::TypeList || p->GetListType() != type) XLI_THROW("Parameter has wrong type: " + name);
				return (T*)p;
			}

			Matrix4Param* GetMatrix4Param(const String& name)
			{
				return GetParameter<Matrix4Param>(name, Parameter::TypeMatrix4);
			}
			Matrix4ParamList* GetMatrix4List(const String& name)
			{
				return GetParameterList<Matrix4ParamList>(name, Parameter::TypeMatrix4);
			}
		};

		class Node: public Object
		{
		public:
			String Id;
			Array<Managed<Matrix4Param> > Transforms;
			Array<Managed<Node> > Children;
			Array<Managed<GeometrySpline> > Splines;
			Array<Drawable> Drawables;
		};

		enum NodeItemType
		{
			NodeItemTypeEnd = -1,
			NodeItemTypeDrawable = 1,
			NodeItemTypeSpline = 2,
		};

		enum SceneItemType
		{
			SceneItemTypeEnd = -1,
			SceneItemTypeMesh = 100,
		};
	}
}

#endif
