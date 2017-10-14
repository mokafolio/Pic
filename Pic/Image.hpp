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

        virtual stick::Size bytesPerPixel() const = 0;

        virtual stick::Size rowPadding() const = 0;

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

    typedef stick::UniquePtr<Image> ImageUniquePtr;

    STICK_API stick::Result<ImageUniquePtr> decodeImage(const stick::ByteArray & _data, stick::Allocator & _alloc = stick::defaultAllocator());

    STICK_API stick::Result<ImageUniquePtr> decodeImage(const void * _data, stick::Size _byteCount, stick::Allocator & _alloc = stick::defaultAllocator());

    STICK_API stick::Result<ImageUniquePtr> loadImage(const stick::URI & _path, stick::Allocator & _alloc = stick::defaultAllocator());


    template<class PixelT>
    class STICK_API ImageT : public Image
    {
    public:

        typedef typename PixelT::ChannelLayout ChannelLayout;

        typedef typename PixelT::ValueType ValueType;

        typedef PixelT Pixel;

        typedef stick::DynamicArray<char> DataArray;

        typedef PixelIteratorT<ImageT, char*> PixelIter;

        typedef PixelIteratorT<const ImageT, const char*> PixelConstIter;

        static constexpr stick::TypeID channelLayoutTID = ChannelLayout::TypeInfo::typeID();

        static constexpr stick::TypeID pixelTID = Pixel::TypeInfo::typeID();


        ImageT(stick::Allocator & _alloc = stick::defaultAllocator());

        ImageT(stick::Size _width, stick::Size _height, stick::Size _depth = 1, stick::Size _rowPadding = 0, stick::Allocator & _alloc = stick::defaultAllocator());

        ImageT(stick::Size _width, const ValueType * _pixels, stick::Size _rowPadding = 0, stick::Allocator & _alloc = stick::defaultAllocator());

        ImageT(stick::Size _width, stick::Size _height, const ValueType * _pixels, stick::Size _rowPadding = 0, stick::Allocator & _alloc = stick::defaultAllocator());

        ImageT(stick::Size _width, stick::Size _height, stick::Size _depth, const ValueType * _pixels, stick::Size _rowPadding = 0, stick::Allocator & _alloc = stick::defaultAllocator());

        ImageT(const ImageT & _other);

        ImageT(ImageT && _other);

        ImageT & operator = (const ImageT & _other);

        ImageT & operator = (ImageT && _other);

        Image * clone(stick::Allocator & _alloc = stick::defaultAllocator()) const override;

        void resize(stick::Size _width, stick::Size _height) override;

        void resize(stick::Size _width, stick::Size _height, stick::Size _depth, stick::Size _rowPadding = 0) override;

        void clear();

        void loadPixels(stick::Size _width, stick::Size _height, const ValueType * _pixels, stick::Size _rowPadding = 0);

        void loadPixels(stick::Size _width, stick::Size _height, stick::Size _depth, const ValueType * _pixels, stick::Size _rowPadding = 0);

        void updatePixels(stick::Size _left, stick::Size _top, stick::Size _width, stick::Size _height, const ValueType * _pixels);

        void updatePixels(stick::Size _left, stick::Size _top, stick::Size _layer, stick::Size _width, stick::Size _height, stick::Size _depth, const ValueType * _pixels);

        void loadRawPixels(stick::Size _width, stick::Size _height, stick::Size _depth, const char * _src, stick::Size _rowPadding = 0) override;

        void updateRawPixels(stick::Size _left, stick::Size _top, stick::Size _layer, stick::Size _width, stick::Size _height, stick::Size _depth, const char * _src) override;

        void setPixel(stick::Size _left, stick::Size _top, const ValueType * _value);

        void setPixel(stick::Size _left, stick::Size _top, stick::Size _layer, const ValueType * _value);

        void setPixelRaw(stick::Size _left, stick::Size _top, const char * _data) override;

        void setPixelRaw(stick::Size _left, stick::Size _top, stick::Size _layer, const char * _data) override;


        //flips all the pixel rows
        void flipRows() override;

        void flipColumns() override;

        void flipLayers() override;

        void flip() override;

        void swapChannels(stick::UInt32 _a, stick::UInt32 _b) override;


        const Pixel & pixel(stick::Size _left, stick::Size _top) const;

        const Pixel & pixel(stick::Size _left, stick::Size _top, stick::Size _layer) const;

        Pixel & pixel(stick::Size _left, stick::Size _top);

        Pixel & pixel(stick::Size _left, stick::Size _top, stick::Size _layer);


        PixelIter begin();

        PixelIter end();

        PixelConstIter begin() const;

        PixelConstIter end() const;


        stick::Size width() const override;

        stick::Size height() const override;

        stick::Size depth() const override;

        stick::Size byteCount() const override;

        stick::Size bytesPerRow() const override;

        stick::Size pixelCount() const override;

        stick::Size rowPadding() const override;

        stick::UInt32 channelCount() const override;

        stick::Size bitsPerPixel() const override;

        stick::Size bitsPerChannel() const override;

        stick::Size bytesPerPixel() const override;

        stick::TypeID channelLayoutTypeID() const override;

        stick::TypeID pixelTypeID() const override;

        stick::TypeID valueTypeID() const override;

        const ValueType * ptr() const;

        ValueType * ptr();

        bool isFloatingPoint() const override;

        bool hasAlpha() const override;

        stick::Int32 alphaPosition() const override;

        const char * bytePtr() const override;

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
        m_data(std::move(_other.m_data)),
        m_width(std::move(_other.m_width)),
        m_height(std::move(_other.m_height)),
        m_depth(std::move(_other.m_depth)),
        m_rowPadding(std::move(_other.m_rowPadding))
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
        m_data = std::move(_other.m_data);
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

    typedef ImageT<PixelGray8> ImageGray8;
    typedef ImageT<PixelGrayAlpha8> ImageGrayAlpha8;
    typedef ImageT<PixelAlphaGray8> ImageAlphaGray8;
    typedef ImageT<PixelRGB8> ImageRGB8;
    typedef ImageT<PixelBGR8> ImageBGR8;
    typedef ImageT<PixelRGBA8> ImageRGBA8;
    typedef ImageT<PixelBGRA8> ImageBGRA8;
    typedef ImageT<PixelARGB8> ImageARGB8;
    typedef ImageT<PixelABGR8> ImageABGR8;

    typedef ImageT<PixelGray16> ImageGray16;
    typedef ImageT<PixelGrayAlpha16> ImageGrayAlpha16;
    typedef ImageT<PixelAlphaGray16> ImageAlphaGray16;
    typedef ImageT<PixelRGB16> ImageRGB16;
    typedef ImageT<PixelBGR16>  ImageBGR16;
    typedef ImageT<PixelRGBA16> ImageRGBA16;
    typedef ImageT<PixelBGRA16> ImageBGRA16;
    typedef ImageT<PixelARGB16> ImageARGB16;
    typedef ImageT<PixelABGR16> ImageABGR16;

    typedef ImageT<PixelGray32> ImageGray32;
    typedef ImageT<PixelGrayAlpha32> ImageGrayAlpha32;
    typedef ImageT<PixelAlphaGray32> ImageAlphaGray32;
    typedef ImageT<PixelRGB32> ImageRGB32;
    typedef ImageT<PixelBGR32>  ImageBGR32;
    typedef ImageT<PixelRGBA32> ImageRGBA32;
    typedef ImageT<PixelBGRA32> ImageBGRA32;
    typedef ImageT<PixelARGB32> ImageARGB32;
    typedef ImageT<PixelABGR32> ImageABGR32;

    typedef ImageT<PixelGray32f> ImageGray32f;
    typedef ImageT<PixelGrayAlpha32f> ImageGrayAlpha32f;
    typedef ImageT<PixelAlphaGray32f> ImageAlphaGray32f;
    typedef ImageT<PixelRGB32f> ImageRGB32f;
    typedef ImageT<PixelBGR32f>  ImageBGR32f;
    typedef ImageT<PixelRGBA32f> ImageRGBA32f;
    typedef ImageT<PixelBGRA32f> ImageBGRA32f;
    typedef ImageT<PixelARGB32f> ImageARGB32f;
    typedef ImageT<PixelABGR32f> ImageABGR32f;
}

#endif //PIC_IMAGE_HPP
