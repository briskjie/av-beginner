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


#include <iostream>
#include <vector>
#include <thread>

using namespace std;

#include "thread_demo.h"


int main() {

//    thread t(thread_run);
//
//    t.join();
//
//    TheadRun theadRun;
//    thread t_obj(theadRun);
//
//    t_obj.join();
//
//    int i = 6;
//    ThreadRun2 threadRun2(i);

//    thread t_obj2(threadRun2);
//
//    t_obj2.join();

    int j = 6;
    ThreadRun3 threadRun333(j);

    cout << "start copy\n";

//    thread t_obj3(thread_run);
    thread t_obj3{threadRun333};

    if (t_obj3.joinable()){
        t_obj3.detach();
    }

    void * data = new vector<string>();


    delete (vector<string> *)data;

    cout << "end\n" ;

    return 0;
}