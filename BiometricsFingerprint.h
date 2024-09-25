#ifndef ANDROID_HARDWARE_BIOMETRICS_FINGERPRINT_AIDL_BIOMETRICSFINGERPRINT_H
#define ANDROID_HARDWARE_BIOMETRICS_FINGERPRINT_AIDL_BIOMETRICSFINGERPRINT_H

#include <aidl/android/hardware/biometrics/fingerprint/BnBiometricsFingerprint.h>
#include <aidl/android/hardware/biometrics/fingerprint/IBiometricsFingerprint.h>
#include <memory>
#include <vector>
#include <string>

namespace aidl {
namespace android {
namespace hardware {
namespace biometrics {
namespace fingerprint {

using ::aidl::android::hardware::biometrics::fingerprint::BnBiometricsFingerprint;
using ::aidl::android::hardware::biometrics::fingerprint::IBiometricsFingerprint;
using ndk::ScopedAStatus;

struct BiometricsFingerprint : public BnBiometricsFingerprint {
    BiometricsFingerprint();

    // Métodos adaptados de IBiometricsFingerprint.
    ScopedAStatus setNotify(
        const std::shared_ptr<IBiometricsFingerprintClientCallback>& clientCallback) override;
    ScopedAStatus preEnroll() override;
    ScopedAStatus enroll(const std::vector<uint8_t>& hat, int32_t gid, int32_t timeoutSec) override;
    ScopedAStatus postEnroll() override;
    ScopedAStatus getAuthenticatorId(int64_t* _aidl_return) override;
    ScopedAStatus cancel() override;
    ScopedAStatus enumerate() override;
    ScopedAStatus remove(int32_t gid, int32_t fid) override;
    ScopedAStatus setActiveGroup(int32_t gid, const std::string &storePath) override;
    ScopedAStatus authenticate(int64_t operationId, int32_t gid) override;

    // Métodos relacionados ao UDFPS
    ScopedAStatus isUdfps(int32_t sensorId, bool* _aidl_return) override;
    ScopedAStatus onFingerDown(int32_t x, int32_t y, float minor, float major) override;
    ScopedAStatus onFingerUp() override;

    ScopedAStatus extraApiWrapper(int cidValue);

  private:
    std::shared_ptr<IBiometricsFingerprint> biometrics_aidl_service;
};

}  // namespace fingerprint
}  // namespace biometrics
}  // namespace hardware
}  // namespace android
}  // namespace aidl
#endif  // ANDROID_HARDWARE_BIOMETRICS_FINGERPRINT_AIDL_BIOMETRICSFINGERPRINT_H
