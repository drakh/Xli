#include <XliGraphics/PsgReader.h>

namespace Xli
{
	namespace Psg
	{
		Reader::Reader(Stream* stream): BinaryReader(stream)
		{
		}

		Parameter* Reader::ReadParameterIndex(Parameter::Type type)
		{
			int index = ReadInt32();
			if (params[index]->GetType() == type)
			{
				params[index]->AddRef();
				return params[index];
			}
			XLI_THROW("Unexpected Parameter Type: " + (int)params[index]->GetType());
		}

		Parameter* Reader::ReadParameter(ParamType paramType)
		{
			switch (paramType)
			{
			case ParamTypeConstant:
				{
					Parameter::Type type = (Parameter::Type)ReadInt8();
					switch (type)
					{
					case Parameter::TypeFloat:
						{
							FloatConst* p = new FloatConst();
							Read(p->Value);
							return p;
						}
						break;

					case Parameter::TypeVector2:
						{
							Vector2Const* p = new Vector2Const();
							Read(p->Value);
							return p;
						}
						break;

					case Parameter::TypeVector3:
						{
							Vector3Const* p = new Vector3Const();
							Read(p->Value);
							return p;
						}
						break;

					case Parameter::TypeVector4:
						{
							Vector4Const* p = new Vector4Const();
							Read(p->Value);
							return p;
						}
						break;

					case Parameter::TypeMatrix4:
						{
							Matrix4Const* p = new Matrix4Const();
							Read(p->Value);
							return p;
						}
						break;
					};
				}
				break;

			case ParamTypeSpline:
				{
					SplineType animType = (SplineType)ReadInt8();
					switch (animType)
					{
					case SplineTypeLinear:
						{
							Parameter::Type type = (Parameter::Type)ReadInt8();
							switch (type)
							{
							case Parameter::TypeFloat:
								{
									LinearSpline<double, float>* spline = new LinearSpline<double, float>();
									ReadKeyframedSpline(spline);
									return new FloatLinearSpline(spline);
								}
								break;

							case Parameter::TypeVector2:
								{
									LinearSpline<double, Vector2>* spline = new LinearSpline<double, Vector2>();
									ReadKeyframedSpline(spline);
									return new Vector2LinearSpline(spline);
								}
								break;

							case Parameter::TypeVector3:
								{
									LinearSpline<double, Vector3>* spline = new LinearSpline<double, Vector3>();
									ReadKeyframedSpline(spline);
									return new Vector3LinearSpline(spline);
								}
								break;

							case Parameter::TypeVector4:
								{
									LinearSpline<double, Vector4>* spline = new LinearSpline<double, Vector4>();
									ReadKeyframedSpline(spline);
									return new Vector4LinearSpline(spline);
								}
								break;

							case Parameter::TypeMatrix4:
								{
									LinearSpline<double, Matrix4>* spline = new LinearSpline<double, Matrix4>();
									ReadKeyframedSpline(spline);
									return new Matrix4LinearSpline(spline);
								}
								break;
							};
						}
						break;
					}
				}
				break;

			case ParamTypeTranslation:
				{
					Translation* p = new Translation();
					p->Value = ReadVector3ParamIndex();
					return p;
				}
				break;

			case ParamTypeRotation:
				{
					Rotation* p = new Rotation();
					p->Axis = ReadVector3ParamIndex();
					p->Angle = ReadFloatParamIndex();
					return p;
				}
				break;

			case ParamTypeScaling:
				{
					Scaling* p = new Scaling();
					p->Value = ReadVector3ParamIndex();
					return p;
				}
				break;

			case ParamTypeTransformNode:
				{
					Matrix4Param* parent = ReadMatrix4ParamIndex();
					Matrix4Param* local = ReadMatrix4ParamIndex();
					return new TransformNode(parent, local);
				}
				break;

			case ParamTypeList:
				{
					Parameter::Type type = (Parameter::Type)ReadInt8();

					switch (type)
					{
					case Parameter::TypeMatrix4:
						{
							Matrix4ParamList* list = new Matrix4ParamList();
							list->Values.Resize(ReadInt32());

							for (int i = 0; i < list->Values.Length(); i++)
							{
								Read(list->Values[i]);
							}

							int animCount = ReadInt32();

							for (int i = 0; i < animCount; i++)
							{
								int j = ReadInt32();
								Matrix4Param* m = ReadMatrix4ParamIndex();
								list->AnimatedValues.Add(j, m);
							}

							return list;
						}
					}
				}
				break;
			};

			XLI_THROW(String("Unsupported Parmeter type ") + (int)paramType);
		}

		Mesh* Reader::ReadMesh()
		{
			Mesh* m = new Mesh();

			m->Type = (PrimitiveType)ReadUInt32();
			m->VertexCount = ReadUInt32();
			m->IndexCount = ReadUInt32();

			m->Attributes.Resize(ReadInt32());
			for (int i = 0; i < m->Attributes.Length(); i++)
			{
				m->Attributes[i].Semantic = ReadCharString(ReadInt32());
				m->Attributes[i].ComponentCount = ReadUInt32();

				DecoderInfo::Type type = (DecoderInfo::Type)ReadUInt32();

				switch (type)
				{
				case DecoderInfo::TypeNone:
					m->Attributes[i].Decoder = new DecoderInfoNone();
					break;

				case DecoderInfo::TypeScaleBias:
					{
						DecoderInfoScaleBias* d = new DecoderInfoScaleBias();
						Read(d->Scale, sizeof(float), m->Attributes[i].ComponentCount);
						Read(d->Bias, sizeof(float), m->Attributes[i].ComponentCount);
						m->Attributes[i].Decoder = d;
					}
					break;

				default:
					XLI_THROW(String("Unsupported Decoder Type: ") + type);
				}
			}

			m->Layout.Resize(ReadInt32());
			for (int i = 0; i < m->Layout.Length(); i++)
			{
				m->Layout[i].Format = (Format)ReadUInt32();
				m->Layout[i].Buffer = ReadUInt32();
				m->Layout[i].Offset = ReadUInt32();
			}

			m->VertexBuffers.Resize(ReadInt32());
			for (int i = 0; i < m->VertexBuffers.Length(); i++)
			{
				uint stride = ReadUInt32();
				m->VertexBuffers[i] = new ArrayBuffer(stride, m->VertexCount);
				Read(m->VertexBuffers[i]->GetData(), 1, m->VertexBuffers[i]->GetDataAccessor()->GetSizeInBytes());
			}

			if (m->IndexCount)
			{
				uint stride = ReadUInt32();
				m->Indices = new ArrayBuffer(stride, m->IndexCount);
				Read(m->Indices->GetData(), stride, m->IndexCount);
			}

			return m;
		}

		Matrix4Param* Reader::ReadTransform()
		{
			Int32 ti = ReadInt32();
			if (ti != -1 && params[ti]->GetType() == Parameter::TypeMatrix4)
			{
				return (Matrix4Param*)Share(params[ti].Pointer());
			}
			return 0;
		}

		Node* Reader::readNode()
		{
			Psg::Node* node = new Psg::Node();
			node->Id = ReadCharString(ReadInt32());

			node->Transforms.Resize(ReadInt32());

			for (int i = 0; i < node->Transforms.Length(); i++)
			{
				node->Transforms[i] = ReadMatrix4ParamIndex();
			}
			
			while (true)
			{
				NodeItemType type = (NodeItemType)ReadInt8();

				if (type == NodeItemTypeEnd)
				{
					break;
				}
				else if (type == NodeItemTypeDrawable)
				{
					Psg::Drawable& draw = node->Drawables[node->Drawables.Add()];

					Int32 pc = ReadInt32();
					for (int j = 0; j < pc; j++)
					{
						String p = ReadCharString(ReadInt32());
						Int32 idx = ReadInt32();
						draw.Params.Add(p, params[idx]);
					}

					draw.DrawMesh = meshes[ReadInt32()];
				}
				else
				{
					XLI_THROW("Unsupported node item type: " + (String)(int)type);
				}
			}

			node->Children.Resize(ReadInt32());

			for (int i = 0; i < node->Children.Length(); i++)
			{
				node->Children[i] = readNode();
			}

			return node;
		}

		Node* Reader::ReadPsg()
		{
			if (ReadCStr(3) != "PSG") XLI_THROW("Invalid PSG file");
			if (ReadUInt8() != 2) XLI_THROW("Unsupported PSG version");

			while (true)
			{
				SceneItemType type = (SceneItemType)ReadInt8();
				if (type == SceneItemTypeEnd) break;
				else if (type == SceneItemTypeMesh) meshes.Add(ReadMesh());
				else params.Add(ReadParameter((ParamType)type));
			}

			return readNode();
		}

		Mesh* Reader::ReadPsm()
		{
			if (ReadCStr(3) != "PSM") XLI_THROW("Invalid PSM file");
			if (ReadUInt8() != 1) XLI_THROW("Unsupported PSM version");

			return ReadMesh();
		}
	}
}
