//defines
#define GAUGE_MAX_CV 300
#define GAUGE_MIN_CV 100
#define GAUGE_MAX_TEMPERATURE 70




//Prototypes
void GAUGE_vfnSubSysInit(void);
void GAUGE_vfnSendCV(uint_8 *bpTemp);
uint_8 GAUGE_bfnTempToCV(uint_8 *bpTemp);

