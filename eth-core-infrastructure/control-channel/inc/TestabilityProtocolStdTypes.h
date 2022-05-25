/**
 *  Copyright (C) [2021] Focus Corporation
 *
 *  \file   TestabilityProtocolStdTypes.h
 *  @brief  AUTOSAR Base - Testability Protocol.
 *  \author uid1145 Mejdi Zayani
 *
 *  \addtogroup Testability_Protocol_package
 *  \{
 */
#ifndef TESTABILITYPROTOCOLSTDTYPES_H
#define TESTABILITYPROTOCOLSTDTYPES_H
#ifdef __cplusplus
extern "C"
{
#endif  /* #ifdef __cplusplus*/
/*==================================================================================================
*                                        Include Files
==================================================================================================*/

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
#define IPV4_ADDRESS_SIZE 4U  /*  4 bytes */
#define IPV6_ADDRESS_SIZE 16U /* 16 bytes */

#ifndef TRUE
#define TRUE (boolean)1
#endif
#ifndef FALSE
#define FALSE (boolean)0
#endif

#ifndef NULL_PTR
#define NULL_PTR ((void *)0)
#endif
#ifndef NULL
#define NULL 0
#endif

/*==================================================================================================
*                                             TYPES
==================================================================================================*/

/** 
* @brief The standard AUTOSAR type boolean shall be implemented on basis of an eight bits long 
*        unsigned integer.
*   range: 0, 1 … 255 [0x00, 0x01 - 0xFF] (false, true) 
* @implements boolean_t
*/
typedef unsigned char boolean;

/** 
* @brief Unsigned 8 bit integer with range of 0 ..+255 (0x00..0xFF) - 
*        8 bit
* @implements uint8_type
*/
typedef unsigned char uint8;

/** 
* @brief Unsigned 16 bit integer with range of 0 ..+65535 (0x0000..0xFFFF) - 
*        16 bit
* @implements uint16_type
*/
typedef unsigned short int uint16;

/** 
* @brief Unsigned 32 bit integer with range of 0 ..+4294967295 (0x00000000..0xFFFFFFFF) - 
*        32 bit
* @implements uint32_type
*/
typedef unsigned int uint32;

/** 
* @brief Unsigned 64 bit integer with range of 0..18446744073709551615 (0x0000000000000000..0xFFFFFFFFFFFFFFFF)-
*        64 bit
* 
*/
typedef unsigned long int uint64;

/** 
* @brief Signed 8 bit integer with range of -128 ..+127 (0x80..0x7F) - 
*        7 bit + 1 sign bit 
* @implements sint8_type
*/
typedef signed char sint8;

/** 
* @brief Signed 16 bit integer with range of -32768 ..+32767 (0x8000..0x7FFF) - 
*        15 bit + 1 sign bit 
* @implements sint16_type
*/
typedef signed  short int sint16;

/** 
* @brief Signed 32 bit integer with range of -2147483648.. +2147483647 (0x80000000..0x7FFFFFFF) - 
*        31 bit + 1 sign bit 
* @implements sint32_type
*/
typedef signed int sint32;

/** 
* @brief Signed 64 bit integer with range of -9223372036854775808..9223372036854775807 (0x8000000000000000..0x7FFFFFFFFFFFFFFF )- 
*        63 bit + 1 sign bit 
* 
*/
typedef signed long int sint64;

/** 
* @brief 32bit long floating point data type
* @implements float32_type
*/
typedef float float32;

/** 
* @brief 64bit long floating point data type
* @implements float64_type
*/
typedef double float64;

#pragma pack(1)
/** 
* @brief A variable length type always contains a uint16 variable named dataLength to indicate the
* length of following elements of type uint8.
*
* @implements vint8
*/
typedef struct vint8
{
    uint16 dataLength;
    uint8 *Data;
} vint8;

/** 
* @brief Data type contains the ipv4 address
* @implements ip4addr
*/
typedef struct ip4addr
{
    uint16 dataLength;
    uint8 Data[IPV4_ADDRESS_SIZE];
} ip4addr;

/** 
* @brief Data type contains the ipv6 address
* @implements ip6addr
*/
typedef struct ip6addr
{
    uint16 dataLength;
    uint8 Data[IPV6_ADDRESS_SIZE];
} ip6addr;

/** 
* @brief Data type contains the ipv4 address | ipv6 address
* @implements ipxaddr
*/
typedef union
{
    ip6addr ipv6_Addr;
    ip4addr ipv4_Addr;
} ipxaddr;

/** 
* @brief  Dynamic length character strings transporting ascii, UTF-8. 
* The data field starts with Byte Order Mark (BOM) and ends by null termination.
* @implements text
*/
typedef vint8 text;

/**
* @brief  Dynamic linked list: contains all the opened socket used for TCP/UDP
* @implements TP_socket_t
*/
typedef struct TP_socket_t
{
    uint16 socketId;
    boolean RecvFwdActive;
    uint16 maxFwd;
    uint16 maxLen;
    uint16 dropCnt;
    uint8 *recvBuffer;
    pthread_t pthread_id;
    struct TP_socket_t *next;
} TP_socket_t;

/**
* @brief  Dynamic array: contains all the created threads
* @implements TP_thread_t
*/
typedef struct TP_threadSocket_t
{
    uint32 numOfThread;
    pthread_t  *pthread_id;
} TP_threadSocket_t;

/** 
* @brief
* @implements Tcp_ListenAndAcceptParam_t
*/
typedef struct Tcp_ListenAndAcceptParam_t{
    int socketId;
    uint16 maxCon;
} Tcp_ListenAndAcceptParam_t;

#pragma pack(0)

#ifdef __cplusplus
}
#endif  /* #ifdef __cplusplus*/
#endif /* TESTABILITYPROTOCOLSTDTYPES_H */

/** \}    end of addtogroup */
