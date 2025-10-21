/*
 Copyright (c) 2025 ETIB Corporation

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <cstring>

#include "environments_parser.hpp"

EnvironmentsParser::EnvironmentsParser(const char *const envp[]) : envp(envp) {}

EnvironmentsParser::~EnvironmentsParser(void) {}

const char *const *EnvironmentsParser::getEnvp(void) const {
  return this->envp;
}

const char *EnvironmentsParser::getEnv(const int index) const {
  if (index < 0 || this->envp == nullptr) {
    return nullptr;
  }

  int count = 0;
  while (this->envp[count] != nullptr) {
    if (count == index) {
      return this->envp[count];
    }
    count++;
  }

  return nullptr;
}

const char *EnvironmentsParser::findEnv(const char *key) const {
  if (key == nullptr || this->envp == nullptr) {
    return nullptr;
  }

  const size_t keyLen = std::strlen(key);

  for (int i = 0; this->envp[i] != nullptr; i++) {
    const char *env = this->envp[i];
    if (std::strncmp(env, key, keyLen) == 0 && env[keyLen] == '=') {
      return env + keyLen + 1;
    }
  }

  return nullptr;
}
