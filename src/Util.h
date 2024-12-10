#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include "frida-gum.h"
#include <bits/elf_common.h>
#include <stddef.h>
#include <stdint.h>
#include <link.h>
#include <string.h>

#define FindSignature FindSig
#define FindSignatures(moduleInfo, ...) FindSigs(moduleInfo, {__VA_ARGS__})


struct ModuleRange {
   uintptr_t addr;
   size_t len;
};

int callback(struct dl_phdr_info* dl_info, size_t _size, void* data) {
  // Skip if module is not minecraftpe
  if (strstr(dl_info->dlpi_name, "libminecraftpe") == NULL)  {
    return 0;
  }
  
  for (size_t i = 0; i < dl_info->dlpi_phnum; i++) {
    const ElfW(Phdr) phdr = dl_info->dlpi_phdr[i];
    // Check if we found a loadable and executable section
    // should be where the code is at
    if (phdr.p_type == PT_LOAD && phdr.p_flags & PF_X) {   
      // Retrieve ModuleRange
      struct ModuleRange* mod_range = (struct ModuleRange*) data;
      mod_range->addr = dl_info->dlpi_addr + phdr.p_paddr;
      mod_range->len = phdr.p_memsz;
      // Stop dl_iteraye_phdr
      return -1;
    }
  }
  return 0;
}

/// Get module range of minecraftpe
struct ModuleRange get_module_range() {
  struct ModuleRange mod_range = {0,0};
  dl_iterate_phdr(callback,(void*) &mod_range);
  return mod_range;
}

inline uintptr_t FindSig(const ModuleRange* moduleInfo, const std::string& signature) {
    uintptr_t moduleBase = moduleInfo->addr;
    size_t moduleSize = moduleInfo->len;

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

inline uintptr_t FindSigs(const ModuleRange* moduleInfo, const std::initializer_list<std::string>& signatures) {
    uintptr_t ptr = 0;
    for (auto& sig : signatures) {
        if ((ptr = FindSig(moduleInfo, sig))) {
            break;
        }
    }
    return ptr;
}
