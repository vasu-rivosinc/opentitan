// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//
// xbar_env_pkg__params generated by `tlgen.py` tool


// List of Xbar device memory map
tl_device_t xbar_devices[$] = '{
    '{"uart", '{
        '{32'h40000000, 32'h40000fff}
    }},
    '{"gpio", '{
        '{32'h40010000, 32'h40010fff}
    }},
    '{"spi_device", '{
        '{32'h40020000, 32'h40020fff}
    }},
    '{"rv_timer", '{
        '{32'h40080000, 32'h40080fff}
    }},
    '{"i2c", '{
        '{32'h40050000, 32'h40050fff}
    }},
    '{"usbdev", '{
        '{32'h40150000, 32'h40150fff}
    }},
    '{"pwrmgr", '{
        '{32'h400a0000, 32'h400a0fff}
    }},
    '{"rstmgr", '{
        '{32'h400b0000, 32'h400b0fff}
    }},
    '{"clkmgr", '{
        '{32'h400c0000, 32'h400c0fff}
    }},
    '{"pattgen", '{
        '{32'h400e0000, 32'h400e0fff}
}}};

  // List of Xbar hosts
tl_host_t xbar_hosts[$] = '{
    '{"main", 0, '{
        "uart",
        "gpio",
        "spi_device",
        "rv_timer",
        "usbdev",
        "pwrmgr",
        "rstmgr",
        "clkmgr",
        "i2c",
        "pattgen"}}
};
