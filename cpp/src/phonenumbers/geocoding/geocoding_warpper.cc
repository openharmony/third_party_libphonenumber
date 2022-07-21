// Copyright (C) 2012 The Libphonenumber Authors

#include "phonenumbers/geocoding/phonenumber_offline_geocoder.h"
#include "phonenumbers/geocoding/geocoding_warpper.h"
#include <algorithm>
#include <string>

#include <unicode/unistr.h>  // NOLINT(build/include_order)
#include <unicode/locid.h>

using icu::UnicodeString;
using std::string;
using i18n::phonenumbers::PhoneNumber;
using i18n::phonenumbers::PhoneNumberOfflineGeocoder;
using icu::Locale;

const char* exposeLocationName(const PhoneNumber& number, const Locale& language) {
    if(offlineGeocoder == NULL) {
        offlineGeocoder = new PhoneNumberOfflineGeocoder();
    }
    const char* chars = offlineGeocoder->GetDescriptionForNumber(number, language).c_str();
    return chars;
}

