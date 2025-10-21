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

#include "test_arguments_parser.hpp"

// ============================================================================
// ArgumentsParser Tests
// ============================================================================

class ArgumentsParserTest : public ::testing::Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(ArgumentsParserTest, ConstructorAndGetArgc) {
  const char *argv[] = {"program", "arg1", "arg2"};
  int argc = 3;
  
  ArgumentsParser parser(argc, argv);
  
  EXPECT_EQ(parser.getArgc(), 3);
}

TEST_F(ArgumentsParserTest, GetArgv) {
  const char *argv[] = {"program", "arg1", "arg2"};
  int argc = 3;
  
  ArgumentsParser parser(argc, argv);
  const char *const *result = parser.getArgv();
  
  EXPECT_EQ(result, argv);
  EXPECT_STREQ(result[0], "program");
  EXPECT_STREQ(result[1], "arg1");
  EXPECT_STREQ(result[2], "arg2");
}

TEST_F(ArgumentsParserTest, GetArgValidIndex) {
  const char *argv[] = {"program", "arg1", "arg2", "arg3"};
  int argc = 4;
  
  ArgumentsParser parser(argc, argv);
  
  EXPECT_STREQ(parser.getArg(0), "program");
  EXPECT_STREQ(parser.getArg(1), "arg1");
  EXPECT_STREQ(parser.getArg(2), "arg2");
  EXPECT_STREQ(parser.getArg(3), "arg3");
}

TEST_F(ArgumentsParserTest, GetArgInvalidNegativeIndex) {
  const char *argv[] = {"program", "arg1"};
  int argc = 2;
  
  ArgumentsParser parser(argc, argv);
  
  EXPECT_EQ(parser.getArg(-1), nullptr);
  EXPECT_EQ(parser.getArg(-5), nullptr);
}

TEST_F(ArgumentsParserTest, GetArgInvalidIndexOutOfBounds) {
  const char *argv[] = {"program", "arg1"};
  int argc = 2;
  
  ArgumentsParser parser(argc, argv);
  
  EXPECT_EQ(parser.getArg(2), nullptr);
  EXPECT_EQ(parser.getArg(10), nullptr);
}

TEST_F(ArgumentsParserTest, GetArgBoundaryConditions) {
  const char *argv[] = {"program"};
  int argc = 1;
  
  ArgumentsParser parser(argc, argv);
  
  EXPECT_STREQ(parser.getArg(0), "program");
  EXPECT_EQ(parser.getArg(1), nullptr);
}

TEST_F(ArgumentsParserTest, EmptyArguments) {
  int argc = 0;
  const char **argv = nullptr;
  
  ArgumentsParser parser(argc, argv);
  
  EXPECT_EQ(parser.getArgc(), 0);
  EXPECT_EQ(parser.getArg(0), nullptr);
}

TEST_F(ArgumentsParserTest, ArgumentsWithSpecialCharacters) {
  const char *argv[] = {"./program", "--flag", "-v", "--output=file.txt", "arg with spaces"};
  int argc = 5;
  
  ArgumentsParser parser(argc, argv);
  
  EXPECT_EQ(parser.getArgc(), 5);
  EXPECT_STREQ(parser.getArg(0), "./program");
  EXPECT_STREQ(parser.getArg(1), "--flag");
  EXPECT_STREQ(parser.getArg(2), "-v");
  EXPECT_STREQ(parser.getArg(3), "--output=file.txt");
  EXPECT_STREQ(parser.getArg(4), "arg with spaces");
}

TEST_F(ArgumentsParserTest, ArgumentsWithEmptyStrings) {
  const char *argv[] = {"program", "", "arg2"};
  int argc = 3;
  
  ArgumentsParser parser(argc, argv);
  
  EXPECT_EQ(parser.getArgc(), 3);
  EXPECT_STREQ(parser.getArg(0), "program");
  EXPECT_STREQ(parser.getArg(1), "");
  EXPECT_STREQ(parser.getArg(2), "arg2");
}
