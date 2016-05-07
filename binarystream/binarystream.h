#ifndef BASIC_BINARYBUF_H
#define BASIC_BINARYBUF_H

#include <istream>
#include <ostream>
#include <vector>

namespace cc
{
    template<class elem_type, class out_it_type = std::ostreambuf_iterator<elem_type, std::char_traits<elem_type> > >
    class binary_num_put : public std::num_put<elem_type, out_it_type>
    {
    public:
        typedef binary_num_put<elem_type, out_it_type>	binary_num_put_type;
        typedef std::num_put<elem_type, out_it_type>	num_put_type;
        virtual ~binary_num_put() {};
        
    protected:
        virtual out_it_type do_put(out_it_type dest_it,
                                   std::ios_base& ios_base, elem_type fill, long val) const
        {	// put formatted long to _Dest
            const char* val_ptr = reinterpret_cast<const char *>(&val);
            const int val_size = sizeof(val);
            out_it_type& result_it = dest_it;
            for (int i = 0; i < val_size; ++i)
            {
                *result_it = val_ptr[i];
                ++result_it;
            }
            
            return result_it;
        }
    };
    
    template<typename _CharT, typename _Traits, typename _Alloc>
    class basic_binarybuf : public std::basic_streambuf<_CharT, _Traits>
    {
    public:
        typedef _CharT                              char_type;
        typedef _Traits                             traits_type;
        typedef _Alloc                              allocator_type;
        typedef typename traits_type::int_type      int_type;
        typedef typename traits_type::pos_type      pos_type;
        typedef typename traits_type::off_type      off_type;
        
        typedef std::basic_streambuf<char_type, traits_type>    streambuf_type;
        typedef std::vector<char_type, allocator_type>          vector_type;
        typedef std::ios_base::openmode                         openmode_type;
        
    protected:
        openmode_type               _open_mode;
        vector_type                 _vector;
        allocator_type              _allocator;
        
    public:
        explicit basic_binarybuf(std::ios_base::openmode __mode = std::ios_base::in | std::ios_base::out)
        : streambuf_type(), _open_mode(__mode), _vector()
        {
        }
        
        vector_type vector() const
        {
            vector_type ret;
            if (this->pptr())
            {
                // The current egptr() may not be the actual string end.
                if (this->pptr() > this->egptr())
                    ret = vector_type(this->pbase(), this->pptr());
                else
                    ret = vector_type(this->pbase(), this->egptr());
            }
            else
                ret = _vector;
            return ret;
        }
        
    protected:
        virtual void imbue(const std::locale& loc)
        {
            streambuf_type::imbue(loc);
        }
        
        virtual int_type overflow(int_type meta = traits_type::eof())
        {
            // put an element to stream when overflow
            _vector.push_back(meta);
            return meta;
        }
    };
    
    template<typename _CharT, typename _Traits = std::char_traits<_CharT>,
    typename _Alloc = std::allocator<_CharT> >
    class basic_binarystream : public std::basic_iostream<_CharT, _Traits>
    {
    public:
        typedef _CharT                              char_type;
        typedef _Traits                             traits_type;
        typedef _Alloc                              allocator_type;
        typedef typename traits_type::int_type      int_type;
        typedef typename traits_type::pos_type      pos_type;
        typedef typename traits_type::off_type      off_type;
        
        
        typedef basic_binarybuf<_CharT, _Traits, _Alloc>        binarybuf_type;
        typedef std::basic_iostream<char_type, traits_type>     iostream_type;
        typedef std::basic_ostream<char_type, traits_type>      ostream_type;
        typedef std::vector<char_type, allocator_type>          vector_type;
        typedef basic_binarystream<_CharT, _Traits, _Alloc>     binarystream_type;
        typedef binary_num_put<char_type>                       num_put_type;
        
    private:
        mutable binarybuf_type	_binarybuf;
        
    public:
        explicit basic_binarystream(std::ios_base::openmode openmode = std::ios_base::out | std::ios_base::in)
        : iostream_type(&_binarybuf), _binarybuf(openmode)
        {
            this->init(&_binarybuf);
            
            num_put_type* binary_num_put = new num_put_type();
            const std::locale& locale = iostream_type::getloc();
            this->imbue(std::locale(locale, binary_num_put));
        }
        
        binarybuf_type* rdbuf() const
        {
            return &_binarybuf;
        }
        
        vector_type vector() const
        {
            return _binarybuf.vector();
        }
    };
    
    typedef basic_binarystream<char> binarystream;
}

#endif // BASIC_BINARYBUF_H
