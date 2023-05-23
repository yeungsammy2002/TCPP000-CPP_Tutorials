#include <iostream>

static const char BASE16_ENCODE_TABLE[] = "0123456789ABCDEF";

// mapping table, 128 is for fault tolerance, make sure not overflow
// remaining numbers set to default
static const char BASE16_DECODE_TABLE[128] = {
        -1,                                             // 0
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,         // 1 - 10
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,         // 11 - 20
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,         // 21 - 30
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,         // 31 - 40
        -1, -1, -1, -1, -1, -1, -1, 0, 1, 2,            // 41 - 50
        3, 4, 5, 6, 7, 8, 9, -1, -1, -1,                // 51 - 60
        -1, -1, -1, -1, 10, 11, 12, 13, 14, 15,         // 61 - 70 ('A' - 'F')
};

// input - input data (could be a string or binary), it must be unsigned to perform bitwise operations
// because bitwise operations will remove sign bit that lead to incorrect result
// size - the size of the input data, because input data may not be a string, it can be a binary
// output - output encoded Base16 string
// return value - in original data each byte contains 2 base16 characters (4-bit)
// memory 2 times bigger after encoded because each 4-bit character convert to char type (1 byte)
int Base16Encode(const unsigned char *input, int size, char *output) {
    for (int i = 0; i < size; ++i) {
        // get higher 4 bit from each char of input
        unsigned char higher4bit = input[i] >> 4;

        // get lower 4 bit from each char of input
        unsigned char lower4bit = input[i] & 0x0F;
        // bitwise AND: 1100 0011
        //              0000 1111 (0x0F)
        // result:      0000 0011

        // convert current value to printable character using BASE16_ENCODE_TABLE
        output[i * 2] = BASE16_ENCODE_TABLE[higher4bit];
        output[i * 2 + 1] = BASE16_ENCODE_TABLE[lower4bit];
    }
    return size * 2;
}

// We don't need to know input size, because input must be a string
int Base16Decode(const std::string &input, unsigned char *output) {

    // combine 2 Base16 characters into 1 byte E6B8ACE8A9A642617365313600
    for (int i = 0; i < input.size(); i += 2) {
        // first higher Base16 character 'E' => 69 (Ascii code)
        unsigned char higher_ch = input[i];

        // first lower Base16 character '6' => 54 (Ascii code)
        unsigned char lower_ch = input[i + 1];

        // convert Ascii code to Base16 underlying value
        // we can use BASE16_ENCODE_TABLE, but it spend too much resources to get the value
        // so we use BASE16_DECODE_TABLE (mapping table) instead
        unsigned char higher4bit = BASE16_DECODE_TABLE[higher_ch];
        unsigned char lower4bit = BASE16_DECODE_TABLE[lower_ch];

        // combine higher 4 bit and lower 4 bit to create 1 byte
        output[i / 2] = higher4bit << 4 | lower4bit;
        // higher4bit:  0000 1100
        // left shift:  1100 0000
        // bitwise OR:  0000 0011 (lower4bit)
        // result:      1100 0011
    }

    return input.size() / 2;
}

int main(int argc, char *argv[]) {
    std::cout << "Test Base16" << std::endl;

    // use chinese character to check if any problem after converted to binary
    const unsigned char input_data[] = "測試Base16";
    int length = sizeof(input_data);

    char output_data_1[1024] = {0};
    unsigned char output_data_2[1024] = {0};
    int re = Base16Encode(input_data, length, output_data_1);
    std::cout << re << ":" << output_data_1 << std::endl;

    re = Base16Decode(output_data_1, output_data_2);
    std::cout << re << ":" << output_data_2 << std::endl;
}
