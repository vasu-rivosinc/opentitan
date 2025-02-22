// Copyright lowRISC contributors.
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#ifndef OPENTITAN_SW_DEVICE_LIB_CRYPTO_INCLUDE_RSA_H_
#define OPENTITAN_SW_DEVICE_LIB_CRYPTO_INCLUDE_RSA_H_

#include "sw/device/lib/crypto/include/datatypes.h"

/**
 * @file
 * @brief RSA signature operations for the OpenTitan cryptography library.
 */

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

/**
 * Enum to define padding scheme for RSA data.
 *
 * Values are hardened.
 */
typedef enum rsa_padding {
  // Pads input data according to the PKCS#1 (v1.5) scheme.
  kRsaPaddingPkcs = 0x94e,
  // Pads input data according to the PKCS#1-PSS scheme. The mask generation
  // function is MGF1 with the same hash function as the input message
  // (supported SHA2 or SHA3 hash functions only).
  kRsaPaddingPss = 0x6b1,
} rsa_padding_t;

/**
 * Enum to define possible lengths of RSA (public) keys.
 *
 * Values are hardened.
 */
typedef enum rsa_size {
  // 2048-bit RSA.
  kRsaSize2048 = 0x5d1,
  // 3072-bit RSA.
  kRsaSize3072 = 0xc35,
  // 4096-bit RSA.
  kRsaSize4096 = 0x8da,
} rsa_size_t;

enum {
  /**
   * Number of bytes needed for RSA public keys.
   *
   * The exact representation is an implementation-specific detail and subject
   * to change. This is the length that the caller should set as `key_length`
   * and allocate for the `key` buffer in unblinded keys.
   */
  kRsa2048PublicKeyBytes = 260,
  kRsa3072PublicKeyBytes = 388,
  kRsa4096PublicKeyBytes = 516,
  /**
   * Number of bytes needed for RSA private keys.
   *
   * The exact representation is an implementation-specific detail and subject
   * to change. This is the length that the caller should set in `key_length`
   * for the blinded key configuration (NOT the blinded keyblob length).
   */
  kRsa2048PrivateKeyBytes = 256,
  kRsa3072PrivateKeyBytes = 384,
  kRsa4096PrivateKeyBytes = 512,
  /**
   * Number of bytes needed for RSA private keyblobs.
   *
   * The exact representation is an implementation-specific detail and subject
   * to change. This is the length that the caller should set in
   * `keyblob_length` and allocate for the `keyblob` buffer in blinded keys.
   */
  kRsa2048PrivateKeyblobBytes = 512,
  kRsa3072PrivateKeyblobBytes = 768,
  kRsa4096PrivateKeyblobBytes = 1024,
};

/**
 * Performs the RSA key generation.
 *
 * Computes RSA private key (d) and RSA public key exponent (e) and
 * modulus (n).
 *
 * The caller should allocate space for the public key and set the `key` and
 * `key_length` fields accordingly.
 *
 * The caller should fully populate the blinded key configuration and allocate
 * space for the keyblob, setting `config.key_length` and `keyblob_length`
 * accordingly.
 *
 * The value in the `checksum` field of key structs is not checked here and
 * will be populated by the key generation function.
 *
 * @param size RSA size parameter.
 * @param[out] public_key Pointer to public key struct.
 * @param[out] private_key Pointer to blinded private key struct.
 * @return Result of the RSA key generation.
 */
crypto_status_t otcrypto_rsa_keygen(rsa_size_t size,
                                    crypto_unblinded_key_t *public_key,
                                    crypto_blinded_key_t *private_key);

/**
 * Constructs an RSA public key from the modulus and public exponent.
 *
 * The caller should allocate space for the public key and set the `key` and
 * `key_length` fields accordingly.
 *
 * @param size RSA size parameter.
 * @param modulus RSA modulus (n).
 * @param exponent RSA public exponent (e).
 * @param[out] public_key Destination public key struct.
 * @return Result of the RSA key construction.
 */
crypto_status_t otcrypto_rsa_public_key_construct(
    rsa_size_t size, crypto_const_word32_buf_t modulus, uint32_t exponent,
    crypto_unblinded_key_t *public_key);

/**
 * Constructs an RSA private key from the modulus and public/private exponents.
 *
 * The caller should allocate space for the private key and set the `keyblob`,
 * `keyblob_length`, and `key_length` fields accordingly.
 *
 * @param size RSA size parameter.
 * @param modulus RSA modulus (n).
 * @param exponent RSA public exponent (e).
 * @param d_share0 First share of the RSA private exponent d.
 * @param d_share1 Second share of the RSA private exponent d.
 * @param[out] public_key Destination public key struct.
 * @return Result of the RSA key construction.
 */
crypto_status_t otcrypto_rsa_private_key_from_exponents(
    rsa_size_t size, crypto_const_word32_buf_t modulus, uint32_t e,
    crypto_const_word32_buf_t d_share0, crypto_const_word32_buf_t d_share1,
    crypto_blinded_key_t *private_key);

/**
 * Computes the digital signature on the input message data.
 *
 * The caller should allocate space for the `signature` buffer
 * and set the length of expected output in the `len` field of
 * `signature`. If the user-set length and the output length does not
 * match, an error message will be returned.
 *
 * @param private_key Pointer to blinded private key struct.
 * @param message_digest Message digest to be signed (pre-hashed).
 * @param padding_mode Padding scheme to be used for the data.
 * @param[out] signature Pointer to the generated signature struct.
 * @return The result of the RSA signature generation.
 */
crypto_status_t otcrypto_rsa_sign(const crypto_blinded_key_t *private_key,
                                  const hash_digest_t *message_digest,
                                  rsa_padding_t padding_mode,
                                  crypto_word32_buf_t *signature);

/**
 * Verifies the authenticity of the input signature.
 *
 * An "OK" status code does not mean that the signature passed verification;
 * the caller must check both the returned status and `verification_result`
 * before trusting the signature.
 *
 * @param public_key Pointer to public key struct.
 * @param message_digest Message digest to be verified (pre-hashed).
 * @param padding_mode Padding scheme to be used for the data.
 * @param signature Pointer to the input signature to be verified.
 * @param[out] verification_result Result of signature verification.
 * @return Result of the RSA verify operation.
 */
crypto_status_t otcrypto_rsa_verify(const crypto_unblinded_key_t *public_key,
                                    const hash_digest_t *message_digest,
                                    rsa_padding_t padding_mode,
                                    crypto_const_word32_buf_t signature,
                                    hardened_bool_t *verification_result);

/**
 * Starts the asynchronous RSA key generation function.
 *
 * Initializes OTBN and starts the OTBN routine to compute the RSA
 * private key (d), RSA public key exponent (e) and modulus (n).
 *
 * Returns `kCryptoStatusOK` if the operation was successfully
 * started, or`kCryptoStatusInternalError` if the operation cannot be
 * started.
 *
 * @param size RSA size parameter.
 * @return Result of async RSA keygen start operation.
 */
crypto_status_t otcrypto_rsa_keygen_async_start(rsa_size_t size);

/**
 * Finalizes the asynchronous RSA key generation function.
 *
 * See `otcrypto_rsa_keygen` for details on the requirements for `public_key`
 * and `private_key`.
 *
 * @param[out] public_key Pointer to public key struct.
 * @param[out] private_key Pointer to blinded private key struct.
 * @return Result of asynchronous RSA keygen finalize operation.
 */
crypto_status_t otcrypto_rsa_keygen_async_finalize(
    crypto_unblinded_key_t *public_key, crypto_blinded_key_t *private_key);

/**
 * Starts the asynchronous digital signature generation function.
 *
 * Initializes OTBN and starts the OTBN routine to compute the digital
 * signature on the input message.
 *
 * Returns `kCryptoStatusOK` if the operation was successfully
 * started, or`kCryptoStatusInternalError` if the operation cannot be
 * started.
 *
 * @param private_key Pointer to blinded private key struct.
 * @param message_digest Message digest to be signed (pre-hashed).
 * @param padding_mode Padding scheme to be used for the data.
 * @return Result of async RSA sign start operation.
 */
crypto_status_t otcrypto_rsa_sign_async_start(
    const crypto_blinded_key_t *private_key,
    const hash_digest_t *message_digest, rsa_padding_t padding_mode);

/**
 * Finalizes the asynchronous digital signature generation function.
 *
 * See `otcrypto_rsa_sign` for details on the requirements for `signature`.
 *
 * @param[out] signature Pointer to generated signature struct.
 * @return Result of async RSA sign finalize operation.
 */
crypto_status_t otcrypto_rsa_sign_async_finalize(
    crypto_word32_buf_t *signature);

/**
 * Starts the asynchronous signature verification function.
 *
 * Initializes OTBN and starts the OTBN routine to recover the message
 * from the input signature.
 *
 * @param public_key Pointer to public key struct.
 * @param signature Pointer to the input signature to be verified.
 * @return Result of async RSA verify start operation.
 */
crypto_status_t otcrypto_rsa_verify_async_start(
    const crypto_unblinded_key_t *public_key,
    crypto_const_word32_buf_t signature);

/**
 * Finalizes the asynchronous signature verification function.
 *
 * An "OK" status code does not mean that the signature passed verification;
 * the caller must check both the returned status and `verification_result`
 * before trusting the signature.
 *
 * @param message_digest Message digest to be verified (pre-hashed).
 * @param padding_mode Padding scheme to be used for the data.
 * @param[out] verification_result Result of signature verification.
 * @return Result of async RSA verify finalize operation.
 */
crypto_status_t otcrypto_rsa_verify_async_finalize(
    const hash_digest_t *message_digest, rsa_padding_t padding_mode,
    hardened_bool_t *verification_result);

#ifdef __cplusplus
}  // extern "C"
#endif  // __cplusplus

#endif  // OPENTITAN_SW_DEVICE_LIB_CRYPTO_INCLUDE_RSA_H_
