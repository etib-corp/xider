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

#include "test_environments_parser.hpp"

class EnvironmentsParserTest : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(EnvironmentsParserTest, ConstructorAndGetEnvp) {
  const char *envp[] = {"HOME=/home/user", "PATH=/usr/bin", nullptr};
  
  EnvironmentsParser parser(envp);
  
  EXPECT_EQ(parser.getEnvp(), envp);
}

TEST_F(EnvironmentsParserTest, GetEnvValidIndex) {
  const char *envp[] = {"HOME=/home/user", "PATH=/usr/bin", "USER=john", nullptr};
  
  EnvironmentsParser parser(envp);
  
  EXPECT_STREQ(parser.getEnv(0), "HOME=/home/user");
  EXPECT_STREQ(parser.getEnv(1), "PATH=/usr/bin");
  EXPECT_STREQ(parser.getEnv(2), "USER=john");
}

TEST_F(EnvironmentsParserTest, GetEnvInvalidNegativeIndex) {
  const char *envp[] = {"HOME=/home/user", nullptr};
  
  EnvironmentsParser parser(envp);
  
  EXPECT_EQ(parser.getEnv(-1), nullptr);
  EXPECT_EQ(parser.getEnv(-5), nullptr);
}

TEST_F(EnvironmentsParserTest, GetEnvInvalidIndexOutOfBounds) {
  const char *envp[] = {"HOME=/home/user", "PATH=/usr/bin", nullptr};
  
  EnvironmentsParser parser(envp);
  
  EXPECT_EQ(parser.getEnv(2), nullptr);
  EXPECT_EQ(parser.getEnv(10), nullptr);
}

TEST_F(EnvironmentsParserTest, GetEnvNullEnvp) {
  const char *const *envp = nullptr;
  
  EnvironmentsParser parser(envp);
  
  EXPECT_EQ(parser.getEnv(0), nullptr);
}

TEST_F(EnvironmentsParserTest, FindEnvExistingKey) {
  const char *envp[] = {
    "HOME=/home/user",
    "PATH=/usr/bin:/bin",
    "USER=john",
    "SHELL=/bin/bash",
    nullptr
  };
  
  EnvironmentsParser parser(envp);
  
  EXPECT_STREQ(parser.findEnv("HOME"), "/home/user");
  EXPECT_STREQ(parser.findEnv("PATH"), "/usr/bin:/bin");
  EXPECT_STREQ(parser.findEnv("USER"), "john");
  EXPECT_STREQ(parser.findEnv("SHELL"), "/bin/bash");
}

TEST_F(EnvironmentsParserTest, FindEnvNonExistingKey) {
  const char *envp[] = {"HOME=/home/user", "PATH=/usr/bin", nullptr};
  
  EnvironmentsParser parser(envp);
  
  EXPECT_EQ(parser.findEnv("NONEXISTENT"), nullptr);
  EXPECT_EQ(parser.findEnv("FOO"), nullptr);
}

TEST_F(EnvironmentsParserTest, FindEnvNullKey) {
  const char *envp[] = {"HOME=/home/user", nullptr};
  
  EnvironmentsParser parser(envp);
  
  EXPECT_EQ(parser.findEnv(nullptr), nullptr);
}

TEST_F(EnvironmentsParserTest, FindEnvNullEnvp) {
  const char *const *envp = nullptr;
  
  EnvironmentsParser parser(envp);
  
  EXPECT_EQ(parser.findEnv("HOME"), nullptr);
}

TEST_F(EnvironmentsParserTest, FindEnvEmptyValue) {
  const char *envp[] = {"EMPTY=", "HOME=/home/user", nullptr};
  
  EnvironmentsParser parser(envp);
  
  const char *result = parser.findEnv("EMPTY");
  EXPECT_NE(result, nullptr);
  EXPECT_STREQ(result, "");
}

TEST_F(EnvironmentsParserTest, FindEnvKeyWithoutEquals) {
  // This tests the case where an environment variable doesn't have '='
  // The implementation should not match these
  const char *envp[] = {"HOME=/home/user", "NOEQUALS", nullptr};
  
  EnvironmentsParser parser(envp);
  
  EXPECT_STREQ(parser.findEnv("HOME"), "/home/user");
  EXPECT_EQ(parser.findEnv("NOEQUALS"), nullptr);
}

TEST_F(EnvironmentsParserTest, FindEnvPartialKeyMatch) {
  // Test that partial matches don't work
  const char *envp[] = {"HOMELIKE=/some/path", "HOME=/home/user", nullptr};
  
  EnvironmentsParser parser(envp);
  
  EXPECT_STREQ(parser.findEnv("HOME"), "/home/user");
  EXPECT_EQ(parser.findEnv("HOM"), nullptr);
}

TEST_F(EnvironmentsParserTest, FindEnvMultipleEqualsInValue) {
  const char *envp[] = {"VAR=value=with=equals", nullptr};
  
  EnvironmentsParser parser(envp);
  
  EXPECT_STREQ(parser.findEnv("VAR"), "value=with=equals");
}

TEST_F(EnvironmentsParserTest, EmptyEnvironment) {
  const char *envp[] = {nullptr};
  
  EnvironmentsParser parser(envp);
  
  EXPECT_EQ(parser.getEnv(0), nullptr);
  EXPECT_EQ(parser.findEnv("HOME"), nullptr);
}

TEST_F(EnvironmentsParserTest, FindEnvCaseSensitive) {
  const char *envp[] = {"Path=/usr/bin", "PATH=/bin", nullptr};
  
  EnvironmentsParser parser(envp);
  
  EXPECT_STREQ(parser.findEnv("Path"), "/usr/bin");
  EXPECT_STREQ(parser.findEnv("PATH"), "/bin");
  EXPECT_EQ(parser.findEnv("path"), nullptr);
}

TEST_F(EnvironmentsParserTest, FindEnvWithSpecialCharacters) {
  const char *envp[] = {
    "VAR1=value with spaces",
    "VAR2=/path:to:something",
    "VAR3=123-456_789",
    nullptr
  };
  
  EnvironmentsParser parser(envp);
  
  EXPECT_STREQ(parser.findEnv("VAR1"), "value with spaces");
  EXPECT_STREQ(parser.findEnv("VAR2"), "/path:to:something");
  EXPECT_STREQ(parser.findEnv("VAR3"), "123-456_789");
}

TEST_F(EnvironmentsParserTest, GetEnvIterateAll) {
  const char *envp[] = {
    "VAR1=value1",
    "VAR2=value2",
    "VAR3=value3",
    nullptr
  };
  
  EnvironmentsParser parser(envp);
  
  // Iterate through all environment variables
  int count = 0;
  for (int i = 0; parser.getEnv(i) != nullptr; i++) {
    count++;
  }
  
  EXPECT_EQ(count, 3);
}
