load("//transpiler:fhe.bzl", "fhe_cc_library")

licenses(["notice"])

cc_library(
    name = "rock_paper_scissor",
    srcs = ["rock_paper_scissor.cc"],
    hdrs = ["rock_paper_scissor.h"],
)

fhe_cc_library(
    name = "rock_paper_scissor_tfhe",
    src = "rock_paper_scissor.cc",
    hdrs = ["rock_paper_scissor.h"],
    encryption = "tfhe",
    optimizer = "xls",
)

fhe_cc_library(
    name = "rock_paper_scissor_interpreted_tfhe",
    src = "rock_paper_scissor.cc",
    hdrs = ["rock_paper_scissor.h"],
    encryption = "tfhe",
    interpreter = True,
    optimizer = "xls",
)

fhe_cc_library(
    name = "rock_paper_scissor_openfhe",
    src = "rock_paper_scissor.cc",
    hdrs = ["rock_paper_scissor.h"],
    encryption = "openfhe",
    optimizer = "xls",
)

fhe_cc_library(
    name = "rock_paper_scissor_interpreted_openfhe",
    src = "rock_paper_scissor.cc",
    hdrs = ["rock_paper_scissor.h"],
    encryption = "openfhe",
    interpreter = True,
    optimizer = "xls",
)

cc_binary(
    name = "rock_paper_scissor_tfhe_testbench",
    srcs = ["rock_paper_scissor_tfhe_testbench.cc"],
    deps = [
        ":rock_paper_scissor_tfhe",
        "//transpiler/data:tfhe_data",
        "@com_google_xls//xls/common/logging",
        "@com_google_xls//xls/common/status:status_macros",
        "@tfhe//:libtfhe",
    ],
)

cc_binary(
    name = "rock_paper_scissor_interpreted_tfhe_testbench",
    srcs = ["rock_paper_scissor_tfhe_testbench.cc"],
    copts = ["-DUSE_INTERPRETED_TFHE"],
    deps = [
        ":rock_paper_scissor_interpreted_tfhe",
        "//transpiler/data:tfhe_data",
        "@com_google_xls//xls/common/logging",
        "@com_google_xls//xls/common/status:status_macros",
        "@tfhe//:libtfhe",
    ],
)

cc_binary(
    name = "rock_paper_scissor_openfhe_testbench",
    srcs = ["rock_paper_scissor_openfhe_testbench.cc"],
    deps = [
        ":rock_paper_scissor_openfhe",
        "//transpiler/data:openfhe_data",
        "@com_google_xls//xls/common/logging",
        "@com_google_xls//xls/common/status:status_macros",
        "@openfhe//:binfhe",
    ],
)

cc_binary(
    name = "rock_paper_scissor_interpreted_openfhe_testbench",
    srcs = ["rock_paper_scissor_openfhe_testbench.cc"],
    copts = ["-DUSE_INTERPRETED_OPENFHE"],
    deps = [
        ":rock_paper_scissor_interpreted_openfhe",
        "//transpiler/data:openfhe_data",
        "@com_google_xls//xls/common/logging",
        "@com_google_xls//xls/common/status:status_macros",
        "@openfhe//:binfhe",
    ],
)

cc_test(
    name = "rock_paper_scissor_test",
    srcs = ["rock_paper_scissor_test.cc"],
    deps = [
        ":rock_paper_scissor",
        "@com_google_googletest//:gtest_main",
        "@com_google_xls//xls/common/status:matchers",
    ],
)
