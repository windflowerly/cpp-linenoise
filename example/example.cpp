#include <iostream>
#include "../linenoise.hpp"
#include <string.h>
#include <stdlib.h>
#include <set>

typedef std::set< std::string > words_t;
typedef std::map< char, words_t > readline_dic_t;

static readline_dic_t keyDic = {
     {'a',{"abc", "cdb"} }
    ,{'b',{"abc", "cdb", "bird", "buck"} }
    ,{'c',{"abc", "cdb"} }
    ,{'h',{"help", "hello"}  }
    ,{'s',{"select"}  }
    ,{'i',{"insert", "in" } }
    ,{'u',{"update" } }
    ,{'d',{"drop" } }
    ,{'f',{"from" } }
    ,{'j',{"join" } }
    ,{'l',{"left" }  }
    ,{'s',{"subscribe", "sub" }  }
};

void regist_word(const char* pWord){
    char ch = pWord[0];
    readline_dic_t::iterator it = keyDic.find(ch);
    if( it != keyDic.end() ){
        it->second.insert( std::string(pWord));
    }
    else{
        std::set< std::string > s;
        s.insert(std::string(pWord));
        keyDic[ch] = s;
    }
}

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
        
        std::unique_ptr< char[] > cptr(new char[len+1]);

        char * pstr = cptr.get();
        memset( pstr, 0x00, len+1 );

        strncpy( pstr, editBuffer, len);
        char ch =  trim_space_left(pstr,  len );

        std::string c = pstr;

        readline_dic_t::iterator it = keyDic.find( ch );
        if( it != keyDic.end() ){
            for( words_t::iterator k = it->second.begin(); k != it->second.end(); ++k ){
                completions.push_back(  c + *k );
            }
        }

    });

    // Load history
    linenoise::LoadHistory(path);

    while (true) {
        std::string line;
        auto quit = linenoise::Readline("hello> ", line);
        if (quit) {
            break;
        }

        // Add line to history
        linenoise::AddHistory(line.c_str());

        // Save history
        linenoise::SaveHistory(path);
    }

    return 0;
}
