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

#include <fcntl.h>
#include <unistd.h>
#include "update_metadata.h"
#include "update_libphonenumber.h"

#define FDSAN_TAG 1

namespace i18n {
namespace phonenumbers {
const std::string UpdateLibphonenumber::METADATAINFO_PATH = "/system/etc/LIBPHONENUMBER/mount_dir/MetadataInfo";

void UpdateLibphonenumber::LoadUpdateData()
{
    int metadataFd = open(METADATAINFO_PATH.c_str(), O_RDONLY);
    if (metadataFd == -1) {
        return;
    }
    // shift the domain ID left by 32 bits
    uint64_t fdTag = (static_cast<uint64_t>(0xD001E00) << 32) | FDSAN_TAG;
    fdsan_exchange_owner_tag(metadataFd, 0, fdTag);
    UpdateMetadata::LoadUpdatedMetadata(metadataFd);
    fdsan_close_with_tag(metadataFd, fdTag);
}
}  // namespace phonenumbers
}  // namespace i18n
