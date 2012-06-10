#ifndef __XLI_MAP_2D_H__
#define __XLI_MAP_2D_H__

#include <Xli/Object.h>
#include <Xli/Rectangle.h>
#include <Xli/Color.h>
#include <Xli/Buffer.h>
#include <Xli/Array.h>
#include <Xli/Managed.h>

namespace Xli
{
	struct Scanline
	{
		int Width;
		void* Scan;
		Scanline() {}
		Scanline(int width, void* scan)
		{
			this->Width = width;
			this->Scan = scan;
		}
	};

	struct Surface
	{
		int Width;
		int Height;
		int Pitch;
		void* Scan0;

		Surface() {}
		Surface(int width, int height, int pitch, void* scan0)
		{
			this->Width = width;
			this->Height = height;
			this->Pitch = pitch;
			this->Scan0 = scan0;
		}
	};

	/**
		Represents a 2-dimensional grid of elements (i.e. graphic images).
		The template parameter T is the type of each element in the map.
		For instance, use Coloru8 to create a bitmap in RGBA8888 format. (See the Bitmap typedef).
		Note that if the type T is not tightly packed, the map data will not be either, so don't expect
		element formats like RGB888 to be tightly packed. However, all functions in this class will still work.
		The type T must be of an atomic type that does not require invocation of constructor/destructor.
	*/
	template <typename T> class Map2D: public Object
	{
		Managed<DataAccessor> buffer;
		int bufferOffset;
		Recti rect;
		int width;
		int height;
		int pitch;
		T* data;

		void init(int width, int height)
		{
			this->buffer = Buffer::Create(width * height * sizeof(T));
			this->data = (T*)buffer->GetData();
			this->bufferOffset = 0;
			this->pitch = width * sizeof(T);
			this->width = width;
			this->height = height;
			this->rect = Recti(0, 0, width, height);
		}

	public:
		typedef Vector2i Iterator;

		Surface GetSurface()
		{
			Surface s;
			s.Width = width;
			s.Height = height;
			s.Pitch = pitch;
			s.Scan0 = data;
			return s;
		}

		/**
			Creates a new map with a given height an width with its own memory.
			@param width Width, in elements, of the map.
			@param height Height, in elements, of the map.
		*/
		Map2D(int width, int height)
		{
			init(width, height);
		}
		Map2D(const Vector2i& size) { init(size.X, size.Y); }

		/**
			Creates a 2D map view on a DataAccessor object.
			@param width Width, in elements, of the map.
			@param height Height, in elements, of the map.
			@param data Pointer to the first scanline
			@param pitch Pitch, in bytes between each scanline in the buffer
		*/
		Map2D(int width, int height, DataAccessor* data, int pitch, int offset = 0)
		{
			buffer->AddRef();
			this->buffer = data;
			this->bufferOffset = 0;
			this->data = (T*)(buffer->GetData() + offset);
			this->pitch = pitch;
			this->width = width;
			this->height = height;
			this->rect = Recti(0, 0, width, height);
		}

		int GetWidth() const { return width; }
		int GetHeight() const { return height; }

		/**
			Returns a Buffer object that governs the memory used by this map.
			The first byte in the buffer used by the map is given by GetBufferOffset().
			@return The buffer that governs the memory used by this map.
		*/
		Buffer* GetBuffer() { return buffer; }
		const Buffer* GetBuffer() const { return buffer; }

		/**
			Returns the offset of the first byte in the buffer returned by GetBuffer() that is used by this map.
		*/
		int GetBufferOffset() const { return bufferOffset; }

		/**
			Returns the pitch, in bytes, between each scanline in the map.
		*/
		int GetPitch() const { return pitch; }

		void Set(int x, int y, const T& value)
		{
			GetScanline(y)[x] = value;
		}

		void Set(const Iterator& i, const T& value)
		{
			GetScanline(i.Y)[i.X] = value;
		}

		void Fill(const T& value)
		{
			for (int y = 0; y < height; y++)
				for (int x = 0; x < width; x++)
				{
					Set(x, y, value);
				}
		}

		T& Get(int x, int y)
		{
			return GetScanline(y)[x];
		}
		T& Get(const Iterator& i)
		{
			return GetScanline(i.Y)[i.X];
		}
		const T& Get(int x, int y) const
		{
			return GetScanline(y)[x];
		}
		const T& Get(const Iterator& i) const
		{
			return GetScanline(i.Y)[i.X];
		}

		/**
			Returns a reference to the value at a cell given by the clamped coordinates.
			If the coordinates exceeds the bounds of the map, the coordinate is clamped to the edge of the map.
		*/
		const T& GetClamp(int x, int y) const
		{
			if (x < 0) x = 0;
			if (y < 0) y = 0;
			if (x >= width) x = width-1;
			if (y >= height) y = height-1;
			return Get(x,y);
		}

		const T& GetClamp(const Iterator& i) const { return GetClamp(i.X, i.Y); }

		/**
			Returns a reference to the value at a cell given by the wrapped coordinates.
			If the coordinates exceeds the bounds of the map, the coordinate is wrapped around the edge of the map.
		*/
		const T& GetWrap(int x, int y) const
		{
			if (x < 0) x = width + x%width;
			if (y < 0) y = height + y%height;
			x = x%width;
			y = y%height;
			return Get(x,y);
		}

		const T& GetWrap(const Iterator& i) const { return GetWrap(i.X, i.Y); }

		/**
			Gives an iterator to the beginning of the map.
			This offers an effective way of looping through all the elements of a map.
			Use Next() to advance the iterator, and End() to check if the iterator has reached the end of the map.
			Use Get() to get the value of the map at the iterator.
		*/
		Iterator Begin() const
		{
			return Iterator(0,0);
		}
		void Next(Iterator& i) const
		{
			i.X++;
			if (i.X >= width) { i.X = 0; i.Y++; }
		}
		Iterator End() const
		{
			return Iterator(0,height);
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
		Map2D* SubSection(int x, int y, int width, int height) const
		{
			return new Map2D(width, height, (Buffer*)buffer.Pointer(), (x+y*pitch)*sizeof(T), pitch);
		}
		Map2D* SubSection(const Recti& rect)
		{
			return SubSection(rect.Left, rect.Top, rect.Width(), rect.Height());
		}

		/**
			Returns a pointer to a map that is ensured to have only one reference.
			If the reference count is more than 1, this function returns a copy of the map.
			If the reference count is 1, this function returns this map.
		*/
		Map2D* Detatch() const
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
		template <typename TDst> Map2D<TDst>* Convert() const
		{
			Map2D<TDst>* m = new Map2D<TDst>(width, height);
			for (int y = 0; y < height; y++)
			{
				T* scanSrc = GetScanline(y);
				TDst* scanDst = m->GetScanline(y);
				for (int x = 0; x < width; x++) scanDst[x] = (TDst)scanSrc[x];
			}
			return m;
		}

		/**
			Returns a copy of the map with its own memory and the same element format.
			To perform element conversion during copy, use Convert<TDst>().
		*/
		Map2D* Copy() const
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
		T* GetScanline(int y)
		{
			return (T*)(data + y*pitch);
		}
		const T* GetScanline(int y) const
		{
			return (T*)(data + y*pitch);
		}

		/**
			Returns a pointer to a given scanline.
			Allows the access pattern map2d[x][y].
		*/
		T* operator [] (int scanline)
		{
			return GetScanline(scanline);
		}
		const T* operator [] (int scanline) const
		{
			return GetScanline(scanline);
		}

		/**
			Returns the size of the map as Vector2i.
		*/
		Vector2i GetSize() const
		{
			return Vector2i(width, height);
		}

		/**
			Returns a rectangle with the extends of the map located at (0,0).
		*/
		const Recti& GetRect() const
		{
			return rect;
		}

		template <typename O> Map2D& operator *= (const O& o)
		{
			for (Iterator i = Begin(); i != End(); Next(i)) Get(i) *= o;
			return *this;
		}
		template <typename O> Map2D& operator += (const O& o)
		{
			for (Iterator i = Begin(); i != End(); Next(i)) Get(i) += o;
			return *this;
		}
		template <typename O> Map2D& operator -= (const O& o)
		{
			for (Iterator i = Begin(); i != End(); Next(i)) Get(i) -= o;
			return *this;
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
		template <typename TSrc> void Blit(Map2D<TSrc>* srcMap, const Recti& srcRect, const Vector2i& dst)
		{


			Recti sr = srcRect.Clip(rect).Clip(srcMap->GetRect());
			Recti dr = Recti(dst.X, dst.Y, dst.X+srcRect.Width(), dst.Y+srcRect.Height()).Clip(rect);
			int rw = Min(sr.Width(), dr.Width());
			int rh = Min(sr.Height(), dr.Height());
			for (int y = 0; y < rh; y++)
			{
				TSrc* scanSrc = srcMap->GetScanline(y);
				T* scanDst = GetScanline(y+dr.Top);
				for (int x = 0; x < rw; x++) scanDst[x+dr.Left] = (T)scanSrc[x];
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
		template <typename TSrc> void Blit(Map2D<TSrc>* srcMap, int srcx, int srcy, int width, int height, int dstx, int dsty)
		{
			Recti srcRect(srcx, srcy, srcx+width, srcx+height);
			Vector2i dst(dstx, dsty);
			Blit(srcMap, srcRect, dst);
		}

		/**
			Down-samples the map to half its original dimensions using a 2x2 box average filter.
			Supports on-the-fly element type conversion between compatible element types.
			The TDst type will be used while filtering the arguments.
			The TDst type should support the static TDst Average4(TDst*) function which computes the
			average of four TDst values.
			If the source map is of width or height 1, the destination map should also have the sime dimension on
			the corresponding axis.
			This function is fast and correct if the destination map is exactly half the dimensions of the source map.
			If the desitnation map is not of the exact half dimensions of the source map,
			the destination map's dimensions are adhered and the data is read from the map using
			GetClamp(), which is slower and introduces clipping or edge repetion.
			This allows this function to handle the corner cases i.e. in mip map generation on non-power-of-two images
			when one of the dimensions reach 1.
			@param dstMap The map to write the filtered down image to.
		*/
		template <typename TDst> void DownSample2x2(Map2D<TDst>* dstMap)
		{
			if ((dstMap->GetWidth() != GetWidth()/2) || (dstMap->GetHeight() != GetWidth()/2))
			{
				for (int y = 0; y < dstMap->GetHeight(); y++)
				{
					for (int x = 0; x < dstMap->GetWidth(); x++)
					{
						TDst p[4];
						p[0] = (TDst)GetClamp(x*2, y*2);
						p[1] = (TDst)GetClamp(x*2+1, y*2);
						p[2] = (TDst)GetClamp(x*2+1, y*2+1);
						p[3] = (TDst)GetClamp(x*2, y*2+1);
						Xli::Colort<unsigned char, unsigned int, int, 255> c = TDst::Average4(p);
						dstMap->Set(x, y, c);
					}
				}
			}
			else
			{

				for (int y = 0; y < dstMap->GetHeight(); y++)
				{
					TDst* dstScan0 = dstMap->GetScanline(y);
					T* srcScan0 = GetScanline(y*2+0);
					T* srcScan1 = GetScanline(y*2+1);
					for (int x = 0; x < dstMap->GetWidth(); x++)
					{
						TDst p[4];
						p[0] = (TDst)srcScan0[x*2+0];
						p[1] = (TDst)srcScan0[x*2+1];
						p[2] = (TDst)srcScan1[x*2+0];
						p[3] = (TDst)srcScan1[x*2+1];
						dstScan0[x] = TDst::Average4(p);
					}
				}
			}
		}
		/**
			Generates a mipmap chain for the bitmap using 2x2 downsampling.
			The resulting mip chain is suitable for e.g. OpenGL or Direct3D textures.
			The output array will also include a pointer to this bitmap, with increased reference count.
			@param chain The array to which to add the generating bitmaps, in order from largest to smallest (1x1).
		*/
		void GenerateMipmaps(Array<Map2D*>& chain)
		{
			AddRef();
			chain.Add(this);
			Map2D* map = this;
			while (map->GetWidth() > 1 || map->GetHeight() > 1)
			{
				Map2D* newMap = new Map2D(Max(map->GetWidth()/2,1), Max(map->GetHeight()/2,1));
				map->DownSample2x2(newMap);
				chain.Add(newMap);
				map = newMap;
			}
		}

	};
}

#endif
