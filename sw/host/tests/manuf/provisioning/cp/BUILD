# Copyright lowRISC contributors.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

load("@rules_rust//rust:defs.bzl", "rust_binary")
load("//rules/opentitan:openocd.bzl", "OPENTITANTOOL_OPENOCD_CMSIS_DATA_DEPS")

package(default_visibility = ["//visibility:public"])

rust_binary(
    name = "cp",
    testonly = True,
    srcs = ["src/main.rs"],
    data = [
        "//sw/device/silicon_creator/manuf/skus/earlgrey_a0/sival_bringup:sram_cp_provision",
    ] + OPENTITANTOOL_OPENOCD_CMSIS_DATA_DEPS,
    deps = [
        "//sw/host/opentitanlib",
        "//sw/host/provisioning/cp_lib",
        "//sw/host/provisioning/ujson_lib",
        "//sw/host/provisioning/util_lib",
        "@crate_index//:anyhow",
        "@crate_index//:clap",
        "@crate_index//:humantime",
        "@crate_index//:log",
    ],
)
