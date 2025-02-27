#include "LanguageManager.h"

std::string LanguageManager::getCurrentLanguage() const {
    return currentLanguage;
}

void LanguageManager::setCurrentLanguage(const std::string& language) {
    currentLanguage = language;
}