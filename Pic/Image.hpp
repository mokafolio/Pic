#ifndef PIC_IMAGE_HPP
#define PIC_IMAGE_HPP

#include <Pic/Pixel.hpp>
#include <Pic/PixelIterator.hpp>
#include <Stick/URI.hpp>
#include <Stick/Result.hpp>
#include <Stick/FileUtilities.hpp>
#include <Stick/DynamicArray.hpp>
#include <Stick/UniquePtr.hpp>
#include <new>
#include <algorithm>

namespace pic
{
    class STICK_API Image
    {
    public:

        virtual ~Image() = default;

        //the base class it not copyable to prevent splicing
        Image(const Image & _other) = delete;
        Image & operator = (const Image & _other) = delete;

        virtual Image * clone(stick::Allocator & _alloc = stick::defaultAllocator()) const = 0;

        virtual void resize(stick::Size _width, stick::Size _height) = 0;

        virtual void resize(stick::Size _width, stick::Size _height, stick::Size _depth, stick::Size _rowPadding = 0) = 0;

        virtual void setPixelRaw(stick::Size _left, stick::Size _top, const char * _data) = 0;

        virtual void setPixelRaw(stick::Size _left, stick::Size _top, stick::Size _layer, const char * _data) = 0;

        virtual stick::Size width() const = 0;

        virtual stick::Size height() const = 0;

        virtual stick::Size depth() const = 0;

        virtual stick::UInt32 channelCount() const = 0;

        virtual stick::Size pixelCount() const = 0;

        virtual stick::TypeID pixelTypeID() const = 0;

        virtual stick::TypeID channelLayoutTypeID() const = 0;

        virtual stick::TypeID valueTypeID() const = 0;

        virtual stick::Size byteCount() const = 0;

        virtual stick::Size bytesPerRow() const = 0;

        virtual stick::Size bitsPerChannel() const = 0;

        virtual stick::Size bitsPerPixel() const = 0;

        virtual bool hasAlpha() const = 0;

        virtual bool isFloatingPoint() const = 0;

        virtual stick::Int32 alphaPosition() const = 0;

        virtual const char * bytePtr() const = 0;

        virtual void flipRows() = 0;

        virtual void flipColumns() = 0;

        virtual void flipLayers() = 0;

        virtual void flip() = 0;

        virtual void swapChannels(stick::UInt32 _a, stick::UInt32 _b) = 0;

        virtual void loadRawPixels(stick::Size _width, stick::Size _height, stick::Size _depth, const char * _src, stick::Size _rowPadding = 0) = 0;

        virtual void updateRawPixels(stick::Size _left, stick::Size _top, stick::Size _layer, stick::Size _width, stick::Size _height, stick::Size _depth, const char * _src) = 0;

        stick::Error saveFile(const stick::URI & _uri);

    protected:

        Image() {}
    };

    STICK_RESULT_HOLDER(ImageResultHolder, image);

    typedef stick::UniquePtr<Image> ImageUniquePtr;

    STICK_API stick::Result<ImageUniquePtr, ImageResultHolder> decodeImage(const stick::ByteArray & _data, stick::Allocator & _alloc = stick::defaultAllocator());

    STICK_API stick::Result<ImageUniquePtr, ImageResultHolder> decodeImage(const void * _data, stick::Size _byteCount, stick::Allocator & _alloc = stick::defaultAllocator());

    STICK_API stick::Result<ImageUniquePtr, ImageResultHolder> loadImage(const stick::URI & _path, stick::Allocator & _alloc = stick::defaultAllocator());


    template<class C>
    class STICK_API ImageT : public Image
    {
    public:

        typedef C Channels;

        typedef typename Channels::ChannelLayout ChannelLayout;

        typedef typename Channels::ValueType ValueType;

        typedef PixelT<Channels> Pixel;

        typedef stick::DynamicArray<char> DataArray;

        typedef PixelIteratorT<ImageT, char*> PixelIter;

        typedef PixelIteratorT<const ImageT, const char*> PixelConstIter;

        //TODO: make type id work with constexprs
        static constexpr stick::TypeID channelLayoutTID = ChannelLayout::TypeInfo::typeID();
        static constexpr stick::TypeID pixeTID = Pixel::TypeInfo::typeID();


        ImageT(stick::Allocator & _alloc = stick::defaultAllocator());

        ImageT(stick::Size _width, stick::Size _height, stick::Size _depth = 1, stick::Size _rowPadding = 0, stick::Allocator & _alloc = stick::defaultAllocator());

        ImageT(stick::Size _width, const ValueType * _pixels, stick::Size _rowPadding = 0, stick::Allocator & _alloc = stick::defaultAllocator());

        ImageT(stick::Size _width, stick::Size _height, const ValueType * _pixels, stick::Size _rowPadding = 0, stick::Allocator & _alloc = stick::defaultAllocator());

        ImageT(stick::Size _width, stick::Size _height, stick::Size _depth, const ValueType * _pixels, stick::Size _rowPadding = 0, stick::Allocator & _alloc = stick::defaultAllocator());

        ImageT(const ImageT & _other);

        ImageT(ImageT && _other);

        ImageT & operator = (const ImageT & _other);

        ImageT & operator = (ImageT && _other);

        Image * clone(stick::Allocator & _alloc = stick::defaultAllocator()) const;

        void resize(stick::Size _width, stick::Size _height);

        void resize(stick::Size _width, stick::Size _height, stick::Size _depth, stick::Size _rowPadding = 0);

        void clear();

        void loadPixels(stick::Size _width, stick::Size _height, const ValueType * _pixels, stick::Size _rowPadding = 0);

        void loadPixels(stick::Size _width, stick::Size _height, stick::Size _depth, const ValueType * _pixels, stick::Size _rowPadding = 0);

        void updatePixels(stick::Size _left, stick::Size _top, stick::Size _width, stick::Size _height, const ValueType * _pixels);

        void updatePixels(stick::Size _left, stick::Size _top, stick::Size _layer, stick::Size _width, stick::Size _height, stick::Size _depth, const ValueType * _pixels);

        void loadRawPixels(stick::Size _width, stick::Size _height, stick::Size _depth, const char * _src, stick::Size _rowPadding = 0);

        void updateRawPixels(stick::Size _left, stick::Size _top, stick::Size _layer, stick::Size _width, stick::Size _height, stick::Size _depth, const char * _src);

        void setPixel(stick::Size _left, stick::Size _top, const ValueType * _value);

        void setPixel(stick::Size _left, stick::Size _top, stick::Size _layer, const ValueType * _value);

        void setPixelRaw(stick::Size _left, stick::Size _top, const char * _data);

        void setPixelRaw(stick::Size _left, stick::Size _top, stick::Size _layer, const char * _data);


        //flips all the pixel rows
        void flipRows();

        void flipColumns();

        void flipLayers();

        void flip();

        void swapChannels(stick::UInt32 _a, stick::UInt32 _b);


        const Pixel & pixel(stick::Size _left, stick::Size _top) const;

        const Pixel & pixel(stick::Size _left, stick::Size _top, stick::Size _layer) const;

        Pixel & pixel(stick::Size _left, stick::Size _top);

        Pixel & pixel(stick::Size _left, stick::Size _top, stick::Size _layer);


        PixelIter begin();

        PixelIter end();

        PixelConstIter begin() const;

        PixelConstIter end() const;


        stick::Size width() const;

        stick::Size height() const;

        stick::Size depth() const;

        stick::Size byteCount() const;

        stick::Size bytesPerRow() const;

        stick::Size pixelCount() const;

        stick::Size rowPadding() const;

        stick::UInt32 channelCount() const;

        stick::Size bitsPerPixel() const;

        stick::Size bitsPerChannel() const;

        stick::Size bytesPerPixel() const;

        stick::TypeID channelLayoutTypeID() const;

        stick::TypeID pixelTypeID() const;

        stick::TypeID valueTypeID() const;

        const ValueType * ptr() const;

        ValueType * ptr();

        bool isFloatingPoint() const;

        bool hasAlpha() const;

        stick::Int32 alphaPosition() const;

        const char * bytePtr() const;

    private:

        void updateArraySize();

        DataArray m_data;

        stick::Size m_width;

        stick::Size m_height;

        stick::Size m_depth;

        stick::Size m_rowPadding;
    };

    template<class C>
    ImageT<C>::ImageT(stick::Allocator & _alloc) :
        m_data(_alloc),
        m_width(0),
        m_height(0),
        m_depth(0),
        m_rowPadding(0)
    {

    }

    template<class C>
    ImageT<C>::ImageT(stick::Size _width, stick::Size _height, stick::Size _depth, stick::Size _rowPadding, stick::Allocator & _alloc) :
        m_data(_alloc),
        m_width(_width),
        m_height(_height),
        m_depth(_depth),
        m_rowPadding(_rowPadding)
    {
        updateArraySize();
    }
    template<class C>
    ImageT<C>::ImageT(stick::Size _width, const ValueType * _pixels, stick::Size _rowPadding, stick::Allocator & _alloc) :
        m_data(_alloc)
    {
        loadPixels(_width, 1, 1, _pixels, _rowPadding);
    }

    template<class C>
    ImageT<C>::ImageT(stick::Size _width, stick::Size _height, const ValueType * _pixels, stick::Size _rowPadding, stick::Allocator & _alloc) :
        m_data(_alloc)
    {
        loadPixels(_width, _height, 1, _pixels, _rowPadding);
    }

    template<class C>
    ImageT<C>::ImageT(stick::Size _width, stick::Size _height, stick::Size _depth, const ValueType * _pixels, stick::Size _rowPadding, stick::Allocator & _alloc) :
        m_data(_alloc)
    {
        loadPixels(_width, _height, _depth, _pixels, _rowPadding);
    }

    template<class C>
    ImageT<C>::ImageT(const ImageT & _other) :
        m_data(_other.m_data),
        m_width(_other.m_width),
        m_height(_other.m_height),
        m_depth(_other.m_depth),
        m_rowPadding(_other.m_rowPadding)
    {

    }

    template<class C>
    ImageT<C>::ImageT(ImageT && _other) :
        m_data(stick::move(_other.m_data)),
        m_width(stick::move(_other.m_width)),
        m_height(stick::move(_other.m_height)),
        m_depth(stick::move(_other.m_depth)),
        m_rowPadding(stick::move(_other.m_rowPadding))
    {

    }

    template<class C>
    Image * ImageT<C>::clone(stick::Allocator & _alloc) const
    {
        return _alloc.create<ImageT>(*this);
    }

    template<class C>
    ImageT<C> & ImageT<C>::operator = (const ImageT & _other)
    {
        m_data = _other.m_data;
        m_width = _other.m_width;
        m_height = _other.m_height;
        m_depth = _other.m_depth;
        m_rowPadding = _other.m_rowPadding;

        return *this;
    }

    template<class C>
    ImageT<C> & ImageT<C>::operator = (ImageT && _other)
    {
        m_data = stick::move(_other.m_data);
        m_width = _other.m_width;
        m_height = _other.m_height;
        m_depth = _other.m_depth;
        m_rowPadding = _other.m_rowPadding;

        return *this;
    }

    template<class C>
    void ImageT<C>::resize(stick::Size _width, stick::Size _height)
    {
        m_width = _width;
        m_height = _height;
        m_depth = 1;
        updateArraySize();
    }

    template<class C>
    void ImageT<C>::resize(stick::Size _width, stick::Size _height, stick::Size _depth, stick::Size _rowPadding)
    {
        m_width = _width;
        m_height = _height;
        m_depth = _depth;
        m_rowPadding = _rowPadding;
        updateArraySize();
    }

    template<class C>
    void ImageT<C>::clear()
    {
        m_data.clear();
    }

    template<class C>
    void ImageT<C>::loadPixels(stick::Size _width, stick::Size _height, const ValueType * _pixels, stick::Size _rowPaddingToSkip)
    {
        loadPixels(_width, _height, 1, _pixels, _rowPaddingToSkip);
    }

    template<class C>
    void ImageT<C>::loadPixels(stick::Size _width, stick::Size _height, stick::Size _depth, const ValueType * _pixels, stick::Size _rowPadding)
    {
        resize(_width, _height, _depth, _rowPadding);
        memcpy(&m_data[0], _pixels, m_data.count());
    }

    template<class C>
    void ImageT<C>::updatePixels(stick::Size _left, stick::Size _top, stick::Size _width, stick::Size _height, const ValueType * _pixels)
    {
        updatePixels(_left, _top, 0, _width, _height, 1, _pixels);
    }

    template<class C>
    void ImageT<C>::updatePixels(stick::Size _left, stick::Size _top, stick::Size _layer, stick::Size _width, stick::Size _height, stick::Size _depth, const ValueType * _pixels)
    {
        stick::Size layerWidth = bytesPerRow() * m_height;
        stick::Size byteOffset = ((_layer * layerWidth) + _top * bytesPerRow() + _left * bytesPerPixel());
        stick::Size rowWidth = _width * bytesPerPixel();
        stick::Size pixelOff = 0;

        for (stick::Size z = 0; z < _depth; ++z)
        {
            stick::Size startOff = byteOffset;
            for (stick::Size y = 0; y < _height; ++y)
            {
                memcpy(&m_data[0] + byteOffset, _pixels + pixelOff, rowWidth);
                byteOffset += bytesPerRow();
                pixelOff += rowWidth;
            }
            byteOffset = startOff + layerWidth;
        }
    }

    template<class C>
    void ImageT<C>::loadRawPixels(stick::Size _width, stick::Size _height, stick::Size _depth, const char * _src, stick::Size _rowPadding)
    {
        loadPixels(_width, _height, _depth, reinterpret_cast<const ValueType *>(_src), _rowPadding);
    }

    template<class C>
    void ImageT<C>::updateRawPixels(stick::Size _left, stick::Size _top, stick::Size _layer, stick::Size _width, stick::Size _height, stick::Size _depth, const char * _src)
    {
        updatePixels(_left, _top, _layer, _width, _height, _depth, reinterpret_cast<const ValueType *>(_src));
    }

    template<class C>
    void ImageT<C>::setPixel(stick::Size _left, stick::Size _top, const ValueType * _value)
    {
        pixel(_left, _top).setValue(_value);
    }

    template<class C>
    void ImageT<C>::setPixel(stick::Size _left, stick::Size _top, stick::Size _layer, const ValueType * _value)
    {
        pixel(_left, _top, _layer).setValue(_value);
    }

    template<class C>
    void ImageT<C>::setPixelRaw(stick::Size _left, stick::Size _top, const char * _data)
    {
        setPixel(_left, _top, reinterpret_cast<const ValueType *>(_data));
    }

    template<class C>
    void ImageT<C>::setPixelRaw(stick::Size _left, stick::Size _top, stick::Size _layer, const char * _data)
    {
        setPixel(_left, _top, _layer, reinterpret_cast<const ValueType *>(_data));
    }

    template<class C>
    void ImageT<C>::flipRows()
    {
        stick::Size bpr = bytesPerRow();
        stick::Size wh = bpr * m_height;
        for (stick::Size z = 0; z < m_depth; ++z)
        {
            auto layerStart = m_data.begin() + (z * wh);
            for (stick::Size y = 0; y < m_height / 2; ++y)
            {
                auto start = layerStart + y * bpr;
                std::swap_ranges(start, start + bpr, layerStart + (m_height - y - 1) * bpr);
            }
        }
    }

    template<class C>
    void ImageT<C>::flipColumns()
    {
        stick::Size bpr = bytesPerRow();
        stick::Size wh = bpr * m_height;
        for (stick::Size z = 0; z < m_depth; ++z)
        {
            for (stick::Size y = 0; y < m_height; ++y)
            {
                Pixel * rowStart = (Pixel *)(&m_data[0] + (z * wh) + y * bpr);
                std::reverse(rowStart, rowStart + m_width);
            }
        }
    }

    template<class C>
    void ImageT<C>::flipLayers()
    {
        stick::Size bpr = bytesPerRow();
        stick::Size wh = bpr * m_height;
        for (stick::Size z = 0; z < m_depth / 2; ++z)
        {
            auto start = m_data.begin() + z * wh;
            std::swap_ranges(start, start + wh, m_data.begin() + (m_depth - z - 1) * wh);
        }
    }

    template<class C>
    void ImageT<C>::flip()
    {
        flipLayers();
        flipRows();
        flipColumns();
    }

    template<class C>
    void ImageT<C>::swapChannels(stick::UInt32 _a, stick::UInt32 _b)
    {
        for (stick::Size z = 0; z < m_depth; ++z)
        {
            for (stick::Size y = 0; y < m_height; ++y)
            {
                for (stick::Size x = 0; x < m_width; ++x)
                {
                    Pixel & p = pixel(x, y, z);
                    std::swap(p.channel(_a), p.channel(_b));
                }
            }
        }
    }

    template<class C>
    const typename ImageT<C>::Pixel & ImageT<C>::pixel(stick::Size _left, stick::Size _top) const
    {
        return reinterpret_cast<const Pixel &>(m_data[_top * bytesPerRow() + _left * bytesPerPixel()]);
    }

    template<class C>
    const typename ImageT<C>::Pixel & ImageT<C>::pixel(stick::Size _left, stick::Size _top, stick::Size _layer) const
    {
        return reinterpret_cast<const Pixel &>(m_data[(_layer * bytesPerRow() * m_height) + _top * bytesPerRow() + _left * bytesPerPixel()]);
    }

    template<class C>
    typename ImageT<C>::Pixel & ImageT<C>::pixel(stick::Size _left, stick::Size _top)
    {
        return reinterpret_cast<Pixel &>(m_data[_top * bytesPerRow() + _left * bytesPerPixel()]);
    }

    template<class C>
    typename ImageT<C>::Pixel & ImageT<C>::pixel(stick::Size _left, stick::Size _top, stick::Size _layer)
    {
        return reinterpret_cast<Pixel &>(m_data[(_layer * bytesPerRow() * m_height) + _top * bytesPerRow() + _left * bytesPerPixel()]);
    }

    template<class C>
    stick::Size ImageT<C>::width() const
    {
        return m_width;
    }

    template<class C>
    stick::Size ImageT<C>::height() const
    {
        return m_height;
    }

    template<class C>
    stick::Size ImageT<C>::depth() const
    {
        return m_depth;
    }

    template<class C>
    void ImageT<C>::updateArraySize()
    {
        m_data.resize(bytesPerRow() * m_height * m_depth);
    }

    template<class C>
    stick::Size ImageT<C>::byteCount() const
    {
        return m_data.count();
    }

    template<class C>
    stick::Size ImageT<C>::bytesPerRow() const
    {
        return m_width * sizeof(ValueType) * channelCount() + m_rowPadding;
    }

    template<class C>
    stick::Size ImageT<C>::rowPadding() const
    {
        return m_rowPadding;
    }

    template<class C>
    stick::UInt32 ImageT<C>::channelCount() const
    {
        return ChannelLayout::channelCount();
    }

    template<class C>
    stick::Size ImageT<C>::bitsPerPixel() const
    {
        return ChannelLayout::channelCount() * bitsPerChannel();
    }

    template<class C>
    stick::Size ImageT<C>::bytesPerPixel() const
    {
        return sizeof(Pixel);
    }

    template<class C>
    stick::Size ImageT<C>::bitsPerChannel() const
    {
        return sizeof(ValueType) * 8;
    }

    template<class C>
    stick::TypeID ImageT<C>::channelLayoutTypeID() const
    {
        return ChannelLayout::channelLayoutTypeID();
    }

    template<class C>
    stick::TypeID ImageT<C>::pixelTypeID() const
    {
        return Pixel::pixelTypeID();
    }

    template<class C>
    stick::TypeID ImageT<C>::valueTypeID() const
    {
        return Pixel::valueTypeID();
    }

    template<class C>
    typename ImageT<C>::PixelIter ImageT<C>::begin()
    {
        return PixelIter(*this);
    }

    template<class C>
    typename ImageT<C>::PixelIter ImageT<C>::end()
    {
        return PixelIter(*this, pixelCount());
    }

    template<class C>
    typename ImageT<C>::PixelConstIter ImageT<C>::begin() const
    {
        return PixelConstIter(*this);
    }

    template<class C>
    typename ImageT<C>::PixelConstIter ImageT<C>::end() const
    {
        return PixelIter(*this, pixelCount());
    }

    template<class C>
    const typename ImageT<C>::ValueType * ImageT<C>::ptr() const
    {
        return reinterpret_cast<const ValueType *>(&m_data[0]);
    }

    template<class C>
    typename ImageT<C>::ValueType * ImageT<C>::ptr()
    {
        return reinterpret_cast<ValueType *>(&m_data[0]);
    }

    template<class C>
    stick::Size ImageT<C>::pixelCount() const
    {
        return m_width * m_depth * m_height;
    }

    template<class C>
    bool ImageT<C>::isFloatingPoint() const
    {
        return Pixel::isFloatingPoint();
    }

    template<class C>
    bool ImageT<C>::hasAlpha() const
    {
        return Pixel::hasAlpha();
    }

    template<class C>
    stick::Int32 ImageT<C>::alphaPosition() const
    {
        return Pixel::alphaPosition();
    }

    template<class C>
    const char * ImageT<C>::bytePtr() const
    {
        return reinterpret_cast<const char *>(ptr());
    }

    typedef ImageT<ChannelsGray8> ImageGray8;
    typedef ImageT<ChannelsGrayAlpha8> ImageGrayAlpha8;
    typedef ImageT<ChannelsAlphaGray8> ImageAlphaGray8;
    typedef ImageT<ChannelsRGB8> ImageRGB8;
    typedef ImageT<ChannelsBGR8> ImageBGR8;
    typedef ImageT<ChannelsRGBA8> ImageRGBA8;
    typedef ImageT<ChannelsBGRA8> ImageBGRA8;
    typedef ImageT<ChannelsARGB8> ImageARGB8;
    typedef ImageT<ChannelsABGR8> ImageABGR8;

    typedef ImageT<ChannelsGray16> ImageGray16;
    typedef ImageT<ChannelsGrayAlpha16> ImageGrayAlpha16;
    typedef ImageT<ChannelsAlphaGray16> ImageAlphaGray16;
    typedef ImageT<ChannelsRGB16> ImageRGB16;
    typedef ImageT<ChannelsBGR16>  ImageBGR16;
    typedef ImageT<ChannelsRGBA16> ImageRGBA16;
    typedef ImageT<ChannelsBGRA16> ImageBGRA16;
    typedef ImageT<ChannelsARGB16> ImageARGB16;
    typedef ImageT<ChannelsABGR16> ImageABGR16;

    typedef ImageT<ChannelsGray32> ImageGray32;
    typedef ImageT<ChannelsGrayAlpha32> ImageGrayAlpha32;
    typedef ImageT<ChannelsAlphaGray32> ImageAlphaGray32;
    typedef ImageT<ChannelsRGB32> ImageRGB32;
    typedef ImageT<ChannelsBGR32>  ImageBGR32;
    typedef ImageT<ChannelsRGBA32> ImageRGBA32;
    typedef ImageT<ChannelsBGRA32> ImageBGRA32;
    typedef ImageT<ChannelsARGB32> ImageARGB32;
    typedef ImageT<ChannelsABGR32> ImageABGR32;

    typedef ImageT<ChannelsGray32f> ImageGray32f;
    typedef ImageT<ChannelsGrayAlpha32f> ImageGrayAlpha32f;
    typedef ImageT<ChannelsAlphaGray32f> ImageAlphaGray32f;
    typedef ImageT<ChannelsRGB32f> ImageRGB32f;
    typedef ImageT<ChannelsBGR32f>  ImageBGR32f;
    typedef ImageT<ChannelsRGBA32f> ImageRGBA32f;
    typedef ImageT<ChannelsBGRA32f> ImageBGRA32f;
    typedef ImageT<ChannelsARGB32f> ImageARGB32f;
    typedef ImageT<ChannelsABGR32f> ImageABGR32f;

}

#endif //PIC_IMAGE_HPP
