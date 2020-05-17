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

#ifndef AV_BEGINNER_THREAD_DEMO_H
#define AV_BEGINNER_THREAD_DEMO_H

#include <iostream>

using namespace std;

void thread_run() {
    cout << "thread_run method \n";
}

class TheadRun {
public:
    void operator()() {
        cout << "thread_run object\n";
    }
};

class ThreadRun2 {
    int &m_obj;
public:
    ThreadRun2(int &i):m_obj(i){

    }

    void operator()(){
        cout << "thread_run object2\n";
    }
};

class ThreadRun3{
    int &m_obj;
public:
    ThreadRun3(int &i):m_obj(i){
        cout << "ThreadRun3 init\n";
    }

    ThreadRun3(const ThreadRun3& m):m_obj(m.m_obj){
        cout << "copy ThreadRun3 init\n";
    }

    ~ThreadRun3(){
        cout << "delete copy\n";
    }

    void operator()(){
        cout << "thread_run object3\n";
    }
};

#endif //AV_BEGINNER_THREAD_DEMO_H
