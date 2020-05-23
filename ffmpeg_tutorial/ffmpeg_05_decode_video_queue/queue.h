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

#ifndef AV_BEGINNER_QUEUE_H
#define AV_BEGINNER_QUEUE_H

extern "C" {
#include <libavformat/avformat.h>
}

#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <atomic>
#include <condition_variable>

struct AVPacket;
struct AVFrame;

template<class T>
class Queue {
protected:
    std::queue<T> queue_;
    typename std::queue<T>::size_type size_max_;

    std::mutex mutex_;
    std::condition_variable full_;
    std::condition_variable empty_;

    std::atomic_bool quit_{false};
    std::atomic_bool finished_{false};

public:

    Queue(const size_t size_max);

    bool push(T &&data);

    bool pop(T &data);

    void finished();

    void quit();
};


using PacketQueue = Queue<std::unique_ptr<AVPacket, std::function<void(AVPacket *)>>>;
using FrameQueue = Queue<std::unique_ptr<AVFrame, std::function<void(AVFrame *)>>>;

template<class T>
Queue<T>::Queue(size_t size_max):
        size_max_{size_max} {

}

template<class T>
bool Queue<T>::push(T &&data) {
    std::unique_lock<std::mutex> lock(mutex_);

    while (!quit_ && !finished_) {
        if (queue_.size() < size_max_) {
            queue_.push(std::move(data));
            empty_.notify_all();
            return true;
        } else {
            full_.wait(lock);
        }
    }
    return false;
}

template<class T>
bool Queue<T>::pop(T &data) {
    std::unique_lock<std::mutex> lock(mutex_);

    while (!quit_) {
        if (!queue_.empty()) {
            data = std::move(queue_.front());
            queue_.pop();
            full_.notify_all();
            return true;
        } else if (queue_.empty() && finished_) {
            return false;
        } else {
            empty_.wait(lock);
        }
    }
    return false;
}


template<class T>
void Queue<T>::finished() {
    finished_ = true;
    empty_.notify_all();
}

template<class T>
void Queue<T>::quit() {
    quit_ = true;
    empty_.notify_all();
    full_.notify_all();
}

template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params){
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}

#endif //AV_BEGINNER_QUEUE_H
