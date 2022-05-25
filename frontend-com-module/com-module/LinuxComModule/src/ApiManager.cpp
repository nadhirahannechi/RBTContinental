#include "ApiManager.h"
#include "TestabilityProtocol_Intern.h"

bool channelState = 0;

uint16_t gen_crc16(const uint8_t *data, uint16_t size)
{
    uint16_t out = 0;
    int bits_read = 0, bit_flag;

    /* Sanity check: */
    if (data == NULL)
        return 0;

    while (size > 0)
    {
        bit_flag = out >> 15;

        /* Get next bit: */
        out <<= 1;
        out |= (*data >> bits_read) & 1; // item a) work from the least significant bits

        /* Increment bit counter: */
        bits_read++;
        if (bits_read > 7)
        {
            bits_read = 0;
            data++;
            size--;
        }

        /* Cycle check: */
        if (bit_flag)
            out ^= CRC16;
    }

    // item b) "push out" the last 16 bits
    int i;
    for (i = 0; i < 16; ++i)
    {
        bit_flag = out >> 15;
        out <<= 1;
        if (bit_flag)
            out ^= CRC16;
    }

    // item c) reverse the bits
    uint16_t crc = 0;
    i = 0x8000;
    int j = 0x0001;
    for (; i != 0; i >>= 1, j <<= 1)
    {
        if (i & out)
            crc |= j;
    }

    return crc;
}

test_result_frame_t APIrunner(internal_protocol_frame_t rf)
{
    if (!channelState)
    {
        ip4addr ipv4;
        ipv4.dataLength = 4;
        ipv4.Data[0] = 169;
        ipv4.Data[1] = 254;
        ipv4.Data[2] = 159;
        ipv4.Data[3] = 220;
        TP_OpenControlChannel(ipv4, 56000);
        channelState = 1;
    }

    test_result_frame_t test_result;
    test_result.Opcode = rf.Opcode;
    test_result.TC8ID = rf.TC8ID;
    test_result.Test = rf.Test;
    test_result.Api = rf.Api;
    test_result.Reserved = rf.Reserved;

    //TP_OpenControlChannel(ipv4, 56000);
    switch (rf.Api)
    {
    case 1:
    {
        std::cout << "starting TCP_BASICS_01 test\n";

        test_result.ErrorCode = TCP_BASICS_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 2:
    {
        std::cout << "starting TCP_BASICS_02 test\n";

        test_result.ErrorCode = TCP_BASICS_02();

        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);

        break;
    }
    case 3:
    {
        std::cout << "starting TCP_BASICS_03 test\n";
        test_result.ErrorCode = TCP_BASICS_03();

        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 4:
    {
        std::cout << "starting TCP_BASICS_04_IT1 test\n";
        test_result.ErrorCode = TCP_BASICS_04_IT1() | TCP_BASICS_04_IT2() | TCP_BASICS_04_IT3();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 5:
    {
        std::cout << "starting TCP_BASICS_05_IT1 test\n";
        test_result.ErrorCode = TCP_BASICS_05_IT1() | TCP_BASICS_05_IT2();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 6:
    {
        std::cout << "starting TCP_BASICS_06 test\n";
        test_result.ErrorCode = TCP_BASICS_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 7:
    {
        std::cout << "starting TCP_BASICS_07 test\n";
        test_result.ErrorCode = TCP_BASICS_07();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 8:
    {
        std::cout << "starting TCP_BASICS_08_IT1 test\n";
        test_result.ErrorCode = TCP_BASICS_08_IT1() | TCP_BASICS_08_IT2();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 9:
    {
        std::cout << "starting TCP_BASICS_09 test\n";
        test_result.ErrorCode = TCP_BASICS_09();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 10:
    {
        std::cout << "starting TCP_BASICS_10_IT1 test\n";
        test_result.ErrorCode = TCP_BASICS_10_IT1() | TCP_BASICS_10_IT2();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 11:
    {
        std::cout << "starting TCP_BASICS_11 test\n";
        test_result.ErrorCode = TCP_BASICS_11();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 12:
    {
        std::cout << "starting TCP_BASICS_12 test\n";
        test_result.ErrorCode = TCP_BASICS_12();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 13:
    {
        std::cout << "starting TCP_BASICS_13 test\n";
        test_result.ErrorCode = TCP_BASICS_13();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 14:
    {
        std::cout << "starting TCP_BASICS_14 test\n";
        test_result.ErrorCode = TCP_BASICS_14();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 15:
    {
        std::cout << "starting TCP_BASICS_17 test\n";
        test_result.ErrorCode = TCP_BASICS_17();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 16:
    {
        std::cout << "starting TCP_CHECKSUM_01 test\n";
        test_result.ErrorCode = TCP_CHECKSUM_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 17:
    {
        std::cout << "starting TCP_CHECKSUM_02 test\n";
        test_result.ErrorCode = TCP_CHECKSUM_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 18:
    {
        std::cout << "starting TCP_CHECKSUM_03 test\n";
        test_result.ErrorCode = TCP_CHECKSUM_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 19:
    {
        std::cout << "starting TCP_CHECKSUM_04 test\n";
        test_result.ErrorCode = TCP_CHECKSUM_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 20:
    {
        std::cout << "starting TCP_UNACCEPTABLE_01 test\n";
        test_result.ErrorCode = TCP_UNACCEPTABLE_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 21:
    {
        std::cout << "starting TCP_UNACCEPTABLE_02 test\n";
        test_result.ErrorCode = TCP_UNACCEPTABLE_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 22:
    {
        std::cout << "starting TCP_UNACCEPTABLE_03 test\n";
        test_result.ErrorCode = TCP_UNACCEPTABLE_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 23:
    {
        std::cout << "starting TCP_UNACCEPTABLE_04_IT1 test\n";
        test_result.ErrorCode = TCP_UNACCEPTABLE_04_IT1() | TCP_UNACCEPTABLE_04_IT2();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 24:
    {
        std::cout << "starting TCP_UNACCEPTABLE_05_IT1 test\n";
        test_result.ErrorCode = TCP_UNACCEPTABLE_05_IT1() | TCP_UNACCEPTABLE_05_IT2();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 25:
    {
        std::cout << "starting TCP_UNACCEPTABLE_06 test\n";
        test_result.ErrorCode = TCP_UNACCEPTABLE_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 26:
    {
        std::cout << "starting TCP_UNACCEPTABLE_07 test\n";
        test_result.ErrorCode = TCP_UNACCEPTABLE_07();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 27:
    {
        std::cout << "starting TCP_UNACCEPTABLE_08_IT1 test\n";
        test_result.ErrorCode = TCP_UNACCEPTABLE_08_IT1() | TCP_UNACCEPTABLE_08_IT2();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 28:
    {
        std::cout << "starting TCP_UNACCEPTABLE_09_IT1 test\n";
        test_result.ErrorCode = TCP_UNACCEPTABLE_09_IT1() | TCP_UNACCEPTABLE_09_IT2();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 29:
    {
        std::cout << "starting TCP_UNACCEPTABLE_10_IT1 test\n";
        test_result.ErrorCode = TCP_UNACCEPTABLE_10_IT1() | TCP_UNACCEPTABLE_10_IT2();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 30:
    {
        std::cout << "starting TCP_UNACCEPTABLE_11_IT1 test\n";
        test_result.ErrorCode = TCP_UNACCEPTABLE_11_IT1() | TCP_UNACCEPTABLE_11_IT2();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 31:
    {
        std::cout << "starting TCP_UNACCEPTABLE_12_IT1 test\n";
        test_result.ErrorCode = TCP_UNACCEPTABLE_12_IT1() | TCP_UNACCEPTABLE_12_IT2();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 32:
    {
        std::cout << "starting TCP_UNACCEPTABLE_13_IT1 test\n";
        test_result.ErrorCode = TCP_UNACCEPTABLE_13_IT1() | TCP_UNACCEPTABLE_13_IT2();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 33:
    {
        std::cout << "starting TCP_UNACCEPTABLE_14_IT1 test\n";
        test_result.ErrorCode = TCP_UNACCEPTABLE_14_IT1() | TCP_UNACCEPTABLE_14_IT2();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 34:
    {
        std::cout << "starting TCP_FLAGS_INVALID_01 test\n";
        test_result.ErrorCode = TCP_FLAGS_INVALID_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 35:
    {
        std::cout << "starting TCP_FLAGS_INVALID_02 test\n";
        test_result.ErrorCode = TCP_FLAGS_INVALID_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 36:
    {
        std::cout << "starting TCP_FLAGS_INVALID_03 test\n";
        test_result.ErrorCode = TCP_FLAGS_INVALID_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 37:
    {
        std::cout << "starting TCP_FLAGS_INVALID_04 test\n";
        test_result.ErrorCode = TCP_FLAGS_INVALID_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 38:
    {
        std::cout << "starting TCP_FLAGS_INVALID_05_IT1 test\n";
        test_result.ErrorCode = TCP_FLAGS_INVALID_05_IT1() | TCP_FLAGS_INVALID_05_IT2();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 39:
    {
        std::cout << "starting TCP_FLAGS_INVALID_06_IT1 test\n";
        test_result.ErrorCode = TCP_FLAGS_INVALID_06_IT1() | TCP_FLAGS_INVALID_06_IT2();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 40:
    {
        std::cout << "starting TCP_FLAGS_INVALID_07_IT1 test\n";
        test_result.ErrorCode = TCP_FLAGS_INVALID_07_IT1() | TCP_FLAGS_INVALID_07_IT2() | TCP_FLAGS_INVALID_07_IT3() | TCP_FLAGS_INVALID_07_IT4() | TCP_FLAGS_INVALID_07_IT5();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 41:
    {
        std::cout << "starting TCP_FLAGS_INVALID_08_IT1 test\n";
        test_result.ErrorCode = TCP_FLAGS_INVALID_08_IT1() | TCP_FLAGS_INVALID_08_IT2() | TCP_FLAGS_INVALID_08_IT3() | TCP_FLAGS_INVALID_08_IT4() | TCP_FLAGS_INVALID_08_IT5();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 42:
    {
        std::cout << "starting TCP_FLAGS_INVALID_09_IT1 test\n";
        test_result.ErrorCode = TCP_FLAGS_INVALID_09_IT1() | TCP_FLAGS_INVALID_09_IT2() | TCP_FLAGS_INVALID_09_IT3() | TCP_FLAGS_INVALID_09_IT4() | TCP_FLAGS_INVALID_09_IT5();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 43:
    {
        std::cout << "starting TCP_FLAGS_INVALID_10_IT1 test\n";
        test_result.ErrorCode = TCP_FLAGS_INVALID_10_IT1() | TCP_FLAGS_INVALID_10_IT2() | TCP_FLAGS_INVALID_10_IT3() | TCP_FLAGS_INVALID_10_IT4() | TCP_FLAGS_INVALID_10_IT5();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 44:
    {
        std::cout << "starting TCP_FLAGS_INVALID_11_IT1 test\n";
        test_result.ErrorCode = TCP_FLAGS_INVALID_11_IT1() | TCP_FLAGS_INVALID_11_IT2() | TCP_FLAGS_INVALID_11_IT3() | TCP_FLAGS_INVALID_11_IT4() | TCP_FLAGS_INVALID_11_IT5();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 45:
    {
        std::cout << "starting TCP_FLAGS_INVALID_12_IT1 test\n";
        test_result.ErrorCode = TCP_FLAGS_INVALID_12_IT1() | TCP_FLAGS_INVALID_12_IT2() | TCP_FLAGS_INVALID_12_IT3() | TCP_FLAGS_INVALID_12_IT4() | TCP_FLAGS_INVALID_12_IT5();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 46:
    {
        std::cout << "starting TCP_FLAGS_INVALID_13_IT1 test\n";
        test_result.ErrorCode = TCP_FLAGS_INVALID_13_IT1() | TCP_FLAGS_INVALID_13_IT2() | TCP_FLAGS_INVALID_13_IT3() | TCP_FLAGS_INVALID_13_IT4() | TCP_FLAGS_INVALID_13_IT5();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 47:
    {
        std::cout << "starting TCP_FLAGS_INVALID_15_IT1 test\n";
        test_result.ErrorCode = TCP_FLAGS_INVALID_15_IT1() | TCP_FLAGS_INVALID_15_IT2() | TCP_FLAGS_INVALID_15_IT3() | TCP_FLAGS_INVALID_15_IT4() | TCP_FLAGS_INVALID_15_IT5() | TCP_FLAGS_INVALID_15_IT6() | TCP_FLAGS_INVALID_15_IT7() | TCP_FLAGS_INVALID_15_IT8();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 48:
    {
        std::cout << "starting TCP_FLAGS_PROCESSING_01 test\n";
        test_result.ErrorCode = TCP_FLAGS_PROCESSING_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 49:
    {
        std::cout << "starting TCP_FLAGS_PROCESSING_02_IT1 test\n";
        test_result.ErrorCode = TCP_FLAGS_PROCESSING_02_IT1() | TCP_FLAGS_PROCESSING_02_IT2() | TCP_FLAGS_PROCESSING_02_IT3() | TCP_FLAGS_PROCESSING_02_IT4() | TCP_FLAGS_PROCESSING_02_IT5();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 50:
    {
        std::cout << "starting TCP_FLAGS_PROCESSING_03_IT1 test\n";
        test_result.ErrorCode = TCP_FLAGS_PROCESSING_03_IT1() | TCP_FLAGS_PROCESSING_03_IT2();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 51:
    {
        std::cout << "starting TCP_FLAGS_PROCESSING_04 test\n";
        test_result.ErrorCode = TCP_FLAGS_PROCESSING_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 52:
    {
        std::cout << "starting TCP_FLAGS_PROCESSING_05_IT2 test\n";
        test_result.ErrorCode = TCP_FLAGS_PROCESSING_05_IT2();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 53:
    {
        std::cout << "starting TCP_FLAGS_PROCESSING_06 test\n";
        test_result.ErrorCode = TCP_FLAGS_PROCESSING_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 54:
    {
        std::cout << "starting TCP_FLAGS_PROCESSING_07_IT1 test\n";
        test_result.ErrorCode = TCP_FLAGS_PROCESSING_07_IT1() | TCP_FLAGS_PROCESSING_07_IT2() | TCP_FLAGS_PROCESSING_07_IT3() | TCP_FLAGS_PROCESSING_07_IT4();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 55:
    {
        std::cout << "starting TCP_FLAGS_PROCESSING_08_IT1 test\n";
        test_result.ErrorCode = TCP_FLAGS_PROCESSING_08_IT1() | TCP_FLAGS_PROCESSING_08_IT2() | TCP_FLAGS_PROCESSING_08_IT3();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 56:
    {
        std::cout << "starting TCP_FLAGS_PROCESSING_09_IT1 test\n";
        test_result.ErrorCode = TCP_FLAGS_PROCESSING_09_IT1() | TCP_FLAGS_PROCESSING_09_IT2() | TCP_FLAGS_PROCESSING_09_IT3();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 57:
    {
        std::cout << "starting TCP_FLAGS_PROCESSING_10 test\n";
        test_result.ErrorCode = TCP_FLAGS_PROCESSING_10();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 58:
    {
        std::cout << "starting TCP_FLAGS_PROCESSING_11 test\n";
        test_result.ErrorCode = TCP_FLAGS_PROCESSING_11();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 59:
    {
        std::cout << "starting TCP_MSS_OPTIONS_01_IT1 test\n";
        test_result.ErrorCode = TCP_MSS_OPTIONS_01_IT1() | TCP_MSS_OPTIONS_01_IT2();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 60:
    {
        std::cout << "starting TCP_MSS_OPTIONS_02 test\n";
        test_result.ErrorCode = TCP_MSS_OPTIONS_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 61:
    {
        std::cout << "starting TCP_MSS_OPTIONS_03 test\n";
        test_result.ErrorCode = TCP_MSS_OPTIONS_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 62:
    {
        std::cout << "starting TCP_MSS_OPTIONS_05_IT1 test\n";
        test_result.ErrorCode = TCP_MSS_OPTIONS_05_IT1() | TCP_MSS_OPTIONS_05_IT2();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 63:
    {
        std::cout << "starting TCP_MSS_OPTIONS_06_IT1 test\n";
        test_result.ErrorCode = TCP_MSS_OPTIONS_06_IT1() | TCP_MSS_OPTIONS_06_IT2();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 64:
    {
        std::cout << "starting TCP_MSS_OPTIONS_09_IT1 test\n";
        test_result.ErrorCode = TCP_MSS_OPTIONS_09_IT1() | TCP_MSS_OPTIONS_09_IT2();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 65:
    {
        std::cout << "starting TCP_MSS_OPTIONS_10 test\n";
        test_result.ErrorCode = TCP_MSS_OPTIONS_10();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 66:
    {
        std::cout << "starting TCP_MSS_OPTIONS_11 test\n";
        test_result.ErrorCode = TCP_MSS_OPTIONS_11();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 67:
    {
        std::cout << "starting TCP_MSS_OPTIONS_12 test\n";
        test_result.ErrorCode = TCP_MSS_OPTIONS_12();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 68:
    {
        std::cout << "starting TCP_OUT_OF_ORDER_01 test\n";
        test_result.ErrorCode = TCP_OUT_OF_ORDER_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 69:
    {
        std::cout << "starting TCP_OUT_OF_ORDER_02 test\n";
        test_result.ErrorCode = TCP_OUT_OF_ORDER_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 70:
    {
        std::cout << "starting TCP_OUT_OF_ORDER_03 test\n";
        test_result.ErrorCode = TCP_OUT_OF_ORDER_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 71:
    {
        std::cout << "starting TCP_OUT_OF_ORDER_05 test\n";
        test_result.ErrorCode = TCP_OUT_OF_ORDER_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 72:
    {
        std::cout << "starting TCP_RETRANSMISSION_TO_03 test\n";
        test_result.ErrorCode = TCP_RETRANSMISSION_TO_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 73:
    {
        std::cout << "starting TCP_RETRANSMISSION_TO_04 test\n";
        test_result.ErrorCode = TCP_RETRANSMISSION_TO_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 74:
    {
        std::cout << "starting TCP_RETRANSMISSION_TO_05 test\n";
        test_result.ErrorCode = TCP_RETRANSMISSION_TO_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 75:
    {
        std::cout << "starting TCP_RETRANSMISSION_TO_06 test\n";
        test_result.ErrorCode = TCP_RETRANSMISSION_TO_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 76:
    {
        std::cout << "starting TCP_RETRANSMISSION_TO_08 test\n";
        test_result.ErrorCode = TCP_RETRANSMISSION_TO_08();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 77:
    {
        std::cout << "starting TCP_RETRANSMISSION_TO_09 test\n";
        test_result.ErrorCode = TCP_RETRANSMISSION_TO_09();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 78:
    {
        std::cout << "starting UDP_MessageFormat_02 test\n";
        test_result.ErrorCode = UDP_MessageFormat_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 79:
    {
        std::cout << "starting UDP_DatagramLength_01 test\n";
        test_result.ErrorCode = UDP_DatagramLength_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 80:
    {
        std::cout << "starting UDP_Padding_02 test\n";
        test_result.ErrorCode = UDP_Padding_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 81:
    {
        std::cout << "starting UDP_FIELDS_01 test\n";
        test_result.ErrorCode = UDP_FIELDS_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 82:
    {
        std::cout << "starting UDP_FIELDS_02 test\n";
        test_result.ErrorCode = UDP_FIELDS_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 83:
    {
        std::cout << "starting UDP_FIELDS_03 test\n";
        test_result.ErrorCode = UDP_FIELDS_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 84:
    {
        std::cout << "starting UDP_FIELDS_04 test\n";
        test_result.ErrorCode = UDP_FIELDS_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 85:
    {
        std::cout << "starting UDP_FIELDS_05 test\n";
        test_result.ErrorCode = UDP_FIELDS_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 86:
    {
        std::cout << "starting UDP_FIELDS_06 test\n";
        test_result.ErrorCode = UDP_FIELDS_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 87:
    {
        std::cout << "starting UDP_FIELDS_07 test\n";
        test_result.ErrorCode = UDP_FIELDS_07();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 88:
    {
        std::cout << "starting UDP_FIELDS_08 test\n";
        test_result.ErrorCode = UDP_FIELDS_08();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 89:
    {
        std::cout << "starting UDP_FIELDS_09 test\n";
        test_result.ErrorCode = UDP_FIELDS_09();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 90:
    {
        std::cout << "starting UDP_FIELDS_10 test\n";
        test_result.ErrorCode = UDP_FIELDS_10();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 91:
    {
        std::cout << "starting UDP_FIELDS_12 test\n";
        test_result.ErrorCode = UDP_FIELDS_12();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 92:
    {
        std::cout << "starting UDP_FIELDS_13 test\n";
        test_result.ErrorCode = UDP_FIELDS_13();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 93:
    {
        std::cout << "starting UDP_FIELDS_14 test\n";
        test_result.ErrorCode = UDP_FIELDS_14();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 94:
    {
        std::cout << "starting UDP_FIELDS_15 test\n";
        test_result.ErrorCode = UDP_FIELDS_15();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 95:
    {
        std::cout << "starting UDP_FIELDS_16 test\n";
        test_result.ErrorCode = UDP_FIELDS_16();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 96:
    {
        std::cout << "starting UDP_USER_INTERFACE_01 test\n";
        test_result.ErrorCode = UDP_USER_INTERFACE_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 97:
    {
        std::cout << "starting UDP_USER_INTERFACE_02 test\n";
        test_result.ErrorCode = UDP_USER_INTERFACE_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 98:
    {
        std::cout << "starting UDP_USER_INTERFACE_03 test\n";
        test_result.ErrorCode = UDP_USER_INTERFACE_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 99:
    {
        std::cout << "starting UDP_USER_INTERFACE_04 test\n";
        test_result.ErrorCode = UDP_USER_INTERFACE_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 100:
    {
        std::cout << "starting UDP_USER_INTERFACE_05 test\n";
        test_result.ErrorCode = UDP_USER_INTERFACE_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 101:
    {
        std::cout << "starting UDP_USER_INTERFACE_06 test\n";
        test_result.ErrorCode = UDP_USER_INTERFACE_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 102:
    {
        std::cout << "starting UDP_USER_INTERFACE_07 test\n";
        test_result.ErrorCode = UDP_USER_INTERFACE_07();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 103:
    {
        std::cout << "starting UDP_USER_INTERFACE_08 test\n";
        test_result.ErrorCode = UDP_USER_INTERFACE_08();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 104:
    {
        std::cout << "starting UDP_INTRODUCTION_01 test\n";
        test_result.ErrorCode = UDP_INTRODUCTION_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 105:
    {
        std::cout << "starting UDP_INTRODUCTION_02 test\n";
        test_result.ErrorCode = UDP_INTRODUCTION_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 106:
    {
        std::cout << "starting UDP_INTRODUCTION_03 test\n";
        test_result.ErrorCode = UDP_INTRODUCTION_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 107:
    {
        std::cout << "starting UDP_INVALID_ADDRESSES_01 test\n";
        test_result.ErrorCode = UDP_INVALID_ADDRESSES_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 108:
    {
        std::cout << "starting UDP_INVALID_ADDRESSES_02 test\n";
        test_result.ErrorCode = UDP_INVALID_ADDRESSES_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 109:
    {
        std::cout << "starting ARP_01 test\n";
        test_result.ErrorCode = ARP_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 110:
    {
        std::cout << "starting ARP_02 test\n";
        test_result.ErrorCode = ARP_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 111:
    {
        std::cout << "starting ARP_03 test\n";
        test_result.ErrorCode = ARP_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 112:
    {
        std::cout << "starting ARP_04 test\n";
        test_result.ErrorCode = ARP_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 113:
    {
        std::cout << "starting ARP_05 test\n";
        test_result.ErrorCode = ARP_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 114:
    {
        std::cout << "starting ARP_06 test\n";
        test_result.ErrorCode = ARP_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 115:
    {
        std::cout << "starting ARP_07 test\n";
        test_result.ErrorCode = ARP_07();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 116:
    {
        std::cout << "starting ARP_08 test\n";
        test_result.ErrorCode = ARP_08();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 117:
    {
        std::cout << "starting ARP_09 test\n";
        test_result.ErrorCode = ARP_09();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 118:
    {
        std::cout << "starting ARP_10 test\n";
        test_result.ErrorCode = ARP_10();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 119:
    {
        std::cout << "starting ARP_11 test\n";
        test_result.ErrorCode = ARP_11();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 120:
    {
        std::cout << "starting ARP_12 test\n";
        test_result.ErrorCode = ARP_12();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 121:
    {
        std::cout << "starting ARP_13 test\n";
        test_result.ErrorCode = ARP_13();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 122:
    {
        std::cout << "starting ARP_14 test\n";
        test_result.ErrorCode = ARP_14();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 123:
    {
        std::cout << "starting ARP_15 test\n";
        test_result.ErrorCode = ARP_15();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 124:
    {
        std::cout << "starting ARP_16 test\n";
        test_result.ErrorCode = ARP_16();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 125:
    {
        std::cout << "starting ARP_17 test\n";
        test_result.ErrorCode = ARP_17();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 126:
    {
        std::cout << "starting ARP_18 test\n";
        test_result.ErrorCode = ARP_18();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 127:
    {
        std::cout << "starting ARP_19 test\n";
        test_result.ErrorCode = ARP_19();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 128:
    {
        std::cout << "starting ARP_20 test\n";
        test_result.ErrorCode = ARP_20();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 129:
    {
        std::cout << "starting ARP_21 test\n";
        test_result.ErrorCode = ARP_21();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 130:
    {
        std::cout << "starting ARP_22 test\n";
        test_result.ErrorCode = ARP_22();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 131:
    {
        std::cout << "starting ARP_26 test\n";
        test_result.ErrorCode = ARP_26();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 132:
    {
        std::cout << "starting ARP_27 test\n";
        test_result.ErrorCode = ARP_27();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 133:
    {
        std::cout << "starting ARP_28 test\n";
        test_result.ErrorCode = ARP_28();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 134:
    {
        std::cout << "starting ARP_32 test\n";
        test_result.ErrorCode = ARP_32();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 135:
    {
        std::cout << "starting ARP_33 test\n";
        test_result.ErrorCode = ARP_33();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 136:
    {
        std::cout << "starting ARP_34 test\n";
        test_result.ErrorCode = ARP_34();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 137:
    {
        std::cout << "starting ARP_35 test\n";
        test_result.ErrorCode = ARP_35();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 138:
    {
        std::cout << "starting ARP_36 test\n";
        test_result.ErrorCode = ARP_36();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 139:
    {
        std::cout << "starting ARP_37 test\n";
        test_result.ErrorCode = ARP_37();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 140:
    {
        std::cout << "starting ARP_38 test\n";
        test_result.ErrorCode = ARP_38();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 141:
    {
        std::cout << "starting ARP_39 test\n";
        test_result.ErrorCode = ARP_39();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 142:
    {
        std::cout << "starting ARP_40 test\n";
        test_result.ErrorCode = ARP_40();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 143:
    {
        std::cout << "starting ARP_41 test\n";
        test_result.ErrorCode = ARP_41();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 144:
    {
        std::cout << "starting ARP_42 test\n";
        test_result.ErrorCode = ARP_42();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 145:
    {
        std::cout << "starting ARP_43 test\n";
        test_result.ErrorCode = ARP_43();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 146:
    {
        std::cout << "starting ARP_44 test\n";
        test_result.ErrorCode = ARP_44();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 147:
    {
        std::cout << "starting ARP_45 test\n";
        test_result.ErrorCode = ARP_45();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 148:
    {
        std::cout << "starting ARP_46 test\n";
        test_result.ErrorCode = ARP_46();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 149:
    {
        std::cout << "starting ARP_47 test\n";
        test_result.ErrorCode = ARP_47();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 150:
    {
        std::cout << "starting ARP_48 test\n";
        test_result.ErrorCode = ARP_48();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 151:
    {
        std::cout << "starting ARP_49 test\n";
        test_result.ErrorCode = ARP_49();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 152:
    {
        std::cout << "starting ICMPv4_ERROR_02 test\n";
        test_result.ErrorCode = ICMPv4_ERROR_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 153:
    {
        std::cout << "starting ICMPv4_ERROR_03 test\n";
        test_result.ErrorCode = ICMPv4_ERROR_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 154:
    {
        std::cout << "starting ICMPv4_ERROR_04 test\n";
        test_result.ErrorCode = ICMPv4_ERROR_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 155:
    {
        std::cout << "starting ICMPv4_ERROR_05 test\n";
        test_result.ErrorCode = ICMPv4_ERROR_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 156:
    {
        std::cout << "starting ICMPv4_TYPE_04 test\n";
        test_result.ErrorCode = ICMPv4_TYPE_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 157:
    {
        std::cout << "starting ICMPv4_TYPE_05 test\n";
        test_result.ErrorCode = ICMPv4_TYPE_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 158:
    {
        std::cout << "starting ICMPv4_TYPE_08 test\n";
        test_result.ErrorCode = ICMPv4_TYPE_08();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 159:
    {
        std::cout << "starting ICMPv4_TYPE_09 test\n";
        test_result.ErrorCode = ICMPv4_TYPE_09();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 160:
    {
        std::cout << "starting ICMPv4_TYPE_10 test\n";
        test_result.ErrorCode = ICMPv4_TYPE_10();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 161:
    {
        std::cout << "starting ICMPv4_TYPE_11 test\n";
        test_result.ErrorCode = ICMPv4_TYPE_11();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 162:
    {
        std::cout << "starting ICMPv4_TYPE_12 test\n";
        test_result.ErrorCode = ICMPv4_TYPE_12();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 163:
    {
        std::cout << "starting ICMPv4_TYPE_16 test\n";
        test_result.ErrorCode = ICMPv4_TYPE_16();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 164:
    {
        std::cout << "starting ICMPv4_TYPE_18 test\n";
        test_result.ErrorCode = ICMPv4_TYPE_18();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 165:
    {
        std::cout << "starting ICMPv4_TYPE_22 test\n";
        test_result.ErrorCode = ICMPv4_TYPE_22();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 166:
    {
        std::cout << "starting IPv4_HEADER_01 test\n";
        test_result.ErrorCode = IPv4_HEADER_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 167:
    {
        std::cout << "starting IPv4_HEADER_02 test\n";
        test_result.ErrorCode = IPv4_HEADER_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 168:
    {
        std::cout << "starting IPv4_HEADER_03 test\n";
        test_result.ErrorCode = IPv4_HEADER_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 169:
    {
        std::cout << "starting IPv4_HEADER_04 test\n";
        test_result.ErrorCode = IPv4_HEADER_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 170:
    {
        std::cout << "starting IPv4_HEADER_05 test\n";
        test_result.ErrorCode = IPv4_HEADER_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 171:
    {
        std::cout << "starting IPv4_HEADER_08 test\n";
        test_result.ErrorCode = IPv4_HEADER_08();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 172:
    {
        std::cout << "starting IPv4_HEADER_09 test\n";
        test_result.ErrorCode = IPv4_HEADER_09();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 173:
    {
        std::cout << "starting IPv4_CHECKSUM_02 test\n";
        test_result.ErrorCode = IPv4_CHECKSUM_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 174:
    {
        std::cout << "starting IPv4_CHECKSUM_05 test\n";
        test_result.ErrorCode = IPv4_CHECKSUM_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 175:
    {
        std::cout << "starting IPv4_TTL_01 test\n";
        test_result.ErrorCode = IPv4_TTL_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 176:
    {
        std::cout << "starting IPv4_TTL_05 test\n";
        test_result.ErrorCode = IPv4_TTL_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 177:
    {
        std::cout << "starting IPv4_VERSION_01 test\n";
        test_result.ErrorCode = IPv4_VERSION_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 178:
    {
        std::cout << "starting IPv4_VERSION_03 test\n";
        test_result.ErrorCode = IPv4_VERSION_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 179:
    {
        std::cout << "starting IPv4_VERSION_04 test\n";
        test_result.ErrorCode = IPv4_VERSION_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 180:
    {
        std::cout << "starting IPv4_ADDRESSING_01 test\n";
        test_result.ErrorCode = IPv4_ADDRESSING_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 181:
    {
        std::cout << "starting IPv4_ADDRESSING_02 test\n";
        test_result.ErrorCode = IPv4_ADDRESSING_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 182:
    {
        std::cout << "starting IPv4_ADDRESSING_03 test\n";
        test_result.ErrorCode = IPv4_ADDRESSING_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 183:
    {
        std::cout << "starting IPv4_FRAGMENTS_01 test\n";
        test_result.ErrorCode = IPv4_FRAGMENTS_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 184:
    {
        std::cout << "starting IPv4_FRAGMENTS_02 test\n";
        test_result.ErrorCode = IPv4_FRAGMENTS_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 185:
    {
        std::cout << "starting IPv4_FRAGMENTS_03 test\n";
        test_result.ErrorCode = IPv4_FRAGMENTS_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 186:
    {
        std::cout << "starting IPv4_FRAGMENTS_04 test\n";
        test_result.ErrorCode = IPv4_FRAGMENTS_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 187:
    {
        std::cout << "starting IPv4_FRAGMENTS_05 test\n";
        test_result.ErrorCode = IPv4_FRAGMENTS_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 188:
    {
        std::cout << "starting IPv4_REASSEMBLY_04 test\n";
        test_result.ErrorCode = IPv4_REASSEMBLY_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 189:
    {
        std::cout << "starting IPv4_REASSEMBLY_06 test\n";
        test_result.ErrorCode = IPv4_REASSEMBLY_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 190:
    {
        std::cout << "starting IPv4_REASSEMBLY_07 test\n";
        test_result.ErrorCode = IPv4_REASSEMBLY_07();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 191:
    {
        std::cout << "starting IPv4_REASSEMBLY_09 test\n";
        test_result.ErrorCode = IPv4_REASSEMBLY_09();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 192:
    {
        std::cout << "starting IPv4_REASSEMBLY_10 test\n";
        test_result.ErrorCode = IPv4_REASSEMBLY_10();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 193:
    {
        std::cout << "starting IPv4_REASSEMBLY_11 test\n";
        test_result.ErrorCode = IPv4_REASSEMBLY_11();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 194:
    {
        std::cout << "starting IPv4_REASSEMBLY_12 test\n";
        test_result.ErrorCode = IPv4_REASSEMBLY_12();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 195:
    {
        std::cout << "starting IPv4_REASSEMBLY_13 test\n";
        test_result.ErrorCode = IPv4_REASSEMBLY_13();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 196:
    {
        std::cout << "starting IPv4_AUTOCONF_ADDRESS_SELECTION_01 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_ADDRESS_SELECTION_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 197:
    {
        std::cout << "starting IPv4_AUTOCONF_ADDRESS_SELECTION_03 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_ADDRESS_SELECTION_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 198:
    {
        std::cout << "starting IPv4_AUTOCONF_ADDRESS_SELECTION_05 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_ADDRESS_SELECTION_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 199:
    {
        std::cout << "starting IPv4_AUTOCONF_ADDRESS_SELECTION_06 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_ADDRESS_SELECTION_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 200:
    {
        std::cout << "starting IPv4_AUTOCONF_ADDRESS_SELECTION_07 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_ADDRESS_SELECTION_07();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 201:
    {
        std::cout << "starting IPv4_AUTOCONF_ADDRESS_SELECTION_08 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_ADDRESS_SELECTION_08();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 202:
    {
        std::cout << "starting IPv4_AUTOCONF_ADDRESS_SELECTION_09 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_ADDRESS_SELECTION_09();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 203:
    {
        std::cout << "starting IPv4_AUTOCONF_ADDRESS_SELECTION_10 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_ADDRESS_SELECTION_10();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 204:
    {
        std::cout << "starting IPv4_AUTOCONF_ADDRESS_SELECTION_11 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_ADDRESS_SELECTION_11();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 205:
    {
        std::cout << "starting IPv4_AUTOCONF_ADDRESS_SELECTION_12 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_ADDRESS_SELECTION_12();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 206:
    {
        std::cout << "starting IPv4_AUTOCONF_ADDRESS_SELECTION_13 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_ADDRESS_SELECTION_13();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 207:
    {
        std::cout << "starting IPv4_AUTOCONF_ADDRESS_SELECTION_14 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_ADDRESS_SELECTION_14();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 208:
    {
        std::cout << "starting IPv4_AUTOCONF_ADDRESS_SELECTION_15 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_ADDRESS_SELECTION_15();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 209:
    {
        std::cout << "starting IPv4_AUTOCONF_ADDRESS_SELECTION_16 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_ADDRESS_SELECTION_16();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 210:
    {
        std::cout << "starting IPv4_AUTOCONF_ANNOUNCING_01 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_ANNOUNCING_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 211:
    {
        std::cout << "starting IPv4_AUTOCONF_ANNOUNCING_02 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_ANNOUNCING_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 212:
    {
        std::cout << "starting IPv4_AUTOCONF_ANNOUNCING_03 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_ANNOUNCING_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 213:
    {
        std::cout << "starting IPv4_AUTOCONF_ANNOUNCING_04 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_ANNOUNCING_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 214:
    {
        std::cout << "starting IPv4_AUTOCONF_ANNOUNCING_05 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_ANNOUNCING_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 215:
    {
        std::cout << "starting IPv4_AUTOCONF_ANNOUNCING_06 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_ANNOUNCING_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 216:
    {
        std::cout << "starting IPv4_AUTOCONF_CONFLICT_06 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_CONFLICT_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 217:
    {
        std::cout << "starting IPv4_AUTOCONF_CONFLICT_07 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_CONFLICT_07();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 218:
    {
        std::cout << "starting IPv4_AUTOCONF_CONFLICT_08 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_CONFLICT_08();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 219:
    {
        std::cout << "starting IPv4_AUTOCONF_CONFLICT_09 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_CONFLICT_09();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 220:
    {
        std::cout << "starting IPv4_AUTOCONF_CONFLICT_10 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_CONFLICT_10();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 221:
    {
        std::cout << "starting IPv4_AUTOCONF_CONFLICT_11 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_CONFLICT_11();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 222:
    {
        std::cout << "starting IPv4_AUTOCONF_LINKLOCAL_PACKETS_04 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_LINKLOCAL_PACKETS_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 223:
    {
        std::cout << "starting IPv4_AUTOCONF_NETWORK_PARTITIONS_01 test\n";
        test_result.ErrorCode = IPv4_AUTOCONF_NETWORK_PARTITIONS_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 224:
    {
        std::cout << "starting DHCPv4_CLIENT_SUMMARY_01 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_SUMMARY_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 225:
    {
        std::cout << "starting DHCPv4_CLIENT_SUMMARY_02 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_SUMMARY_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 226:
    {
        std::cout << "starting DHCPv4_CLIENT_SUMMARY_03 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_SUMMARY_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 227:
    {
        std::cout << "starting DHCPv4_CLIENT_PROTOCOL_01 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_PROTOCOL_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 228:
    {
        std::cout << "starting DHCPv4_CLIENT_PROTOCOL_02 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_PROTOCOL_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 229:
    {
        std::cout << "starting DHCPv4_CLIENT_PROTOCOL_03 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_PROTOCOL_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 230:
    {
        std::cout << "starting DHCPv4_CLIENT_ALLOCATING_01 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_ALLOCATING_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 231:
    {
        std::cout << "starting DHCPv4_CLIENT_ALLOCATING_03 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_ALLOCATING_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 232:
    {
        std::cout << "starting DHCPv4_CLIENT_ALLOCATING_04 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_ALLOCATING_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 233:
    {
        std::cout << "starting DHCPv4_CLIENT_ALLOCATING_05 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_ALLOCATING_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 234:
    {
        std::cout << "starting DHCPv4_CLIENT_ALLOCATING_06 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_ALLOCATING_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 235:
    {
        std::cout << "starting DHCPv4_CLIENT_ALLOCATING_07 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_ALLOCATING_07();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 236:
    {
        std::cout << "starting DHCPv4_CLIENT_ALLOCATING_08 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_ALLOCATING_08();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 237:
    {
        std::cout << "starting DHCPv4_CLIENT_ALLOCATING_09 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_ALLOCATING_09();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 238:
    {
        std::cout << "starting DHCPv4_CLIENT_ALLOCATING_10 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_ALLOCATING_10();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 239:
    {
        std::cout << "starting DHCPv4_CLIENT_PARAMETERS_03 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_PARAMETERS_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 240:
    {
        std::cout << "starting DHCPv4_CLIENT_PARAMETERS_04 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_PARAMETERS_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 241:
    {
        std::cout << "starting DHCPv4_CLIENT_USAGE_01 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_USAGE_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 242:
    {
        std::cout << "starting DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_01 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 243:
    {
        std::cout << "starting DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_02 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 244:
    {
        std::cout << "starting DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_03 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 245:
    {
        std::cout << "starting DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_04 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 246:
    {
        std::cout << "starting DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_05 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 247:
    {
        std::cout << "starting DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 248:
    {
        std::cout << "starting DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_12 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_12();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 249:
    {
        std::cout << "starting DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_13 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_13();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 250:
    {
        std::cout << "starting DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_14 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_CONSTRUCTING_MESSAGES_14();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 251:
    {
        std::cout << "starting DHCPv4_CLIENT_REQUEST_01 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_REQUEST_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 252:
    {
        std::cout << "starting DHCPv4_CLIENT_REQUEST_02 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_REQUEST_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 253:
    {
        std::cout << "starting DHCPv4_CLIENT_REQUEST_06 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_REQUEST_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 254:
    {
        std::cout << "starting DHCPv4_CLIENT_REQUEST_07 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_REQUEST_07();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 255:
    {
        std::cout << "starting DHCPv4_CLIENT_REQUEST_08 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_REQUEST_08();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 256:
    {
        std::cout << "starting DHCPv4_CLIENT_REQUEST_09 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_REQUEST_09();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 257:
    {
        std::cout << "starting DHCPv4_CLIENT_REQUEST_10 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_REQUEST_10();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 258:
    {
        std::cout << "starting DHCPv4_CLIENT_REQUEST_11 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_REQUEST_11();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 259:
    {
        std::cout << "starting DHCPv4_CLIENT_REQUEST_12 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_REQUEST_12();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 260:
    {
        std::cout << "starting DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_01 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 261:
    {
        std::cout << "starting DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_02 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 262:
    {
        std::cout << "starting DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_03 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 263:
    {
        std::cout << "starting DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_04 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 264:
    {
        std::cout << "starting DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_05 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 265:
    {
        std::cout << "starting DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_06 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 266:
    {
        std::cout << "starting DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_08 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_08();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 267:
    {
        std::cout << "starting DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_09 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_09();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 268:
    {
        std::cout << "starting DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_10 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_INITIALIZATION_ALLOCATION_10();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 269:
    {
        std::cout << "starting DHCPv4_CLIENT_REACQUISITION_01 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_REACQUISITION_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 270:
    {
        std::cout << "starting DHCPv4_CLIENT_REACQUISITION_02 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_REACQUISITION_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 271:
    {
        std::cout << "starting DHCPv4_CLIENT_REACQUISITION_03 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_REACQUISITION_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 272:
    {
        std::cout << "starting DHCPv4_CLIENT_REACQUISITION_04 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_REACQUISITION_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 273:
    {
        std::cout << "starting DHCPv4_CLIENT_REACQUISITION_05 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_REACQUISITION_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 274:
    {
        std::cout << "starting DHCPv4_CLIENT_REACQUISITION_06 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_REACQUISITION_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 275:
    {
        std::cout << "starting DHCPv4_CLIENT_REACQUISITION_07 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_REACQUISITION_07();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 276:
    {
        std::cout << "starting DHCPv4_CLIENT_REACQUISITION_08 test\n";
        test_result.ErrorCode = DHCPv4_CLIENT_REACQUISITION_08();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 277:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_01 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 278:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_02 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 279:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_03 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 280:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_04 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 281:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_05 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 282:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_06 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 283:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_07 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_07();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 284:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_08 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_08();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 285:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_09 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_09();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 286:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_10 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_10();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 287:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_11 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_11();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 288:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_12 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_12();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 289:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_13 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_13();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 290:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_14 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_14();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 291:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_15 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_15();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 292:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_16 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_16();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 293:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_17 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_17();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 294:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_18 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_18();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 295:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_19 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_19();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 296:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_20 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_20();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 297:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_21 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_21();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 298:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_23 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_23();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 299:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_24 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_24();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 300:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_25 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_25();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 301:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_26 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_26();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 302:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_27 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_27();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 303:
    {
        std::cout << "starting SOMEIPSRV_FORMAT_28 test\n";
        test_result.ErrorCode = SOMEIPSRV_FORMAT_28();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 304:
    {
        std::cout << "starting SOMEIPSRV_OPTIONS_01 test\n";
        test_result.ErrorCode = SOMEIPSRV_OPTIONS_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 305:
    {
        std::cout << "starting SOMEIPSRV_OPTIONS_02 test\n";
        test_result.ErrorCode = SOMEIPSRV_OPTIONS_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 306:
    {
        std::cout << "starting SOMEIPSRV_OPTIONS_03 test\n";
        test_result.ErrorCode = SOMEIPSRV_OPTIONS_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 307:
    {
        std::cout << "starting SOMEIPSRV_OPTIONS_04 test\n";
        test_result.ErrorCode = SOMEIPSRV_OPTIONS_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 308:
    {
        std::cout << "starting SOMEIPSRV_OPTIONS_05 test\n";
        test_result.ErrorCode = SOMEIPSRV_OPTIONS_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 309:
    {
        std::cout << "starting SOMEIPSRV_OPTIONS_06 test\n";
        test_result.ErrorCode = SOMEIPSRV_OPTIONS_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 310:
    {
        std::cout << "starting SOMEIPSRV_OPTIONS_07 test\n";
        test_result.ErrorCode = SOMEIPSRV_OPTIONS_07();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 311:
    {
        std::cout << "starting SOMEIPSRV_OPTIONS_08 test\n";
        test_result.ErrorCode = SOMEIPSRV_OPTIONS_08();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 312:
    {
        std::cout << "starting SOMEIPSRV_OPTIONS_09 test\n";
        test_result.ErrorCode = SOMEIPSRV_OPTIONS_09();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 313:
    {
        std::cout << "starting SOMEIPSRV_OPTIONS_10 test\n";
        test_result.ErrorCode = SOMEIPSRV_OPTIONS_10();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 314:
    {
        std::cout << "starting SOMEIPSRV_OPTIONS_11 test\n";
        test_result.ErrorCode = SOMEIPSRV_OPTIONS_11();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 315:
    {
        std::cout << "starting SOMEIPSRV_OPTIONS_12 test\n";
        test_result.ErrorCode = SOMEIPSRV_OPTIONS_12();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 316:
    {
        std::cout << "starting SOMEIPSRV_OPTIONS_13 test\n";
        test_result.ErrorCode = SOMEIPSRV_OPTIONS_13();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 317:
    {
        std::cout << "starting SOMEIPSRV_OPTIONS_14 test\n";
        test_result.ErrorCode = SOMEIPSRV_OPTIONS_14();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 318:
    {
        std::cout << "starting SOMEIPSRV_OPTIONS_15 test\n";
        test_result.ErrorCode = SOMEIPSRV_OPTIONS_15();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 319:
    {
        std::cout << "starting SOMEIPSRV_SD_MESSAGE_01 test\n";
        test_result.ErrorCode = SOMEIPSRV_SD_MESSAGE_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 320:
    {
        std::cout << "starting SOMEIPSRV_SD_MESSAGE_02 test\n";
        test_result.ErrorCode = SOMEIPSRV_SD_MESSAGE_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 321:
    {
        std::cout << "starting SOMEIPSRV_SD_MESSAGE_03 test\n";
        test_result.ErrorCode = SOMEIPSRV_SD_MESSAGE_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 322:
    {
        std::cout << "starting SOMEIPSRV_SD_MESSAGE_04 test\n";
        test_result.ErrorCode = SOMEIPSRV_SD_MESSAGE_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 323:
    {
        std::cout << "starting SOMEIPSRV_SD_MESSAGE_05 test\n";
        test_result.ErrorCode = SOMEIPSRV_SD_MESSAGE_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 324:
    {
        std::cout << "starting SOMEIPSRV_SD_MESSAGE_06 test\n";
        test_result.ErrorCode = SOMEIPSRV_SD_MESSAGE_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 325:
    {
        std::cout << "starting SOMEIPSRV_SD_MESSAGE_07 test\n";
        test_result.ErrorCode = SOMEIPSRV_SD_MESSAGE_07();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 326:
    {
        std::cout << "starting SOMEIPSRV_SD_MESSAGE_08 test\n";
        test_result.ErrorCode = SOMEIPSRV_SD_MESSAGE_08();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 327:
    {
        std::cout << "starting SOMEIPSRV_SD_MESSAGE_09 test\n";
        test_result.ErrorCode = SOMEIPSRV_SD_MESSAGE_09();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 328:
    {
        std::cout << "starting SOMEIPSRV_SD_MESSAGE_11 test\n";
        test_result.ErrorCode = SOMEIPSRV_SD_MESSAGE_11();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 329:
    {
        std::cout << "starting SOMEIPSRV_SD_MESSAGE_13 test\n";
        test_result.ErrorCode = SOMEIPSRV_SD_MESSAGE_13();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 330:
    {
        std::cout << "starting SOMEIPSRV_SD_MESSAGE_14 test\n";
        test_result.ErrorCode = SOMEIPSRV_SD_MESSAGE_14();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 331:
    {
        std::cout << "starting SOMEIPSRV_SD_MESSAGE_15 test\n";
        test_result.ErrorCode = SOMEIPSRV_SD_MESSAGE_15();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 332:
    {
        std::cout << "starting SOMEIPSRV_SD_MESSAGE_16 test\n";
        test_result.ErrorCode = SOMEIPSRV_SD_MESSAGE_16();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 333:
    {
        std::cout << "starting SOMEIPSRV_SD_MESSAGE_17 test\n";
        test_result.ErrorCode = SOMEIPSRV_SD_MESSAGE_17();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 334:
    {
        std::cout << "starting SOMEIPSRV_SD_MESSAGE_18 test\n";
        test_result.ErrorCode = SOMEIPSRV_SD_MESSAGE_18();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 335:
    {
        std::cout << "starting SOMEIPSRV_SD_MESSAGE_19 test\n";
        test_result.ErrorCode = SOMEIPSRV_SD_MESSAGE_19();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 336:
    {
        std::cout << "starting SOMEIPSRV_SD_BEHAVIOR_01 test\n";
        test_result.ErrorCode = SOMEIPSRV_SD_BEHAVIOR_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 337:
    {
        std::cout << "starting SOMEIPSRV_SD_BEHAVIOR_02 test\n";
        test_result.ErrorCode = SOMEIPSRV_SD_BEHAVIOR_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 338:
    {
        std::cout << "starting SOMEIPSRV_SD_BEHAVIOR_03 test\n";
        test_result.ErrorCode = SOMEIPSRV_SD_BEHAVIOR_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 339:
    {
        std::cout << "starting SOMEIPSRV_SD_BEHAVIOR_04 test\n";
        test_result.ErrorCode = SOMEIPSRV_SD_BEHAVIOR_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 340:
    {
        std::cout << "starting SOMEIPSRV_BASIC_01 test\n";
        test_result.ErrorCode = SOMEIPSRV_BASIC_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 341:
    {
        std::cout << "starting SOMEIPSRV_BASIC_02 test\n";
        test_result.ErrorCode = SOMEIPSRV_BASIC_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 342:
    {
        std::cout << "starting SOMEIPSRV_BASIC_03 test\n";
        test_result.ErrorCode = SOMEIPSRV_BASIC_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 343:
    {
        std::cout << "starting SOMEIPSRV_ONWIRE_MESSAGE_01 test\n";
        test_result.ErrorCode = SOMEIPSRV_ONWIRE_MESSAGE_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 344:
    {
        std::cout << "starting SOMEIPSRV_ONWIRE_MESSAGE_02 test\n";
        test_result.ErrorCode = SOMEIPSRV_ONWIRE_MESSAGE_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 345:
    {
        std::cout << "starting SOMEIPSRV_ONWIRE_MESSAGE_03 test\n";
        test_result.ErrorCode = SOMEIPSRV_ONWIRE_MESSAGE_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 346:
    {
        std::cout << "starting SOMEIPSRV_ONWIRE_MESSAGE_04 test\n";
        test_result.ErrorCode = SOMEIPSRV_ONWIRE_MESSAGE_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 347:
    {
        std::cout << "starting SOMEIPSRV_ONWIRE_MESSAGE_05 test\n";
        test_result.ErrorCode = SOMEIPSRV_ONWIRE_MESSAGE_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 348:
    {
        std::cout << "starting SOMEIPSRV_ONWIRE_MESSAGE_06 test\n";
        test_result.ErrorCode = SOMEIPSRV_ONWIRE_MESSAGE_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 349:
    {
        std::cout << "starting SOMEIPSRV_ONWIRE_MESSAGE_07 test\n";
        test_result.ErrorCode = SOMEIPSRV_ONWIRE_MESSAGE_07();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 350:
    {
        std::cout << "starting SOMEIPSRV_ONWIRE_MESSAGE_10 test\n";
        test_result.ErrorCode = SOMEIPSRV_ONWIRE_MESSAGE_10();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 351:
    {
        std::cout << "starting SOMEIPSRV_ONWIRE_MESSAGE_11 test\n";
        test_result.ErrorCode = SOMEIPSRV_ONWIRE_MESSAGE_11();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 352:
    {
        std::cout << "starting SOMEIPSRV_ONWIRE_MESSAGE_12 test\n";
        test_result.ErrorCode = SOMEIPSRV_ONWIRE_MESSAGE_12();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 353:
    {
        std::cout << "starting SOMEIPSRV_RPC_MESSAGE_01 test\n";
        test_result.ErrorCode = SOMEIPSRV_RPC_MESSAGE_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 354:
    {
        std::cout << "starting SOMEIPSRV_RPC_MESSAGE_02 test\n";
        test_result.ErrorCode = SOMEIPSRV_RPC_MESSAGE_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 355:
    {
        std::cout << "starting SOMEIPSRV_RPC_MESSAGE_03 test\n";
        test_result.ErrorCode = SOMEIPSRV_RPC_MESSAGE_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 356:
    {
        std::cout << "starting SOMEIPSRV_RPC_MESSAGE_04 test\n";
        test_result.ErrorCode = SOMEIPSRV_RPC_MESSAGE_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 357:
    {
        std::cout << "starting SOMEIPSRV_RPC_MESSAGE_05 test\n";
        test_result.ErrorCode = SOMEIPSRV_RPC_MESSAGE_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 358:
    {
        std::cout << "starting SOMEIPSRV_RPC_MESSAGE_06 test\n";
        test_result.ErrorCode = SOMEIPSRV_RPC_MESSAGE_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 359:
    {
        std::cout << "starting SOMEIPSRV_RPC_MESSAGE_07 test\n";
        test_result.ErrorCode = SOMEIPSRV_RPC_MESSAGE_07();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 360:
    {
        std::cout << "starting SOMEIPSRV_RPC_MESSAGE_08 test\n";
        test_result.ErrorCode = SOMEIPSRV_RPC_MESSAGE_08();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 361:
    {
        std::cout << "starting SOMEIPSRV_RPC_MESSAGE_09 test\n";
        test_result.ErrorCode = SOMEIPSRV_RPC_MESSAGE_09();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 362:
    {
        std::cout << "starting SOMEIPSRV_RPC_MESSAGE_10 test\n";
        test_result.ErrorCode = SOMEIPSRV_RPC_MESSAGE_10();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 363:
    {
        std::cout << "starting SOMEIPSRV_RPC_MESSAGE_11 test\n";
        test_result.ErrorCode = SOMEIPSRV_RPC_MESSAGE_11();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 364:
    {
        std::cout << "starting SOMEIPSRV_RPC_MESSAGE_13 test\n";
        test_result.ErrorCode = SOMEIPSRV_RPC_MESSAGE_13();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 365:
    {
        std::cout << "starting SOMEIPSRV_RPC_MESSAGE_14 test\n";
        test_result.ErrorCode = SOMEIPSRV_RPC_MESSAGE_14();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 366:
    {
        std::cout << "starting SOMEIPSRV_RPC_MESSAGE_17 test\n";
        test_result.ErrorCode = SOMEIPSRV_RPC_MESSAGE_17();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 367:
    {
        std::cout << "starting SOMEIPSRV_RPC_MESSAGE_18 test\n";
        test_result.ErrorCode = SOMEIPSRV_RPC_MESSAGE_18();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 368:
    {
        std::cout << "starting SOMEIPSRV_RPC_MESSAGE_19 test\n";
        test_result.ErrorCode = SOMEIPSRV_RPC_MESSAGE_19();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 369:
    {
        std::cout << "starting SOMEIPSRV_RPC_MESSAGE_20 test\n";
        test_result.ErrorCode = SOMEIPSRV_RPC_MESSAGE_20();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 495:
    {
        std::cout << "starting TCP_CLOSING_03 test\n";
        test_result.ErrorCode = TCP_CLOSING_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 496:
    {
        std::cout << "starting TCP_CLOSING_06 test\n";
        test_result.ErrorCode = TCP_CLOSING_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 479:
    {
        std::cout << "starting TCP_CLOSING_07 test\n";
        test_result.ErrorCode = TCP_CLOSING_07();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 498:
    {
        std::cout << "starting TCP_CLOSING_08 test\n";
        test_result.ErrorCode = TCP_CLOSING_08();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 499:
    {
        std::cout << "starting TCP_CLOSING_09 test\n";
        test_result.ErrorCode = TCP_CLOSING_09();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 500:
    {
        std::cout << "starting TCP_CLOSING_13 test\n";
        test_result.ErrorCode = TCP_CLOSING_13();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 501:
    {
        std::cout << "starting TCP_PROBING_WINDOWS_02 test\n";
        test_result.ErrorCode = TCP_PROBING_WINDOWS_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 502:
    {
        std::cout << "starting TCP_PROBING_WINDOWS_03 test\n";
        test_result.ErrorCode = TCP_PROBING_WINDOWS_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 503:
    {
        std::cout << "starting TCP_PROBING_WINDOWS_04 test\n";
        test_result.ErrorCode = TCP_PROBING_WINDOWS_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 504:
    {
        std::cout << "starting TCP_PROBING_WINDOWS_05 test\n";
        test_result.ErrorCode = TCP_PROBING_WINDOWS_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 505:
    {
        std::cout << "starting TCP_PROBING_WINDOWS_06 test\n";
        test_result.ErrorCode = TCP_PROBING_WINDOWS_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 506:
    {
        std::cout << "starting TCP_NAGLE_02 test\n";
        test_result.ErrorCode = TCP_NAGLE_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 507:
    {
        std::cout << "starting TCP_NAGLE_03 test\n";
        test_result.ErrorCode = TCP_NAGLE_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 508:
    {
        std::cout << "starting TCP_CONNECTION_ESTAB_01 test\n";
        test_result.ErrorCode = TCP_CONNECTION_ESTAB_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 509:
    {
        std::cout << "starting TCP_CONNECTION_ESTAB_02 test\n";
        test_result.ErrorCode = TCP_CONNECTION_ESTAB_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 510:
    {
        std::cout << "starting TCP_CONNECTION_ESTAB_03 test\n";
        test_result.ErrorCode = TCP_CONNECTION_ESTAB_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 511:
    {
        std::cout << "starting TCP_CONNECTION_ESTAB_07 test\n";
        test_result.ErrorCode = TCP_CONNECTION_ESTAB_07();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 512:
    {
        std::cout << "starting TCP_HEADER_01 test\n";
        test_result.ErrorCode = TCP_HEADER_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 513:
    {
        std::cout << "starting TCP_HEADER_02 test\n";
        test_result.ErrorCode = TCP_HEADER_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 514:
    {
        std::cout << "starting TCP_HEADER_04 test\n";
        test_result.ErrorCode = TCP_HEADER_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 515:
    {
        std::cout << "starting TCP_HEADER_05 test\n";
        test_result.ErrorCode = TCP_HEADER_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 516:
    {
        std::cout << "starting TCP_HEADER_06 test\n";
        test_result.ErrorCode = TCP_HEADER_06();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 517:
    {
        std::cout << "starting TCP_HEADER_07 test\n";
        test_result.ErrorCode = TCP_HEADER_07();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 518:
    {
        std::cout << "starting TCP_HEADER_08 test\n";
        test_result.ErrorCode = TCP_HEADER_08();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 519:
    {
        std::cout << "starting TCP_HEADER_09 test\n";
        test_result.ErrorCode = TCP_HEADER_09();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 520:
    {
        std::cout << "starting TCP_HEADER_11 test\n";
        test_result.ErrorCode = TCP_HEADER_11();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 521:
    {
        std::cout << "starting TCP_SEQUENCE_01 test\n";
        test_result.ErrorCode = TCP_SEQUENCE_01();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 522:
    {
        std::cout << "starting TCP_SEQUENCE_02 test\n";
        test_result.ErrorCode = TCP_SEQUENCE_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 523:
    {
        std::cout << "starting TCP_SEQUENCE_03 test\n";
        test_result.ErrorCode = TCP_SEQUENCE_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 524:
    {
        std::cout << "starting TCP_SEQUENCE_04 test\n";
        test_result.ErrorCode = TCP_SEQUENCE_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 525:
    {
        std::cout << "starting TCP_SEQUENCE_05 test\n";
        test_result.ErrorCode = TCP_SEQUENCE_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 526:
    {
        std::cout << "starting TCP_ACKNOWLEDGEMENT_02 test\n";
        test_result.ErrorCode = TCP_ACKNOWLEDGEMENT_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 527:
    {
        std::cout << "starting TCP_ACKNOWLEDGEMENT_03 test\n";
        test_result.ErrorCode = TCP_ACKNOWLEDGEMENT_03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 528:
    {
        std::cout << "starting TCP_ACKNOWLEDGEMENT_04 test\n";
        test_result.ErrorCode = TCP_ACKNOWLEDGEMENT_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 529:
    {
        std::cout << "starting TCP_CONTROL_FLAGS_05 test\n";
        test_result.ErrorCode = TCP_CONTROL_FLAGS_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 530:
    {
        std::cout << "starting TCP_CONTROL_FLAGS_08 test\n";
        test_result.ErrorCode = TCP_CONTROL_FLAGS_08();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    case 531:
    {
        std::cout << "starting TCP_CALL_RECEIVE_04 test\n";
        test_result.ErrorCode = TCP_CALL_RECEIVE_04_IT1() | TCP_CALL_RECEIVE_04_IT2() | TCP_CALL_RECEIVE_04_IT3();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }

    case 532:
    {
        std::cout << "starting TCP_CALL_RECEIVE_05 test\n";
        test_result.ErrorCode = TCP_CALL_RECEIVE_05();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }

    case 533:
    {
        std::cout << "starting TCP_CALL_ABORT_02 test\n";
        test_result.ErrorCode = TCP_CALL_ABORT_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }

    case 534:
    {
        std::cout << "starting TCP_CALL_ABORT_03 test\n";
        test_result.ErrorCode = TCP_CALL_ABORT_03_IT01() | TCP_CALL_ABORT_03_IT02() | TCP_CALL_ABORT_03_IT03();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }

    case 535:
    {
        std::cout << "starting TCP_NAGLE_02 test\n";
        test_result.ErrorCode = TCP_NAGLE_02();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }

    case 536:
    {
        std::cout << "starting TCP_FLAGS_INVALID_14 test\n";
        test_result.ErrorCode = TCP_FLAGS_INVALID_14_IT1() | TCP_FLAGS_INVALID_14_IT2();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }

        /*case 537:
    {
        std::cout<<"starting TCP_URGENT_PTR_04 test\n";
        test_result.ErrorCode = TCP_URGENT_PTR_04();
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }*/

    default:
    {
        std::cout << "could not identify the API to be executed\n";
        test_result.ErrorCode = 0x0003; //test not recognized
        test_result.Checksum = gen_crc16((uint8_t *)&test_result, sizeof(test_result_frame_t) - 2);
        break;
    }
    }

    return test_result;
}
