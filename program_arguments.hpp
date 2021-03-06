#ifndef PROGRAMARGUMENTS_HPP
#define PROGRAMARGUMENTS_HPP


#include <memory>
#include <iostream>
#include <sstream>
#include <map>


// TODO: do not need to store the argument NAME
// only need to store the TRIGGER


class ProgramArgument
{

    public:

    ProgramArgument(const std::string& name, const std::string& default_string)
        //: _name_{name}
        /*,*/: _default_string_{default_string}
        , _value_string_{default_string}
        , _was_value_provided_{false}
    {
    }

    void SetValue(const std::string& value_string)
    {
        _was_value_provided_ = true;
        _value_string_ = value_string;
    }

    std::string GetValue() const
    {
        return _value_string_;
    }

    bool GetWasProvided() const
    {
        return _was_value_provided_;
    }

    /*
    std::string GetName() const
    {
        return _name_;
    }
    */

    std::string GetDefaultValue() const
    {
        return _default_string_;
    }

    private:

    //std::string _name_;
    std::string _default_string_;
    std::string _value_string_;

    // if ProgramArguments::Process() sets the value, then this is set
    bool _was_value_provided_;

};


// TODO: multiple triggers
// TODO: fix error messages
// TODO: conversion to double, bool
// TODO: error message to be printed when argument used incorrectly
// (this is a sub part of the help message)
// TODO: program usage message (part of the help message)
// TODO: does not work in the case where a program has 2 argument schemes
// example: range of values to execute algorithm:
// --epsilon VALUE
// or
// --epsilon-min VALUE --epsilon-max VALUE --epsilon-num VALUE
// or
// --epsilon-file FILENAME
// different prog behaviour required for each of these 3 options
// perhaps more than one can be specified simultantiously, or more than one set
// at once could be
// eg
// --epsilon VALUE1 --epsilon VALUE2 --epsilon VALUE3
// there is no flag to state if arg was provided (default value is for a
// different purpose)
// there is no ability to have multiple sets of args (vector of values)
class ProgramArguments
{



    public:

    bool WasProvided(const std::string& name, std::ostream& error_stream = std::cerr) const
    {
        // search for name
        NameConstIterator_t name_it{_name_map_.find(name)};
        if(name_it != _name_map_.end())
        {
            
            // name found
            return name_it->second->GetWasProvided();

        }
        else
        {
            error_stream << error_name_not_found(name) << std::endl;
            throw "error name not found";
        }
    }

    std::string Get(const std::string& name, std::ostream& error_stream = std::cerr) const
    {
        // search for name
        NameConstIterator_t name_it{_name_map_.find(name)};
        if(name_it != _name_map_.end())
        {
            
            // name found
            return name_it->second->GetValue();

        }
        else
        {
            error_stream << error_name_not_found(name) << std::endl;
            throw "error name not found";
        }
    }

    std::string GetDefault(const std::string& name, std::ostream& error_stream = std::cerr) const
    {
        
        // search for name
        NameConstIterator_t name_it{_name_map_.find(name)};
        if(name_it != _name_map_.end())
        {
            
            // name found
            return name_it->second->GetDefaultValue();

        }
        else
        {
            error_stream << error_name_not_found(name) << std::endl;
            throw "error name not found";
        }
    }

    // TODO: I think value can be set multiple times?
    // but multiple values are not accounted for in the ProgramArgument class
    void Process(int argc, char* argv[], std::ostream& error_stream = std::cerr)
    {

        for(int ix{1}; ix < argc; ++ ix)
        {

            // current argument
            std::string trigger(argv[ix]);

            // search for trigger
            TriggerIterator_t trigger_it{_trigger_map_.find(trigger)};
            if(trigger_it != _trigger_map_.end())
            {

                // trigger found
                if(++ ix < argc)
                {
                    // current value corresponding to arg
                    std::string value(argv[ix]);
                    trigger_it->second->SetValue(value);
                }
                else
                {
                    // no value following arg
                    // TODO: what if value is missing but another arg trigger is provided?
                    // eg: --filename --parameter 0.5
                    // the filename following --filename is missing
                    error_stream << error_no_argument_following_trigger(trigger, "NAME", "THE_TYPE", "FOLLOW_ARG") << std::endl;
                }

            }
            else
            {
                // trigger not found
                error_stream << error_arg_trigger_not_found(trigger, ix) << std::endl;
            }

        }

    }

    // TODO: this function cannot print the trigger following the name because it does not know which
    // trigger goes with which name?
    void Print(std::ostream& output_stream = std::cout)
    {
        output_stream << "*** Program Arguments ***\n";
        NameIterator_t it{_name_map_.begin()};
        for(; it != _name_map_.end(); )
        {
            output_stream << "Name: " << it->first << ", Value: " << /*it->second.GetName() << " " <<*/ it->second->GetValue() << ", Default: " << it->second->GetDefaultValue();
            if(++ it != _name_map_.end()) output_stream << "\n";
            else output_stream << std::endl;
        }

    }

    // NOTE: arg_name isn't a "name" it is a reference or key
    // NOTE: changed arg_name -> arg_key
    void Add(const std::string& arg_name, const std::string& arg_trigger, const std::string& arg_default_string, std::ostream& error_stream = std::cerr)
    {

        // check name and trigger are not used

        NameIterator_t it_name{_name_map_.find(arg_name)};
        if(it_name != _name_map_.end())
        {
            // error: name exists
            error_stream << error_name_exists(arg_name) << std::endl;
            throw "error name exists";
        }

        TriggerIterator_t it_trigger{_trigger_map_.find(arg_trigger)};
        if(it_trigger != _trigger_map_.end())
        {
            // error: trigger exists
            error_stream << error_trigger_exists(arg_trigger) << std::endl;
            throw "error trigger exists";
        }

        std::shared_ptr<ProgramArgument> program_argument_p(new ProgramArgument(arg_name, arg_default_string));
        _trigger_map_[arg_trigger] = program_argument_p;
        _name_map_[arg_name] = program_argument_p;

    }

    private:


    std::string error_no_argument_following_trigger(const std::string& arg_trigger, const std::string& arg_name, const std::string& arg_type, const std::string& follow_arg) const
    {
        std::stringstream ret;
        ret << "[ ERROR ] : " << arg_trigger << " " << arg_name << std::endl;
        ret << "[       ] : " << "expected " << arg_type << " " << follow_arg << " following argument " << arg_trigger << std::endl;
        return ret.str();
    }


    std::string error_arg_trigger_not_found(const std::string& arg, const int index) const
    {
        std::stringstream ret;
        ret << "[ ERROR ] : unrecognized argument in ProgramArguments::Process(): " << arg << " at index " << index << std::endl;
        return ret.str();
    }


    std::string error_name_exists(const std::string& name) const
    {
        std::stringstream ret;
        ret << "[ error ] : name " << name << " exists in map" << std::endl;
        ret << "          : ProgramArguments::Add()" << std::endl;
        return ret.str();
    }


    std::string error_trigger_exists(const std::string& trigger) const
    {
        std::stringstream ret;
        ret << "[ error ] : trigger " << trigger << " exists in map" << std::endl;
        ret << "          : ProgramArguments::Add()" << std::endl;
        return ret.str();
    }


    std::string error_name_not_found(const std::string& name) const
    {
        std::stringstream ret;
        ret << "[ ERROR ] : name " << name << " not found" << std::endl;
        ret << "          : ProgramArguments::Get()" << std::endl;
        return ret.str();
    }


    private:

    // todo: at the moment sort this string using arg_trigger variable as the key
    // however should sort using the arg_name key for retrival later!
    // TODO: implement a new map
    std::map<const std::string, std::shared_ptr<ProgramArgument>> _name_map_;
    std::map<const std::string, std::shared_ptr<ProgramArgument>> _trigger_map_;
    typedef std::map<const std::string, std::shared_ptr<ProgramArgument>>::iterator NameIterator_t;
    typedef std::map<const std::string, std::shared_ptr<ProgramArgument>>::iterator TriggerIterator_t;
    typedef std::map<const std::string, std::shared_ptr<ProgramArgument>>::const_iterator NameConstIterator_t;
    typedef std::map<const std::string, std::shared_ptr<ProgramArgument>>::const_iterator TriggerConstIterator_t;

    // TODO: implement as shared_ptr to allocated object

};

#endif
