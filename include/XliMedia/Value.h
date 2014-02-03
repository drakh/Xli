#ifndef __XLI_VALUE_H__
#define __XLI_VALUE_H__

#include <Xli/String.h>
#include <Xli/Managed.h>
#include <Xli/HashMap.h>
#include <Xli/Map.h>
#include <Xli/TextWriter.h>
#include <Xli/Matrix4.h>
#include <Xli/ToString.h>

/**
    \addtogroup XliMedia
    @{
*/
#define XLI_GET_X(label, x) do { try { x; } catch (const Xli::Exception& e) \
    { Xli::ErrorPrintLine((Xli::String)"WARNING: " + XLI_FUNCTION + ": Unable to read '" + label + "': " + e.ToString()); } } while (0)

#define XLI_GET_VALUE(value, result)    XLI_GET_X(#result, result = value[#result])
#define XLI_GET_STRING(value, result)    XLI_GET_X(#result, Value __tmp = value[#result]; if (__tmp.IsUndefined()) XLI_THROW("Value is undefined"); else result = __tmp.ToString())
#define XLI_GET_INT(value, result)        XLI_GET_X(#result, result = value[#result].ToInt32())
#define XLI_GET_BOOL(value, result)        XLI_GET_X(#result, result = value[#result].ToBool())
#define XLI_GET_DOUBLE(value, result)    XLI_GET_X(#result, result = value[#result].ToDouble())
#define XLI_GET_FLOAT(value, result)    XLI_GET_X(#result, result = value[#result].ToFloat())
#define XLI_GET_VECTOR2I(value, result)    XLI_GET_X(#result, result = value[#result].ToVector2i())
#define XLI_GET_VECTOR3I(value, result)    XLI_GET_X(#result, result = value[#result].ToVector3i())
#define XLI_GET_VECTOR4I(value, result)    XLI_GET_X(#result, result = value[#result].ToVector4i())

/** @} */

namespace Xli
{
    class Value;

    /**
        \ingroup XliMedia
    */
    enum ValueType
    {
        ValueTypeUndefined,
        ValueTypeString,
        ValueTypeInteger,
        ValueTypeInt64,
        ValueTypeDouble,
        ValueTypeBool,            
        ValueTypeArray,
        ValueTypeObject,
        ValueTypeOrderedObject,
    };

    /**
        \ingroup XliMedia
    */
    class IValue: public Object
    {
    public:
        virtual ValueType GetType() const = 0;

        virtual UInt32 GetHashCode() const;

        virtual String ToString() const;
        virtual Int32 ToInt32() const;
        virtual Int64 ToInt64() const;
        virtual double ToDouble() const;
        virtual bool ToBool() const;

        virtual int Begin() const { return -1; }
        virtual int End() const { return -1; }
        virtual int Next(int it) const { return -1; }
        virtual void SetValue(int it, const Value& value);
        virtual const Value& GetValue(int it) const;
        virtual const Value& GetKey(int it) const;

        virtual bool ContainsKey(const Value& key) const;
        virtual void Insert(const Value& key, const Value& value);
        virtual void Remove(const Value& value);
        virtual void RemoveAt(const Value& key);
        virtual void Append(const Value& value);
        virtual Value& Get(const Value& key);
        virtual int Count() const;

        virtual Value Add(const Value& v) const;
        virtual Value Sub(const Value& v) const;
        virtual Value Mul(const Value& v) const;
        virtual Value Div(const Value& v) const;
        virtual Value Mod(const Value& v) const;

        virtual bool And(const Value& v) const;
        virtual bool Or(const Value& v) const;
        virtual bool Not() const;

        virtual bool Equal(const Value& v) const;
        virtual bool NotEqual(const Value& v) const;
        virtual bool LessThan(const Value& v) const;
        virtual bool LessThanOrEqual(const Value& v) const;
        virtual bool GreaterThan(const Value& v) const;
        virtual bool GreaterThanOrEqual(const Value& v) const;

        virtual Value BitwiseAnd(const Value& v) const;
        virtual Value BitwiseOr(const Value& v) const;
        virtual Value BitwiseNot() const;
        virtual Value BitwiseXor(const Value& v) const;
    };

    /**
        \ingroup XliMedia
    */
    class Value: public Managed<IValue>
    {
    public:
        Value();
        Value(bool b);
        Value(Int32 v);
        Value(Int64 v);
        Value(float f);
        Value(double d);
        Value(const String& s);
        Value(const char* s);
        Value(IValue* value);

        static Value CreateArray();
        static Value CreateObject();
        static Value CreateOrderedObject();

        static const char* TypeToString(ValueType type);

        UInt32 GetHashCode() const { return object->GetHashCode(); }
        ValueType GetType() const { return object->GetType(); }

        bool IsArray() const;
        bool IsObject() const;
        bool IsUndefined() const;

        String ToString() const { return object->ToString(); }
        Int32 ToInt32() const { return object->ToInt32(); }
        Int64 ToInt64() const { return object->ToInt64(); }
        float ToFloat() const { return (float)object->ToDouble(); }
        double ToDouble() const { return object->ToDouble(); }
        bool ToBool() const { return object->ToBool(); }

        void ToType(String& result) const { result = ToString(); } 
        void ToType(Int32& result) const { result = ToInt32(); } 
        void ToType(Int64& result) const { result = ToInt64(); }
        void ToType(float& result) const { result = ToFloat(); } 
        void ToType(double& result) const { result = ToDouble(); }
        void ToType(bool& result) const { result = ToBool(); } 

        Value(const Vector2i& v);
        Value(const Vector3i& v);
        Value(const Vector4i& v);

        Value(const Vector2& v);
        Value(const Vector3& v);
        Value(const Vector4& v);

        Value(const Matrix3& v);
        Value(const Matrix4& v);

        Vector2i ToVector2i() const;
        Vector3i ToVector3i() const;
        Vector4i ToVector4i() const;

        Vector2 ToVector2() const;
        Vector3 ToVector3() const;
        Vector4 ToVector4() const;

        Matrix3 ToMatrix3() const;
        Matrix4 ToMatrix4() const;

        void ToType(Vector2i& result) const { result = ToVector2i(); } 
        void ToType(Vector3i& result) const { result = ToVector3i(); }
        void ToType(Vector4i& result) const { result = ToVector4i(); } 

        void ToType(Vector2& result) const { result = ToVector2(); } 
        void ToType(Vector3& result) const { result = ToVector3(); } 
        void ToType(Vector4& result) const { result = ToVector4(); }

        void ToType(Matrix3& result) const { result = ToMatrix3(); }
        void ToType(Matrix4& result) const { result = ToMatrix4(); } 

        int Begin() const { return object->Begin(); }
        int End() const { return object->End(); }
        int Next(int it) const { return object->Next(it); }
        void SetValue(int it, const Value& value) { object->SetValue(it, value); }
        const Value& GetValue(int it) const { return object->GetValue(it); }
        const Value& GetKey(int it) const { return object->GetKey(it); }

        bool ContainsKey(const Value& key) const { return object->ContainsKey(key); }
        void Insert(const Value& key, const Value& value) { object->Insert(key, value); }
        void Remove(const Value& value) { object->Remove(value); }
        void RemoveAt(const Value& key) { object->RemoveAt(key); }
        void Append(const Value& value) { object->Append(value); }
        Value& Get(const Value& key) { return object->Get(key); }
        Value& operator [] (const Value& key) { return object->Get(key); }
        const Value& Get(const Value& key) const { return object->Get(key); }
        const Value& operator [] (const Value& key) const { return object->Get(key); }

        int Count() const { return object->Count(); }

        Value operator + (const Value& v) const { return object->Add(v); }
        Value operator - (const Value& v) const { return object->Sub(v); }
        Value operator * (const Value& v) const { return object->Mul(v); }
        Value operator / (const Value& v) const { return object->Div(v); }
        Value operator % (const Value& v) const { return object->Mod(v); }

        bool operator && (const Value& v) const { return object->And(v); }
        bool operator || (const Value& v) const { return object->Or(v); }
        bool operator ! () const { return object->Not(); }

        bool operator == (const Value& v) const { return object->Equal(v); }
        bool operator != (const Value& v) const { return object->NotEqual(v); }
        bool operator < (const Value& v) const { return object->LessThan(v); }
        bool operator <= (const Value& v) const { return object->LessThanOrEqual(v); }
        bool operator > (const Value& v) const { return object->GreaterThan(v); }
        bool operator >= (const Value& v) const { return object->GreaterThanOrEqual(v); }

        Value operator & (const Value& v) const { return object->BitwiseAnd(v); }
        Value operator | (const Value& v) const { return object->BitwiseAnd(v); }
        Value operator ~ () const { return object->BitwiseNot(); }
        Value operator ^ (const Value& v) const { return object->BitwiseXor(v); }
    };

    /**
        \ingroup XliMedia
    */
    class Undefined: public IValue
    {
    public:
        virtual ValueType GetType() const { return ValueTypeUndefined; }
        virtual String ToString() const { return "null"; }
    };

    /**
        \ingroup XliMedia
    */
    class BoolValue: public IValue
    {
        bool value;

    public:
        virtual ValueType GetType() const { return ValueTypeBool; }
        virtual String ToString() const { return Xli::ToString(value); }
        virtual bool ToBool() const { return value; }        
        virtual Int32 ToInt32() const { return value? 1 : 0; }        
        virtual Int64 ToInt64() const { return value? 1 : 0; }        
        virtual double ToDouble() const { return value? 1.0 : 0.0; } 

        virtual UInt32 GetHashCode() const { return Xli::Hash(value); }
        
        BoolValue(bool val) { value = val; }
        operator bool() { return value; }

        virtual bool Equal(const Value& v) const {  try { return value == v->ToBool(); } catch(const Exception&) { return false; } }

        virtual bool And(const Value& v) const { return value && v->ToBool(); }
        virtual bool Or(const Value& v) const { return value || v->ToBool(); }
        virtual bool Not() const { return !value; }
    };

    /**
        \ingroup XliMedia
    */
    class StringValue: public IValue
    {
        String value;

    public:
        virtual ValueType GetType() const { return ValueTypeString; }
        virtual String ToString() const { return value; }
        virtual Int32 ToInt32() const { return value.ToInt(); }
        virtual bool ToBool() const { if (value=="true") return true; if (value=="false") return false; XLI_THROW("Could not convert string to bool"); }

        virtual UInt32 GetHashCode() const { return Xli::Hash(value); }
        
        StringValue(const String& str) { value = str; }
        operator const String&() { return value; }

        virtual Value Add(const Value& v) const { return new StringValue(value + v->ToString()); }

        virtual bool Equal(const Value& v) const { try {return value == v->ToString(); } catch(const Exception&) { return false; } }
    };

    /**
        \ingroup XliMedia
    */
    class IntegerValue: public IValue
    {
        Int32 value;

    public:
        virtual ValueType GetType() const { return ValueTypeInteger; }
        virtual String ToString() const { return Xli::ToString(value); }
        virtual Int32 ToInt32() const { return value; }
        virtual Int64 ToInt64() const { return value; }
        virtual double ToDouble() const { return (double)value; }
        virtual bool ToBool() const { return value != 0; }

        virtual UInt32 GetHashCode() const { return Xli::Hash(value); }
        
        IntegerValue(int val) { value = val; }
        
        virtual Value Add(const Value& v) const { return new IntegerValue(value + v->ToInt32()); }
        virtual Value Sub(const Value& v) const { return new IntegerValue(value - v->ToInt32()); }
        virtual Value Mul(const Value& v) const { return new IntegerValue(value * v->ToInt32()); }
        virtual Value Div(const Value& v) const { return new IntegerValue(value / v->ToInt32()); }
        virtual Value Mod(const Value& v) const { return new IntegerValue(value % v->ToInt32()); }

        virtual bool Equal(const Value& v) const {  try { return value == v->ToInt32(); } catch(const Exception&) { return false; } }

        virtual bool LessThan(const Value& v) const { return value < v->ToInt32(); }
        virtual bool LessThanOrEqual(const Value& v) const { return value <= v->ToInt32(); }
        virtual bool GreaterThan(const Value& v) const { return value > v->ToInt32(); }
        virtual bool GreaterThanOrEqual(const Value& v) const { return value >= v->ToInt32(); }

        virtual Value BitwiseAnd(const Value& v) const { return new IntegerValue(value & v->ToInt32()); }
        virtual Value BitwiseOr(const Value& v) const { return new IntegerValue(value | v->ToInt32()); }
        virtual Value BitwiseNot() const { return new IntegerValue(~value); } 
        virtual Value BitwiseXor(const Value& v) const { return new IntegerValue(value ^ v->ToInt32()); }
    };

    /**
        \ingroup XliMedia
    */
    class Int64Value: public IValue
    {
        Int64 value;

    public:
        virtual ValueType GetType() const { return ValueTypeInteger; }
        virtual String ToString() const { return Xli::ToString(value); }
        virtual Int32 ToInt32() const { return (Int32)value; }
        virtual Int64 ToInt64() const { return value; }
        virtual double ToDouble() const { return (double)value; }
        virtual bool ToBool() const { return value != 0; }
        
        virtual UInt32 GetHashCode() const { return Xli::Hash(value); }

        Int64Value(Int64 val) { value = val; }

        virtual Value Add(const Value& v) const { return new Int64Value(value + v->ToInt64()); }
        virtual Value Sub(const Value& v) const { return new Int64Value(value - v->ToInt64()); }
        virtual Value Mul(const Value& v) const { return new Int64Value(value * v->ToInt64()); }
        virtual Value Div(const Value& v) const { return new Int64Value(value / v->ToInt64()); }
        virtual Value Mod(const Value& v) const { return new Int64Value(value % v->ToInt64()); }

        virtual bool Equal(const Value& v) const {  try { return value == v->ToInt64(); } catch(const Exception&) { return false; } }

        virtual bool LessThan(const Value& v) const { return value < v->ToInt64(); }
        virtual bool LessThanOrEqual(const Value& v) const { return value <= v->ToInt64(); }
        virtual bool GreaterThan(const Value& v) const { return value > v->ToInt64(); }
        virtual bool GreaterThanOrEqual(const Value& v) const { return value >= v->ToInt64(); }

        virtual Value BitwiseAnd(const Value& v) const { return new Int64Value(value & v->ToInt64()); }
        virtual Value BitwiseOr(const Value& v) const { return new Int64Value(value | v->ToInt64()); }
        virtual Value BitwiseNot() const { return new Int64Value(~value); } 
        virtual Value BitwiseXor(const Value& v) const { return new Int64Value(value ^ v->ToInt64()); }
    };
    
    /**
        \ingroup XliMedia
    */
    class DoubleValue: public IValue
    {
        double value;

    public:
        virtual ValueType GetType() const { return ValueTypeDouble; }
        virtual String ToString() const { return Xli::ToString(value); }
        virtual Int32 ToInt32() const { return (Int32)value; }
        virtual Int64 ToInt64() const { return (Int64)value; }
        virtual double ToDouble() const { return value; }
        virtual bool ToBool() const { return value != 0.0; }

        virtual UInt32 GetHashCode() const { return Xli::Hash(value); }

        DoubleValue(double val) { value = val; }
        operator double() { return value; }

        virtual Value Add(const Value& v) const { return new DoubleValue(value + v->ToDouble()); }
        virtual Value Sub(const Value& v) const { return new DoubleValue(value - v->ToDouble()); }
        virtual Value Mul(const Value& v) const { return new DoubleValue(value * v->ToDouble()); }
        virtual Value Div(const Value& v) const { return new DoubleValue(value / v->ToDouble()); }

        virtual bool Equal(const Value& v) const { try{return value == v->ToDouble(); } catch(const Exception&) { return false; } }
        virtual bool LessThan(const Value& v) const { return value < v->ToDouble(); }
        virtual bool LessThanOrEqual(const Value& v) const { return value <= v->ToDouble(); }
        virtual bool GreaterThan(const Value& v) const { return value > v->ToDouble(); }
        virtual bool GreaterThanOrEqual(const Value& v) const { return value >= v->ToDouble(); }
    };

    /**
        \ingroup XliMedia
    */
    class ArrayValue: public IValue
    {
    public:
        Array<Value> Values;

        virtual ValueType GetType() const { return ValueTypeArray; }

        virtual int Begin() const { return 0; }
        virtual int End() const { return Values.Length(); }
        virtual int Next(int it) const { return it + 1; }
        virtual void SetValue(int it, const Value& value) { Values[it] = value; }
        virtual const Value& GetValue(int it) const { return Values[it]; }

        virtual bool ContainsKey(const Value& key) const { int k = key.ToInt32(); return (k >= 0) && (k < Values.Length()); }
        virtual void Insert(const Value& key, const Value& value) { Values.Insert(key.ToInt32(), value); }
        virtual void Remove(const Value& value) { Values.Remove(value); }
        virtual void RemoveAt(const Value& key) { Values.RemoveAt(key.ToInt32()); }
        virtual void Append(const Value& value) { Values.Add(value); }
        virtual Value& Get(const Value& key) { return Values[key.ToInt32()]; }
        virtual int Count() const { return Values.Length(); }        
    };

    /**
        \ingroup XliMedia
    */
    class ObjectValue: public IValue
    {
    public:
        HashMap<Value, Value> Values;

        virtual ValueType GetType() const { return ValueTypeObject; }

        virtual int Begin() const { return Values.Begin(); }
        virtual int End() const { return Values.End(); }
        virtual int Next(int it) const { return Values.Next(it); }
        virtual void SetValue(int it, const Value& value) { Values.SetValue(it, value); }
        virtual const Value& GetValue(int it) const { return Values.GetValue(it); }
        virtual const Value& GetKey(int it) const { return Values.GetKey(it); }

        virtual bool ContainsKey(const Value& key) const { return Values.ContainsKey(key); }
        virtual void Insert(const Value& key, const Value& value) { Values[key] = value; } // changed from .Add!!!!
        virtual void RemoveAt(const Value& key) { Values.Remove(key); }
        virtual Value& Get(const Value& key) { return Values[key]; }
        virtual int Count() const { return Values.Count(); }
    };

    /**
        \ingroup XliMedia
    */
    class OrderedObjectValue: public IValue
    {
    public:
        Map<Value, Value> Values;

        virtual ValueType GetType() const { return ValueTypeOrderedObject; }

        virtual int Begin() const { return Values.Begin(); }
        virtual int End() const { return Values.End(); }
        virtual int Next(int it) const { return Values.Next(it); }
        virtual void SetValue(int it, const Value& value) { Values.SetValue(it, value); }
        virtual const Value& GetValue(int it) const { return Values.GetValue(it); }
        virtual const Value& GetKey(int it) const { return Values.GetKey(it); }

        virtual bool ContainsKey(const Value& key) const { return Values.ContainsKey(key); }
        virtual void Insert(const Value& key, const Value& value) { Values.Add(key, value); }
        virtual void RemoveAt(const Value& key) { Values.Remove(key); }
        virtual Value& Get(const Value& key) { return Values[key]; }
        virtual int Count() const { return Values.Count(); }
    };

    /**
        \addtogroup XliMedia
        @{
    */

    UInt32 Hash(const Value& value);

    /** @} */
};

#endif
