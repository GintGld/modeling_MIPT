#pragma once

#include <iostream>
#include <fstream>

class _message_log {
    std::ofstream out_stream;
    std::string filename, start_phrase, stop_phrase;
    bool opened;

public:
    _message_log(): opened(false) {}
    _message_log(
        const std::string& _filename,
        const std::string& _start_phrase = "Start program",
        const std::string& _stop_phrase = "Stop program"
    ) {open(_filename, _start_phrase, _stop_phrase);}

    ~ _message_log() {close();}

    bool open(
        const std::string& _filename = "prog",
        const std::string& _start_phrase = "Start program",
        const std::string& _stop_phrase = "Stop program"
    ) {
        filename = _filename+".log"; start_phrase = _start_phrase; stop_phrase = _stop_phrase;
        out_stream.open(filename);
        if (!out_stream.good())
            return false;
        opened = true;
        out_stream << start_phrase << "\n";
        return true;
    }

    bool is_open() {return opened;}

    void close() {
        if (opened) {
            out_stream << stop_phrase << "\n";
            opened = false;
            out_stream.close();
        }
    }

    bool good() {return out_stream.good();}

    const std::string& get_filename() {return filename;}

    template<class T>
    _message_log& operator <<(const T message) {
        if (opened)
            out_stream << message;
        return *this;
    }
};