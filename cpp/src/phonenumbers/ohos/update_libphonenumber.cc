/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <dlfcn.h>
#include <unistd.h>
#include "update_geocoding.h"
#include "update_metadata.h"
#include "update_libphonenumber.h"

namespace i18n {
namespace phonenumbers {
const std::string UpdateLibphonenumber::LIBSO_NAME = "libi18n_sa_client.z.so";
const std::string UpdateLibphonenumber::UPDATE_FUNCTION_NAME = "VerifyLibphonenumberUpdate";
bool UpdateLibphonenumber::isLoad = false;

void UpdateLibphonenumber::LoadUpdateData()
{
    if (isLoad) {
        return;
    }
    void* handler = dlopen(LIBSO_NAME.c_str(), RTLD_NOW | RTLD_GLOBAL);    
    if (handler == nullptr) {
        return;
    }
    GetUpdateFileDescriptor updateFunc = (GetUpdateFileDescriptor)dlsym(handler, UPDATE_FUNCTION_NAME.c_str());
    int metadatafd = -1;
    int geocodingfd = -1;
    updateFunc(&metadatafd, &geocodingfd);
    UpdateMetadata::LoadUpdatedMetadata(metadatafd);
    UpdateGeocoding::LoadGeocodingData(geocodingfd);
    if (metadatafd != -1) {
        close(metadatafd);
    }
    if (geocodingfd != -1) {
        close(geocodingfd);
    }
    isLoad = true;
}
}  // namespace phonenumbers
}  // namespace i18n
