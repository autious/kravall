#ifndef CLOP_COMMAND_LINE_OPTIONS_H
#define CLOP_COMMAND_LINE_OPTIONS_H
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
#include <map>
#include <vector>

#define CLOPSTR std::string

namespace clop
{

	class Arg{
	public:
		Arg();
		~Arg();
		Arg( CLOPSTR s );
		Arg( int i );
		Arg( double d );
		void operator=( CLOPSTR s );
		void operator=( int i );
		void operator=( double d );
		operator int() const;
		operator double() const;
		operator CLOPSTR() const;

	private:
		void set( CLOPSTR s );
		void set( int i );
		void set( double d );

		CLOPSTR val;

	};
	
	typedef  std::vector<Arg> const&  ArgList ;
	typedef void(*FunPtr)(ArgList);

	class CLI
	{
		// Interface functions
		friend CLI& cli();
	private:
		friend void Register( CLOPSTR cmd, FunPtr cbptr );
		friend bool Command( CLOPSTR cmd );
		
		CLI();
		CLI(CLI const&);
		
		/* Splits a command into tokens */
		std::vector<Arg> parse_line( CLOPSTR cmd );

		/* Calls the function associated with the command string cmd, with parameters in a void* array params. */
		bool call( CLOPSTR cmd, ArgList params );

		/* Registers a callback and associates it with the string command cmd */
		void register_callback( CLOPSTR cmd, FunPtr cbptr );

		/* Calls the function associated with the specified command. */
		bool command( CLOPSTR cmd );
	private:
		std::map< CLOPSTR, FunPtr > callback_functions;
	};

	/* Returns a single instance of clop */
	static CLI& cli();

	/* Registers a function for specified command. */
	void Register( CLOPSTR cmd, FunPtr cbptr );

	/* Parse and run a command cmd. */
	bool Command( CLOPSTR cmd );
}
#undef CLOPSTR
#endif