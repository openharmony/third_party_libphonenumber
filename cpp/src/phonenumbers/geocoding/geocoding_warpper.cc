// Copyright (C) 2012 The Libphonenumber Authors

#include <algorithm>
#include "geocoding_warpper.h"
#include "phonenumbers/geocoding/phonenumber_offline_geocoder.h"
#include "phonenumbers/phonenumberutil.h"
#include <string>

#include <unicode/unistr.h>  // NOLINT(build/include_order)
#include <unicode/locid.h>

using icu::UnicodeString;
using i18n::phonenumbers::PhoneNumber;
using i18n::phonenumbers::PhoneNumberUtil;
using i18n::phonenumbers::PhoneNumberOfflineGeocoder;
using icu::Locale;

extern "C" const char* exposeLocationName(const char* pNumber, const char* locale) {
    if(offlineGeocoder == NULL) {
        offlineGeocoder = new PhoneNumberOfflineGeocoder();
    }
    if (util == NULL) {
        util = PhoneNumberUtil::GetInstance();
    }
    //const char *l_name = locale.c_str();
    icu::Locale uLocale = icu::Locale::createFromName(locale);
    i18n::phonenumbers::PhoneNumber phoneNumber;
    std::string number = pNumber;
    PhoneNumberUtil::ErrorType type = util->Parse(number, uLocale.getCountry(), &phoneNumber);
    if (type != PhoneNumberUtil::ErrorType::NO_PARSING_ERROR) {
        return "";
    }
    const char* chars = offlineGeocoder->GetDescriptionForNumber(phoneNumber, uLocale).c_str();
    return chars;
}

