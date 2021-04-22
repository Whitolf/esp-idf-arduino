/**
 * \file padlock.h
 *
 * \brief VIA PadLock ACE for HW encryption/decryption supported by some
 *        processors
 *
 * \warning These functions are only for internal use by other library
 *          functions; you must not call them directly.
 */
/*
 *  Copyright (C) 2006-2015, ARM Limited, All Rights Reserved
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 *
 *  This file is provided under the Apache License 2.0, or the
 *  GNU General Public License v2.0 or later.
 *
 *  **********
 *  Apache License 2.0:
 *
 *  Licensed under the Apache License, Version 2.0 (the "License"); you may
 *  not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  **********
 *
 *  **********
 *  GNU General Public License v2.0 or later:
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *  **********
 *
 *  This file is part of mbed TLS (https://tls.mbed.org)
 */
#ifndef MBEDTLS_PADLOCK_H
#define MBEDTLS_PADLOCK_H

#if !defined(MBEDTLS_CONFIG_FILE)
#include "config.h"
#else
#include MBEDTLS_CONFIG_FILE
#endif

#include "aes.h"

#define MBEDTLS_ERR_PADLOCK_DATA_MISALIGNED               -0x0030  /**< Input data should be aligned. */

#if defined(__has_feature)
#if __has_feature(address_sanitizer)
#define MBEDTLS_HAVE_ASAN
#endif
#endif

/* Some versions of ASan result in errors about not enough registers */
#if defined(MBEDTLS_HAVE_ASM) && defined(__GNUC__) && defined(__i386__) && \
    !defined(MBEDTLS_HAVE_ASAN)

#ifndef MBEDTLS_HAVE_X86
#define MBEDTLS_HAVE_X86
#endif

#include <stdint.h>

#define MBEDTLS_PADLOCK_RNG 0x000C
#define MBEDTLS_PADLOCK_ACE 0x00C0
#define MBEDTLS_PADLOCK_PHE 0x0C00
#define MBEDTLS_PADLOCK_PMM 0x3000

#define MBEDTLS_PADLOCK_ALIGN16(x) (uint32_t *) (16 + ((int32_t) (x) & ~15))

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          Internal PadLock detection routine
 *
 * \note           This function is only for internal use by other library
 *                 functions; you must not call it directly.
 *
 * \param feature  The feature to detect
 *
 * \return         1 if CPU has support for the feature, 0 otherwise
 */
int mbedtls_padlock_has_support( int feature );

/**
 * \brief          Internal PadLock AES-ECB block en(de)cryption
 *
 * \note           This function is only for internal use by other library
 *                 functions; you must not call it directly.
 *
 * \param ctx      AES context
 * \param mode     MBEDTLS_AES_ENCRYPT or MBEDTLS_AES_DECRYPT
 * \param input    16-byte input block
 * \param output   16-byte output block
 *
 * \return         0 if success, 1 if operation failed
 */
int mbedtls_padlock_xcryptecb( mbedtls_aes_context *ctx,
                               int mode,
                               const unsigned char input[16],
                               unsigned char output[16] );

/**
 * \brief          Internal PadLock AES-CBC buffer en(de)cryption
 *
 * \note           This function is only for internal use by other library
 *                 functions; you must not call it directly.
 *
 * \param ctx      AES context
 * \param mode     MBEDTLS_AES_ENCRYPT or MBEDTLS_AES_DECRYPT
 * \param length   length of the input data
 * \param iv       initialization vector (updated after use)
 * \param input    buffer holding the input data
 * \param output   buffer holding the output data
 *
 * \return         0 if success, 1 if operation failed
 */
int mbedtls_padlock_xcryptcbc( mbedtls_aes_context *ctx,
                               int mode,
                               size_t length,
                               unsigned char iv[16],
                               const unsigned char *input,
                               unsigned char *output );

#ifdef __cplusplus
}
#endif

#endif /* HAVE_X86  */

#endif /* padlock.h */
