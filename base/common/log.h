/*
* Copyright 2020 https://glumes.com
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     https://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef AV_BEGINNER_BASE_H
#define AV_BEGINNER_BASE_H

#include <iostream>
#include <string>

using namespace std;

static void logging(const char *fmt, ...)
{
    va_list args;
    fprintf( stderr, "LOG: " );
    va_start( args, fmt );
    vfprintf( stderr, fmt, args );
    va_end( args );
    fprintf( stderr, "\n" );
}

class Log {
public:
    static void print(string str){
        std::cout << str << endl;
    }

    static void print(const char *fmt, ...){
        va_list args;
        fprintf( stdout, "LOG: " );
        va_start( args, fmt );
        vfprintf( stdout, fmt, args );
        va_end( args );
        fprintf( stdout, "\n" );
    }
};

#endif //AV_BEGINNER_BASE_H
