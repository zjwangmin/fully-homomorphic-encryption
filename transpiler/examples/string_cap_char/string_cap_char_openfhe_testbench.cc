// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <time.h>

#include <array>
#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include "absl/time/clock.h"
#include "absl/time/time.h"
#include "palisade/binfhe/binfhecontext.h"
#include "transpiler/data/openfhe_data.h"
#include "transpiler/examples/string_cap_char/string_cap_char_openfhe.h"
#include "transpiler/examples/string_cap_char/string_cap_char_openfhe.types.h"
#include "xls/common/logging/logging.h"

constexpr int kMainMinimumLambda = 120;

void OpenFheStringCap(OpenFheString& cipherresult, OpenFheString& ciphertext,
                      int data_size, OpenFheState& cipherstate,
                      lbcrypto::BinFHEContext cc) {
  absl::Duration total_time = absl::ZeroDuration();
  double total_cpu_time = 0.0;
  for (int i = 0; i < data_size; i++) {
    const absl::Time start_time = absl::Now();
    const double cpu_start_time = clock();
    XLS_CHECK_OK(my_package(cipherresult[i], cipherstate, ciphertext[i], cc));
    const double cpu_end_time = clock();
    const absl::Time end_time = absl::Now();
    const absl::Duration char_time = end_time - start_time;
    const double cpu_char_time = cpu_end_time - cpu_start_time;
    std::cout << "\t\t\t\t\tchar " << i << ": "
              << absl::ToDoubleSeconds(char_time) << " secs ("
              << (cpu_char_time / 1'000'000) << " CPU secs)" << std::endl;
    total_time += char_time;
    total_cpu_time += cpu_char_time;
  }
  std::cout << "\t\t\t\t\t    Total time: " << absl::ToDoubleSeconds(total_time)
            << " secs" << std::endl;
  std::cout << "\t\t\t\t\tTotal CPU time: " << (total_cpu_time / 1'000'000)
            << " secs" << std::endl;
}

int main(int argc, char** argv) {
  if (argc < 2) {
    fprintf(stderr, "Usage: string_cap_fhe_testbench string_input\n\n");
    return 1;
  }

  const char* input = argv[1];

  // generate a keyset
  std::cout << "Starting key generation..." << std::endl;
  const absl::Time key_gen_start = absl::Now();

  auto cc = lbcrypto::BinFHEContext();
  cc.GenerateBinFHEContext(lbcrypto::MEDIUM);

  // generate a random key.
  std::cout << "Generating the secret key..." << std::endl;
  auto sk = cc.KeyGen();

  std::cout << "Generating the bootstrapping keys..." << std::endl;
  cc.BTKeyGen(sk);

  std::cout << "Completed key generation ("
            << absl::ToDoubleSeconds(absl::Now() - key_gen_start) << " secs)"
            << std::endl;

  std::string plaintext(input);
  size_t data_size = plaintext.size();
  std::cout << "plaintext(" << data_size << "):" << plaintext << std::endl;

  // Encrypt data
  auto ciphertext = OpenFheString::Encrypt(plaintext, cc, sk);
  std::cout << "Encryption done" << std::endl;

  std::cout << "Initial state check by decryption: " << std::endl;
  std::cout << ciphertext.Decrypt(sk) << "\n";
  std::cout << "\n";

  State st;
  OpenFheState cipherstate(cc);
  cipherstate.SetEncrypted(st, sk);
  std::cout << "\t\t\t\t\tServer side computation:" << std::endl;
  // Perform string capitalization
  OpenFheString cipher_result = {data_size, cc};
  OpenFheStringCap(cipher_result, ciphertext, data_size, cipherstate, cc);
  std::cout << "\t\t\t\t\tComputation done" << std::endl;

  std::cout << "Decrypted result: ";
  std::cout << cipher_result.Decrypt(sk) << "\n";
  std::cout << "Decryption done" << std::endl;
}
