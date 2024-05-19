#include <iostream>

#include <utilities/cmd_opt.h>

using namespace regulus::utilities;

int main(int argc, char * argv[])
{
    std::cout << std::boolalpha;
    CmdOpt cmd_opt;

// check empty option name
//    cmd_opt.add("", CmdOpt::ARG_NONE);

// check option name contains non-alphabet characters
//    cmd_opt.add("-abc", CmdOpt::ARG_NONE);

// check option name already exists
//    cmd_opt.add("a", CmdOpt::ARG_NONE);
//    cmd_opt.add("a", CmdOpt::ARG_OPTIONAL);

// check is_register() option work properly
//    cmd_opt.add("a", CmdOpt::ARG_NONE);
//    std::cout << cmd_opt.is_register("a") << std::endl;
//    std::cout << cmd_opt.is_register("b") << std::endl;

// check get_opt_type() work properly
//    cmd_opt.add("a", CmdOpt::ARG_NONE, CmdOpt::OPT_OPTIONAL);
//    cmd_opt.add("a", CmdOpt::ARG_NONE, CmdOpt::OPT_REQUIRED);
//    auto opt_type = cmd_opt.get_opt_type("a");
//    std::cout << opt_type << std::endl;

    cmd_opt.add("a", CmdOpt::ARG_NONE);
    cmd_opt.add("d", CmdOpt::ARG_NONE);
    cmd_opt.add("b", CmdOpt::ARG_OPTIONAL);
    cmd_opt.add("f", CmdOpt::ARG_OPTIONAL);
    cmd_opt.add("c", CmdOpt::ARG_REQUIRED);

    cmd_opt.add("abc", CmdOpt::ARG_NONE);
    cmd_opt.add("def", CmdOpt::ARG_OPTIONAL);
    cmd_opt.add("ghi", CmdOpt::ARG_REQUIRED);

    cmd_opt.parse(argc, argv);

    cmd_opt.show();

    return 0;
}
