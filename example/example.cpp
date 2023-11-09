#include <iostream>
#include "../linenoise.hpp"
#include <string.h>
#include <stdlib.h>

typedef std::vector< std::string > words_t;
typedef std::map< char, words_t > readline_dic_t;

readline_dic_t keyDic = {
    {'a',{"abc", "cdb"} }
    ,{'b',{"abc", "cdb", "bird", "buck"} }
    ,{'c',{"abc", "cdb"} }
    ,{'d',{"abc", "cdb"}  }
    ,{'h',{"help", "hello"}  }
};

char trim_space_left( char* p, int len){
    char ch = ' ';

    if( len <= 1){
        ch = p[0];
        memset( p, 0x00, len );
        return ch;
    }

    if( p[len - 1] == ' '){
        return trim_space_left(p, len - 1);
    }

    for( int i = len - 1; i >= 1; --i ){
        if( p[i] == ' ' ){
            ch = p[i + 1];
            p[i + 1] = 0x00;
            return ch;
        }
    }

    ch = p[0];
    memset(p, 0x00, len);

    return ch;
}

int main(int argc, const char** argv)
{
    const auto path = "history.txt";

    // Enable the multi-line mode
    linenoise::SetMultiLine(true);

    // Set max length of the history
    linenoise::SetHistoryMaxLen(1000);

    // Setup completion words every time when a user types
    linenoise::SetCompletionCallback([](const char* editBuffer, std::vector<std::string>& completions) {
        int len = strlen(editBuffer);

        char * pstr = (char*)malloc( len + 1 );
        memset( pstr, 0x00, len );
        strncpy( pstr, editBuffer, len);

        char ch =  trim_space_left(pstr,  len );

        std::string c = pstr;

        readline_dic_t::iterator it = keyDic.find( ch );
        if( it != keyDic.end() ){
            for( words_t::iterator k = it->second.begin(); k != it->second.end(); ++k ){
                completions.push_back(  c + *k );
            }
        }

        free( pstr );
    });

    // Load history
    linenoise::LoadHistory(path);

    while (true) {
        std::string line;
        auto quit = linenoise::Readline("hello> ", line);
        if (quit) {
            break;
        }

//        cout <<  "echo: '" << line << "'" << endl;

        // Add line to history
        linenoise::AddHistory(line.c_str());

        // Save history
        linenoise::SaveHistory(path);
    }

    return 0;
}
