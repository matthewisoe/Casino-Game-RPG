
#ifndef LANGUAGE_MANAGER_H
#define LANGUAGE_MANAGER_H

#include <string>
#include <unordered_map>

class LanguageManager {
public:
    static LanguageManager& getInstance() {
        static LanguageManager instance;
        return instance;
    }
    std::string getCurrentLanguage() const;
    void setCurrentLanguage(const std::string& language);

    void setLanguage(const std::string& language) {
        currentLanguage = language;
    }

    const std::string& getLanguage() const {
        return currentLanguage;
    }

    std::string getLocalizedResource(const std::string& key) const {
        auto it = localizedResources.find(currentLanguage + "_" + key);
        if (it != localizedResources.end()) {
            return it->second;
        }
        return ""; // Return empty string if the key isn't found
    }

    void addLocalizedResource(const std::string& lang, const std::string& key, const std::string& value) {
        localizedResources[lang + "_" + key] = value;
    }

private:
    LanguageManager() = default;
    std::string currentLanguage = "en"; // Default language
    std::unordered_map<std::string, std::string> localizedResources;
    LanguageManager(const LanguageManager&) = delete;
    LanguageManager& operator=(const LanguageManager&) = delete;
};

#endif // LANGUAGE_MANAGER_H
