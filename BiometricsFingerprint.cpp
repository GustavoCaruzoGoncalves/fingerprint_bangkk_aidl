#define LOG_TAG "fingerprint-aidl-service.bangkk"

#include "BiometricsFingerprint.h"

#include <android-base/file.h>
#include <android-base/logging.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <chrono>
#include <cmath>
#include <fstream>
#include <thread>

#include <display/drm/sde_drm.h>
#include <aidl/android/hardware/biometrics/fingerprint/BnBiometricsFingerprint.h>
#include <aidl/android/hardware/biometrics/fingerprint/IBiometricsFingerprint.h>

enum HBM_STATE {
    OFF = 0,
    ON = 2
};

void setHbmState(int state) {
    struct panel_param_info param_info;
    int32_t node = open("/dev/dri/card0", O_RDWR);
    int32_t ret = 0;

    if (node < 0) {
        LOG(ERROR) << "Failed to get card0!";
        return;
    }

    param_info.param_idx = PARAM_HBM;
    param_info.value = state;

    ret = ioctl(node, DRM_IOCTL_SET_PANEL_FEATURE, &param_info);
    if (ret < 0) {
        LOG(ERROR) << "IOCTL call failed with ret = " << ret;
    } else {
        LOG(INFO) << "HBM state set successfully. New state: " << state;
    }

    close(node);
}

namespace aidl {
namespace android {
namespace hardware {
namespace biometrics {
namespace fingerprint {

BiometricsFingerprint::BiometricsFingerprint() {
    // Substituir chamadas HIDL por AIDL
    biometrics_aidl_service = IBiometricsFingerprint::fromBinder();  // Adaptado para AIDL
}

ndk::ScopedAStatus BiometricsFingerprint::setNotify(
    const std::shared_ptr<IBiometricsFingerprintClientCallback>& clientCallback) {
    return biometrics_aidl_service->setNotify(clientCallback);
}

ndk::ScopedAStatus BiometricsFingerprint::preEnroll() {
    return biometrics_aidl_service->preEnroll();
}

ndk::ScopedAStatus BiometricsFingerprint::enroll(const std::vector<uint8_t>& hat,
                                                 int32_t gid, int32_t timeoutSec) {
    return biometrics_aidl_service->enroll(hat, gid, timeoutSec);
}

ndk::ScopedAStatus BiometricsFingerprint::postEnroll() {
    return biometrics_aidl_service->postEnroll();
}

ndk::ScopedAStatus BiometricsFingerprint::getAuthenticatorId() {
    return biometrics_aidl_service->getAuthenticatorId();
}

ndk::ScopedAStatus BiometricsFingerprint::cancel() {
    setHbmState(OFF);
    return biometrics_aidl_service->cancel();
}

ndk::ScopedAStatus BiometricsFingerprint::enumerate() {
    return biometrics_aidl_service->enumerate();
}

ndk::ScopedAStatus BiometricsFingerprint::remove(int32_t gid, int32_t fid) {
    return biometrics_aidl_service->remove(gid, fid);
}

ndk::ScopedAStatus BiometricsFingerprint::setActiveGroup(int32_t gid,
                                                         const std::string &storePath) {
    return biometrics_aidl_service->setActiveGroup(gid, storePath);
}

ndk::ScopedAStatus BiometricsFingerprint::authenticate(int64_t operationId, int32_t gid) {
    setHbmState(OFF);
    return biometrics_aidl_service->authenticate(operationId, gid);
}

ndk::ScopedAStatus BiometricsFingerprint::isUdfps(int32_t, bool* _aidl_return) {
    *_aidl_return = true;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus BiometricsFingerprint::onFingerDown(int32_t, int32_t, float, float) {
    setHbmState(ON);
    extraApiWrapper(101);

    std::thread([this]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        BiometricsFingerprint::onFingerUp();
    }).detach();

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus BiometricsFingerprint::onFingerUp() {
    setHbmState(OFF);
    extraApiWrapper(102);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus BiometricsFingerprint::extraApiWrapper(int cidValue) {
    int cid[1] = {cidValue};

    // Criando std::vector<uint8_t> para armazenar os dados de 'cid'
    std::vector<uint8_t> cid_data(reinterpret_cast<uint8_t*>(cid), reinterpret_cast<uint8_t*>(cid) + sizeof(cid));

    // Chamada extra_api com o buffer correto de entrada
    biometrics_aidl_service->extraApi(7, cid_data);

    return ndk::ScopedAStatus::ok();
}

}  // namespace fingerprint
}  // namespace biometrics
}  // namespace hardware
}  // namespace android
}  // namespace aidl
