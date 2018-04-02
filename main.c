#include <modbus/modbus-rtu.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

void print_usage(int argc, char* argv[])
{
    printf( "Usage: %s COMMAND\n"
            "\tCOMMAND is one of:\n"
            "\t\tname : Name of device\n"
            "\t\tmodel: Model of device\n"
            "\t\tmanufacturer: Manufacturer of device\n"
            "\t\tserial: Serial number\n"
            "\t\tmanufdate: Date of manufacturing of the device\n"
            "\t\trevision: Hardware revision\n"
            "\t\tfirmware_version: Firmware version\n"
            "\t\tlanguage_version: Language version\n"
            "\t\ttime: Date/Time\n"
            "\t\ttimer_status: Meter Operation timer status (seconds)\n"
            "\t\tphases: Number of phases\n"
            "\t\twires: Number of wires\n"
            "\t\tpower_system: Power system configuration\n"
            "\t\tfrequency: Nominal frequency\n"
            "\t\tphase: Nominal phase order\n"
            "\t\tvts: Number Volt Transfos\n"
            "\t\tvt_primary: Primary VT\n"
            "\t\tvt_secondary: Secondary VT\n"
            "\t\tcts: Number Current Transfos\n"
            "\t\tct_primary: Primary CT\n"
            "\t\tct_secondary: Secondary CT\n"
            "\t\tvt_type: VT Connection Type\n"

            "\t\tI_1: Measured phase 1 current\n"
            "\t\tI_2: Measured phase 2 current\n"
            "\t\tI_3: Measured phase 3 current\n"
            "\t\tI_n: Measured neutral current\n"
            "\t\tI_avg: Measured average current\n"

            "\t\tV_L1_L2: Voltage L1-L2\n"
            "\t\tV_L2_L3: Voltage L2-L3\n"
            "\t\tV_l3_L1: Voltage L3-L1\n"
            "\t\tV_LL_avg: Voltage L-L Average\n"
            "\t\tV_L1_N: Voltage L1-N\n"
            "\t\tV_L2_N: Voltage L2-N\n"
            "\t\tV_L3_N: Voltage L3-N\n"
            "\t\tV_L_N_avg: Voltage L-N Average\n"

            "\t\tP_1: Active power phase 1\n"
            "\t\tP_2: Active power phase 2\n"
            "\t\tP_3: Active power phase 3\n"
            "\t\tP_total: Total active power\n"

            "\t\tPr_1: Reactive power phase 1\n"
            "\t\tPr_2: Reactive power phase 2\n"
            "\t\tPr_3: Ractive power phase 3\n"
            "\t\tPr_total: Total reactive power\n"

            "\t\tPa_1: Apparent power phase 1\n"
            "\t\tPa_2: Apparent power phase 2\n"
            "\t\tPa_3: Apparent power phase 3\n"
            "\t\tPa_total: Total apparent power\n"

            "\t\tPf_1: Power factor phase 1\n"
            "\t\tPf_2: Power factor phase 2\n"
            "\t\tPf_3: Power factor phase 3\n"
            "\t\tPf_total: Total power factor\n"

            "\t\tCu_1: Current unbalance I1\n"
            "\t\tCu_2: Current unbalance I2\n"
            "\t\tCu_3: Current unbalance I3\n"
            "\t\tCu_worst: Worst current unbalance\n"

            "\t\tVu_L1_L2: Voltage unbalance L1-L2\n"
            "\t\tVu_L2_L3: Voltage unbalance L2-L3\n"
            "\t\tVu_L3_L1: Voltage unbalance L3-L1\n"
            "\t\tVu_L_L_worst: Worst voltage unbalance L-L\n"

            "\t\tVu_L1_N: Voltage unbalance L1-N\n"
            "\t\tVu_L2_N: Voltage unbalance L2-N\n"
            "\t\tVu_L3_N: Voltage unbalance L3-N\n"
            "\t\tVu_L_N_worst: Worst voltage unbalance L-N\n"
            "\t\ttangent_phi: Tangent phi\n"
            "\t\tfreq: Frequency\n"
            "\t\ttemp: Temperature\n"
            "\t\tTE_import: Total active Energy import\n"
            "\t\tTE_export: Total active Energy export\n"
            "\t\tTRE_import: Total reactive Energy import\n"
            "\t\tTRE_export: Total reactive Energy export\n"
            "\t\tTAE_import: Total apparent Energy import\n"
            "\t\tTAE_export: Total apparent Energy export\n"
            "\t\tPE_import: Partial active Energy import\n"
            "\t\tPRE_import: Partial reactive Energy import\n"
            "\t\tPAE_import: Partial apparent Energy import\n"

            , argv[0]);
}

int print_register_string(modbus_t* ctx, int reg, int len )
{
    uint16_t buffer[len];
    memset(buffer, 0, sizeof(uint16_t)*len);
    int reg_num = modbus_read_registers(ctx, reg, len, buffer);
    if (reg_num == -1)
    {
        printf("Could not read register %d: %s\n",reg, modbus_strerror(errno));
        return -1;
    }
    for (int i=0; i<reg_num; ++i)
        buffer[i] = ((buffer[i]&0xff00)>>8) | ((buffer[i]&0x00ff)<<8);
    uint8_t* byte_ptr = (uint8_t*)buffer;
    for (int i=0; i<reg_num; ++i)
        printf("%c", byte_ptr[i] );

    printf("\n");
    return 0;
}
int print_register_uint32(modbus_t* ctx, int reg, int len )
{
    uint32_t data[len];
    memset(data, 0, sizeof(data[0]) * len);
    int reg_num = modbus_read_registers(ctx, reg, len, (uint16_t*)&data);
    if (reg_num == -1)
    {
        printf("Could not read register %d: %s\n", reg, modbus_strerror(errno));
        return -1;
    }
    for (int i=0; i<len; ++i)
        printf("%u (0x%x)\n", (unsigned)data[i], (int)data[i]);
    return 0;
}
int print_register_uint16(modbus_t* ctx, int reg, int len )
{
    uint16_t data[len];
    memset(data, 0, sizeof(data[0]) * len);
    int reg_num = modbus_read_registers(ctx, reg, len, (uint16_t*)&data);
    if (reg_num == -1)
    {
        printf("Could not read register %d: %s\n", reg, modbus_strerror(errno));
        return -1;
    }
    for (int i=0; i<len; ++i)
        printf("%u (0x%x)\n", (unsigned)data[i], (int)data[i]);
    return 0;
}
int print_register_float32(modbus_t* ctx, int reg, int len, char* units )
{
    float data[len];
    memset(data, 0, sizeof(data[0]) * len);
    int reg_num = modbus_read_registers(ctx, reg, len * sizeof(float) / 2, (uint16_t*)&data);
    if (reg_num == -1)
    {
        printf("Could not read register %d: %s\n", reg, modbus_strerror(errno));
        return -1;
    }
    for (int i=0; i<len; ++i)
        printf("%f %s (0x%x)\n", (float)data[i], units, (int)data[i]);
    return 0;
}
int print_register_int64(modbus_t* ctx, int reg, int len, char* units )
{
    int64_t data[len];
    memset(data, 0, sizeof(data[0]) * len);
    int reg_num = modbus_read_registers(ctx, reg, len * sizeof(float) / 2, (uint16_t*)&data);
    if (reg_num == -1)
    {
        printf("Could not read register %d: %s\n", reg, modbus_strerror(errno));
        return -1;
    }
    for (int i=0; i<len; ++i)
        printf("%lld %s (0x%llx)\n", (long long)data[i], units, (long long)data[i]);
    return 0;
}
int get_register_uint16(modbus_t* ctx, int reg, uint16_t* data, int len)
{
    memset(data, 0, sizeof(*data) * len);
    int reg_num = modbus_read_registers(ctx, reg, len, data);
    if (reg_num == -1)
    {
        printf("Could not read register %d: %s\n", reg, modbus_strerror(errno));
        return -1;
    }
    return reg_num;
}

int main( int argc, char* argv[])
{
    printf("%s starting\n", argv[0]);

    if (argc < 2)
    {
        print_usage(argc, argv);
        return -1;
    }

    modbus_t* ctx;
    ctx = modbus_new_rtu("/dev/ttyUSB0", 19200, 'E', 8, 1);
    if (!ctx)
    {
        printf("Error communicating with the device\n");
        return -1;
    }
    if (modbus_connect(ctx) == -1)
    {
        printf("Connection failed: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }
#if 0
    if (modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485) != 0)
    {
        printf("Could not set serial mode: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    }
#endif
    if (modbus_set_slave(ctx, 1) != 0)
    {
        printf("Could not set slave: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return -1;
    } 
    else if (strcmp(argv[1], "name") == 0 )
    {
        print_register_string(ctx, 29, 20);
    }
    else if (strcmp(argv[1], "model") == 0 )
    {
        print_register_string(ctx, 50, 20);
    }
    else if (strcmp(argv[1], "manufacturer") == 0 )
    {
        print_register_string(ctx, 70, 20);
    }
    else if (strcmp(argv[1], "serial") == 0 )
    {
        print_register_uint32(ctx, 130, 2);
    }
    else if (strcmp(argv[1], "revision") == 0 )
    {
        print_register_string(ctx, 136, 5);
    }
    else if (strcmp(argv[1], "firmware_version") == 0 )
    {
        print_register_uint16(ctx, 1637, 1);
    }
    else if (strcmp(argv[1], "language_version") == 0 )
    {
        print_register_uint16(ctx, 1701, 1);
    }
    else if (strcmp(argv[1], "time") == 0 )
    {
        print_register_uint16(ctx, 1845, 1);
    }
    else if (strcmp(argv[1], "timer_status") == 0 )
    {
        print_register_uint32(ctx, 2004, 2);
    }
    else if (strcmp(argv[1], "phases") == 0 )
    {
        print_register_uint16(ctx, 2014, 1);
    }
    else if (strcmp(argv[1], "wires") == 0 )
    {
        print_register_uint16(ctx, 2015, 1);
    }
    else if (strcmp(argv[1], "power_system") == 0 )
    {
        uint16_t val = 0;
        int cnt = get_register_uint16(ctx, 2016, &val, 1);
        if (cnt > 0)
        switch (val)
        {
            case 0:
                printf("1PH2W L-N\n");
                break;
            case 1:
                printf("1PH2W L-L\n");
                break;
            case 2:
                printf("1PH3W L-L with N\n");
                break;
            case 3:
                printf("3PH3W\n");
                break;
            case 11:
                printf("3PH4W\n");
                break;
            case 13:
                printf("1PH4W multi-L with N\n");
                break;
            default:
                printf("Unknown power system: %d\n", (int)val);
                break;
        }
    }
    else if (strcmp(argv[1], "frequency") == 0 )
    {
        print_register_uint16(ctx, 2016, 1);
    }
    else if (strcmp(argv[1], "phase") == 0 )
    {
        uint16_t val = 0;
        int cnt = get_register_uint16(ctx, 2024, &val, 1);
        if (cnt > 0)
        switch (val)
        {
            case 0:
                printf("A-B-C\n");
                break;
            case 1:
                printf("C-B-A\n");
                break;
            default:
                printf("Unknown nominal phase order: %d\n", (int)val);
                break;
        }
    }
    else if (strcmp(argv[1], "vts") == 0)
    {
        print_register_uint16(ctx, 2025, 1);
    }
    else if (strcmp(argv[1], "vt_primary") == 0)
    {
        print_register_uint16(ctx, 2026, 1);
    }
    else if (strcmp(argv[1], "vt_secondary") == 0)
    {
        print_register_uint16(ctx, 2028, 1);
    }
    else if (strcmp(argv[1], "cts") == 0)
    {
        print_register_uint16(ctx, 2029, 1);
    }
    else if (strcmp(argv[1], "ct_primary") == 0)
    {
        print_register_uint16(ctx, 2030, 1);
    }
    else if (strcmp(argv[1], "ct_secondary") == 0)
    {
        print_register_uint16(ctx, 2031, 1);
    }
    else if (strcmp(argv[1], "vt_type") == 0 )
    {
        uint16_t val = 0;
        int cnt = get_register_uint16(ctx, 2036, &val, 1);
        if (cnt > 0)
        switch (val)
        {
            case 0:
                printf("Direct\n");
                break;
            case 1:
                printf("3PH3W (2VTs)\n");
                break;
            case 2:
                printf("3PH4W (3VTs)\n");
                break;
            default:
                printf("Unknown VT Connection type: %d\n", (int)val);
                break;
        }
    }
    else if (strcmp(argv[1], "I_1") == 0)
    {
        print_register_float32(ctx, 3000, 1, "A");
    }
    else if (strcmp(argv[1], "I_2") == 0)
    {
        print_register_float32(ctx, 3002, 1, "A");
    }
    else if (strcmp(argv[1], "I_3") == 0)
    {
        print_register_float32(ctx, 3004, 1, "A");
    }
    else if (strcmp(argv[1], "I_n") == 0)
    {
        print_register_float32(ctx, 3006, 1, "A");
    }
    else if (strcmp(argv[1], "I_avg") == 0)
    {
        print_register_float32(ctx, 3010, 1, "A");
    }
    else if (strcmp(argv[1], "V_L1_L2") == 0)
    {
        print_register_float32(ctx, 3020, 1, "V");
    }
    else if (strcmp(argv[1], "V_L2_L3") == 0)
    {
        print_register_float32(ctx, 3022, 1, "V");
    }
    else if (strcmp(argv[1], "V_L3_L1") == 0)
    {
        print_register_float32(ctx, 3024, 1, "V");
    }
    else if (strcmp(argv[1], "V_LL_avg") == 0)
    {
        print_register_float32(ctx, 3026, 1, "V");
    }
    else if (strcmp(argv[1], "V_L1_N") == 0)
    {
        print_register_float32(ctx, 3028, 1, "V");
    }
    else if (strcmp(argv[1], "V_L2_N") == 0)
    {
        print_register_float32(ctx, 3030, 1, "V");
    }
    else if (strcmp(argv[1], "V_L3_N") == 0)
    {
        print_register_float32(ctx, 3032, 1, "V");
    }
    else if (strcmp(argv[1], "V_L_N_avg") == 0)
    {
        print_register_float32(ctx, 3036, 1, "V");
    }
    else if (strcmp(argv[1], "P_1") == 0)
    {
        print_register_float32(ctx, 3054, 1, "kW");
    }
    else if (strcmp(argv[1], "P_2") == 0)
    {
        print_register_float32(ctx, 3056, 1, "kW");
    }
    else if (strcmp(argv[1], "P_3") == 0)
    {
        print_register_float32(ctx, 3058, 1, "kW");
    }
    else if (strcmp(argv[1], "P_total") == 0)
    {
        print_register_float32(ctx, 3060, 1, "kW");
    }
    else if (strcmp(argv[1], "Pr_1") == 0)
    {
        print_register_float32(ctx, 3062, 1, "kVAR");
    }
    else if (strcmp(argv[1], "Pr_2") == 0)
    {
        print_register_float32(ctx, 3064, 1, "kVAR");
    }
    else if (strcmp(argv[1], "Pr_3") == 0)
    {
        print_register_float32(ctx, 3066, 1, "kVAR");
    }
    else if (strcmp(argv[1], "Pr_total") == 0)
    {
        print_register_float32(ctx, 3068, 1, "kVA");
    }
    else if (strcmp(argv[1], "Pa_1") == 0)
    {
        print_register_float32(ctx, 3070, 1, "kVA");
    }
    else if (strcmp(argv[1], "Pa_2") == 0)
    {
        print_register_float32(ctx, 3072, 1, "kVA");
    }
    else if (strcmp(argv[1], "Pa_3") == 0)
    {
        print_register_float32(ctx, 3074, 1, "kVA");
    }
    else if (strcmp(argv[1], "Pa_total") == 0)
    {
        print_register_float32(ctx, 3076, 1, "kVA");
    }
    else if (strcmp(argv[1], "Pf_1") == 0 )
    {
        print_register_float32(ctx, 3078, 1, "");
    }
    else if (strcmp(argv[1], "Pf_2") == 0 )
    {
        print_register_float32(ctx, 3080, 1, "");
    }
    else if (strcmp(argv[1], "Pf_3") == 0 )
    {
        print_register_float32(ctx, 3082, 1, "");
    }
    else if (strcmp(argv[1], "Pf_total") == 0 )
    {
        print_register_float32(ctx, 3084, 1, "");
    }
    else if (strcmp(argv[1], "Cu_1") == 0 )
    {
        print_register_float32(ctx, 3012, 1, "%");
    }
    else if (strcmp(argv[1], "Cu_2") == 0 )
    {
        print_register_float32(ctx, 3014, 1, "%");
    }
    else if (strcmp(argv[1], "Cu_3") == 0 )
    {
        print_register_float32(ctx, 3016, 1, "%");
    }
    else if (strcmp(argv[1], "Cu_worst") == 0 )
    {
        print_register_float32(ctx, 3018, 1, "%");
    }
    else if (strcmp(argv[1], "Vu_L1_L2") == 0 )
    {
        print_register_float32(ctx, 3038, 1, "%");
    }
    else if (strcmp(argv[1], "Vu_L2_L3") == 0 )
    {
        print_register_float32(ctx, 3040, 1, "%");
    }
    else if (strcmp(argv[1], "Vu_L3_L1") == 0 )
    {
        print_register_float32(ctx, 3042, 1, "%");
    }
    else if (strcmp(argv[1], "Vu_L_L_worst") == 0 )
    {
        print_register_float32(ctx, 3044, 1, "%");
    }
    else if (strcmp(argv[1], "Vu_L1_N") == 0 )
    {
        print_register_float32(ctx, 3046, 1, "%");
    }
    else if (strcmp(argv[1], "Vu_L2_N") == 0 )
    {
        print_register_float32(ctx, 3048, 1, "%");
    }
    else if (strcmp(argv[1], "Vu_L3_N") == 0 )
    {
        print_register_float32(ctx, 3050, 1, "%");
    }
    else if (strcmp(argv[1], "Vu_L_N_worst") == 0 )
    {
        print_register_float32(ctx, 3052, 1, "%");
    }
    else if (strcmp(argv[1], "tangent_phi") == 0 )
    {
        print_register_float32(ctx, 3108, 1, "");
    }
    else if (strcmp(argv[1], "freq") == 0 )
    {
        print_register_float32(ctx, 3110, 1, "Hz");
    }
    else if (strcmp(argv[1], "temp") == 0 )
    {
        print_register_float32(ctx, 3132, 1, "degC");
    }
    else if (strcmp(argv[1], "TE_import") == 0 )
    {
        print_register_int64(ctx, 3204, 1, "Wh");
    }
    else if (strcmp(argv[1], "TE_export") == 0 )
    {
        print_register_int64(ctx, 3208, 1, "Wh");
    }
    else if (strcmp(argv[1], "TRE_import") == 0 )
    {
        print_register_int64(ctx, 3220, 1, "VARh");
    }
    else if (strcmp(argv[1], "TRE_export") == 0 )
    {
        print_register_int64(ctx, 3224, 1, "VARh");
    }
    else if (strcmp(argv[1], "TAE_import") == 0 )
    {
        print_register_int64(ctx, 3236, 1, "VAh");
    }
    else if (strcmp(argv[1], "TAE_export") == 0 )
    {
        print_register_int64(ctx, 3240, 1, "VAh");
    }
    else if (strcmp(argv[1], "PE_import") == 0 )
    {
        print_register_int64(ctx, 3256, 1, "Wh");
    }
    else if (strcmp(argv[1], "PRE_import") == 0 )
    {
        print_register_int64(ctx, 3272, 1, "VARh");
    }
    else if (strcmp(argv[1], "PAE_import") == 0 )
    {
        print_register_int64(ctx, 3288, 1, "VAh");
    }
    else
    {
        printf("Unknown command: %s\n", argv[1]);
        print_usage(argc, argv);
    }
    
    modbus_free(ctx);
    return 0;
}
