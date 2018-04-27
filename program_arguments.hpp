#ifndef PROGRAMARGUMENTS_HPP
#define PROGRAMARGUMENTS_HPP


#include <iostream>
#include <sstream>
#include <map>


class ProgramArgument
{

    ProgramArgument(const std::string& name, const std::string& default_string)
        : _name_{name}
        , _default_string_{default_string}
        , _value_string_{default_string}
    {
    }

    void Set(const std::string& value_string)
    {
        _value_string_ = value_string;
    }

    std::string Get() const
    {
        return _value_string_;
    }

    private:

    std::string _name_;
    std::string _default_string_;
    std::string _value_string_;

};

class ProgramArguments
{



    public:

    void Process(int argc, char* argv[], std::ofstream& error_stream = std::cerr)
    {

        for(int ix{1}; ix < argc; ++ ix)
        {

            // current argument
            std::string arg(argv[ix]);

            ArgumentMapIterator_t it{_argument_map_.find(arg)};
            if(it != _argument_map_.end())
            {
                if(++ ix < argc)
                {
                    // current value corresponding to arg
                    std::string value(argv[ix]);
                    it->second = value;
                }
                else
                {
                    error_stream << error_message_no_argument_following_trigger() << std::endl;
                }

            }
            else
            {
                error_stream << error_message_arg_trigger_not_found(arg, ix) << std::endl;
            }

        }

    }

    void Add(const std::string& arg_trigger, const std::string& arg_name, const std::string& arg_defaut_string, std::ofstream& error_stream = std::cerr)
    {

        ArgumentMapIterator_t it{_argument_map_.find(arg)};
        if(it == _argument_map_.end())
        {
            ProgramArgument new_program_argument(arg_name, arg_default_string);
            _argument_map_[arg_trigger] = new_program_argument;
        }
        else
        {
            error_stream << error();
        }
    }

    private:

    std::string error_message_no_argument_arg_namewing_trigger(const std::string& arg_trigger, const std::string& arg_name, const std::string arg_type) const
    {
        std::stringstream ret;
        ret << "[ ERROR ] : " << arg_trigger << " " << arg_name << std::endl;
        ret << "[       ] : " << expected " " << arg_type << " " << follow_arg << " following argument " << arg_trigger << std::endl;
        return ret.str();
    }

    std::string error_message_arg_trigger_not_found(const std::string& arg, const int index) const
    {
        std::stringstream ret;
        ret << "[ ERROR ] : unrecognized argument in ProgramArguments::Process(): " << arg << " at index " << index << std::endl;
        return ret.str();
    }


    private:

    // todo: at the moment sort this string using arg_trigger variable as the key
    // however should sort using the arg_name key for retrival later!
    // TODO: implement a new map
    std::map<const std::string, ProgramArgument> _argument_map_;
    typedef std::map<const std::string, ProgramArgument>::iterator ArgumentMapIterator_t;

};

#endif
