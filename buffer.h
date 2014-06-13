#pragma once

typedef std::vector<char> buffer_t;
typedef boost::shared_ptr<buffer_t> buffer_ref;

class buffer_reader_t {
public:
    buffer_reader_t(const char* buffer)
        : _buffer(buffer) {}

    template <typename _Ty>
    friend buffer_reader_t& operator >> (buffer_reader_t& reader, _Ty& value) {
        value = *reinterpret_cast<const _Ty*>(reader._buffer);
        reader._buffer += sizeof(_Ty);
        return reader;
    }
private:
    const char* _buffer;
};

class buffer_writer_t {
public:
    buffer_writer_t()
        : _buffer(boost::make_shared<buffer_t>()) {}

    template <typename _Ty>
    friend buffer_writer_t& operator << (buffer_writer_t& writer, const _Ty& value) {
        const char* byte_begin = reinterpret_cast<const char*>(&value);
        const char* byte_end = byte_begin + sizeof(value);
        std::copy(byte_begin, byte_end, std::back_inserter(*writer._buffer.get()));
        return writer;
    }

    buffer_ref buffer() { return _buffer; }
private:
    buffer_ref _buffer;
};
