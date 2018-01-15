#include "stdafx.h"


int run( int argc, char** argv );
void showHelp();


int
main( int argc, char** argv ) {

    setlocale( LC_NUMERIC, "C" );

    int result = 0;
    try {
        result = run( argc, argv );
    }
    catch ( ... ) {
        result = -1000;
    }

    std::cout << "\n^\n";
    std::cin.ignore();

    return result;

} // main()




void
showHelp() {

    std::cout << "Bridge for schemes."
        "\nUsage:"
        "\n  > bridge --source config.txt --run callto skype /callto:echo123"
        "\n  > bridge --source config.txt --run callto viber /some-viber-command+12345678"
        "\n  > bridge --source config.txt --run mailto outlook any@example.com"
        "\n";
}




int
run( int argc, char** argv ) {

    // parse parameters
    if ( argc < 2 ) {
        showHelp();
        return -1;
    }
    for ( int i = 1; i < argc; ++i ) {
        const std::string&  a = argv[ i ];
        if ( (a == "-h") || (a == "--help") ) {
            showHelp();
            return -1;
        }
    }


    typedef std::string  key_t;
    typedef std::string  run_t;
    const auto  createKey = [] ( const std::string& a, const std::string& b ) -> key_t {
        return a + "." + b;
    };


    std::string  source = "";
    std::string  runFor = "";
    std::string  runParam = "";
    for ( int i = 1; i < argc; ++i ) {
        const std::string&  a = argv[ i ];
        //cout << "argv[" << i << "] = " << a << endl;
        if ( (a == "-s") || (a == "--source") ) {
            if ( (i + 1) >= argc ) {
                std::cout << "(!) After '-s | --source' waiting a name of file.\n";
                return -2;
            }
            source = argv[ i + 1 ];
            i += 1;

        } else if ( (a == "-r") || (a == "--run") ) {
            if ( (i + 3) >= argc ) {
                std::cout << "(!) After '-r | --run' waiting a name of scheme and applicaion.\n";
                return -3;
            }
            runFor = createKey( argv[ i + 1 ], argv[ i + 2 ] );
            runParam = argv[ i + 3 ];
            i += 3;
        }
    } // for (int i = 1; i < argc; ++i)


    // read schemes
    std::map< key_t, run_t >  schemes;
    std::ifstream  file( source );
    if ( !file.is_open() ) {
        std::cout << "(!) File '" << source << "' not found.\n";
        return -101;
    }
    std::string  line;
    std::cout << "Data from '" << source << "'\n";
    for ( size_t k = 0;  std::getline( file, line, '\n' );  ++k ) {
        std::cout << "  " << line << "\n";
        const std::regex  re( "\\s+" );
        const std::sregex_token_iterator  itrBegin( line.begin(), line.end(), re, -1 );
        const std::sregex_token_iterator  itrEnd;
        std::vector< std::string >  v;
        for ( auto itr = itrBegin; itr != itrEnd; ++itr ) {
            const auto  s = std::regex_replace( itr->str(), std::regex( "\\s+" ), "" );
            if ( !s.empty() ) {
                v.emplace_back( s );
            }
        }
        // wrap path
        for ( size_t i = 3; i < v.size(); ++i ) {
            v[ 2 ] += " " + v[ i ];
        }
        v.erase( v.begin() + std::min( 3u, v.size() ), v.end() );
        if ( v.size() != 3 ) {
            if ( !line.empty() ) {
                std::cout << "(?) Line '" << line << "' not recognized. Skipped.\n";
            }
            continue;
        }
        const key_t  key = createKey( v[ 0 ], v[ 1 ] );
        const run_t  run = v[ 2 ];
        std::cout << "    " << key << " -> " << run << "\n";
        schemes.emplace( key, run );

    } // for ( size_t k = 0; ...


    // run application
    const auto  ftr = schemes.find( runFor );
    if ( ftr == schemes.cend() ) {
        std::cout << "(!) Application for '" << runFor << "' not found. See '" << source << "'.\n";
        return -202;
    }
    try {
        const std::string  runWithParam = ftr->second + " " + runParam;
        std::cout << "\n" << "Run\n  > " << runWithParam << "\n";
        std::system( runWithParam.c_str() );
    }
    catch ( const std::exception& ex ) {
        std::cout << "(!) " << ex.what() << "\n";
        return -10;
    }
    catch ( const std::string& ex ) {
        std::cout << "(!) " << ex << "\n";
        return -11;
    }
    catch ( ... ) {
        std::cout << "(!) Undefined exception.\n";
        return -12;
    }


    // ok
    return 0;
}
