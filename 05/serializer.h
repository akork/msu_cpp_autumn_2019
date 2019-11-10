#pragma once

enum class Error { NoError, CorruptedArchive };

class Serializer
{
    static constexpr char Separator = ' ';

  public:
    explicit Serializer(std::ostream &out) : out_(out) {}

    template <class T> Error save(T &object) { return object.serialize(*this); }

    template <class... ArgsT> Error operator()(ArgsT... args)
    {
        return process(args...);
    }

  private:
    std::ostream &out_;
    Error process() { return Error::NoError; }

    template <class... Args> Error process(bool val, Args... args)
    {
        if (val) {
            out_ << "true" << Separator;
        } else {
            out_ << "false" << Separator;
        }
        return process(args...);
    }

    template <class... Args> Error process(uint64_t val, Args... args)
    {
        out_ << val << Separator;
        return process(args...);
    }
};

class Deserializer
{
    static constexpr char Separator = ' ';

  public:
    explicit Deserializer(std::istream &in) : in_(in) {}

    template <class T> Error load(T &object) { return object.serialize(*this); }

    template <class... ArgsT> Error operator()(ArgsT &... args)
    {
        return process(args...);
    }

  private:
    std::istream &in_;
    Error process() { return Error::NoError; }
    template <class... Args> Error process(bool &val, Args &... args)
    {
        std::string text;
        in_ >> text;

        if (text == "true")
            val = true;
        else if (text == "false")
            val = false;
        else
            return Error::CorruptedArchive;
        return process(args...);
    }

    template <class... Args> Error process(uint64_t &val, Args &... args)
    {
        std::string text;
        in_ >> text;
        try {
            if (text.find(".") != std::string::npos)
                return Error::CorruptedArchive;
            size_t idx;
            double temp = stod(text, &idx);
            if (temp < 0 || idx != text.size())
                return Error::CorruptedArchive;
            val = stoul(text);
        } catch (std::exception &e) {
            return Error::CorruptedArchive;
        }
        return process(args...);
    }
};
