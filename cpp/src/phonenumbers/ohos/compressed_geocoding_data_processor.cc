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

#include <cstdlib>
#include <cstring>
#include "compressed_geocoding_data_processor.h"

namespace i18n {
namespace phonenumbers {

PrefixDescriptions* CompressedGeocodingDataProcessor::ExpandCompressedData(const CompressedPrefixDescriptions*
    compressed)
{
    if (!compressed) {
        return nullptr;
    }

    int32_t* prefixes = new int32_t[compressed->prefixes_size];
    if (!prefixes) {
        return nullptr;
    }

    char** descriptions = new char*[compressed->prefixes_size];
    if (!descriptions) {
        delete[] prefixes;
        return nullptr;
    }

    DecodeArray(compressed->base_prefix, compressed->encoded_deltas, compressed->encoded_deltas_size, prefixes,
        compressed->prefixes_size);

    for (int i = 0; i < compressed->prefixes_size; ++i) {
        uint16_t idx = compressed->description_indices[i];
        descriptions[i] = const_cast<char*>(compressed->string_pool + compressed->string_offsets[idx]);
    }

    PrefixDescriptions* expanded = new PrefixDescriptions{
        prefixes,
        compressed->prefixes_size,
        const_cast<const char**>(descriptions),
        compressed->possible_lengths,
        compressed->possible_lengths_size
    };

    return expanded;
}

void CompressedGeocodingDataProcessor::FreeExpandedData(PrefixDescriptions* expanded)
{
    if (expanded) {
        if (expanded->prefixes) {
            delete[] expanded->prefixes;
        }
        if (expanded->descriptions) {
            delete[] expanded->descriptions;
        }
        delete expanded;
    }
}

int CompressedGeocodingDataProcessor::GetEncodedSize(int32_t delta)
{
    if (delta >= -128 && delta < 128) {
        return 1;
    } else if (delta >= -32768 && delta < 32768) {
        return 2;
    } else if (delta >= -8388608 && delta < 8388608) {
        return 3;
    }
    return 4;
}

int32_t CompressedGeocodingDataProcessor::Decode(const uint8_t*& ptr, const uint8_t* end)
{
    if (ptr >= end) {
        return 0;
    }

    uint8_t firstByte = *ptr++;
    if ((firstByte & 0x80) == 0) {
        int8_t v = static_cast<int8_t>(firstByte);
        return static_cast<int32_t>(v);
    } else if ((firstByte & 0xC0) == 0x80) {
        uint8_t secondByte = *ptr++;
        uint16_t v = firstByte & 0x3F;
        v = v << 8;
        v += secondByte;
        return v;
    } else if ((firstByte & 0xE0) == 0xC0) {
        if (ptr + 1 >= end) return 0;
        uint16_t secondByte = *ptr++;
        uint8_t thirdByte = *ptr++;
        int32_t v = firstByte & 0x1F;
        v = v << 16;
        v += (secondByte << 8);
        v += thirdByte;
        return v;
    } else if ((firstByte & 0xF0) == 0xE0) {
        if (ptr + 2 >= end) return 0;
        uint32_t secondByte = *ptr++;
        uint32_t thirdByte = *ptr++;
        uint32_t forthByte = *ptr++;
        int32_t v = firstByte & 0xF;
        v = v << 24;
        v += (secondByte << 16);
        v += (thirdByte << 8);
        v += forthByte;
        return v;
    } else {
        if (ptr + 3 >= end) return 0;
        uint32_t secondByte = *ptr++;
        uint32_t thirdByte = *ptr++;
        uint32_t forthByte = *ptr++;
        uint32_t fifthByte = *ptr++;
        int32_t v = (secondByte << 24) + (thirdByte << 16) + (forthByte << 8) + fifthByte;
        return v;
    }
}

int CompressedGeocodingDataProcessor::DecodeArray(int32_t base_prefix, const uint8_t* encoded_data,
    size_t encoded_size, int32_t* output_prefixes, int output_size)
{
    const uint8_t* ptr = encoded_data;
    const uint8_t* end = encoded_data + encoded_size;

    int32_t current_prefix = base_prefix;
    int count = 0;

    output_prefixes[count++] = current_prefix;

    while (ptr < end && count < output_size) {
        int32_t delta = Decode(ptr, end);
        current_prefix += delta;
        output_prefixes[count++] = current_prefix;
    }

    return count;
}
}  // namespace phonenumbers
}  // namespace i18n
