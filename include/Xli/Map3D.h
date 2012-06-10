#ifndef __XLI_MAP_3D_H__
#define __XLI_MAP_3D_H__

#include <Xli/Object.h>
#include <Xli/Rectangle.h>
#include <Xli/Color.h>
#include <Xli/Buffer.h>

namespace Xli
{
	struct Volume
	{
	public:
		int Width;
		int Height;
		int Depth;
		int RowPitch;
		int SlicePitch;
		void* Scan0;

		Volume() {}
		Volume(int width, int height, int depth, int rowPitch, int slicePitch, void* scan0)
		{
			this->Width = width;
			this->Height = height;
			this->Depth = depth;
			this->RowPitch = rowPitch;
			this->SlicePitch = slicePitch;
			this->Scan0 = scan0;
		}
	};

	/**
		Represents a 3-dimensional grid of elements.
		The template parameter T is the type of each element in the map.
		Note that if the type T is not tightly packed, the map data will not be either, so don't expect
		element formats like RGB888 to be tightly packed. However, all functions in this class will still work.
		The type T must be of an atomic type that does not require invocation of constructor/destructor.
	*/
	template <typename T> class Map3D: public Object
	{
		Managed<DataAccessor> buffer;
		int bufferOffset;
		int width;
		int height;
		int depth;
		int pitch;
		int slicePitch;
		T* data;

		void init(int width, int height, int depth)
		{
			this->data = new T[width*height*depth];
			this->buffer = new BufferPointer(data, width * height * depth * sizeof(T), true);
			this->bufferOffset = 0;
			this->pitch = width * sizeof(T);
			this->slicePitch = width * height * sizeof(T);
			this->width = width;
			this->height = height;
			this->depth = depth;
		}

	public:
		typedef Vector3i Iterator;

		Volume GetVolume()
		{
			Volume v;
			v.Width = width;
			v.Height = height;
			v.Depth = depth;
			v.RowPitch = pitch;
			v.SlicePitch = slicePitch;
			v.Scan0 = data;
			return v;
		}

		Surface GetSurface(int slice)
		{
			Surface s;
			s.Width = width;
			s.Height = height;
			s.Pitch = pitch;
			s.Scan0 = GetScanline(0, slice);
			return s;
		}

		/**
			Creates a new map with a given height an width with its own memory.
			@param width Width, in elements, of the map.
			@param height Height, in elements, of the map.
			@param depth Depth, in elements, of the map.
		*/
		Map3D(int width, int height, int depth)
		{
			init(width, height, depth);
		}

		Map3D(const Vector3i& size) { init(size.X, size.Y, size.Z); }

		/**
			Creates a 3D map view on data in an existing Buffer object.
			The Map3D will keep a reference on the buffer, so the caller is free to release
			the buffer after constructing the map.
			@param width Width, in elements, of the map.
			@param height Height, in elements, of the map.
			@param depth Depth, in elements, of the map.
			@param buffer The buffer object to create the map on.
			@param offset Offset, in bytes, into the buffer to the start of the first scanline of elements to map.
			@param pitch Pitch, in bytes, between each scanline in the buffer
			@param slicePitch Pitch, in bytes, between each slice in the buffer
		*/
		Map3D(int width, int height, int depth, DataAccessor* buffer, int offset, int pitch, int slicePitch)
		{
			buffer->AddRef();
			this->buffer = buffer;
			this->bufferOffset = offset;
			this->data = (T*)(buffer->GetData() + offset);
			this->pitch = pitch;
			this->slicePitch = slicePitch;
			this->width = width;
			this->height = height;
			this->depth = depth;
		}

		/**
			Creates a 3D map view on data in memory.
			Do not release the memory pointed to during the lifetime of the Map or its buffer.
			(If this can not be assured, use another overload for safety)
			@param width Width, in elements, of the map.
			@param height Height, in elements, of the map.
			@param depth Depth, in elements, of the map.
			@param data Pointer to the first scanline
			@param pitch Pitch, in bytes, between each scanline in the buffer
			@param slicePitch Pitch, in bytes, between each slice in the buffer
		*/
		Map3D(int width, int height, int depth, T* data, int pitch, int slicePitch)
		{
			this->buffer = new BufferPointer(data, depth*slicePitch, false);
			this->bufferOffset = 0;
			this->data = (T*)buffer->GetData();
			this->pitch = pitch;
			this->slicePitch = slicePitch;
			this->width = width;
			this->height = height;
			this->depth = depth;
		}

		void Fill(const T& value)
		{
			for (Iterator i = Begin(); i != End(); Next(i)) Set(i, value);
		}

		int GetWidth() const { return width; }
		int GetHeight() const { return height; }
		int GetDepth() const { return depth; }

		/**
			Returns a Buffer object that governs the memory used by this map.
			The first byte in the buffer used by the map is given by GetBufferOffset().
			@return The buffer that governs the memory used by this map.
		*/
		DataAccessor* GetBuffer() { return buffer; }
		const DataAccessor* GetBuffer() const { return buffer; }

		/**
			Returns the offset of the first byte in the buffer returned by GetBuffer() that is used by this map.
		*/
		int GetBufferOffset() const { return bufferOffset; }

		/**
			Returns the pitch, in bytes, between each scanline in the map.
		*/
		int GetPitch() const { return pitch; }

		/**
			Returns the pich, in bytes, between each slice of the map.
		*/
		int GetSlicePitch() const { return slicePitch; }

		void Set(int x, int y, int z, const T& value)
		{
			GetScanline(y, z)[x] = value;
		}

		void Set(const Iterator& i, const T& value)
		{
			GetScanline(i.Y, i.Z)[i.X] = value;
		}

		T& Get(int x, int y, int z)
		{
			return GetScanline(y, z)[x];
		}

		T& Get(const Iterator& p)
		{
			return GetScanline(p.Y, p.Z)[p.X];
		}

		const T& Get(int x, int y, int z) const
		{
			return GetScanline(y, z)[x];
		}

		/**
			Returns a reference to the value at a cell given by the clamped coordinates.
			If the coordinates exceeds the bounds of the map, the coordinate is clamped to the edge of the map.
		*/
		const T& GetClamp(int x, int y, int z) const
		{
			if (x < 0) x = 0;
			if (y < 0) y = 0;
			if (z < 0) z = 0;
			if (x >= width) x = width-1;
			if (y >= height) y = height-1;
			if (z >= depth) z = depth-1;
			return Get(x,y,z);
		}

		const T& GetClamp(const Iterator& i) const { return GetClamp(i.X, i.Y, i.Z); }

		/**
			Returns a reference to the value at a cell given by the wrapped coordinates.
			If the coordinates exceeds the bounds of the map, the coordinate is wrapped around the edge of the map.
		*/
		const T& GetWrap(int x, int y, int z) const
		{
			if (x < 0) x = width + x%width;
			if (y < 0) y = height + y%height;
			if (z < 0) z = depth + z%depth;
			x = x%width;
			y = y%height;
			z = z%depth;
			return Get(x,y,z);
		}

		const T& GetWrap(const Iterator& i) const { return GetWrap(i.X, i.Y, i.Z); }

		/**
			Gives an iterator to the beginning of the map.
			This offers an effective way of looping through all the elements of a map.
			Use Next() to advance the iterator, and End() to check if the iterator has reached the end of the map.
			Use Get() to get the value of the map at the iterator.
		*/
		Iterator Begin() const
		{
			return Iterator(0,0,0);
		}
		void Next(Iterator& i) const
		{
			i.X++;
			if (i.X >= width) { i.X = 0; i.Y++; }
			if (i.Y >= height) { i.Y = 0; i.Z++; }
		}
		Iterator End() const
		{
			return Iterator(0,0,depth);
		}

		template <typename O> Map3D& operator *= (const O& o)
		{
			for (Iterator i = Begin(); i != End(); Next(i)) Get(i) *= o;
			return *this;
		}
		template <typename O> Map3D& operator += (const O& o)
		{
			for (Iterator i = Begin(); i != End(); Next(i)) Get(i) += o;
			return *this;
		}
		template <typename O> Map3D& operator -= (const O& o)
		{
			for (Iterator i = Begin(); i != End(); Next(i)) Get(i) -= o;
			return *this;
		}

		/**
			Creates a map that represents a subsection of this map.
			The returned map does not have unique data, but shares data with the other map.
			To ensure that a map has only one reference, use Detatch().
			To ensure that a map is universally unique, use Copy().
			@param x X-coordinate of subsection rectangle, in elements
			@param y Y-coordinate of subsection rectangle, in elements
			@param width Width of subsection rectangle, in elements
			@param height Height of subsection rectangle, in elements
		*/
		Map3D* SubSection(int x, int y, int z, int width, int height, int depth) const
		{
			return new Map3D(width, height, depth, buffer, (x+y*pitch+z*slicePitch)*sizeof(T), pitch, slicePitch);
		}
		Map3D* SubSection(const Boxi& box)
		{
			return SubSection(box.Left, box.Top, box.Front, box.Width(), box.Height(), box.Depth());
		}

		/**
			Returns a pointer to a map that is ensured to have only one reference.
			If the reference count is more than 1, this function returns a copy of the map.
			If the reference count is 1, this function returns this map.
		*/
		Map3D* Detatch() const
		{
			if (refCount == 1) return this;
			else return Copy();
		}

		/**
			Returns a copy of the map with its own memory with a given element format.
			This function can perform element conversion while blitting as long as the element types
			are implicitly convertible. Set TDst to the desired return type.
			The returned map will have a tightly packed pitch (pitch = width*sizeof(TDst))
		*/
		template <typename TDst> Map3D<TDst>* Convert() const
		{
			Map3D<TDst>* m = new Map3D<TDst>(width, height, depth);
			for (int z = 0; z < depth; z++)
			{
				for (int y = 0; y < height; y++)
				{
					T* scanSrc = GetScanline(y, z);
					TDst* scanDst = m->GetScanline(y, z);
					for (int x = 0; x < width; x++) scanDst[x] = (TDst)scanSrc[x];
				}
			}
			return m;
		}

		/**
			Returns a copy of the map with its own memory and the same element format.
			To perform element conversion during copy, use Convert<TDst>().
		*/
		Map3D* Copy() const
		{
			return Convert<T>();
		}

		const T* GetData() const
		{
			return data;
		}
		T* GetData()
		{
			return data;
		}

		/**
			Returns a pointer to a given scanline.
			@param y Scanline index (y-coordinate, in elements).
		*/
		T* GetScanline(int y, int z)
		{
			return (T*)(data + y*pitch + z*slicePitch);
		}
		const T* GetScanline(int y, int z) const
		{
			return (T*)(data + y*pitch + z*slicePitch);
		}



		/**
			Returns the size of the map as Vector3i.
		*/
		Vector3i GetSize() const
		{
			return Vector3i(width, height, depth);
		}

		/**
			Blits data from a region in another map into this one.
			The source and destination maps can be the same, but if source and destination regions
			overlap, the outcome in the overlapping region is undefined.
			This function can perform element conversion while blitting as long as the element types
			are implicitly convertible. Set TSrc to the input type.
			Clipping will occur if the source or destination rectangle exceeds the map.
			@param srcMap The map to read data from
			@param srcRect The region to read from
			@param dst The upper-left coorner of the region to write to
		*/
		template <typename TSrc> void Blit(Map3D<TSrc>* srcMap, const Boxi& srcBox, const Vector3i& dst)
		{
			Boxi box(0,0,0,width,height,depth);
			Boxi sr = srcBox.Clip(box).Clip(srcMap->GetRect());
			Boxi dr = Boxi(dst.X, dst.Y, srcBox.Width, srcBox.Height, srcBox.Width).Clip(box);
			int rw = Min(sr.Width(), dr.Width());
			int rh = Min(sr.Height(), dr.Height());
			int rd = Min(sr.Depth(), dr.Depth());
			for (int z = 0; z < rd; z++)
			{
				for (int y = 0; y < rh; y++)
				{
					TSrc* scanSrc = srcMap->GetScanline(y, z);
					T* scanDst = GetScanline(y, z);
					for (int x = 0; x < rw; x++) scanDst[x] = (T)scanSrc[x];
				}
			}
		}

		/**
			Blits data from a region in another map into this one.
			See the other overload of Blit() for more info.
			@param srcMap The map to read data from
			@param srcx X-coordinate of the region to read from, in elements
			@param srcy Y-coordinate of the region to read from, in elements
			@param width Width of the region to copy, in elements
			@param height Height of the region to copy, in elements
			@param dstx X-coordinate of the region to write to, in elements
			@param dsty Y-coordinate of the region to write to, in elements
		*/
		template <typename TSrc> void Blit(Map3D<TSrc>* srcMap, int srcx, int srcy, int srcz, int width, int height, int depth, int dstx, int dsty, int dstz)
		{
			Boxi srcBox(srcx, srcy, srcz, srcx+width, srcx+height, srcz+depth);
			Vector3i dst(dstx, dsty, dstz);
			Blit(srcMap, srcBox, dst);
		}
	};
}

#endif
