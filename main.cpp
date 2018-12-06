

#include "program_arguments.hpp"



int main(int argc, char* argv[])
{

    ////////////////////////////////////////////////////////////////////////////
    // PROCESS PROGRAM ARGUMENTS
    ////////////////////////////////////////////////////////////////////////////

    // new argument processing class
    ProgramArguments pa;
    //pa.Add("filename", "--filename", "NewElectronNtuplizerExe_Int_ManDB_output.root");
    //pa.Add("epsilon", "--epsilon", "0.0");
    //pa.Add("batch_mode", "--batch-mode", "false");
    //pa.Add("help", "--help", "false");
    pa.Add("multi", "--multi", std::vector<std::string>{"one", "two"});
    pa.Print();
    pa.Process(argc, argv);

    // specify argument defaults, as string
    //const std::string arg_default_filename("NewElectronNtuplizerExe_Int_ManDB_output.root");
    //const std::string arg_default_epsilon_31("0.0");
    //const std::string arg_default_batch_mode("false");

    // specify actual argument, as string
    //std::string arg_filename(arg_default_filename);
    //std::string arg_epsilon_31(arg_default_epsilon_31);
    //std::string arg_batch_mode(arg_default_batch_mode);
    std::string arg_filename;
    std::string arg_epsilon_31;
    std::string arg_batch_mode;
    std::string arg_help;

    // specify actual argument, as required value, no value set yet
    std::string filename;
    double epsilon_31;
    bool batch_mode;
    std::string help;

    // specify argument trigger strings
    //std::string arg_trigger_filename("--filename");
    //std::string arg_trigger_epsilon_31("--epsilon");
    //std::string arg_trigger_batch_mode("--batch-mode");

    // process arguments
    for(int ix{1}; ix < argc; ++ ix)
    {
        // current argument
        std::string arg{std::string(argv[ix])};

        // TODO: this always true?
        if(ix < argc)
        {
            // switch arg, search for trigger
            /*
            if(arg == std::string("-h") || arg == std::string("--help"))
            {
                //print_general_help(std::cout, argv);
                std::cout << "todo: help" << std::endl;
                std::cout.flush();
            }
            
            else if(arg == arg_trigger_filename)
            {
                if(++ ix < argc)
                {
                    arg_filename = std::string(argv[ix]);
                }
                else
                {
                    std::cout << "[ WARNING ] : " << arg << " FILENAME" << std::endl;
                    std::cout << "[ WARNING ] : expected string FILENAME following argument " << arg << std::endl;
                }
            } 
            else if(arg == arg_trigger_epsilon_31)
            {
                if(++ ix < argc)
                {
                    arg_epsilon_31 = std::string(argv[ix]);
                }
                else
                {
                    std::cout << "[ WARNING ] : " << arg << " EPSILON_31" << std::endl;
                    std::cout << "[ WARNING ] : expected floating point EPSILON_31 following argument " << arg << std::endl;
                }
            }
            else if(arg == arg_trigger_batch_mode)
            {
                if(++ ix < argc)
                {
                    arg_batch_mode = std::string(argv[ix]);
                }
                else
                {
                    std::cout << "[ WARNING ] : " << arg << " BOOLEAN" << std::endl;
                    std::cout << "[ WARNING ] : expected [true/false] following argument " << arg << std::endl;
                }
            }
            else
            {
                std::cerr << "[ WARNING ] : unrecognized argument " << arg << " at index " << ix << std::endl;
            }
            */
        }
    }

    // process gathered argument data
    arg_filename = pa.Get("filename").at(0); // TODO
    filename = arg_filename;
    //std::string filename_default(arg_default_filename); // required for later if test
    std::string filename_default(pa.GetDefault("filename").at(0)); // required for later if test
    // TODO
    bool gen_weight_enable{false};
    //if(arg_filename != filename_default)
    if(filename != filename_default)
    {
        gen_weight_enable = true;
    }
    // TODO: check filename exists!

    // TODO
    arg_epsilon_31 = pa.Get("epsilon").at(0);
    epsilon_31 = std::stod(arg_epsilon_31);
    // todo: check valid
    if(0.0 <= epsilon_31 && epsilon_31 <= 0.8)
    {
        std::cout << "[ INFO ] : Set epsilon_31 = " << epsilon_31 << std::endl;
    }
    else
    {
        std::cout << "invalid epsilon_31 value" << std::endl;
        throw "invalid epsilon_31 value";
    }

    // TODO
    arg_batch_mode = pa.Get("batch_mode").at(0);
    if(arg_batch_mode == std::string("true"))
    {
        batch_mode = true;
        std::string batch_mode_enable_string("false");
        if(batch_mode)
        {
            batch_mode_enable_string = std::string("true");
        }
        std::cout << "[ INFO ] : Batch mode: " << batch_mode_enable_string << std::endl;
    }

    arg_help = pa.Get("help").at(0); // TODO
    if(arg_help == std::string("true"))
    {
        // print help
    }


}
