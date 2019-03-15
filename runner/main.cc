/*
 * Copyright 2016 WebAssembly Community Group participants
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <algorithm>
#include <cassert>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

#include "src/option-parser.h"
#include "runtime.h"

using namespace wabt;

static const char* s_infile;
static const char* s_fn_name = "apply";
static int s_param = 0;
static Features s_features;

static const char s_description[] =
    R"(  run apply function from wasm file.

examples:
  # parse binary file test.wasm, and execute funciton 'apply' with paramter int '9'
  $ wasm-runner test.wasm apply -p 9
)";

static void ParseOptions(int argc, char** argv) {
  OptionParser parser("wasm-runner", s_description);

  parser.AddHelpOption();
  s_features.AddOptions(&parser);

  parser.AddArgument("filename", OptionParser::ArgumentCount::One,
                     [](const char* argument) { s_infile = argument; });

  parser.AddArgument("fn_name", OptionParser::ArgumentCount::One,
                     [](const char* argument) { s_fn_name = argument; });

  parser.AddOption('p', "apply-parameter", "PARAM",
                   "parameter for apply function",
                   [](const std::string& argument) {
                     s_param = atoi(argument.c_str());
                   });

  parser.Parse(argc, argv);
}

int ProgramMain(int argc, char** argv) {
  ParseOptions(argc, argv);

  wabt_runtime wr(s_infile);
  wr.init();

  interp::TypedValues args{1, interp::TypedValue(Type::I32)};
  args[0].set_i32(s_param);

  wr.run(s_fn_name, args, wabt::RunVerbosity::Verbose);

  return 0;
}

int main(int argc, char** argv) {
  WABT_TRY
  return ProgramMain(argc, argv);
  WABT_CATCH_BAD_ALLOC_AND_EXIT
}
