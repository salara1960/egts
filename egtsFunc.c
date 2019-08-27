#include "egtsFunc.h"

//----------------------------------------------------------------------------------------------------

const char *SrcLocation[] = {
"таймер при включенном зажигании",
"пробег заданной дистанции",
"превышение установленного значения угла поворота",
"ответ на запрос",
"изменение состояния входа X",
"таймер при выключенном зажигании",
"отключение периферийного оборудования",
"превышение одного из заданных порогов скорости",
"рестарт",
"перегрузка по выходу Y",
"сработал датчик вскрытия корпуса прибора",
"переход на резервное питание/отключение внешнего питания",
"снижение напряжения источника резервного питания ниже порогового значения",
"нажата 'тревожная кнопка'",
"запрос на установление голосовой связи с оператором",
"экстренный вызов",
"появление данных от внешнего сервиса",
"зарезервировано",
"зарезервировано",
"неисправность резервного аккумулятора",
"резкий разгон",
"резкое торможение",
"отключение или неисправность навигационного модуля",
"отключение или неисправность датчика автоматической идентификации события ДТП",
"отключение или неисправность антенны GSM/UMTS",
"отключение или неисправность антенны навигационной системы",
"зарезервировано",
"снижение скорости ниже одного из заданных порогов",
"перемещение при выключенном зажигании",
"таймер в режиме 'экстренное слежение'",
"начало/окончание навигации",
"нестабильная навигация",
"установка IP соединения",
"нестабильная регистрация в сети подвижной радиотелефонной связи",
"нестабильная связь",
"изменение режима работы",
"неизвестно"
};

//-----------------------------------------------------------------------------------------------------------

//char const * vers = "0.1";//21.08.2018
//char const *vers = "0.2";//18.09.2018
//char const *vers = "0.3";//25.06.2019
//char const *vers = "0.4";//28.06.2019
//char const *vers = "0.5";//29.06.2019
//char const *vers = "0.6";//24.07.2019
//char const *vers = "0.7";//29.07.2019
//char const *vers = "0.8";//30.07.2019
//char const *vers = "0.9";//05.08.2019
//char const *vers = "1.0";//22.08.2019
//char const *vers = "1.1";//23.08.2019
//char const *vers = "1.2";//26.08.2019
char const *vers = "1.3";//27.08.2019

//-----------------------------------------------------------------------------------------------------------

int fd_log = -1;

uint8_t QuitAll  = 0;
uint8_t SIGHUPs  = 1;
uint8_t SIGTERMs = 1;
uint8_t SIGINTs  = 1;
uint8_t SIGKILLs = 1;
uint8_t SIGSEGVs = 1;
uint8_t SIGABRTs = 1;
uint8_t SIGSYSs  = 1;
uint8_t SIGTRAPs = 1;

pthread_mutex_t prn_mutex = PTHREAD_MUTEX_INITIALIZER;

uint8_t QuitCli = 1;

const char *the_log = "log.txt";
int max_size_log = 1024 * 32000;

int MaxLogLevel = LOG_DEBUG + 1;


uint8_t service_flag = 0;

const uint8_t CRC8Table[256] = {
    0x00, 0x31, 0x62, 0x53, 0xC4, 0xF5, 0xA6, 0x97,
    0xB9, 0x88, 0xDB, 0xEA, 0x7D, 0x4C, 0x1F, 0x2E,
    0x43, 0x72, 0x21, 0x10, 0x87, 0xB6, 0xE5, 0xD4,
    0xFA, 0xCB, 0x98, 0xA9, 0x3E, 0x0F, 0x5C, 0x6D,
    0x86, 0xB7, 0xE4, 0xD5, 0x42, 0x73, 0x20, 0x11,
    0x3F, 0x0E, 0x5D, 0x6C, 0xFB, 0xCA, 0x99, 0xA8,
    0xC5, 0xF4, 0xA7, 0x96, 0x01, 0x30, 0x63, 0x52,
    0x7C, 0x4D, 0x1E, 0x2F, 0xB8, 0x89, 0xDA, 0xEB,
    0x3D, 0x0C, 0x5F, 0x6E, 0xF9, 0xC8, 0x9B, 0xAA,
    0x84, 0xB5, 0xE6, 0xD7, 0x40, 0x71, 0x22, 0x13,
    0x7E, 0x4F, 0x1C, 0x2D, 0xBA, 0x8B, 0xD8, 0xE9,
    0xC7, 0xF6, 0xA5, 0x94, 0x03, 0x32, 0x61, 0x50,
    0xBB, 0x8A, 0xD9, 0xE8, 0x7F, 0x4E, 0x1D, 0x2C,
    0x02, 0x33, 0x60, 0x51, 0xC6, 0xF7, 0xA4, 0x95,
    0xF8, 0xC9, 0x9A, 0xAB, 0x3C, 0x0D, 0x5E, 0x6F,
    0x41, 0x70, 0x23, 0x12, 0x85, 0xB4, 0xE7, 0xD6,
    0x7A, 0x4B, 0x18, 0x29, 0xBE, 0x8F, 0xDC, 0xED,
    0xC3, 0xF2, 0xA1, 0x90, 0x07, 0x36, 0x65, 0x54,
    0x39, 0x08, 0x5B, 0x6A, 0xFD, 0xCC, 0x9F, 0xAE,
    0x80, 0xB1, 0xE2, 0xD3, 0x44, 0x75, 0x26, 0x17,
    0xFC, 0xCD, 0x9E, 0xAF, 0x38, 0x09, 0x5A, 0x6B,
    0x45, 0x74, 0x27, 0x16, 0x81, 0xB0, 0xE3, 0xD2,
    0xBF, 0x8E, 0xDD, 0xEC, 0x7B, 0x4A, 0x19, 0x28,
    0x06, 0x37, 0x64, 0x55, 0xC2, 0xF3, 0xA0, 0x91,
    0x47, 0x76, 0x25, 0x14, 0x83, 0xB2, 0xE1, 0xD0,
    0xFE, 0xCF, 0x9C, 0xAD, 0x3A, 0x0B, 0x58, 0x69,
    0x04, 0x35, 0x66, 0x57, 0xC0, 0xF1, 0xA2, 0x93,
    0xBD, 0x8C, 0xDF, 0xEE, 0x79, 0x48, 0x1B, 0x2A,
    0xC1, 0xF0, 0xA3, 0x92, 0x05, 0x34, 0x67, 0x56,
    0x78, 0x49, 0x1A, 0x2B, 0xBC, 0x8D, 0xDE, 0xEF,
    0x82, 0xB3, 0xE0, 0xD1, 0x46, 0x77, 0x24, 0x15,
    0x3B, 0x0A, 0x59, 0x68, 0xFF, 0xCE, 0x9D, 0xAC
};

const uint16_t Crc16Table[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
    0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
    0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
    0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
    0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
    0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
    0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

//------------------------------------------------------------------------------
uint8_t CRC8EGTS(uint8_t *uk, uint8_t len)
{
uint8_t crc = 0xFF;

    while (len--) crc = CRC8Table[crc ^ *uk++];

    return crc;
}
//------------------------------------------------------------------------------
uint16_t CRC16EGTS(uint8_t *uk, uint16_t len)
{
uint16_t crc = 0xFFFF;

    while (len--) crc = (crc << 8) ^ Crc16Table[(crc >> 8) ^ *uk++];

    return crc;
}
//------------------------------------------------------------------------------
char *ShowTime(time_t ct)
{
char *arba = ctime(&ct);

    arba[strlen(arba) - 1] = 0;

    return (arba);
}
//------------------------------------------------------------------------------
inline char *TimeNowPrn(char *ts)
{
struct tm *ctimka;
struct timeval tvl;

    gettimeofday(&tvl, NULL);
    ctimka = localtime(&tvl.tv_sec);
    sprintf(ts,"%02d.%02d %02d:%02d:%02d.%03d | ",
               ctimka->tm_mday, ctimka->tm_mon + 1,
               ctimka->tm_hour, ctimka->tm_min, ctimka->tm_sec, (int)(tvl.tv_usec/1000));
    return ts;
}
//------------------------------------------------------------------------
char *TimeToStr(time_t ct)
{
char *arba = ctime(&ct);

    arba[strlen(arba) - 1] = 0;

    return (arba);
}
//----------------------------------------------------------------------
void ToSysLogMsg(int LogLevel, const char * const Msg)
{
    if (LogLevel <= MaxLogLevel) syslog(LogLevel, "%s", Msg);
}
//----------------------------------------------------------------------
void print_msg(uint8_t dt, const char *fmt, ...)
{
size_t len = buf_size;
char dts[TIME_STR_LEN] = {0};
char *udt = TimeNowPrn(dts);

    if (dt) len += sizeof(dts);
    char *st = (char *)calloc(1, len + 1);
    if (st) {
        int dl = 0, sz;
        va_list args;

        if (dt) dl = sprintf(st, "%s", udt);
        sz = dl;

        va_start(args, fmt);
        sz += vsnprintf(st + dl, len - dl, fmt, args);
        va_end(args);

        pthread_mutex_lock(&prn_mutex);
            printf("%s", st);
            if (fd_log) write(fd_log, st, strlen(st));

            struct stat sb;
            if (!fstat(fd_log, &sb)) {
                if (sb.st_size > max_size_log) {
                    close(fd_log);
                    fd_log = -1;
                    char name[128] = {0};
                    sprintf(name, "%u_%s", (uint32_t)time(NULL), udt);
                    rename(the_log, name);
                    fd_log = open(the_log, O_WRONLY | O_APPEND | O_CREAT, 0664); //create new file
                    if (fd_log <= 0) sprintf(name, "%sVer.%s Can't open file %s (%d)\n", udt, vers, the_log, fd_log);
                                else sprintf(name, "%sVer.%s Open new file %s (%d)\n", udt, vers, the_log, fd_log);
                    ToSysLogMsg(LOG_INFO, name);
                }
            }
        pthread_mutex_unlock(&prn_mutex);

        free(st);

    }

}
//--------------------  function for recive SIGNAL from system -----------
void GetSignal_(int sig)
{
int out = 0;
char stx[64] = {0};

    switch (sig) {
        case SIGHUP:
            strcpy(stx, "\tSignal SIGHUP\n");
        break;
        case SIGKILL:
            if (SIGKILLs) {
                SIGKILLs = 0;
                strcpy(stx, "\tSignal SIGKILL\n");
                out = 1;
            }
        break;
        case SIGPIPE:
            strcpy(stx, "\tSignal SIGPIPE\n");
        break;
        case SIGTERM:
            if (SIGTERMs) {
                SIGTERMs = 0;
                strcpy(stx, "\tSignal SIGTERM\n");
                out = 1;
            }
        break;
        case SIGINT:
            if (SIGINTs) {
                SIGINTs = 0;
                strcpy(stx, "\tSignal SIGINT\n");
                out = 1;
            }
        break;
        case SIGSEGV:
            if (SIGSEGVs) {
                SIGSEGVs = 0;
                strcpy(stx, "\tSignal SIGSEGV\n");
                out = 1;
            }
        break;
        case SIGABRT:
            if (SIGABRTs) {
                SIGABRTs = 0;
                strcpy(stx, "\tSignal SIGABRT\n");
                out = 1;
            }
        break;
        case SIGSYS:
            if (SIGSYSs) {
                SIGSYSs = 0;
                strcpy(stx, "\tSignal SIGSYS\n");
                out = 1;
            }
        break;
        case SIGTRAP:
            if (SIGTRAPs) {
                SIGTRAPs = 0;
                strcpy(stx, "\tSignal SIGTRAP\n");
                out = 1;
            }
        break;
            default : sprintf(stx, "\tUNKNOWN signal %d", sig);
    }

    print_msg(1, stx);

    if (out) QuitAll = out;
}
//-----------------------------------------------------------------------
static const char *PackName(uint8_t pt)
{
    switch (pt) {
        case EGTS_PT_RESPONSE://0
            return "EGTS_PT_RESPONSE";
        case EGTS_PT_APPDATA://1
            return "EGTS_PT_APPDATA";
        case EGTS_PT_SIGNED_APPDATA://2
            return "EGTS_PT_SIGNED_APPDATA";
    }

    return "EGTS_PT_UNKNOWN";
}
//-----------------------------------------------------------------------
static const char *VHTName(uint32_t tip)
{
    switch (tip & 0x1f) {
        case 1:
            return "Class M1 (пассажирский)";
        case 2:
            return "Class M2 (автобус)";
        case 3:
            return "Class M3 (автобус)";
        case 4:
            return "Class N1 (легкая грузовая)";
        case 5:
            return "Class N2 (тяжелая грузовая)";
        case 6:
            return "Class N3 (тяжелая грузовая)";
        case 7:
            return "Class L1e (мотоцикл)";
        case 8:
            return "Class L2e (мотоцикл)";
        case 9:
            return "Class L3e (мотоцикл)";
        case 10:
            return "Class L4e (мотоцикл)";
        case 11:
            return "Class L5e (мотоцикл)";
        case 12:
            return "Class L6e (мотоцикл)";
        case 13:
            return "Class L7e (мотоцикл)";
    }

    return "Class Uknown";
}
//-----------------------------------------------------------------------
static const char *VPSTName(uint32_t tip)
{
    if (!(tip & 0x3f))   return "PowerType undefined";
         if (tip & 1)    return "Бензин";
    else if (tip & 2)    return "Дизел";
    else if (tip & 4)    return "Сжиженный природный газ (CNG)";
    else if (tip & 8)    return "Жидкий пропан (LPG)";
    else if (tip & 0x10) return "Электро. (более 42B и 100 А/ч)";
    else if (tip & 0x20) return "Водород";

    return "PowerType unknown";
}
//-----------------------------------------------------------------------
static const char *SSTName(uint8_t stat)
{
    switch (stat) {
        case EGTS_SST_IN_SERVICE://0, //сервис в рабочем состоянии и разрешен к использованию
            return "EGTS_SST_IN_SERVICE";
        case EGTS_SST_OUT_OF_SERVICE://128, //сервис в нерабочем состоянии (выключен)
            return "EGTS_SST_OUT_OF_SERVICE";
        case EGTS_SST_DENIED://129//сервис апрещен для использования
            return "EGTS_SST_DENIED";
        case EGTS_SST_NO_CONF://130//сервис не настроен
            return "EGTS_SST_NO_CONF";
        case EGTS_SST_TEMP_UNAVAIL://131//сервис временно недоступен
            return "EGTS_SST_TEMP_UNAVAIL";
    }

    return "EGTS_SST_UNKNOWN";
}
//-----------------------------------------------------------------------
static const char *ServName(uint8_t stp)
{
    switch (stp) {
        case EGTS_AUTH_SERVICE://         (1)
            return "EGTS_AUTH_SERVICE";
        case EGTS_TELEDATA_SERVICE://     (2)
            return "EGTS_TELEDATA_SERVICE";
        case EGTS_COMMANDS_SERVICE://     (4)
            return "EGTS_COMMANDS_SERVICE";
        case EGTS_FIRMWARE_SERVICE://     (9)
            return "EGTS_FIRMWARE_SERVICE";
        case EGTS_ECALL_SERVICE://        (10)
            return "EGTS_ECALL_SERVICE";
    }

    return "EGTS_UNKNOWN_SERVICE";
}
//----------------------------------------------------------------------
static const char *SubRecName(uint8_t srt)
{
    switch (srt) {
        case EGTS_SR_RECORD_RESPONSE:
            return "EGTS_SR_RECORD_RESPONSE";//(0)
        case EGTS_SR_TERM_IDENTITY:
            return "EGTS_SR_TERM_IDENTITY";//(1)
        case EGTS_SR_MODULE_DATA:
            return "EGTS_SR_MODULE_DATA";//(2)
        case EGTS_SR_VEHICLE_DATA:
            return "EGTS_SR_VEHICLE_DATA";//(3)

        case EGTS_SR_DISPATCHER_IDENTITY:
            return "EGTS_SR_DISPATCHER_IDENTITY";//5
        case EGTS_SR_AUTH_PARAMS:
            return "EGTS_SR_AUTH_PARAMS";//(6)
        case EGTS_SR_AUTH_INFO:
            return "EGTS_SR_AUTH_INFO";//(7)
        case EGTS_SR_SERVICE_INFO:
            return "EGTS_SR_SERVICE_INFO";//(8)
        case EGTS_SR_RESULT_CODE:
            return "EGTS_SR_RESULT_CODE";//(9)

        case EGTS_SR_POS_DATA:
            return "EGTS_SR_POS_DATA";//16
        case EGTS_SR_EXT_POS_DATA:
            return "EGTS_SR_EXT_POS_DATA";//17
        case EGTS_SR_AD_SENSORS_DATA:
            return "EGTS_SR_AD_SENSORS_DATA";//18
        case EGTS_SR_COUNTERS_DATA:
            return "EGTS_SR_COUNTERS_DATA";//19
        case EGTS_SR_ACCEL_DATA:
            return "EGTS_SR_ACCEL_DATA";//20
        case EGTS_SR_STATE_DATA:
            return "EGTS_SR_STATE_DATA";//21
        case EGTS_SR_LOOPIN_DATA:
            return "EGTS_SR_LOOPIN_DATA";//22
        case EGTS_SR_ABS_DIG_SENS_DATA:
            return "EGTS_SR_ABS_DIG_SENS_DATA";//23
        case EGTS_SR_ABS_AN_SENS_DATA:
            return "EGTS_SR_ABS_AN_SENS_DATA";//24
        case EGTS_SR_ABS_CNTR_DATA:
            return "EGTS_SR_ABS_CNTR_DATA";//25
        case EGTS_SR_ABS_LOOPIN_DATA:
            return "EGTS_SR_ABS_LOOPIN_DATA";//26
        case EGTS_SR_LIQUID_LEVEL_SENSOR:
            return "EGTS_SR_LIQUID_LEVEL_SENSOR";//27
        case EGTS_SR_PASSENGERS_COUNTERS:
            return "EGTS_SR_PASSENGERS_COUNTERS";//28
    }

    return "EGTS_SR_UNKNOWN";
}
//-----------------------------------------------------------------------
static const char *SRVRPName(uint8_t v)//Service Routing Priority
{
    switch (v&3) {
        case 0:
            return "TopPriority";
        case 1:
            return "HighPriority";
        case 2:
            return "MidlePriority";
        case 3:
            return "LowPriority";
    }

    return "UnknownPriority";
}
//------------------------------------------------------------------------------
static int make_ack(uint8_t *buf, uint16_t rn, uint16_t pid, uint8_t res, uint8_t prn)
{
static uint16_t PaketNumber = 0;
s_min_hdr *hdr = (s_min_hdr *)buf;
int dl = sizeof(s_min_hdr);


    memset(buf, 0, sizeof(s_min_hdr));
    hdr->PRV = 1;
    hdr->SKID = 0;
    //hdr->PR = 0;unsigned PR:2;//Priority: 00-высший, 01-высокий, 10-средний, 11-низкий
    //hdr->CMP = 0;//unsigned CMP:1;//Compressed
    //hdr->ENA = 0;//unsigned ENA:2;//Encryption Algorithm
    //hdr->RTE = 0;//RTE (Route)
    //hdr->PRF = 0;//unsigned RTE:1;//1-PeerAddress+RecipientAddress+TTL present
    //hdr->PRF = 0;//unsigned PRF:2;//Prefix=0x00
    hdr->HL = sizeof(s_min_hdr);// 11 //uint8_t HL;//Header Length
    //hdr->HE = 0;//uint8_t HE;//Header Encoding
    hdr->FDL = sizeof(s_min_ack);//uint16_t FDL;//Frame Data Length
    hdr->PID = ++PaketNumber;//uint16_t PID;//Packet Identifier
    hdr->PT = EGTS_PT_RESPONSE;// Тип пакета Транспортного Уровня //uint8_t PT;//Packet Type
    hdr->HCS = CRC8EGTS((uint8_t *)hdr, hdr->HL - 1);//uint8_t HCS;//Header Check Sum
//SFRD
    s_min_ack *ack = (s_min_ack *)(buf + dl);
    memset((uint8_t *)ack, 0, sizeof(s_min_ack));
    ack->RPID = pid;
    ack->RESULT = res;
    dl += sizeof(s_min_ack);
//SFRCS
    //

    uint16_t word = CRC16EGTS((uint8_t *)ack, sizeof(s_min_ack));
    memcpy(buf + dl, &word, sizeof(uint16_t));
    dl += sizeof(uint16_t);

    if (prn) print_msg(1, "[%s]:\n\tPRV:0x%02X\n\tSKID:0x%02X\n\tFlags:0x%02X\n"
                         "\t\tPRF:%02u RTE:%u ENA:%02u CMP:%u PR:%02u\n"
                         "\tHL:%u\n\tHE:%u\n\tFDL:%u(0x%04X)\n\tPID:%u(0x%04X)\n"
                         "\tPT:%u\t'%s'\n\tHCS:0x%02X\n"
                         "\tSFRD: RPID:%u(0x%04X) RESULT:%u\n\tCRC16:0x%04X\n",
                         __func__, hdr->PRV, hdr->SKID, *(buf + 2),
                         hdr->PRF, hdr->RTE, hdr->ENA, hdr->CMP, hdr->PR,
                         hdr->HL, hdr->HE, hdr->FDL, hdr->FDL,
                         hdr->PID, hdr->PID, hdr->PT, PackName(hdr->PT), hdr->HCS,
                         ack->RPID, ack->RPID, ack->RESULT, word);

    return dl;
}
//-----------------------------------------------------------------------
int egts_parse(char *device, s_min_hdr *hdr, uint8_t *from_cli, int flen, uint8_t *to_cli, uint8_t prn)
{
int uk, dl, err = 0, i, j, iret = 0;
uint8_t res = EGTS_PC_IO_ERROR;
s_rec_hdr *rec_hdr = NULL;
uint8_t sst, rst, rtype, byte, major, minor;
uint16_t rlen = 0, word, rec_num = 0, pid_num, speed, dir;
uint8_t *uki = NULL, *rflag = NULL;
uint8_t *uke = from_cli + flen - 2 - 1;//указатель на последний байт данных (перед CRC16)
uint32_t tim = 0, oid = 0, evid = 0, odometer = 0, altitude = 0;
char tps[vrem_size] = {0};
char vr1[64] = {0}, vr2[64] = {0};
char vrem[buf_size] = {0};
char srst[buf_size] = {0};
char info[256] = {0};
uint16_t *uBIT16 = NULL;
s_term_id *term_id = NULL;
s_mod_data *mod_data = NULL;
s_veh_data *veh_data = NULL;
s_sr_disp_id *sr_disp_id = NULL;
s_auth_params *auth_params = NULL;
s_sr_info *sr_info = NULL;
s_sr_pos_data *sr_pos_data = NULL;
s_sr_ext_pos_data *sr_ext_pos_data = NULL;
s_sr_ad_sensors_data *sr_ad_sensors_data = NULL;
uint32_t latit, longit;
float flatit, flongit;

uint16_t *crc16 = (uint16_t *)&from_cli[flen - 2];//CRC16 from device

uint8_t  calc_CRC8  = CRC8EGTS((uint8_t *)hdr, hdr->HL - 1);
uint16_t calc_CRC16 = CRC16EGTS(from_cli, flen - 2);


    pid_num = hdr->PID;

    switch (hdr->PT) {
        case EGTS_PT_RESPONSE://0
            iret = -1;
            s_min_ack *ack = (s_min_ack *)from_cli;
            sprintf(tps, "REC(%u) : '%s'\n\tRPID:%u(0x%04X) RESULT:%u\n",
                                hdr->FDL, PackName(hdr->PT),
                                ack->RPID, ack->RPID, ack->RESULT);
        break;
        case EGTS_PT_APPDATA://1
            if (hdr->FDL) {
                rec_hdr = (s_rec_hdr *)from_cli;
                rec_num = rec_hdr->RN;
                rflag = from_cli + sizeof(s_rec_hdr) - 1;
                uki = &from_cli[0];
                uki += sizeof(s_rec_hdr);//uk to OID
                if (rec_hdr->OBFE) {
                    memcpy(&oid, uki, sizeof(uint32_t));
                    uki += sizeof(uint32_t);//uk to EVID
                }
                if (rec_hdr->EVFE) {
                    memcpy(&evid, uki, sizeof(uint32_t));
                    uki += sizeof(uint32_t);//uk to TM
                }
                if (rec_hdr->TMFE) {
                    memcpy(&tim, uki, sizeof(uint32_t));
                    uki += sizeof(uint32_t);//uk to SST
                    tim += UTS2010;
                }
                sst = *uki++;
                rst = *uki++;
                //uki -> pointer to Record Data (record type)
                rtype = *uki++;//get SRT (Subrecord Type)
                memcpy(&rlen, uki, sizeof(uint16_t));//get SRL (Subrecord Length)
                uki += sizeof(uint16_t);//uk to SRD (Subrecord Data)
                switch (rst) {// switch by Service Type  RST
                    case EGTS_AUTH_SERVICE ://        (1)
                        memset(srst, 0, sizeof(srst));
                        while (uke > uki) {
                            sprintf(srst+strlen(srst),"\tSubRecord '%s' : rLen:%u rType(%u)\n", SubRecName(rtype), rlen, rtype);
                            switch (rtype) {//switch by SubRecord Type  SRT
                                case EGTS_SR_RECORD_RESPONSE:// (0)
                                    uBIT16 = (uint16_t *)uki;
                                    uki += sizeof(uint16_t);
                                    byte = *uki++;
                                    sprintf(srst+strlen(srst), "\t\tCRN %u for RN %u with RS %u\n", *uBIT16, rec_num, byte);
                                break;
                                case EGTS_SR_TERM_IDENTITY://   (1)
                                    term_id = (s_term_id *)uki;
                                    uki += sizeof(uint32_t);//skeep TID (Terminal Identifier)
                                    byte = *uki++;//
                                    sprintf(srst+strlen(srst), "\t\tTID:%u\n\t\tFlags:0x%02X:\n"
                                        "\t\t\tMNE:%u BSE:%u NIDE:%u SSRA:%u LNGCE:%u IMSIE:%u IMEIE:%u HDIDE:%u\n\t\t",
                                        term_id->TID, byte,
                                        term_id->MNE, term_id->BSE, term_id->NIDE, term_id->SSRA,
                                        term_id->LNGCE, term_id->IMSIE, term_id->IMEIE, term_id->HDIDE);
                                    if (term_id->HDIDE) {
                                        uBIT16 = (uint16_t *)uki;
                                        sprintf(srst+strlen(srst), "HDID:%u ", *uBIT16);
                                        uki += sizeof(uint16_t);
                                    }
                                    if (term_id->IMEIE) {
                                        memcpy(device, uki, size_imei);
                                        sprintf(srst+strlen(srst), "IMEI:%.*s ", size_imei, uki);
                                        uki += size_imei;
                                    }
                                    if (term_id->IMSIE) {
                                        sprintf(srst+strlen(srst), "IMSI:%.*s ", size_imsi, uki);
                                        uki += size_imsi;
                                    }
                                    if (term_id->LNGCE) {
                                        sprintf(srst+strlen(srst), "LNGC:%.*s ", 3, uki);
                                        uki += 3;
                                    }
                                    if (term_id->NIDE) {
                                        sprintf(srst+strlen(srst), "NID:%02X%02X%02X ", *uki, *(uki+1), *(uki+2));
                                        uki += 3;
                                    }
                                    if (term_id->BSE) {
                                        uBIT16 = (uint16_t *)uki;
                                        sprintf(srst+strlen(srst), "BS:%u ", *uBIT16);
                                        uki += sizeof(uint16_t);
                                    }
                                    if (term_id->MNE) {
                                        sprintf(srst+strlen(srst), "MSISDN:%.*s ", size_imei, uki);
                                        uki += size_imei;
                                    }
                                    sprintf(srst+strlen(srst), "\n");
                                break;
                                case EGTS_SR_MODULE_DATA://     (2)
                                    mod_data = (s_mod_data *)uki;
                                    word = mod_data->FWV;   minor = word & 0xff;   major = word >> 8;
                                    sprintf(srst+strlen(srst), "\t\tMT:%u VID:%u FWV:%u.%u", mod_data->MT, mod_data->VID, major, minor);
                                    word = mod_data->SWV;   minor = word & 0xff;   major = word >> 8;
                                    sprintf(srst+strlen(srst)," SWV:%u.%u MD:%u ST:%u", major, minor, mod_data->MD, mod_data->ST);
                                    uki += sizeof(s_mod_data);
                                    dl = sprintf(vrem,"%s", uki);   if (dl > 32) vrem[32] = '\0';   dl = strlen(vrem);
                                    if (dl) sprintf(srst+strlen(srst)," SRN:%s", vrem);
                                    uki += dl + 1;
                                    dl = sprintf(vrem,"%s", uki);   if (dl > 32) vrem[32] = '\0';   dl = strlen(vrem);
                                    if (dl) sprintf(srst+strlen(srst)," DSCR:%s", vrem);
                                    sprintf(srst+strlen(srst), "\n");
                                break;
                                case EGTS_SR_VEHICLE_DATA://    (3)
                                    veh_data = (s_veh_data *)uki;
                                    sprintf(srst+strlen(srst), "\t\tVIN:'%.*s' VHT(%u):'%s' VPST(%u):'%s'\n",
                                                    (int)sizeof(veh_data->VIN), veh_data->VIN,
                                                    veh_data->VHT, VHTName(veh_data->VHT),
                                                    veh_data->VPST, VPSTName(veh_data->VPST));
                                    uki += rlen;
                                break;
                                case EGTS_SR_DISPATCHER_IDENTITY:// 5
                                    sr_disp_id = (s_sr_disp_id *)uki;
                                    sprintf(srst+strlen(srst), "\t\tDT:%u DID:%u\n", sr_disp_id->DT, sr_disp_id->DID);
                                    uki += rlen;
                                break;
                                case EGTS_SR_AUTH_PARAMS://     (6)
                                    auth_params = (s_auth_params *)uki;
                                    sprintf(srst+strlen(srst),
                                               "\t\tFlags:0x%02X - EXE:%u SSE:%u MSE:%u ISLE:%u PKE:%u ENA:%u\n",
                                               *uki,
                                               auth_params->EXE, auth_params->SSE, auth_params->MSE,
                                               auth_params->ISLE, auth_params->PKE, auth_params->ENA);
                                    uki++;
                                    if (auth_params->PKE) {
                                        uBIT16 = (uint16_t *)uki;
                                        sprintf(srst+strlen(srst), "\t\t\tPKL:%u PBK:", *uBIT16);
                                        uki += sizeof(uint16_t);//uk to PBK
                                        for (i = 0; i < *uBIT16; i++) sprintf(srst+strlen(srst), "%02X", *(uki + i));
                                        sprintf(srst+strlen(srst), "\n");
                                        uki += *uBIT16;
                                    }
                                    if (auth_params->ISLE) {
                                        uBIT16 = (uint16_t *)uki;
                                        sprintf(srst+strlen(srst), "\t\t\tISL:%u\n", *uBIT16);
                                        uki += sizeof(uint16_t);//uk to MSZ
                                    }
                                    if (auth_params->MSE) {
                                        uBIT16 = (uint16_t *)uki;
                                        sprintf(srst+strlen(srst), "\t\t\tMSZ:%u\n", *uBIT16);
                                        uki += sizeof(uint16_t);//uk to SS
                                    }
                                    if (auth_params->SSE) {
                                        memset(vrem, 0, sizeof(vrem));
                                        dl = sprintf(vrem,"%s", uki);   if (dl > 255) vrem[255] = '\0';   dl = strlen(vrem);
                                        if (dl) sprintf(srst+strlen(srst),"\t\t\tSS:%s\n", vrem);
                                        uki += dl + 1;
                                    }
                                    if (auth_params->EXE) {
                                        memset(vrem, 0, sizeof(vrem));
                                        dl = sprintf(vrem,"%s", uki);   if (dl > 255) vrem[255] = '\0';   dl = strlen(vrem);
                                        if (dl) sprintf(srst+strlen(srst),"\t\t\tEXP:%s\n", vrem);
                                        uki += dl + 1;
                                    }
                                break;
                                case EGTS_SR_AUTH_INFO://       (7)
                                    dl = sprintf(info,"%s", uki);
                                    sprintf(srst+strlen(srst), "\t\tUNM:%s", info);//User Name
                                    uki += dl + 1;//uk to UPSW
                                    dl = sprintf(info,"%s", uki);
                                    sprintf(srst+strlen(srst), " UPSW:%s", info);//User Password
                                    uki += dl + 1;//uk to SS
                                    if (uke > uki) {
                                        dl = sprintf(info,"%s", uki);
                                        sprintf(srst+strlen(srst), " SS:%s", info);//Server Sequence
                                    }
                                    sprintf(srst+strlen(srst), "\n");
                                break;
                                case EGTS_SR_SERVICE_INFO://    (8)
                                    sr_info = (s_sr_info *)uki;
                                    byte = *(uki + 2);
                                    if (sr_info->SRVA) sprintf(vrem, "RequestedService");
                                                  else sprintf(vrem, "SupportedService");
                                    sprintf(srst+strlen(srst), "\t\tST(%u):'%s' SST(%u):'%s' SRVRP(%u):'%s' SRVA(%u):'%s'\n",
                                                    sr_info->ST, ServName(sr_info->ST),
                                                    sr_info->SST, SSTName(sr_info->SST),
                                                    sr_info->SRVRP, SRVRPName(byte),
                                                    sr_info->SRVA, vrem);
                                    uki += rlen;
                                break;
                                case EGTS_SR_RESULT_CODE://     (9)
                                    sprintf(srst+strlen(srst), "\t\tRCD:%u\n", *uki);
                                    uki += rlen;
                                break;
                                    //default : sprintf(srst, "");
                            }//switch (rtype) //switch by SubRecord Type  SRT
                            if (uke > uki) {
                                rtype = *uki++;                       // get SRT (Subrecord Type)
                                memcpy(&rlen, uki, sizeof(uint16_t)); // get SRL (Subrecord Length)
                                uki += sizeof(uint16_t);              // uk to SRD (Subrecord Data)
                            }
                        }//while (uke > uki)
                    break;
                    case EGTS_TELEDATA_SERVICE ://    (2)
                        memset(srst, 0, sizeof(srst));
                        while (uke > uki) {
                            sprintf(srst+strlen(srst),"\tSubRecord '%s' : rLen:%u rType(%u)\n", SubRecName(rtype), rlen, rtype);
                            switch (rtype) {// SRT (Subrecord Type)
                                case EGTS_SR_RECORD_RESPONSE:// (0)
                                {
                                    uki += rlen;
                                    //uBIT16 = (uint16_t *)uki;
                                    //uint8_t bt = *(uki + sizeof(uint16_t));
                                    //sprintf(srst+strlen(srst), "\t\tCRN %u for RN %u with RS %u\n", *uBIT16, rec_num, bt);
                                }
                                break;
                                case EGTS_SR_POS_DATA:// 16
                                    sr_pos_data = (s_sr_pos_data *)uki;
                                    dl  = sizeof(s_sr_pos_data);
                                    byte = *(uki + sizeof(uint32_t) * 3);
                                    odometer = 0;
                                    memcpy(&odometer, &sr_pos_data->ODM, 3);
                                    odometer /= 10;
                                    if (sr_pos_data->LAHS) strcpy(vr1, "South"); else strcpy(vr1, "North");
                                    if (sr_pos_data->LOHS) strcpy(vr2, "West");  else strcpy(vr2, "Ost");
                                    tim = sr_pos_data->NTM + UTS2010;
                                    latit  = sr_pos_data->LAT;    flatit  = latit;   flatit  = (flatit * 90.0)   / 0xffffffff;
                                    longit = sr_pos_data->LONG;   flongit = longit;  flongit = (flongit * 180.0) / 0xffffffff;
                                    speed = (sr_pos_data->SPD & 0x3fff) / 10;
                                    dir = sr_pos_data->DIR;   if (sr_pos_data->SPD & 0x8000) dir |= 0x100;
                                    sprintf(srst+strlen(srst),
                                                "\t\tNTM:'%s' (0x%08X=%u)\n\t\t'%s' LAT:0x%08X/%f  '%s' LONG:0x%08X/%f\n"
                                                "\t\tFlags:0x%02X\n\t\t\tALTE:%u MV:%u BB:%u CS:%u FIX:%u VLD:%u\n"
                                                "\t\tSPD:%u DIR:%u ODM:%u[%02X,%02X,%02X] DIN:0x%02X SRC:%u",
                                                ShowTime(tim), tim, tim,
                                                vr1, latit, flatit, vr2, longit, flongit,
                                                byte, sr_pos_data->ALTE, sr_pos_data->MV, sr_pos_data->BB,
                                                sr_pos_data->CS, sr_pos_data->FIX, sr_pos_data->VLD,
                                                speed, dir, odometer, sr_pos_data->ODM[0], sr_pos_data->ODM[1], sr_pos_data->ODM[2],
                                                sr_pos_data->DIN, sr_pos_data->SRC);
                                    /*byte = sr_pos_data->SRC;
                                    if (byte > MaxSrcLocation) byte = MaxSrcLocation - 1;
                                    sprintf(srst+strlen(srst), "(%s)", SrcLocation[byte]);*/
                                    if (sr_pos_data->ALTE) {
                                        altitude = 0;
                                        memcpy(&altitude, uki + dl, 3);
                                        sprintf(srst+strlen(srst), " ALT:%u[%02X,%02X,%02X]",
                                                                   altitude, *(uki + dl), *(uki + dl + 1),*(uki + dl + 2));
                                        dl += 3;
                                    }
                                    //if (sr_pos_data->SRC) {
                                    //    memcpy(&word, uki + dl, sizeof(uint16_t)); dl += sizeof(uint16_t);
                                    //    sprintf(srst+strlen(srst), " SRCD:%u", word);
                                    //}
                                    sprintf(srst+strlen(srst), "\n");
                                    uki += dl;
                                break;
                                case EGTS_SR_EXT_POS_DATA://17
                                    sr_ext_pos_data = (s_sr_ext_pos_data *)uki;
                                    byte = *uki;
                                    sprintf(srst+strlen(srst), "\t\tFlags:0x%02X\n\t\t\tNSFE:%u SFE:%u PFE:%u HFE:%u VFE:%u\n\t\t",
                                                    byte, sr_ext_pos_data->NSFE, sr_ext_pos_data->SFE, sr_ext_pos_data->PFE,
                                                    sr_ext_pos_data->HFE, sr_ext_pos_data->VFE);
                                    uki += sizeof(s_sr_ext_pos_data);
                                    if (sr_ext_pos_data->VFE) {
                                        memcpy(&word, uki, sizeof(uint16_t));
                                        uki += sizeof(uint16_t);
                                        sprintf(srst+strlen(srst), "VDOP:%u ", word);
                                    }
                                    if (sr_ext_pos_data->HFE) {
                                        memcpy(&word, uki, sizeof(uint16_t));
                                        uki += sizeof(uint16_t);
                                        sprintf(srst+strlen(srst), "HDOP:%u ", word);
                                    }
                                    if (sr_ext_pos_data->PFE) {
                                        memcpy(&word, uki, sizeof(uint16_t));
                                        uki += sizeof(uint16_t);
                                        sprintf(srst+strlen(srst), "PDOP:%u ", word);
                                    }
                                    if (sr_ext_pos_data->SFE) {
                                        byte = *uki++;
                                        sprintf(srst+strlen(srst), "SAT:%u ", byte);
                                    }
                                    if (sr_ext_pos_data->NSFE) {
                                        memcpy(&word, uki, sizeof(uint16_t));
                                        uki += sizeof(uint16_t);
                                        sprintf(srst+strlen(srst), "NS:%u ", word);
                                    }
                                    sprintf(srst+strlen(srst), "\n");
                                break;
                                case EGTS_SR_AD_SENSORS_DATA://18
                                    sr_ad_sensors_data = (s_sr_ad_sensors_data *)uki;
                                    sprintf(srst+strlen(srst), "\t\tDIOE:0x%02X DOUT:0x%02X ASFE:0x%02X\n",
                                                    sr_ad_sensors_data->DIOE, sr_ad_sensors_data->DOUT, sr_ad_sensors_data->ASFE);
                                    uki += sizeof(s_sr_ad_sensors_data);
                                    j = 1;
                                    for (i = 0; i < 8; i++) {
                                        if (sr_ad_sensors_data->ASFE & j) {
                                            memcpy(&word, uki + 1, 2);   word = htons(word);
                                            sprintf(srst+strlen(srst),"\t\tANS[%u]:%u [%02X,%02X,%02X]\n",
                                                                i, word, *(uki), *(uki + 1),*(uki + 2));
                                            uki += 3;
                                        }
                                        j <<= 1;
                                    }
                                break;
                                case EGTS_SR_COUNTERS_DATA://19
                                    uki += rlen;
                                break;
                                case EGTS_SR_ACCEL_DATA://20
                                    uki += rlen;
                                break;
                                case EGTS_SR_STATE_DATA://21
                                    uki += rlen;
                                break;
                                case EGTS_SR_LOOPIN_DATA://22
                                    uki += rlen;
                                break;
                                case EGTS_SR_ABS_DIG_SENS_DATA://23
                                    uki += rlen;
                                break;
                                case EGTS_SR_ABS_AN_SENS_DATA://24
                                    uki += rlen;
                                break;
                                case EGTS_SR_ABS_CNTR_DATA://25
                                    uki += rlen;
                                break;
                                case EGTS_SR_ABS_LOOPIN_DATA://26
                                    uki += rlen;
                                break;
                                case EGTS_SR_LIQUID_LEVEL_SENSOR://27
                                    uki += rlen;
                                break;
                                case EGTS_SR_PASSENGERS_COUNTERS://28
                                    uki += rlen;
                                break;
                                    //default : sprintf(srst+strlen(srst), "EGTS_SR_UNKNOWN\n");
                            }//switch (rtype) - SRT (Subrecord Type)
                            if (uke > uki) {
                                rtype = *uki++;//get SRT (Subrecord Type)
                                memcpy(&rlen, uki, sizeof(uint16_t));//get SRL (Subrecord Length)
                                uki += sizeof(uint16_t);//uk to SRD (Subrecord Data)
                            }
                        }//while (uke > uki)
                    break;
                    case EGTS_COMMANDS_SERVICE ://    (4)
                        //sprintf(tp, "EGTS_COMMANDS_SERVICE");
                    break;
                    case EGTS_FIRMWARE_SERVICE ://    (9)
                        //sprintf(tp, "EGTS_FIRMWARE_SERVICE");
                    break;
                    case EGTS_ECALL_SERVICE ://        (10)
                        //sprintf(tp, "EGTS_ECALL_SERVICE");
                    break;
                        //default : sprintf(tp, "EGTS_UNKNOWN");
                }// end switch by Service Type  RST
                sprintf(tps+strlen(tps), "Record '%s' : RL:%u RN:%u\n\trFlag:0x%02X\n\t\tSSOD:%u RSOD:%u GRP:%u RPP:%02u TMFE:%u EVFE:%u OBFE:%u\n\t",
                                ServName(rst),
                                rec_hdr->RL, rec_hdr->RN, *rflag,
                                rec_hdr->SSOD, rec_hdr->RSOD, rec_hdr->GRP, rec_hdr->RPP, rec_hdr->TMFE, rec_hdr->EVFE, rec_hdr->OBFE);
                if (rec_hdr->OBFE) sprintf(tps+strlen(tps),"OID:%u ", oid);
                if (rec_hdr->EVFE) sprintf(tps+strlen(tps),"EVID:%u ", evid);
                if (rec_hdr->TMFE) sprintf(tps+strlen(tps),"TM:'%s' (0x%08X=%u) ", TimeToStr((time_t)tim), tim, tim);

                sprintf(tps+strlen(tps),"SST:%u RST:%u\n%s", sst, rst, srst);
            }
        break;
        case EGTS_PT_SIGNED_APPDATA://2
            //sprintf(tps+strlen(tps), "packet type : EGTS_PT_SIGNED_APPDATA, ");
        break;
            //default : sprintf(tps+strlen(tps), "packet type : UNKNOWN, ");
    }//switch (hdr->PT)


    if (prn) {
        sprintf(tps+strlen(tps), "\tCRC8=0x%02X/0x%02X CRC16=0x%04X/0x%04X\n",
                                hdr->HCS, calc_CRC8, *crc16, calc_CRC16);
        dl = flen;   if (dl > ((vrem_size >> 1) - 32)) dl = (vrem_size >> 1) - 32;
        for (uk = 0; uk < dl; uk++) sprintf(tps+strlen(tps),"%02X ", from_cli[uk]);
        sprintf(tps+strlen(tps),"\n");
        if (iret < 0) {
            sprintf(tps+strlen(tps),"---------------------------------------------\n");
            print_msg(0, tps);
            return 0;
        } else print_msg(0, tps);
    } else if (iret < 0) return 0;

    //----------------------------   Check   ----------------------------
    dl = 0;
    if ( (hdr->PRV != 1) || (hdr->PRF & 0xC0) ) {
        dl = sprintf(tps, "[%s]: EGTS_PC_UNS_PROTOCOL error\n", __func__);
        res = EGTS_PC_UNS_PROTOCOL;
        err = 1;
        goto done;
    }
    if ((hdr->HL != 11) || (hdr->HE)) {
        dl = sprintf(tps, "[%s]: EGTS_PC_INC_HEADERFORM error\n", __func__);
        res = EGTS_PC_INC_HEADERFORM;
        err = 2;
        goto done;
    }
    if (calc_CRC8 != hdr->HCS) {
        dl = sprintf(tps, "[%s]: EGTS_PC_HEADERCRC_ERROR error 0x%02X/0x%02X\n", __func__, hdr->HCS, calc_CRC8);
        res = EGTS_PC_HEADERCRC_ERROR;
        err = 3;
        goto done;
    }
    if (hdr->RTE) {
        dl = sprintf(tps, "[%s]: EGTS_PC_TTLEXPIRED error\n", __func__);
        res = EGTS_PC_TTLEXPIRED;
        err = 4;
        goto done;
    }
    if (hdr->ENA) {
        dl = sprintf(tps, "[%s]: EGTS_PC_DECRYPT_ERROR error\n", __func__);
        res = EGTS_PC_DECRYPT_ERROR;
        err = 4;
        goto done;
    }
    if (hdr->CMP) {
        dl = sprintf(tps, "[%s]: EGTS_PC_INC_DATAFORM error\n", __func__);
        res = EGTS_PC_INC_DATAFORM;
        err = 4;
        goto done;
    }
    if (calc_CRC16 != *crc16) {
        dl = sprintf(tps, "[%s]: EGTS_PC_DATACRC_ERROR error 0x%04X/0x%04X\n", __func__, *crc16, calc_CRC16);
        res = EGTS_PC_DATACRC_ERROR;
        err = 4;
    }
    //-------------------------------------------------------------------

done:

    if (!err) res = EGTS_PC_OK;

    return (make_ack(to_cli, rec_num, pid_num, res, prn));
}

//-----------------------------------------------------------------------

void *egts_nitka(void *arg)
{
uint8_t from_client[buf_size];
uint8_t to_client[buf_size];
char tps[vrem_size];
char dev[size_imei + 1] = {0};
int lenrecv = 0, lenrecv_tmp = 0, ready = 0, Vixod = 0, uk = 0, client = -1, dl = 0, ack = 0, to_client_len = 0;
fd_set read_Fds;
struct timeval cli_tv;
s_min_hdr min_hdr;
uint32_t gtmr, tmr = 0;
uint8_t *flags = NULL;
//------------   test massage data   --------------
uint8_t test_flag = 0;
uint32_t test_tmr = 0;
const uint8_t test_rec[24] = {
    0x01, 0x00, 0x03, 0x0B, 0x00, 0x0B, 0x00, 0x01, 0x00, 0x01, 0xC2,
    0x04, 0x00, 0x01, 0x00, 0x58, 0x01, 0x01, 0x09, 0x01, 0x00, 0x00, 0xE7, 0x3C
};


    if (service_flag) {
        test_flag = 1;
        test_tmr = get_timer_sec(10);
    }

    QuitCli = 0;

    client = *(int *)arg;

    print_msg(1, "[%s] Start thread (socket %d)\n", dev, client);

    memset(from_client, 0, buf_size);
    gtmr = get_timer_sec(max_data_wait);

    while (!Vixod) {

        cli_tv.tv_sec = 0;   cli_tv.tv_usec = 25000;
        FD_ZERO(&read_Fds);   FD_SET(client, &read_Fds);

        if (select(client + 1, &read_Fds, NULL, NULL, &cli_tv) > 0) {
            lenrecv_tmp = recv(client, &from_client[uk], 1, MSG_DONTWAIT);
            if (!lenrecv_tmp) {
                print_msg(1, "[%s] Client closed connection\n", dev);
                break;
            } else if (lenrecv_tmp > 0) {
                lenrecv += lenrecv_tmp;
                uk      += lenrecv_tmp;
                if ((lenrecv == sizeof(s_min_hdr)) && (!dl)) {
                    memcpy((uint8_t *)&min_hdr.PRV, (uint8_t *)&from_client[0], sizeof(s_min_hdr));
                    dl = min_hdr.FDL + 2;
                    flags = &min_hdr.PRV;   flags += 2;
                    print_msg(1, "[%s] Hdr(%lu):\n\tPRV:0x%02X\n\tSKID:0x%02X\n\tFlags:0x%02X\n"
                                "\t\tPRF:%u RTE:%u ENA:%u CMP:%u PR:%u\n"
                                "\tHL:%u\n\tHE:%u\n\tFDL:%u(0x%04X)\n\tPID:%u(0x%04X)\n\tPT:%u\t'%s'\n\tHCS:0x%02X\n",
                                dev, sizeof(s_min_hdr), min_hdr.PRV, min_hdr.SKID, *flags,
                                min_hdr.PRF, min_hdr.RTE, min_hdr.ENA, min_hdr.CMP, min_hdr.PR,
                                min_hdr.HL, min_hdr.HE, min_hdr.FDL, min_hdr.FDL,
                                min_hdr.PID, min_hdr.PID, min_hdr.PT, PackName(min_hdr.PT), min_hdr.HCS);
                    lenrecv = lenrecv_tmp = 0;   uk = 0;
                    memset(from_client, 0, buf_size);
                    tmr = get_timer_sec(5);
                } else {
                    if (dl > 0) { if (lenrecv == dl) ready = 1; }
                    if (uk >= buf_size - sizeof(s_min_hdr)) break;
                }
            }

            if (tmr && !ready) { if (check_delay_sec(tmr)) ready = 1; }

        }//if select()


        if (ready)  {
            tmr = 0;
            if (lenrecv) {
                to_client_len = egts_parse(dev, &min_hdr, from_client, lenrecv, to_client, 1);
                if (to_client_len) {
                    sprintf(tps, "[%s] Send to device(%d):", dev, to_client_len);
                    for (uk = 0; uk < to_client_len; uk++) sprintf(tps+strlen(tps)," %02X", to_client[uk]);
                    sprintf(tps+strlen(tps),"\n--------------------------------------------\n");
                    print_msg(1, tps);
                    if (send(client, to_client, to_client_len, MSG_DONTWAIT) != to_client_len) {
                        print_msg(1, "[%s] Error sending data to client !!!\n", dev);
                    }
                }
            }
            lenrecv = lenrecv_tmp = 0;
            ready = ack = 0;
            uk = dl = 0;
            memset(from_client, 0, buf_size);
            memset((uint8_t *)&min_hdr, 0, sizeof(s_min_hdr));
            gtmr = get_timer_sec(max_data_wait);
        }//if (ready)


        if (QuitAll) break;

        if (check_delay_sec(gtmr)) {
            print_msg(1, "[%s] Timeout ....(%d sec)\n", dev, max_data_wait);
            break;
        }

        //-------------------------------   Test message   -------------------------------------------
        if (test_flag) {
            if (check_delay_sec(test_tmr)) {
                test_flag = 0;
                to_client_len = sizeof(test_rec);
                memcpy(to_client, test_rec, to_client_len);
                *(to_client + to_client_len) = 0;
                sprintf(tps, "[%s] Send test to device(%d):", dev, to_client_len);
                for (uk = 0; uk < to_client_len; uk++) sprintf(tps+strlen(tps)," %02X", to_client[uk]);
                sprintf(tps+strlen(tps),"\n--------------------------------------------\n");
                print_msg(1, tps);
                //
                if (send(client, to_client, to_client_len, MSG_DONTWAIT) != to_client_len) {
                    print_msg(1, "[%s] Error sending data to client !!!\n", dev);
                }
                //
                lenrecv = lenrecv_tmp = 0;
                ready = ack = 0;
                uk = dl = 0;
                memset(from_client, 0, buf_size);
                gtmr = get_timer_sec(max_data_wait);
                //
            }
        }
        //--------------------------------------------------------------------------------------------

    }//while (!Vixod)

    if (client) {
        shutdown(client, SHUT_RDWR);
        close(client);
    }

    print_msg(1, "[%s] Close thread (socket %d)\n", dev, client);

    QuitCli = 1;

    pthread_exit(NULL);

}

//---------------------------------------------------------------------

