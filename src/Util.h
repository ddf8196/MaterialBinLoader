#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include "frida-gum.h"

#define FindSignature FindSig
#define FindSignatures(moduleInfo, ...) FindSigs(moduleInfo, {__VA_ARGS__})

inline uintptr_t FindSig(const GumModuleDetails* moduleInfo, const std::string& signature) {
    uintptr_t moduleBase = (uintptr_t)moduleInfo->range->base_address;
    size_t moduleSize = moduleInfo->range->size;

    std::vector<uint16_t> pattern;
    for (int i = 0; i < signature.size(); i++) {
        if (signature[i] == ' ')
            continue;
        if (signature[i] == '?') {
            pattern.push_back(0xFF00);
            i++;
        } else {
            char buf[3] { signature[i], signature[++i], 0 };
            pattern.push_back((uint16_t)strtoul(buf, nullptr, 16));
        }
    }

    if (pattern.size() == 0)  {
        return moduleBase;
    }

    int patternIdx = 0;
    uintptr_t match = 0;
    for (uintptr_t i = moduleBase; i < moduleBase + moduleSize; i++) {
        uint8_t current = *(uint8_t*)i;
        if (current == pattern[patternIdx] || pattern[patternIdx] & 0xFF00) {
            if (!match) {
                match = i;
            }
            patternIdx++;
            if (patternIdx == pattern.size()) {
                return match;
            }
        } else {
            if (match) {
                i--;
            }
            match = 0;
            patternIdx = 0;
        }
    }

    return 0;
}

inline uintptr_t FindSigs(const GumModuleDetails* moduleInfo, const std::initializer_list<std::string>& signatures) {
    uintptr_t ptr = 0;
    for (auto& sig : signatures) {
        if ((ptr = FindSig(moduleInfo, sig))) {
            break;
        }
    }
    return ptr;
}
