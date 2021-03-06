/*
 * Copyright 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cstring>
#include <sched.h>

#include "oboe/Oboe.h"
#include "AudioStreamGateway.h"

using namespace flowgraph;

int64_t AudioStreamGateway::mFramePosition = 0;

oboe::DataCallbackResult AudioStreamGateway::onAudioReady(
        oboe::AudioStream *audioStream,
        void *audioData,
        int numFrames) {

    if (!mSchedulerChecked) {
        mScheduler = sched_getscheduler(gettid());
        mSchedulerChecked = true;
    }

    if (mAudioSink != nullptr) {
        mAudioSink->read(mFramePosition, audioData, numFrames);
        mFramePosition += numFrames;
    }

    return oboe::DataCallbackResult::Continue;
}

int AudioStreamGateway::getScheduler() {
    return mScheduler;
}
