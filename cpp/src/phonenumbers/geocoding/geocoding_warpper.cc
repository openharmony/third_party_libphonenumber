/*
 * Copyright (c) 2025 Huawei Device Co., Ltd.
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

#include <algorithm>
#include "geocoding_warpper.h"
#include "phonenumbers/geocoding/phonenumber_offline_geocoder.h"
#include "phonenumbers/ohos/update_libgeocoding.h"
#include "phonenumbers/phonenumberutil.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <unicode/unistr.h>  // NOLINT(build/include_order)
#include <unicode/locid.h>
#include "securec.h"

using icu::UnicodeString;
using i18n::phonenumbers::PhoneNumber;
using i18n::phonenumbers::PhoneNumberUtil;
using i18n::phonenumbers::PhoneNumberOfflineGeocoder;
using icu::Locale;

extern "C" int exposeLocationName(const char* pNumber, const char* locale, char* res, const int resLength = 128) {
    i18n::phonenumbers::UpdateLibgeocoding::LoadUpdateData();
    if(offlineGeocoder == NULL) {
        offlineGeocoder = new PhoneNumberOfflineGeocoder();
    }
    if (util == NULL) {
        util = PhoneNumberUtil::GetInstance();
    }
    icu::Locale uLocale = icu::Locale::createFromName(locale);
    i18n::phonenumbers::PhoneNumber phoneNumber;
    std::string number = pNumber;
    PhoneNumberUtil::ErrorType type = util->Parse(number, uLocale.getCountry(), &phoneNumber);
    if (type != PhoneNumberUtil::ErrorType::NO_PARSING_ERROR) {
        std::string empty = "";
        return strcpy_s(res, resLength, empty.c_str());
    }
    std::string result = offlineGeocoder->GetDescriptionForNumber(phoneNumber, uLocale);
    return strcpy_s(res, resLength, result.c_str());
}

