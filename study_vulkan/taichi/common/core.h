/*******************************************************************************
    copyright (c) the taichi authors (2016- ). all rights reserved.
    the use of this software is governed by the license file.
*******************************************************************************/

#pragma once

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

//******************************************************************************
//                                 System State
//******************************************************************************

// Reference:
// https://blog.kowalczyk.info/article/j/guide-to-predefined-macros-in-c-compilers-gcc-clang-msvc-etc..html

// Platforms
#include "taichi/common/common.h"

void taichi_raise_assertion_failure_in_python(const char *msg);

namespace taichi {

//******************************************************************************
//                                 System State
//******************************************************************************

class CoreState {
 public:
  bool python_imported = false;
  bool trigger_gdb_when_crash = false;

  static CoreState &get_instance();

  static void set_python_imported(bool val) {
    get_instance().python_imported = val;
  }

  static void set_trigger_gdb_when_crash(bool val) {
    get_instance().trigger_gdb_when_crash = val;
  }
};

}  // namespace taichi
//******************************************************************************
//                           Meta-programming
//******************************************************************************

#include "taichi/util/meta.h"
#include "taichi/common/logging.h"

namespace taichi {

namespace zip {

void write(std::string fn, const uint8 *data, std::size_t len);
void write(const std::string &fn, const std::string &data);
std::vector<uint8> read(const std::string fn, bool verbose = false);

}  // namespace zip

}  // namespace taichi

//******************************************************************************
//                               Serialization
//******************************************************************************

#include "taichi/common/serialization.h"

//******************************************************************************
//                                   Misc.
//******************************************************************************

namespace taichi {

extern int __trash__;
template <typename T>
void trash(T &&t) {
  static_assert(!std::is_same<T, void>::value, "");
  __trash__ = *reinterpret_cast<uint8 *>(&t);
}

class DeferedExecution {
  std::function<void(void)> statement_;

 public:
  explicit DeferedExecution(const std::function<void(void)> &statement)
      : statement_(statement) {
  }

  ~DeferedExecution() {
    statement_();
  }
};

#define TI_DEFER(x) taichi::DeferedExecution _defered([&]() { x; });

std::string get_repo_dir();

std::string get_python_package_dir();

void set_python_package_dir(const std::string &dir);

inline std::string assets_dir() {
  return get_repo_dir() + "/assets/";
}

std::string cpp_demangle(const std::string &mangled_name);

int get_version_major();

int get_version_minor();

int get_version_patch();

std::string get_version_string();

std::string get_commit_hash();

std::string get_cuda_version_string();

class PID {
 public:
  static int get_pid();
  static int get_parent_pid();
};

}  // namespace taichi
