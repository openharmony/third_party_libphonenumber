/*
 * Copyright (c) 2026 Huawei Device Co., Ltd.
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

#ifndef I18N_PHONENUMBERS_GEOCODING_COMPRESSED_DATA_H_
#define I18N_PHONENUMBERS_GEOCODING_COMPRESSED_DATA_H_

#include <cstdint>
#include <cstddef>

#include "phonenumbers/geocoding/geocoding_data.h"
namespace i18n {
namespace phonenumbers {

struct CompressedPrefixDescriptions {
    int32_t base_prefix;
    const uint8_t* encoded_deltas;
    int encoded_deltas_size;
    const uint16_t* description_indices;
    int prefixes_size;
    const int32_t* possible_lengths;
    int possible_lengths_size;
    const char* string_pool;
    const uint32_t* string_offsets;
    uint32_t string_pool_size;
};

class CompressedGeocodingDataProcessor {
public:
    static PrefixDescriptions* ExpandCompressedData(const CompressedPrefixDescriptions* compressed);

    static void FreeExpandedData(PrefixDescriptions* expanded);

private:
    static int32_t Decode(const uint8_t*& ptr, const uint8_t* end);

    static int GetEncodedSize(int32_t delta);

    static int DecodeArray(int32_t base_prefix,
                           const uint8_t* encoded_data,
                           size_t encoded_size,
                           int32_t* output_prefixes,
                           int output_size);
};

}  // namespace phonenumbers
}  // namespace i18n

#endif  // I18N_PHONENUMBERS_GEOCODING_COMPRESSED_DATA_H_
