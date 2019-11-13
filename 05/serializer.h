#pragma once
#include <regex>

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

    Error process(bool val)
    {
        if (val) {
            out_ << "true" << Separator;
        } else {
            out_ << "false" << Separator;
        }
        return Error::NoError;
    }

    Error process(uint64_t val)
    {
        out_ << val << Separator;
        return Error::NoError;
    }

    template <class First, class... Args> Error process(First val, Args... args)
    {
        if (process(val) == Error::CorruptedArchive)
            return Error::CorruptedArchive;
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

    template <class First, class... argsT>
    Error process(First &val, argsT &... args)
    {
        if (process(val) == Error::CorruptedArchive)
            return Error::CorruptedArchive;
        return process(args...);
    }

    Error process(bool &val)
    {
        std::string text;
        in_ >> text;

        if (text == "true")
            val = true;
        else if (text == "false")
            val = false;
        else
            return Error::CorruptedArchive;
        return Error::NoError;
    }

    Error process(uint64_t &val)
    {
        std::string text;
        in_ >> text;
        try {
            if (!regex_match(text, std::regex("[0-9]+")))
                return Error::CorruptedArchive;
            val = stoul(text);
        } catch (std::exception &e) {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }
};
