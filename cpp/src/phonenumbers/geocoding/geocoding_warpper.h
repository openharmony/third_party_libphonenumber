// Copyright (C) 2012 The Libphonenumber Authors

#ifdef __cplusplus
extern "C" {
#endif
    #include <map>
    #include <string>
    
    #include <unicode/locid.h>
    using i18n::phonenumbers::PhoneNumber;
    using i18n::phonenumbers::PhoneNumberOfflineGeocoder;
    using icu::Locale;
    
    PhoneNumberOfflineGeocoder* offlineGeocoder = NULL;
    
    const char* exposeLocationName(const PhoneNumber& number, const Locale& language);
#ifdef __cplusplus
}
#endif
