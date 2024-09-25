/*
 * Copyright (C) 2024 The Android Open Source Project
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

#define LOG_TAG "android.hardware.biometrics.fingerprint.aidl-service.bangkk"
#include <android-base/logging.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <aidl/android/hardware/biometrics/fingerprint/BnBiometricsFingerprint.h>
#include "BiometricsFingerprint.h"

using aidl::android::hardware::biometrics::fingerprint::BiometricsFingerprint;
using android::sp;
using android::binder::Status;

int main() {
    LOG(INFO) << "Starting Biometrics AIDL HAL service.";

    // Configura o process state para threads binder
    android::ProcessState::initWithDriver("/dev/vndbinder");

    // Registra o serviço AIDL
    sp<BiometricsFingerprint> service = new BiometricsFingerprint();
    if (android::defaultServiceManager()->addService(
            android::String16("android.hardware.biometrics.fingerprint"), service) != android::OK) {
        LOG(ERROR) << "Failed to register Biometrics AIDL HAL service.";
        return 1;
    }

    LOG(INFO) << "Biometrics AIDL HAL service ready.";

    // Inicializa threadpool de Binder
    android::ProcessState::self()->startThreadPool();
    android::IPCThreadState::self()->joinThreadPool();

    LOG(ERROR) << "Biometrics AIDL HAL service failed to join thread pool.";
    return 1;
}
