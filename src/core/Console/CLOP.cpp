#include "CLOP.hpp"
/* Command line options.
* Usage:
* Each command has to be implemented in a c++ function with the following layout:
* void MyFunction( clop::ArgList )
* The ArgList contains n+1 elements, where n is the number of parameters passed through
* the command line. The first element is the name of the command.
* 
* To register a function simply call
* clop::Register( "CommandName", MyFunction )
* 
* To call a command from c++:
* clop::Command( "Command arg1 arg2 ... argN" );
* String arguments must be outlined by quotation marks.
*/
#include <string>
#include <sstream>
#include <map>
#include <iostream>
#include <vector>

#define CLOPSTR std::string
#define CLOPCH char
#define CLOPSS std::stringstream
#define TOSTRING(x) std::to_string(x)
#define CLOPOUT //std::cout
namespace clop
{
	Arg::Arg(){}
	Arg::~Arg(){}
	Arg::Arg( CLOPSTR s ){ set( s ); }
	Arg::Arg( int i ){ set( i ); }
	Arg::Arg( double d ){ set( d ); }
	void Arg::operator=( CLOPSTR s ){ set( s ); }
	void Arg::operator=( int i ){ set( i ); }
	void Arg::operator=( double d ){ set( d ); }

	Arg::operator int() const
	{
		int rv = 0;
		if( !val.empty() ){
			int i = 0;
			CLOPSS ss;
			ss << val;
			ss >> i;
			if( !ss.fail() )
				rv = i;
		}
		return rv;
	}
	Arg::operator double() const
	{
		double rv = 0.0;
		if( !val.empty() ){
			double d = 0.0;
			CLOPSS ss;
			ss << val;
			ss >> d;
			if( !ss.fail() )
				rv = d;
		}
		return rv;
	}
	Arg::operator CLOPSTR() const{ return val; }
	void Arg::set( CLOPSTR s ){ val = s; }
	void Arg::set( int i ){ val = TOSTRING( i ); }
	void Arg::set( double d ){ val = TOSTRING( d ); }

	CLI::CLI(){};
	CLI::CLI(CLI const&){}

	/* Splits a command into tokens */
	std::vector<Arg> CLI::parse_line( CLOPSTR cmd )
	{
		std::vector<Arg> args;
		CLOPSTR tmp;
		bool quote = false;
		for( CLOPSTR::iterator it = cmd.begin(); it != cmd.end(); ++it )
		{
			CLOPCH ch = *it;
			if( !quote )
			{
				if( ch == ' ' )
				{
					if( !tmp.empty() )
					{
						std::string tmp2(tmp.c_str());
						args.push_back( tmp2 );
						tmp.clear();
					}
				}
				else if( ch == '\"' )
				{
					//if( !tmp.empty() )
					//{
					//	args.push_back( tmp );
					//	tmp.clear();
					//}
					quote = true;
				}
				//else
				{
					tmp.push_back( ch );
				}
			}
			else
			{
				if( ch == '\"' )
				{
					//if( !tmp.empty() )
					//{
					//	args.push_back( tmp );
					//	tmp.clear();
					//}
					quote = false;
				}
				//else
				{
					tmp.push_back( ch );
				}
			}
		}

		// Add last command only if quotes are closed
		if( !tmp.empty() && !quote)
		{
			std::string tmp2(tmp.c_str());
			args.push_back( tmp2 );
			tmp.clear();
		}
		return args;

	}

	/* Calls the function associated with the command string cmd, with parameters in a void* array params. */
	bool CLI::call( CLOPSTR cmd, ArgList params )
	{
		if( callback_functions.find( cmd ) != callback_functions.end() )
		{
			FunPtr callback = callback_functions[cmd];
			callback(params);
			return true;
		}
		else // The command does not exist
		{
			//CLOPOUT << "Unknown command \"" << cmd << "\"\n";
			return false;
		}
	}

	/* Registers a callback and associates it with the string command cmd */
	void CLI::register_callback( CLOPSTR cmd, FunPtr cbptr )
	{
		if (cmd.compare("exit") == 0)
		{
			//char c = cmd[4];
			//int i = 0;
		}
		if( callback_functions.find( cmd ) == callback_functions.end() ) // The command does not exist
		{
			callback_functions[cmd] = cbptr;
		}
		else
		{
			//CLOPOUT << "Command \"" << cmd << "\" attempt to register failed: Already exists.\n";
		}
	}

	/* Calls the function associated with the specified command. */
	bool CLI::command( CLOPSTR cmd )
	{
		// Extract the parameters
		std::vector<Arg> args = parse_line( cmd );
		if( !args.empty() )
		{
			return call( args[0], args );
		}

        return false;
	}

	/* Returns a single instance of clop */
	CLI& cli()
	{
		static CLI cli;
		return cli;
	}
	
	/* Registers a function for specified command. */
	void Register( CLOPSTR cmd, FunPtr cbptr )
	{
		cli().register_callback( cmd, cbptr );
	}

	/* Parse and run a command cmd. */
	bool Command( CLOPSTR cmd )
	{
		return cli().command( cmd );
	}
}
#undef CLOPSTR
#undef CLOPCH
#undef CLOPSS
#undef TOSTRING
#undef CLOPOUT
