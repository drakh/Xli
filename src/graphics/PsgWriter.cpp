#include <XliGraphics/PsgWriter.h>
#include <Xli/Console.h>

namespace Xli
{
	namespace Psg
	{
		int Writer::AddWrittenParam(Parameter* p)
		{
			params.Add(p, paramCount);
			return paramCount++;
		}
		bool Writer::HaveWrittenParam(Parameter* p)
		{
			return params.ContainsKey(p);
		}
		int Writer::GetWrittenParamIndex(Parameter* p)
		{
			return params.Get(p);
		}
		void Writer::WriteParamIndex(Parameter* p)
		{
			WriteInt32(GetWrittenParamIndex(p));
		}

		Writer::Writer(Stream* stream): BinaryWriter(stream)
		{
			paramCount = 0;
			meshCount = 0;
		}
		
		template <typename T> static void WriteKeyframedSpline(Writer* w, KeyframedSpline<double, T>* spline)
		{
			w->WriteInt32(spline->GetKeyCount());
			for (int i = 0; i < spline->GetKeyCount(); i++)
			{
				w->Write(spline->GetTime(i));
				w->Write(spline->GetValue(i));
			}
		}

		template <class T, Parameter::Type TType> static void WriteParamTemplate(Writer* w, ParameterT<T, TType>* param)
		{
			switch (param->GetParamType())
			{
			case ParamTypeSpline:
				{
					SplineParameter<T, TType>* m = (SplineParameter<T, TType>*)param;
					switch (m->GetSplineType())
					{
					case SplineTypeLinear:
						{
							LinearSplineParameter<T, TType>* a = (LinearSplineParameter<T, TType>*)m;
							w->WriteInt8((Int8)a->GetParamType());
							w->WriteInt8((Int8)a->GetSplineType());
							w->WriteInt8((Int8)a->GetType());
							WriteKeyframedSpline(w, a->Data.Pointer());
							w->AddWrittenParam(a);
							return;
						}
						break;
					}
				}
				break;

			case ParamTypeAnimationSampled:
				{
					SampledAnimation<T, TType>* a = (SampledAnimation<T, TType>*)param;
					w->WriteInt8((Int8)a->GetParamType());
					w->WriteInt8((Int8)a->GetType());
					w->Write(a->Start);
					w->Write(a->Fps);
					w->WriteInt32(a->Values.Length());
					for (int i = 0; i < a->Values.Length(); i++)
					{
						w->Write(a->Values[i]);
					}
					w->AddWrittenParam(a);
					return;
				}
				break;

			default:
				break;
			}

			if (param->IsAnimated())
			{
				PrintLine("PSG WRITER WARNING: Skipping unsupported animation on parameter with type: " + (String)(int)param->GetParamType());
			}

			w->WriteInt8((Int8)ParamTypeConstant);
			w->WriteInt8((Int8)param->GetType());
			w->Write(param->GetValue());
			w->AddWrittenParam(param);
		}

		void Writer::WriteParam(FloatParam* param) { WriteParamTemplate(this, param); }
		void Writer::WriteParam(Vector2Param* param) { WriteParamTemplate(this, param); }
		void Writer::WriteParam(Vector3Param* param) { WriteParamTemplate(this, param); }
		void Writer::WriteParam(Vector4Param* param) { WriteParamTemplate(this, param); }

		void Writer::WriteParam(Matrix4Param* param)
		{
			switch (param->GetParamType())
			{
			case ParamTypeTranslation:
				{
					Translation* t = (Translation*)param;
					if (!HaveWrittenParam(t->Value.Pointer())) WriteParam(t->Value);
					WriteInt8((Int8)t->GetParamType());
					WriteParamIndex(t->Value.Pointer());
					AddWrittenParam(t);
					return;
				}
				break;

			case ParamTypeRotation:
				{
					Rotation* t = (Rotation*)param;
					if (!HaveWrittenParam(t->Axis.Pointer())) WriteParam(t->Axis);
					if (!HaveWrittenParam(t->Angle.Pointer())) WriteParam(t->Angle);
					WriteInt8((Int8)t->GetParamType());
					WriteParamIndex(t->Axis.Pointer());
					WriteParamIndex(t->Angle.Pointer());
					AddWrittenParam(t);
					return;
				}
				break;

			case ParamTypeScaling:
				{
					Scaling* t = (Scaling*)param;
					if (!HaveWrittenParam(t->Value.Pointer())) WriteParam(t->Value);
					WriteInt8((Int8)t->GetParamType());
					WriteParamIndex(t->Value.Pointer());
					AddWrittenParam(t);
					return;
				}
				break;

			case ParamTypeTransformNode:
				{
					TransformNode* t = (TransformNode*)param;
					if (!HaveWrittenParam(t->Parent.Pointer())) WriteParam(t->Parent);
					if (!HaveWrittenParam(t->Local.Pointer())) WriteParam(t->Local);
					WriteInt8((Int8)t->GetParamType());
					WriteParamIndex(t->Parent.Pointer());
					WriteParamIndex(t->Local.Pointer());
					AddWrittenParam(t);
					return;
				}
				break;
			}

			WriteParamTemplate(this, param);
		}

		void Writer::WriteParam(StringParam* param)
		{
			if (param->IsAnimated())
			{
				PrintLine("PSG WRITER WARNING: Animated strings is not supported");
			}

			WriteInt8((Int8)ParamTypeConstant);
			WriteInt8((Int8)param->GetType());
			WriteInt32(param->GetValue().Length());
			WriteUtf16StringData(param->GetValue());
			AddWrittenParam(param);
		}

		static void WriteValue(Writer* w, const String& s) { w->WriteInt32(s.Length()); w->WriteUtf16StringData(s); }
		static void WriteValue(Writer* w, const Matrix4& s) { w->Write(s); }
		static void WriteValue(Writer* w, const Vector4& s) { w->Write(s); }
		static void WriteValue(Writer* w, const Vector3& s) { w->Write(s); }
		static void WriteValue(Writer* w, const Vector2& s) { w->Write(s); }
		static void WriteValue(Writer* w, const float& s) { w->Write(s); }

		template <class T, Parameter::Type TType> void WriteListTemplate(Writer* w, ListParameter<T, TType>* list)
		{
			for (int it = list->AnimatedValues.Begin(); it != list->AnimatedValues.End(); it = list->AnimatedValues.Next(it))
			{
				if (!w->HaveWrittenParam(list->AnimatedValues.GetValue(it).Pointer()))
				{
					w->WriteParam(list->AnimatedValues.GetValue(it).Pointer());
				}
			}

			w->WriteInt8((Int8)list->GetParamType());
			w->WriteInt8((Int8)list->GetListType());

			w->WriteInt32(list->Values.Length());
			for (int i = 0; i < list->Values.Length(); i++)
			{
				WriteValue(w, list->Values[i]);
			}

			w->WriteInt32(list->AnimatedValues.Count());
			for (int it = list->AnimatedValues.Begin(); it != list->AnimatedValues.End(); it = list->AnimatedValues.Next(it))
			{
				w->WriteInt32(list->AnimatedValues.GetKey(it));
				w->WriteParamIndex(list->AnimatedValues.GetValue(it).Pointer());
			}

			w->AddWrittenParam(list);
		}

		void Writer::WriteParameter(Parameter* param)
		{
			switch (param->GetType())
			{
			case Parameter::TypeFloat:
				WriteParam((FloatParam*)param);
				break;

			case Parameter::TypeVector2:
				WriteParam((Vector2Param*)param);
				break;

			case Parameter::TypeVector3:
				WriteParam((Vector3Param*)param);
				break;

			case Parameter::TypeVector4:
				WriteParam((Vector4Param*)param);
				break;

			case Parameter::TypeMatrix4:
				WriteParam((Matrix4Param*)param);
				break;

			case Parameter::TypeString:
				WriteParam((StringParam*)param);
				break;

			case Parameter::TypeList:
				switch (param->GetListType())
				{
				case Parameter::TypeMatrix4:
					WriteListTemplate(this, (Matrix4ParamList*)param);
					break;

				case Parameter::TypeVector4:
					WriteListTemplate(this, (Vector4ParamList*)param);
					break;

				case Parameter::TypeVector3:
					WriteListTemplate(this, (Vector3ParamList*)param);
					break;

				case Parameter::TypeVector2:
					WriteListTemplate(this, (Vector2ParamList*)param);
					break;

				case Parameter::TypeFloat:
					WriteListTemplate(this, (FloatParamList*)param);
					break;

				case Parameter::TypeString:
					WriteListTemplate(this, (StringParamList*)param);
					break;

				default:
					XLI_THROW(String("Unsupported List Param Type: ") + (int)param->GetType());
				}
				break;

			default:
				XLI_THROW(String("Unsupported Param Type: ") + (int)param->GetType());
			}
		}

		void Writer::WriteMesh(Mesh* mesh)
		{
			WriteUInt32(mesh->Type);
			WriteUInt32(mesh->VertexCount);
			WriteUInt32(mesh->IndexCount);

			WriteInt32(mesh->Attributes.Length());
			for (int i = 0; i < mesh->Attributes.Length(); i++)
			{
				WriteInt32(mesh->Attributes[i].Semantic.Length());
				WriteCharStringData(mesh->Attributes[i].Semantic);
				WriteUInt32(mesh->Attributes[i].ComponentCount);

				if (mesh->Attributes[i].Decoder.IsNull())
				{
					WriteUInt32(0);
				}
				else
				{
					WriteUInt32(mesh->Attributes[i].Decoder->GetType());

					switch (mesh->Attributes[i].Decoder->GetType())
					{
					case DecoderInfo::TypeNone:
						break;

					case DecoderInfo::TypeScaleBias:
						Write(((DecoderInfoScaleBias*)mesh->Attributes[i].Decoder.Pointer())->Scale, sizeof(float), mesh->Attributes[i].ComponentCount);
						Write(((DecoderInfoScaleBias*)mesh->Attributes[i].Decoder.Pointer())->Bias, sizeof(float), mesh->Attributes[i].ComponentCount);
						break;
					}
				}
			}

			WriteInt32(mesh->Layout.Length());
			for (int i = 0; i < mesh->Layout.Length(); i++)
			{
				WriteUInt32(mesh->Layout[i].Format);
				WriteUInt32(mesh->Layout[i].Buffer);
				WriteUInt32(mesh->Layout[i].Offset);
			}

			WriteInt32(mesh->VertexBuffers.Length());
			for (int i = 0; i < mesh->VertexBuffers.Length(); i++)
			{
				if (mesh->VertexBuffers[i]->GetStride() * mesh->VertexCount != mesh->VertexBuffers[i]->GetDataAccessor()->GetSizeInBytes())
				{
					XLI_THROW("Unexpected size of vertex buffer");
				}

				WriteUInt32(mesh->VertexBuffers[i]->GetStride());
				Write(mesh->VertexBuffers[i]->GetData(), 1, mesh->VertexBuffers[i]->GetDataAccessor()->GetSizeInBytes());
			}

			if (mesh->IndexCount)
			{
				if (mesh->Indices->GetStride() * mesh->IndexCount != mesh->Indices->GetDataAccessor()->GetSizeInBytes())
				{
					XLI_THROW("Unexpected size of index buffer");
				}

				WriteUInt32(mesh->Indices->GetStride());
				Write(mesh->Indices->GetData(), mesh->Indices->GetStride(), mesh->IndexCount);
			}
		}

		void Writer::writeParameters(Node* node)
		{
			for (int i = 0; i < node->Transforms.Length(); i++)
			{
				if (!HaveWrittenParam(node->Transforms[i].Pointer()))
				{
					WriteParameter(node->Transforms[i]);
				}
			}

			for (int i = 0; i < node->Drawables.Length(); i++)
			{
				for (int it = node->Drawables[i].Params.Begin(); it != node->Drawables[i].Params.End(); it = node->Drawables[i].Params.Next(it))
				{
					if (!HaveWrittenParam(node->Drawables[i].Params.GetValue(it).Pointer()))
					{
						WriteParameter(node->Drawables[i].Params.GetValue(it));
					}
				}
			}

			for (int i = 0; i < node->Children.Length(); i++)
			{
				writeParameters(node->Children[i]);
			}
		}

		void Writer::writeMeshes(Node* node)
		{
			for (int i = 0; i < node->Drawables.Length(); i++)
			{
				if (!meshes.ContainsKey(node->Drawables[i].DrawMesh))
				{
					WriteInt8((Int8)SceneItemTypeMesh);
					WriteMesh(node->Drawables[i].DrawMesh);
					meshes.Add(node->Drawables[i].DrawMesh, meshCount++);
				}
			}

			for (int i = 0; i < node->Children.Length(); i++)
			{
				writeMeshes(node->Children[i]);
			}
		}

		void Writer::writeNodes(Node* node)
		{
			WriteInt32(node->Id.Length());
			WriteCharStringData(node->Id);

			WriteInt32(node->Transforms.Length());
			for (int i = 0; i < node->Transforms.Length(); i++)
			{
				WriteInt32(params.Get(node->Transforms[i].Pointer()));
			}

			for (int i = 0; i < node->Drawables.Length(); i++)
			{
				WriteInt8((Int8)NodeItemTypeDrawable);
				WriteInt32(node->Drawables[i].Params.Count());
				for (int it = node->Drawables[i].Params.Begin(); it != node->Drawables[i].Params.End(); it = node->Drawables[i].Params.Next(it))
				{
					WriteInt32(node->Id.Length());
					WriteCharStringData(node->Drawables[i].Params.GetKey(it));
					WriteInt32(params.Get(node->Drawables[i].Params.GetValue(it).Pointer()));
				}

				int meshIndex = meshes.Get(node->Drawables[i].DrawMesh);
				WriteInt32(meshIndex);
			}

			for (int i = 0; i < node->Splines.Length(); i++)
			{
				switch (node->Splines[i]->GetType())
				{
				case GeometrySpline::TypeBezier:
					{
						BezierSpline* spline = (BezierSpline*)node->Splines[i].Pointer();
						WriteInt8(2);
						WriteInt32(spline->ControlPoints.Length());
						for (int i = 0; i < spline->ControlPoints.Length(); i++)
						{
							Write(spline->ControlPoints[i]);
						}

					}
					break;

				case GeometrySpline::TypeNURBS:
					{
						NurbsSpline* spline = (NurbsSpline*)node->Splines[i].Pointer();
						WriteInt8((Int8)NodeItemTypeSpline);
						WriteInt32(spline->ControlPoints.Length());
						for (int i = 0; i < spline->ControlPoints.Length(); i++)
						{
							Write(spline->ControlPoints[i]);
						}
						WriteInt32(spline->Weights.Length());
						for (int i = 0; i < spline->Weights.Length(); i++)
						{
							Write(spline->Weights[i]);
						}
						WriteInt32(spline->Knots.Length());
						for (int i = 0; i < spline->Knots.Length(); i++)
						{
							Write(spline->Knots[i]);
						}
					}
					break;

				default:
					PrintLine("PSG WRITER WARNING: Skipping unsupported geometry spline with type: " + (String)(int)node->Splines[i]->GetType());
					break;
				}
			}
			
			WriteInt8((Int8)NodeItemTypeEnd);
			
			WriteInt32(node->Children.Length());
			for (int i = 0; i < node->Children.Length(); i++)
			{
				writeNodes(node->Children[i]);
			}
		}

		void Writer::WritePsg(Node* scene)
		{
			WriteCStr("PSG");
			WriteUInt8(2);

			writeParameters(scene);
			writeMeshes(scene);
			WriteInt8((Int8)SceneItemTypeEnd);

			writeNodes(scene);
		}

		void Writer::WritePsm(Mesh* mesh)
		{
			WriteCStr("PSM");
			WriteUInt8(1);
			WriteMesh(mesh);
		}
	}
}
