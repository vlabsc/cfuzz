// 0.40

unsigned long int unsigned_char_to_unsigned_long_int(unsigned char *seed_file_max_size_in_bytes)
{
    int len1 = 0;
    int len2 = 0;
    int i = 0;

    unsigned long int power = 1;
    int pi = 0;
    unsigned long int v = 0;

    unsigned long int value = 0;

    len1 = strlen(seed_file_max_size_in_bytes);
    len2 = strlen(seed_file_max_size_in_bytes);


    //printf("converting %s to unsigned long int \n", seed_file_max_size_in_bytes);
    //printf("%d is length. len2: %d \n", len1, len2);

    for(i = 0; i < len1; i++)
    {
        if(seed_file_max_size_in_bytes[i] == '0')
            v = 0;
        else if(seed_file_max_size_in_bytes[i] == '1')
            v = 1;
        else if(seed_file_max_size_in_bytes[i] == '2')
            v = 2;
        else if(seed_file_max_size_in_bytes[i] == '3')
            v = 3;
        else if(seed_file_max_size_in_bytes[i] == '4')
            v = 4;
        else if(seed_file_max_size_in_bytes[i] == '5')
            v = 5;
        else if(seed_file_max_size_in_bytes[i] == '6')
            v = 6;
        else if(seed_file_max_size_in_bytes[i] == '7')
            v = 7;
        else if(seed_file_max_size_in_bytes[i] == '8')
            v = 8;
        else if(seed_file_max_size_in_bytes[i] == '9')
            v = 9;

        for(pi = 0; pi < len2 -1; pi++)
        {
            power = power * 10;
        }
        value = value + (v * power);
        //printf("%c -> , power: %lu, v: %d value: %lu\n", seed_file_max_size_in_bytes[0], power, v, value);
        power = 1;
        len2--;
    }

    return value;

}

unsigned int unsigned_char_to_unsigned_int(unsigned char *seed_file_max_size_in_bytes)
{
    int len1 = 0;
    int len2 = 0;
    int i = 0;
    unsigned long int power = 1;
    int pi = 0;
    unsigned long int v = 0;
    unsigned int value = 0;

    len1 = strlen(seed_file_max_size_in_bytes);
    len2 = strlen(seed_file_max_size_in_bytes);


    //printf("converting %s to unsigned long int \n", seed_file_max_size_in_bytes);
    //printf("%d is length. len2: %d \n", len1, len2);

    for(i = 0; i < len1; i++)
    {
        if(seed_file_max_size_in_bytes[i] == '0')
            v = 0;
        else if(seed_file_max_size_in_bytes[i] == '1')
            v = 1;
        else if(seed_file_max_size_in_bytes[i] == '2')
            v = 2;
        else if(seed_file_max_size_in_bytes[i] == '3')
            v = 3;
        else if(seed_file_max_size_in_bytes[i] == '4')
            v = 4;
        else if(seed_file_max_size_in_bytes[i] == '5')
            v = 5;
        else if(seed_file_max_size_in_bytes[i] == '6')
            v = 6;
        else if(seed_file_max_size_in_bytes[i] == '7')
            v = 7;
        else if(seed_file_max_size_in_bytes[i] == '8')
            v = 8;
        else if(seed_file_max_size_in_bytes[i] == '9')
            v = 9;

        for(pi = 0; pi < len2 -1; pi++)
        {
            power = power * 10;
        }
        value = value + (v * power);
        //printf("%c -> , power: %lu, v: %d value: %lu\n", seed_file_max_size_in_bytes[0], power, v, value);
        power = 1;
        len2--;
    }

    return value;

}

