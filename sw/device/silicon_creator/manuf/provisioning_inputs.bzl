# Copyright lowRISC contributors.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0

EARLGREY_A0_INDIVIDUALIZE_OTP_SW_CFGS = [
    "sival_bringup",
    "sival",
]

_DEVICE_ID_AND_TEST_TOKENS = """
  --device-id="0x11111111_22222222_33333333_44444444_55555555_66666666_77777777_88888888"
  --test-unlock-token="0x11111111_11111111_11111111_11111111"
  --test-exit-token="0x11111111_11111111_11111111_11111111"
"""

CP_PROVISIONING_INPUTS = _DEVICE_ID_AND_TEST_TOKENS + """
  --manuf-state="0x00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000"
  --wafer-auth-secret="0x00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000"
"""

FT_PROVISIONING_INPUTS = _DEVICE_ID_AND_TEST_TOKENS + """
  --target-mission-mode-lc-state="prod"
  --host-ecc-sk="$(rootpath //sw/device/silicon_creator/manuf/keys/fake:rma_unlock_token_export_key.sk_hsm.der)"
  --rom-ext-measurement="0x00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000"
  --owner-manifest-measurement="0x00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000"
  --owner-measurement="0x00000000_00000000_00000000_00000000_00000000_00000000_00000000_00000000"
"""

FT_PERSONALIZE_SIGNING_KEYS = select({
    "//signing:test_keys": {"//sw/device/silicon_creator/rom/keys/fake/rsa:prod_private_key_0": "prod_key_0"},
    "//conditions:default": {"//sw/device/silicon_creator/rom/keys/real/rsa:keyset": "earlgrey_a0_dev_0"},
})
