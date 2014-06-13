#pragma once

typedef boost::function<bool (buffer_reader_t, buffer_writer_t)> handler_t;
typedef std::array<handler_t, 128> handler_array_t;