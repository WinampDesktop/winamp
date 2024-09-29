						/*
                        *  Minimal SSL client, used for memory measurements.
                        *  (meant to be used with config-suite-b.h or config-ccm-psk-tls1_2.h)
                        *
                        *  Copyright The Mbed TLS Contributors
                        *  SPDX-License-Identifier: Apache-2.0
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
                        */
						
						#if !defined(MBEDTLS_CONFIG_FILE)
						#include "mbedtls/config.h"
						#else
						#include MBEDTLS_CONFIG_FILE
						#endif
						
						#if defined(MBEDTLS_PLATFORM_C)
						#include "mbedtls/platform.h"
						#else
						#include <stdio.h>
						#include <stdlib.h>
						#define mbedtls_printf          printf
						#define mbedtls_exit            exit
						#define MBEDTLS_EXIT_SUCCESS    EXIT_SUCCESS
						#define MBEDTLS_EXIT_FAILURE    EXIT_FAILURE
						#endif
						
						/*
                        * We're creating and connecting the socket "manually" rather than using the
                        * NET module, in order to avoid the overhead of getaddrinfo() which tends to
                        * dominate memory usage in small configurations. For the sake of simplicity,
                        * only a Unix version is implemented.
                        *
                        * Warning: we are breaking some of the abtractions from the NET layer here.
                        * This is not a good example for general use. This programs has the specific
                        * goal of minimizing use of the libc functions on full-blown OSes.
                        */
						#if defined(unix) || defined(__unix__) || defined(__unix) || defined(__APPLE__)
						#define UNIX
						#endif
						
						#if !defined(MBEDTLS_CTR_DRBG_C) || !defined(MBEDTLS_ENTROPY_C) || \
                    !defined(MBEDTLS_NET_C) || !defined(MBEDTLS_SSL_CLI_C) || \
                    !defined(UNIX)
						
						int main( void )
						{
                    mbedtls_printf( "MBEDTLS_CTR_DRBG_C and/or MBEDTLS_ENTROPY_C and/or "
            "MBEDTLS_NET_C and/or MBEDTLS_SSL_CLI_C and/or UNIX "
            "not defined.\n");
                    mbedtls_exit( 0 );
						}
						#else
						
						#include <string.h>
						
						#include "mbedtls/net_sockets.h"
						#include "mbedtls/ssl.h"
						#include "mbedtls/entropy.h"
						#include "mbedtls/ctr_drbg.h"
						
						#include <sys/socket.h>
						#include <netinet/in.h>
						#include <arpa/inet.h>
						
						/*
                        * Hardcoded values for server host and port
                        */
						#define PORT_BE 0x1151      /* 4433 */
						#define PORT_LE 0x5111
						#define ADDR_BE 0x7f000001  /* 127.0.0.1 */
						#define ADDR_LE 0x0100007f
						#define HOSTNAME "localhost" /* for cert verification if enabled */
						
						#define GET_REQUEST "GET / HTTP/1.0\r\n\r\n"
						
						const char *pers = "mini_client";
						
						#if defined(MBEDTLS_KEY_EXCHANGE_SOME_PSK_ENABLED)
						const unsigned char psk[] = {
                    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
						};
						const char psk_id[] = "Client_identity";
						#endif
						
						#if defined(MBEDTLS_X509_CRT_PARSE_C)
						/* This is tests/data_files/test-ca2.crt, a CA using EC secp384r1 */
						const unsigned char ca_cert[] = {
                    0x30, 0x82, 0x02, 0x52, 0x30, 0x82, 0x01, 0xd7, 0xa0, 0x03, 0x02, 0x01,
                    0x02, 0x02, 0x09, 0x00, 0xc1, 0x43, 0xe2, 0x7e, 0x62, 0x43, 0xcc, 0xe8,
                    0x30, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02,
                    0x30, 0x3e, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13,
                    0x02, 0x4e, 0x4c, 0x31, 0x11, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x04, 0x0a,
                    0x13, 0x08, 0x50, 0x6f, 0x6c, 0x61, 0x72, 0x53, 0x53, 0x4c, 0x31, 0x1c,
                    0x30, 0x1a, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x13, 0x50, 0x6f, 0x6c,
                    0x61, 0x72, 0x73, 0x73, 0x6c, 0x20, 0x54, 0x65, 0x73, 0x74, 0x20, 0x45,
                    0x43, 0x20, 0x43, 0x41, 0x30, 0x1e, 0x17, 0x0d, 0x31, 0x33, 0x30, 0x39,
                    0x32, 0x34, 0x31, 0x35, 0x34, 0x39, 0x34, 0x38, 0x5a, 0x17, 0x0d, 0x32,
                    0x33, 0x30, 0x39, 0x32, 0x32, 0x31, 0x35, 0x34, 0x39, 0x34, 0x38, 0x5a,
                    0x30, 0x3e, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13,
                    0x02, 0x4e, 0x4c, 0x31, 0x11, 0x30, 0x0f, 0x06, 0x03, 0x55, 0x04, 0x0a,
                    0x13, 0x08, 0x50, 0x6f, 0x6c, 0x61, 0x72, 0x53, 0x53, 0x4c, 0x31, 0x1c,
                    0x30, 0x1a, 0x06, 0x03, 0x55, 0x04, 0x03, 0x13, 0x13, 0x50, 0x6f, 0x6c,
                    0x61, 0x72, 0x73, 0x73, 0x6c, 0x20, 0x54, 0x65, 0x73, 0x74, 0x20, 0x45,
                    0x43, 0x20, 0x43, 0x41, 0x30, 0x76, 0x30, 0x10, 0x06, 0x07, 0x2a, 0x86,
                    0x48, 0xce, 0x3d, 0x02, 0x01, 0x06, 0x05, 0x2b, 0x81, 0x04, 0x00, 0x22,
                    0x03, 0x62, 0x00, 0x04, 0xc3, 0xda, 0x2b, 0x34, 0x41, 0x37, 0x58, 0x2f,
                    0x87, 0x56, 0xfe, 0xfc, 0x89, 0xba, 0x29, 0x43, 0x4b, 0x4e, 0xe0, 0x6e,
                    0xc3, 0x0e, 0x57, 0x53, 0x33, 0x39, 0x58, 0xd4, 0x52, 0xb4, 0x91, 0x95,
                    0x39, 0x0b, 0x23, 0xdf, 0x5f, 0x17, 0x24, 0x62, 0x48, 0xfc, 0x1a, 0x95,
                    0x29, 0xce, 0x2c, 0x2d, 0x87, 0xc2, 0x88, 0x52, 0x80, 0xaf, 0xd6, 0x6a,
                    0xab, 0x21, 0xdd, 0xb8, 0xd3, 0x1c, 0x6e, 0x58, 0xb8, 0xca, 0xe8, 0xb2,
                    0x69, 0x8e, 0xf3, 0x41, 0xad, 0x29, 0xc3, 0xb4, 0x5f, 0x75, 0xa7, 0x47,
                    0x6f, 0xd5, 0x19, 0x29, 0x55, 0x69, 0x9a, 0x53, 0x3b, 0x20, 0xb4, 0x66,
                    0x16, 0x60, 0x33, 0x1e, 0xa3, 0x81, 0xa0, 0x30, 0x81, 0x9d, 0x30, 0x1d,
                    0x06, 0x03, 0x55, 0x1d, 0x0e, 0x04, 0x16, 0x04, 0x14, 0x9d, 0x6d, 0x20,
                    0x24, 0x49, 0x01, 0x3f, 0x2b, 0xcb, 0x78, 0xb5, 0x19, 0xbc, 0x7e, 0x24,
                    0xc9, 0xdb, 0xfb, 0x36, 0x7c, 0x30, 0x6e, 0x06, 0x03, 0x55, 0x1d, 0x23,
                    0x04, 0x67, 0x30, 0x65, 0x80, 0x14, 0x9d, 0x6d, 0x20, 0x24, 0x49, 0x01,
                    0x3f, 0x2b, 0xcb, 0x78, 0xb5, 0x19, 0xbc, 0x7e, 0x24, 0xc9, 0xdb, 0xfb,
                    0x36, 0x7c, 0xa1, 0x42, 0xa4, 0x40, 0x30, 0x3e, 0x31, 0x0b, 0x30, 0x09,
                    0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x4e, 0x4c, 0x31, 0x11, 0x30,
                    0x0f, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x13, 0x08, 0x50, 0x6f, 0x6c, 0x61,
                    0x72, 0x53, 0x53, 0x4c, 0x31, 0x1c, 0x30, 0x1a, 0x06, 0x03, 0x55, 0x04,
                    0x03, 0x13, 0x13, 0x50, 0x6f, 0x6c, 0x61, 0x72, 0x73, 0x73, 0x6c, 0x20,
                    0x54, 0x65, 0x73, 0x74, 0x20, 0x45, 0x43, 0x20, 0x43, 0x41, 0x82, 0x09,
                    0x00, 0xc1, 0x43, 0xe2, 0x7e, 0x62, 0x43, 0xcc, 0xe8, 0x30, 0x0c, 0x06,
                    0x03, 0x55, 0x1d, 0x13, 0x04, 0x05, 0x30, 0x03, 0x01, 0x01, 0xff, 0x30,
                    0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02, 0x03,
                    0x69, 0x00, 0x30, 0x66, 0x02, 0x31, 0x00, 0xc3, 0xb4, 0x62, 0x73, 0x56,
                    0x28, 0x95, 0x00, 0x7d, 0x78, 0x12, 0x26, 0xd2, 0x71, 0x7b, 0x19, 0xf8,
                    0x8a, 0x98, 0x3e, 0x92, 0xfe, 0x33, 0x9e, 0xe4, 0x79, 0xd2, 0xfe, 0x7a,
                    0xb7, 0x87, 0x74, 0x3c, 0x2b, 0xb8, 0xd7, 0x69, 0x94, 0x0b, 0xa3, 0x67,
                    0x77, 0xb8, 0xb3, 0xbe, 0xd1, 0x36, 0x32, 0x02, 0x31, 0x00, 0xfd, 0x67,
                    0x9c, 0x94, 0x23, 0x67, 0xc0, 0x56, 0xba, 0x4b, 0x33, 0x15, 0x00, 0xc6,
                    0xe3, 0xcc, 0x31, 0x08, 0x2c, 0x9c, 0x8b, 0xda, 0xa9, 0x75, 0x23, 0x2f,
                    0xb8, 0x28, 0xe7, 0xf2, 0x9c, 0x14, 0x3a, 0x40, 0x01, 0x5c, 0xaf, 0x0c,
                    0xb2, 0xcf, 0x74, 0x7f, 0x30, 0x9f, 0x08, 0x43, 0xad, 0x20,
						};
						#endif /* MBEDTLS_X509_CRT_PARSE_C */
						
						enum exit_codes
						{
                    exit_ok = 0,
                    ctr_drbg_seed_failed,
                    ssl_config_defaults_failed,
                    ssl_setup_failed,
                    hostname_failed,
                    socket_failed,
                    connect_failed,
                    x509_crt_parse_failed,
                    ssl_handshake_failed,
                    ssl_write_failed,
						};
						
						
						int main( void )
						{
                    int ret = exit_ok;
                    mbedtls_net_context server_fd;
                    struct sockaddr_in addr;
						#if defined(MBEDTLS_X509_CRT_PARSE_C)
                    mbedtls_x509_crt ca;
						#endif
						
                    mbedtls_entropy_context entropy;
                    mbedtls_ctr_drbg_context ctr_drbg;
                    mbedtls_ssl_context ssl;
                    mbedtls_ssl_config conf;
                    mbedtls_ctr_drbg_init( &ctr_drbg );
						
                    /*
                    * 0. Initialize and setup stuff
                    */
                    mbedtls_net_init( &server_fd );
                    mbedtls_ssl_init( &ssl );
                    mbedtls_ssl_config_init( &conf );
						#if defined(MBEDTLS_X509_CRT_PARSE_C)
                    mbedtls_x509_crt_init( &ca );
						#endif
						
                    mbedtls_entropy_init( &entropy );
                    if( mbedtls_ctr_drbg_seed( &ctr_drbg, mbedtls_entropy_func, &entropy,
    (const unsigned char *) pers, strlen( pers ) ) != 0 )
                    {
                ret = ctr_drbg_seed_failed;
                goto exit;
                    }
						
                    if( mbedtls_ssl_config_defaults( &conf,
        MBEDTLS_SSL_IS_CLIENT,
        MBEDTLS_SSL_TRANSPORT_STREAM,
        MBEDTLS_SSL_PRESET_DEFAULT ) != 0 )
                    {
                ret = ssl_config_defaults_failed;
                goto exit;
                    }
						
                    mbedtls_ssl_conf_rng( &conf, mbedtls_ctr_drbg_random, &ctr_drbg );
						
						#if defined(MBEDTLS_KEY_EXCHANGE_SOME_PSK_ENABLED)
                    mbedtls_ssl_conf_psk( &conf, psk, sizeof( psk ),
        (const unsigned char *) psk_id, sizeof( psk_id ) - 1 );
						#endif
						
						#if defined(MBEDTLS_X509_CRT_PARSE_C)
                    if( mbedtls_x509_crt_parse_der( &ca, ca_cert, sizeof( ca_cert ) ) != 0 )
                    {
                ret = x509_crt_parse_failed;
                goto exit;
                    }
						
                    mbedtls_ssl_conf_ca_chain( &conf, &ca, NULL );
                    mbedtls_ssl_conf_authmode( &conf, MBEDTLS_SSL_VERIFY_REQUIRED );
						#endif
						
                    if( mbedtls_ssl_setup( &ssl, &conf ) != 0 )
                    {
                ret = ssl_setup_failed;
                goto exit;
                    }
						
						#if defined(MBEDTLS_X509_CRT_PARSE_C)
                    if( mbedtls_ssl_set_hostname( &ssl, HOSTNAME ) != 0 )
                    {
                ret = hostname_failed;
                goto exit;
                    }
						#endif
						
                    /*
                    * 1. Start the connection
                    */
                    memset( &addr, 0, sizeof( addr ) );
                    addr.sin_family = AF_INET;
						
                    ret = 1; /* for endianness detection */
                    addr.sin_port = *((char *) &ret) == ret ? PORT_LE : PORT_BE;
                    addr.sin_addr.s_addr = *((char *) &ret) == ret ? ADDR_LE : ADDR_BE;
                    ret = 0;
						
                    if( ( server_fd.fd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
                    {
                ret = socket_failed;
                goto exit;
                    }
						
                    if( connect( server_fd.fd,
        (const struct sockaddr *) &addr, sizeof( addr ) ) < 0 )
                    {
                ret = connect_failed;
                goto exit;
                    }
						
                    mbedtls_ssl_set_bio( &ssl, &server_fd, mbedtls_net_send, mbedtls_net_recv, NULL );
						
                    if( mbedtls_ssl_handshake( &ssl ) != 0 )
                    {
                ret = ssl_handshake_failed;
                goto exit;
                    }
						
                    /*
                    * 2. Write the GET request and close the connection
                    */
                    if( mbedtls_ssl_write( &ssl, (const unsigned char *) GET_REQUEST,
sizeof( GET_REQUEST ) - 1 ) <= 0 )
                    {
                ret = ssl_write_failed;
                goto exit;
                    }
						
                    mbedtls_ssl_close_notify( &ssl );
						
						exit:
                    mbedtls_net_free( &server_fd );
						
                    mbedtls_ssl_free( &ssl );
                    mbedtls_ssl_config_free( &conf );
                    mbedtls_ctr_drbg_free( &ctr_drbg );
                    mbedtls_entropy_free( &entropy );
						#if defined(MBEDTLS_X509_CRT_PARSE_C)
                    mbedtls_x509_crt_free( &ca );
						#endif
						
                    mbedtls_exit( ret );
						}
						#endif
						